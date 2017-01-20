#include "defs.h" 
#include "fdbtable.h"
#include "switch-api.h"

// Keeper for the routing table...
static struct FdbTable *fdb_table = NULL;

/**
*   Private access function to find a fdb from a given 
*   Route Descriptor.
*/
/**
*   Initializes the routing table.
*/
void
initFdbTable()
{
   
   

    // Clear routing table...
    fdb_table = NULL;
}
struct FdbTable *
findFdb(uint32 group)
{
    struct FdbTable *cfdb;

    for (cfdb = fdb_table; cfdb; cfdb = cfdb->nextfdb) {
        if (cfdb->group == group) {
            return cfdb;
        }
    }

    return NULL;
}
#if (DNI_MULTI_LAN_SUPPORT)
int
insertFdb(uint32 group, uint32 port)
{
	return 0;
}
int
removeFdb(struct FdbTable *cfdb)
{
	return 0;
}
#else
/**
*   Adds a specified Fdb to the bridgingtable.
*   If the Fdb already exists, the existing fdb 
*   is updated...
*/
int
insertFdb(uint32 group, uint32 port)
{

    //struct Config *conf = getCommonConfig();
    struct FdbTable *cfdb;


    // Sanitycheck the group adress...
    if (!IN_MULTICAST(ntohl(group))) {
        atlog(LOG_WARNING, 0,
            "The group address %s is not a valid Multicast group. Table insert failed.",
            inetFmt(group, s1));
        return 0;
    }

    if (port == 0) {
        atlog(LOG_WARNING, 0,
            "port %d is not a valid port. Table insert failed.", port);
        
        return 0;
    }
    
    // Try to find an existing fdb for this group...
    cfdb = findFdb(group);

    if (cfdb == NULL) {
        struct FdbTable *newfdb;

        IF_DEBUG atlog(LOG_DEBUG, 0, "No existing fdb for %s. Create new.",
                     inetFmt(group, s1));

        // Create and initialize the new route table entry..
        newfdb = (struct FdbTable *) malloc(sizeof (struct FdbTable));
        // Insert the route desc and clear all pointers...
        newfdb->group = group;
        newfdb->nextfdb = NULL;
        newfdb->prevfdb = NULL;

        // The group is not joined initially.
        newfdb->upstrState = FDBSTATE_NOTJOINED;

        //newfdb->ageValue = conf->robustnessValue + 1;
        newfdb->ageValue = 1;	 /* about 60s */
        newfdb->ageActivity = 0;
        
        BIT_ZERO(newfdb->ageSwifBits); // Initially we assume no listeners.

        // Set the listener flag...
        BIT_ZERO(newfdb->swifBits);    // Initially no listeners...
        BIT_SET(newfdb->swifBits, port);

        // Check if there is a table already....
        if (fdb_table == NULL) {

            // No location set, so insert in on the table top.
            fdb_table = newfdb;
            IF_DEBUG atlog(LOG_DEBUG, 0,
                         "No fdbs in table. Insert at beginning.");

        } else {

            IF_DEBUG atlog(LOG_DEBUG, 0,
                         "Found existing fdbs. Find insert location.");

            // Check if the route could be inserted at the beginning...
            if (fdb_table->group > group) {
                IF_DEBUG atlog(LOG_DEBUG, 0,
                             "Inserting at beginning, before route %s",
                             inetFmt(fdb_table->group, s1));

                // Insert at beginning...
                newfdb->nextfdb = fdb_table;
                newfdb->prevfdb = NULL;
                fdb_table = newfdb;

                // If the route has a next node, the previous pointer must be updated.
                if (newfdb->nextfdb != NULL) {
                    newfdb->nextfdb->prevfdb = newfdb;
                }

            } else {

                // Find the location which is closest to the route.
                for (cfdb = fdb_table; cfdb->nextfdb != NULL;
                     cfdb = cfdb->nextfdb) {
                    // Find insert position.
                    if (cfdb->nextfdb->group > group) {
                        break;
                    }
                }

                IF_DEBUG atlog(LOG_DEBUG, 0, "Inserting after route %s",
                             inetFmt(cfdb->group, s1));

                // Insert after current...
                newfdb->nextfdb = cfdb->nextfdb;
                newfdb->prevfdb = cfdb;
                if (cfdb->nextfdb != NULL) {
                    cfdb->nextfdb->prevfdb = newfdb;
                }
                cfdb->nextfdb = newfdb;
            }
        }

        // Set the new route as the current...
        cfdb = newfdb;
        internUpdateFdb(cfdb->group, cfdb->swifBits, 1);

        // Log the cleanup in debugmode...
        atlog(LOG_INFO, 0, "Inserted route table entry for %s on VIF ",
            inetFmt(cfdb->group, s1));

    } else {

        // The route exists already, so just update it.
        BIT_SET(cfdb->swifBits, port);

        // Register the VIF activity for the aging routine
        BIT_SET(cfdb->ageSwifBits, port);

        if (cfdb->swifBits != cfdb->ageSwifBits)
            internUpdateFdb(cfdb->group, cfdb->swifBits, 1);                
        
        // Log the cleanup in debugmode...
        atlog(LOG_INFO, 0, "Updated fdb entry for %s on port #%d",
            inetFmt(cfdb->group, s1), port);

    }
    
    // Send join message upstream, if the route has no joined flag...
    if (cfdb->upstrState != FDBSTATE_JOINED) {
        cfdb->upstrState = FDBSTATE_JOINED;
    }

    IF_DEBUG logFdbTable("Insert Fdb");

    return 1;
}

/**
*   Remove a specified Fdb. Returns 1 on success,
*   and 0 if fdb was not found.
*/
int
removeFdb(struct FdbTable *cfdb)
{
    //struct Config *conf = getCommonConfig();
    int result = 1;

    // If croute is null, no routes was found.
    if (cfdb == NULL) {
        return 0;
    }
    // Log the cleanup in debugmode...
    IF_DEBUG atlog(LOG_DEBUG, 0, "Removed route entry for %s from table.",
                 inetFmt(cfdb->group, s1));

    //BIT_ZERO(croute->vifBits);

    // Uninstall current route from kernel
    if (!internUpdateFdb(cfdb->group, cfdb->swifBits, 0)) {
        atlog(LOG_WARNING, 0, "The removal from Kernel failed.");
        result = 0;
    }


    // Update pointers...
    if (cfdb->prevfdb == NULL) {
        // Topmost node...
        if (cfdb->nextfdb != NULL) {
            cfdb->nextfdb->prevfdb = NULL;
        }
        fdb_table = cfdb->nextfdb;

    } else {
        cfdb->prevfdb->nextfdb = cfdb->nextfdb;
        if (cfdb->nextfdb != NULL) {
            cfdb->nextfdb->prevfdb = cfdb->prevfdb;
        }
    }
    // Free the memory, and set the route to NULL...
    free(cfdb);
    cfdb = NULL;
    
    IF_DEBUG logFdbTable("Remove Fdb");

    return result;
}
#endif
/**
*   Clear all routes from routing table, and alerts Leaves upstream.
*/
void
clearAllFdbs()
{
    struct FdbTable *cfdb, *remainfdb;
    //int i;

    // Loop through all routes...
    for (cfdb = fdb_table; cfdb; cfdb = remainfdb) {

        remainfdb = cfdb->nextfdb;

        // Log the cleanup in debugmode...
        IF_DEBUG atlog(LOG_DEBUG, 0, "Removing route entry for %s",
                     inetFmt(cfdb->group, s1));

        if (!internUpdateFdb(cfdb->group, cfdb->swifBits, 0)) {
            atlog(LOG_WARNING, 0, "The removal from Kernel failed.");
        }

        // Clear memory, and set pointer to next route...
        free(cfdb);
    }
    fdb_table = NULL;

    // Send a notice that the routing table is empty...
    atlog(LOG_NOTICE, 0, "All fdbs removed. FDB table is empty.");
}

/**
*   Ages a specific route
*/
static int
internAgeFdb(struct FdbTable *cfdb)
{
    //struct Config *conf = getCommonConfig();
	int ageValue = 1;
    int result = 0;
   
    // Drop age by 1.
    cfdb->ageValue--;

    // Check if there has been any activity...
    if (cfdb->ageSwifBits > 0 && cfdb->ageActivity == 0) {
        // There was some activity, check if all registered vifs responded.
        if (cfdb->swifBits == cfdb->ageSwifBits) {
            // Everything is in perfect order, so we just update the route age.
            //cfdb->ageValue = conf->robustnessValue;
            cfdb->ageValue = ageValue;
            //croute->ageActivity = 0;
        } else {
            // One or more VIF has not gotten any response.
            // new inserted Fdb don't record in ageActivity
            //cfdb->ageActivity = conf->robustnessValue - cfdb->ageValue;
            cfdb->ageActivity = ageValue - cfdb->ageValue;

            // Update the actual bits for the route...
            cfdb->ageSwifResultBits = cfdb->ageSwifBits;
        }
    }
    // Check if there have been activity in aging process...
    else if (cfdb->ageActivity > 0) {

        // If the bits are different in this round, we must
        if (cfdb->ageSwifResultBits != cfdb->ageSwifBits) {
            // Or the bits together to insure we don't lose any listeners.
            cfdb->ageSwifResultBits |= cfdb->ageSwifBits;

            // Register changes in this round as well..
            //cfdb->ageActivity = conf->robustnessValue - cfdb->ageValue;
            cfdb->ageActivity = ageValue - cfdb->ageValue;
        }
    }
    // If the aging counter has reached zero, its time for updating...
    if (cfdb->ageValue == 0) {
        // Check for activity in the aging process,
        if (cfdb->ageActivity > 0) {

            IF_DEBUG atlog(LOG_DEBUG, 0, "Updating fdb after aging : %s",
                         inetFmt(cfdb->group, s1));
            
            cfdb->ageSwifBits = cfdb->ageSwifResultBits;
            
            // Just update the routing settings in kernel...
            internUpdateFdb(cfdb->group, cfdb->ageSwifBits, 1);           

            // We append the activity counter to the age, and continue...
            cfdb->ageValue = cfdb->ageActivity;
            cfdb->ageActivity = 0;
        } else {

            IF_DEBUG atlog(LOG_DEBUG, 0, "Removing group %s. FDB Died of old age.",
                         inetFmt(cfdb->group, s1));

            // No activity was registered within the timelimit, so remove the route.
            removeFdb(cfdb);
        }
        // Tell that the route was updated...
        result = 1;
    }
    // The aging vif bits must be reset for each round...
    BIT_ZERO(cfdb->ageSwifBits);

    return result;
}

/**
*   This function loops through all Fdbs, and updates the age 
*   of any active Fdbs.
*/
void
ageActiveFdbs()
{
    struct FdbTable *cfdb, *nfdb;

    IF_DEBUG atlog(LOG_DEBUG, 0, "Aging fdbs in table.");

    // Scan all routes...
    for (cfdb = fdb_table; cfdb != NULL; cfdb = nfdb) {

        // Keep the next route (since current route may be removed)...
        nfdb = cfdb->nextfdb;

        // Run the aging round algorithm.
        if (cfdb->upstrState != FDBSTATE_CHECK_LAST_MEMBER) {
            // Only age routes if Last member probe is not active...
            internAgeFdb(cfdb);
        }
    }
    IF_DEBUG logFdbTable("Age active fdbs");    
}


/**
*   Updates the Kernel routing table. If activate is 1, the route
*   is (re-)activated. If activate is false, the route is removed.
*/
int internUpdateFdb(int group, uint32 ifBits, int activate) {


    if (activate) {
        IF_DEBUG atlog(LOG_DEBUG, 0, "group %x swif bits : 0x%08x", group,
                     ifBits);
        
        addMFdb(group, ifBits);
    } else {
        delMFdb(group);
    }
    
    return 1;
}

/**
*   Debug function that writes the routing table entries
*   to the log.
*/
void
logFdbTable(char *header)
{
    IF_DEBUG {
        struct FdbTable *cfdb= fdb_table;
        unsigned rcount = 0;

        atlog(LOG_DEBUG, 0,
            "\nCurrent fdb table (%s);\n-----------------------------------------------------\n",
            header);
        if (cfdb == NULL) {
            atlog(LOG_DEBUG, 0, "No fdb entry in table...");
        } else {
            do {
                /*
                   atlog(LOG_DEBUG, 0, "#%d: Src: %s, Dst: %s, Age:%d, St: %s, Prev: 0x%08x, T: 0x%08x, Next: 0x%08x",
                   rcount, inetFmt(croute->originAddr, s1), inetFmt(croute->group, s2),
                   croute->ageValue,(croute->originAddr>0?"A":"I"),
                   croute->prevroute, croute, croute->nextroute);
                 */
                atlog(LOG_DEBUG, 0,
                    "#%d: Dst: %s, Age:%d, OutVifs: 0x%08x",
                    rcount, 
                    inetFmt(cfdb->group, s2), cfdb->ageValue,
                    cfdb->swifBits);

                cfdb = cfdb->nextfdb;

                rcount++;
            } while (cfdb != NULL);
        }

        atlog(LOG_DEBUG, 0,
            "\n-----------------------------------------------------\n");
    }
}

