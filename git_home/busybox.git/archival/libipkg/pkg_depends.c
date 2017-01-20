/* pkg_depends.c - the itsy package management system

   Steven M. Ayer
   
   Copyright (C) 2002 Compaq Computer Corporation

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
*/

#include "ipkg.h"
#include <errno.h>
#include <ctype.h>
   
#include "pkg.h"
#include "ipkg_utils.h"
#include "pkg_hash.h"
#include "ipkg_message.h"
#include "pkg_parse.h"
#include "hash_table.h"

static int parseDepends(compound_depend_t *compound_depend, hash_table_t * hash, char * depend_str);
static depend_t * depend_init(void);
static void depend_deinit(depend_t *d);
static char ** add_unresolved_dep(pkg_t * pkg, char ** the_lost, int ref_ndx);
static char ** merge_unresolved(char ** oldstuff, char ** newstuff);
static int is_pkg_in_pkg_vec(pkg_vec_t * vec, pkg_t * pkg);

static int pkg_installed_and_constraint_satisfied(pkg_t *pkg, void *cdata)
{
     depend_t *depend = (depend_t *)cdata;
     if ((pkg->state_status == SS_INSTALLED || pkg->state_status == SS_UNPACKED) && version_constraints_satisfied(depend, pkg))
	  return 1;
     else
	  return 0;
}

static int pkg_constraint_satisfied(pkg_t *pkg, void *cdata)
{
     depend_t *depend = (depend_t *)cdata;
#if 0
     pkg_t * temp = pkg_new();
     int comparison;
     parseVersion(temp, depend->version);
     comparison = pkg_compare_versions(pkg, temp);
     free(temp);

     fprintf(stderr, "%s: pkg=%s pkg->version=%s constraint=%p type=%d version=%s comparison=%d satisfied=%d\n", 
	     __FUNCTION__, pkg->name, pkg->version, 
	     depend, depend->constraint, depend->version,
	     comparison, version_constraints_satisfied(depend, pkg));
#endif
     if (version_constraints_satisfied(depend, pkg))
	  return 1;
     else
	  return 0;
}

/* returns ndependences or negative error value */ 
int pkg_hash_fetch_unsatisfied_dependencies(ipkg_conf_t *conf, pkg_t * pkg, 
					    pkg_vec_t *unsatisfied, char *** unresolved)
{
     pkg_t * satisfier_entry_pkg;
     register int i, j, k, l;
     int count, found;
     char ** the_lost;
     abstract_pkg_t * ab_pkg;

     /* 
      * this is a setup to check for redundant/cyclic dependency checks, 
      * which are marked at the abstract_pkg level
      */
     if (!(ab_pkg = pkg->parent)) {
	  fprintf(stderr, "%s:%d: something terribly wrong with pkg %s\n", __FUNCTION__, __LINE__, pkg->name);
	  *unresolved = NULL;
	  return 0;
     }
     if (ab_pkg->dependencies_checked) {    /* avoid duplicate or cyclic checks */
	  *unresolved = NULL;
	  return 0;
     } else { 
	  ab_pkg->dependencies_checked = 1;  /* mark it for subsequent visits */
     }
     /**/

     count = pkg->pre_depends_count + pkg->depends_count + pkg->recommends_count + pkg->suggests_count;
     if (!count){
	  *unresolved = NULL;
	  return 0;
     }

     the_lost = NULL;
	
     /* foreach dependency */
     for (i = 0; i < count; i++) {
	  compound_depend_t * compound_depend = &pkg->depends[i];
	  depend_t ** possible_satisfiers = compound_depend->possibilities;;
	  found = 0;
	  satisfier_entry_pkg = NULL;

	  if (compound_depend->type == GREEDY_DEPEND) {
	       /* foreach possible satisfier */
	       for (j = 0; j < compound_depend->possibility_count; j++) {
		    /* foreach provided_by, which includes the abstract_pkg itself */
		    abstract_pkg_t *abpkg = possible_satisfiers[j]->pkg;
		    abstract_pkg_vec_t *ab_provider_vec = abpkg->provided_by;
		    int nposs = ab_provider_vec->len;
		    abstract_pkg_t **ab_providers = ab_provider_vec->pkgs; 
		    for (l = 0; l < nposs; l++) {
			 pkg_vec_t *test_vec = ab_providers[l]->pkgs;
			 /* if no depends on this one, try the first package that Provides this one */
			 if (!test_vec){   /* no pkg_vec hooked up to the abstract_pkg!  (need another feed?) */
			      continue;
			 }
	      
			 /* cruise this possiblity's pkg_vec looking for an installed version */
			 for (k = 0; k < test_vec->len; k++) {
			      pkg_t *pkg_scout = test_vec->pkgs[k];
			      /* not installed, and not already known about? */
			      if ((pkg_scout->state_want != SW_INSTALL)
				  && !pkg_scout->parent->dependencies_checked
				  && !is_pkg_in_pkg_vec(unsatisfied, pkg_scout)) {
				   char ** newstuff = NULL;
				   int rc;
				   pkg_vec_t *tmp_vec = pkg_vec_alloc ();
				   /* check for not-already-installed dependencies */
				   rc = pkg_hash_fetch_unsatisfied_dependencies(conf, 
										pkg_scout, 
										tmp_vec,
										&newstuff);
				   if (newstuff == NULL) {
					int ok = 1;
					for (l = 0; l < rc; l++) {
					    pkg_t *p = tmp_vec->pkgs[l];
					    if (p->state_want == SW_INSTALL)
						continue;
					    ipkg_message(conf, IPKG_DEBUG, "not installing %s due to requirement for %s\n", pkg_scout->name, p->name);
					    ok = 0;
					    break;
					}
					pkg_vec_free (tmp_vec);
					if (ok) {
					    /* mark this one for installation */
					    ipkg_message(conf, IPKG_NOTICE, "Adding satisfier for greedy dependence: %s\n", pkg_scout->name);
					    pkg_vec_insert(unsatisfied, pkg_scout);
					}
				   } else  {
					ipkg_message(conf, IPKG_DEBUG, "not installing %s due to broken depends \n", pkg_scout->name);
					free (newstuff);
				   }
			      }
			 }
		    }
	       }

	       continue;
	  }

	  /* foreach possible satisfier, look for installed package  */
	  for (j = 0; j < compound_depend->possibility_count; j++) {
	       /* foreach provided_by, which includes the abstract_pkg itself */
	       depend_t *dependence_to_satisfy = possible_satisfiers[j];
	       abstract_pkg_t *satisfying_apkg = possible_satisfiers[j]->pkg;
	       pkg_t *satisfying_pkg = 
		    pkg_hash_fetch_best_installation_candidate(conf, satisfying_apkg, 
							       pkg_installed_and_constraint_satisfied, 
							       dependence_to_satisfy, 1);
               /* Being that I can't test constraing in pkg_hash, I will test it here */
	       if (satisfying_pkg != NULL) {
                  if (!pkg_installed_and_constraint_satisfied ( satisfying_pkg,dependence_to_satisfy)) {
	              satisfying_pkg = NULL;
                  }
               }
	       ipkg_message(conf, IPKG_DEBUG, "%s:%d: satisfying_pkg=%p \n", __FILE__, __LINE__, satisfying_pkg);
	       if (satisfying_pkg != NULL) {
		    found = 1;
		    break;
	       }

	  }
	  /* if nothing installed matches, then look for uninstalled satisfier */
	  if (!found) {
	       /* foreach possible satisfier, look for installed package  */
	       for (j = 0; j < compound_depend->possibility_count; j++) {
		    /* foreach provided_by, which includes the abstract_pkg itself */
		    depend_t *dependence_to_satisfy = possible_satisfiers[j];
		    abstract_pkg_t *satisfying_apkg = possible_satisfiers[j]->pkg;
		    pkg_t *satisfying_pkg = 
			 pkg_hash_fetch_best_installation_candidate(conf, satisfying_apkg, 
								    pkg_constraint_satisfied, 
								    dependence_to_satisfy, 1);
                    /* Being that I can't test constraing in pkg_hash, I will test it here too */
	            if (satisfying_pkg != NULL) {
                         if (!pkg_constraint_satisfied ( satisfying_pkg,dependence_to_satisfy)) {
                            satisfying_pkg = NULL;
                         }
                    }

		    /* user request overrides package recommendation */
		    if (satisfying_pkg != NULL
			&& (compound_depend->type == RECOMMEND || compound_depend->type == SUGGEST)
			&& (satisfying_pkg->state_want == SW_DEINSTALL || satisfying_pkg->state_want == SW_PURGE)) {
			 ipkg_message (conf, IPKG_NOTICE, "%s: ignoring recommendation for %s at user request\n",
				       pkg->name, satisfying_pkg->name);
			 continue;
		    }

		    ipkg_message(conf, IPKG_DEBUG, "%s:%d: satisfying_pkg=%p\n", __FILE__, __LINE__, satisfying_pkg);
		    if (satisfying_pkg != NULL) {
			 satisfier_entry_pkg = satisfying_pkg;
			 break;
		    }
	       }
	  }

	  /* we didn't find one, add something to the unsatisfied vector */
	  if (!found) {
	       if (!satisfier_entry_pkg) {
		    /* failure to meet recommendations is not an error */
		    if (compound_depend->type != RECOMMEND && compound_depend->type != SUGGEST)
			 the_lost = add_unresolved_dep(pkg, the_lost, i);
		    else
			 ipkg_message (conf, IPKG_NOTICE, "%s: unsatisfied recommendation for %s\n",
				       pkg->name, compound_depend->possibilities[0]->pkg->name);
	       }
	       else {
		    if (compound_depend->type == SUGGEST) {
			 /* just mention it politely */
			 ipkg_message (conf, IPKG_NOTICE, "package %s suggests installing %s\n",
				       pkg->name, satisfier_entry_pkg->name);
		    } else {
			 char ** newstuff = NULL;
			 
			 if (satisfier_entry_pkg != pkg &&
			     !is_pkg_in_pkg_vec(unsatisfied, satisfier_entry_pkg)) {
			      pkg_vec_insert(unsatisfied, satisfier_entry_pkg);
			      pkg_hash_fetch_unsatisfied_dependencies(conf, 
								      satisfier_entry_pkg, 
								      unsatisfied,
								      &newstuff);
			      the_lost = merge_unresolved(the_lost, newstuff);
			 }
		    }
	       }
	  }
     }
     *unresolved = the_lost;

     return unsatisfied->len;
}

/*checking for conflicts !in replaces 
  If a packages conflicts with another but is also replacing it, I should not consider it a 
  really conflicts 
  returns 0 if conflicts <> replaces or 1 if conflicts == replaces 
*/
int is_pkg_a_replaces(pkg_t *pkg_scout,pkg_t *pkg)
{
    int i ;
    int replaces_count = pkg->replaces_count;
    abstract_pkg_t **replaces;

    if (pkg->replaces_count==0)    // No replaces, it's surely a conflict
        return 0;

    replaces = pkg->replaces;

    for (i = 0; i < replaces_count; i++) {
        if (strcmp(pkg_scout->name,pkg->replaces[i]->name)==0) {      // Found
            ipkg_message(NULL, IPKG_DEBUG2, "Seems I've found a replace %s %s \n",pkg_scout->name,pkg->replaces[i]->name);
            return 1;
        }
    }
    return 0;

}


/* Abhaya: added support for conflicts */
pkg_vec_t * pkg_hash_fetch_conflicts(hash_table_t * hash, pkg_t * pkg)
{
    pkg_vec_t * installed_conflicts, * test_vec;
    compound_depend_t * conflicts;
    depend_t ** possible_satisfiers;
    depend_t * possible_satisfier;
    register int i, j, k;
    int count;
    abstract_pkg_t * ab_pkg;
    pkg_t **pkg_scouts; 
    pkg_t *pkg_scout; 

    /* 
     * this is a setup to check for redundant/cyclic dependency checks, 
     * which are marked at the abstract_pkg level
     */
    if(!(ab_pkg = pkg->parent)){
	fprintf(stderr, "dependency check error.  pkg %s isn't in hash table\n", pkg->name);
	return (pkg_vec_t *)NULL;
    }

    conflicts = pkg->conflicts;
    if(!conflicts){
	return (pkg_vec_t *)NULL;
    }
    installed_conflicts = pkg_vec_alloc();

    count = pkg->conflicts_count;



    /* foreach conflict */
    for(i = 0; i < pkg->conflicts_count; i++){

	possible_satisfiers = conflicts->possibilities;

	/* foreach possible satisfier */
	for(j = 0; j < conflicts->possibility_count; j++){
            possible_satisfier = possible_satisfiers[j];
            if (!possible_satisfier)
                fprintf(stderr, "%s:%d: possible_satisfier is null\n", __FUNCTION__, __LINE__);
            if (!possible_satisfier->pkg)
                fprintf(stderr, "%s:%d: possible_satisfier->pkg is null\n", __FUNCTION__, __LINE__);
	    test_vec = possible_satisfier->pkg->pkgs;
	    if (test_vec) {
                /* pkg_vec found, it is an actual package conflict
		 * cruise this possiblity's pkg_vec looking for an installed version */
		pkg_scouts = test_vec->pkgs;
		for(k = 0; k < test_vec->len; k++){
                    pkg_scout = pkg_scouts[k];
                    if (!pkg_scout) {
                        fprintf(stderr,  "%s: null pkg scout\n", __FUNCTION__);
                        continue; 
                    }
		    if ((pkg_scout->state_status == SS_INSTALLED || pkg_scout->state_want == SW_INSTALL) &&
		       version_constraints_satisfied(possible_satisfier, pkg_scout) && !is_pkg_a_replaces(pkg_scout,pkg)){
 	 	        if (!is_pkg_in_pkg_vec(installed_conflicts, pkg_scout)){
			    pkg_vec_insert(installed_conflicts, pkg_scout);
			}
		    }
		}
	    }
	}
	conflicts++;
    }

    if (installed_conflicts->len)
	    return installed_conflicts;
    pkg_vec_free(installed_conflicts);
	return (pkg_vec_t *)NULL;
}

int version_constraints_satisfied(depend_t * depends, pkg_t * pkg)
{
    pkg_t * temp;
    int comparison;

    if(depends->constraint == NONE)
	return 1;

    temp = pkg_new();

    parseVersion(temp, depends->version);

    comparison = pkg_compare_versions(pkg, temp);

    free(temp);

    if((depends->constraint == EARLIER) && 
       (comparison < 0))
	return 1;
    else if((depends->constraint == LATER) && 
	    (comparison > 0))
	return 1;
    else if(comparison == 0)
	return 1;
    else if((depends->constraint == LATER_EQUAL) && 
	    (comparison >= 0))
	return 1;
    else if((depends->constraint == EARLIER_EQUAL) && 
	    (comparison <= 0))
	return 1;
    
    return 0;
}

int pkg_dependence_satisfiable(ipkg_conf_t *conf, depend_t *depend)
{
     abstract_pkg_t *apkg = depend->pkg;
     abstract_pkg_vec_t *provider_apkgs = apkg->provided_by;
     int n_providers = provider_apkgs->len;
     abstract_pkg_t **apkgs = provider_apkgs->pkgs;
     pkg_vec_t *pkg_vec;
     int n_pkgs ; 
     int i;
     int j;

     for (i = 0; i < n_providers; i++) {
	  abstract_pkg_t *papkg = apkgs[i];
	  pkg_vec = papkg->pkgs;
	  if (pkg_vec) {
	       n_pkgs = pkg_vec->len;
	       for (j = 0; j < n_pkgs; j++) {
		    pkg_t *pkg = pkg_vec->pkgs[j];
		    if (version_constraints_satisfied(depend, pkg)) {
			 return 1;
		    }
	       }
	  }
     }
     return 0;
}

int pkg_dependence_satisfied(ipkg_conf_t *conf, depend_t *depend)
{
     abstract_pkg_t *apkg = depend->pkg;
     abstract_pkg_vec_t *provider_apkgs = apkg->provided_by;
     int n_providers = provider_apkgs->len;
     abstract_pkg_t **apkgs = provider_apkgs->pkgs;
     int i;
     int n_pkgs;
     int j;

     for (i = 0; i < n_providers; i++) {
	  abstract_pkg_t *papkg = apkgs[i];
	  pkg_vec_t *pkg_vec = papkg->pkgs;
	  if (pkg_vec) {
	       n_pkgs = pkg_vec->len;
	       for (j = 0; j < n_pkgs; j++) {
		    pkg_t *pkg = pkg_vec->pkgs[j];
		    if (version_constraints_satisfied(depend, pkg)) {
			 if (pkg->state_status == SS_INSTALLED || pkg->state_status == SS_UNPACKED)
			      return 1;
		    }
	       }
	  }
     }
     return 0;
}

static int is_pkg_in_pkg_vec(pkg_vec_t * vec, pkg_t * pkg)
{
    register int i;
    pkg_t ** pkgs = vec->pkgs;

    for(i = 0; i < vec->len; i++)
	if((strcmp(pkg->name, (*(pkgs + i))->name) == 0)
	   && (pkg_compare_versions(pkg, *(pkgs + i)) == 0)
	   && (strcmp(pkg->architecture, (*(pkgs + i))->architecture) == 0))
	    return 1;
    return 0;
}


#ifdef DeadCode
/**
 * pkg_has_common_provides returns 1 if pkg and replacee both provide
 * the same abstract package and 0 otherwise.
 */
int pkg_has_common_provides(pkg_t *pkg, pkg_t *replacee)
{
     abstract_pkg_t **provides = pkg->provides;
     int provides_count = pkg->provides_count;
     abstract_pkg_t **replacee_provides = replacee->provides;
     int replacee_provides_count = replacee->provides_count;
     int i, j;
     for (i = 0; i < provides_count; i++) {
	  abstract_pkg_t *apkg = provides[i];
	  for (j = 0; j < replacee_provides_count; j++) {
	       abstract_pkg_t *replacee_apkg = replacee_provides[i];
	       if (apkg == replacee_apkg)
		    return 1;
	  }
     }
     return 0;
}
#endif

/**
 * pkg_provides_abstract returns 1 if pkg->provides contains providee
 * and 0 otherwise.
 */
int pkg_provides_abstract(pkg_t *pkg, abstract_pkg_t *providee)
{
     abstract_pkg_t **provides = pkg->provides;
     int provides_count = pkg->provides_count;
     int i;
     for (i = 0; i < provides_count; i++) {
	  if (provides[i] == providee)
	       return 1;
     }
     return 0;
}

/**
 * pkg_replaces returns 1 if pkg->replaces contains one of replacee's provides and 0
 * otherwise.
 */
int pkg_replaces(pkg_t *pkg, pkg_t *replacee)
{
     abstract_pkg_t **replaces = pkg->replaces;
     int replaces_count = pkg->replaces_count;
     /* abstract_pkg_t **replacee_provides = pkg->provides;
     int replacee_provides_count = pkg->provides_count; */
     int i, j;
     for (i = 0; i < replaces_count; i++) {
	  abstract_pkg_t *abstract_replacee = replaces[i];
	  for (j = 0; j < replaces_count; j++) {
   /*            ipkg_message(NULL, IPKG_DEBUG2, "Searching pkg-name %s repprovname %s absrepname %s \n",
                 pkg->name,replacee->provides[j]->name, abstract_replacee->name); */
	       if (replacee->provides[j] == abstract_replacee)
		    return 1;
	  }
     }
     return 0;
}


/**
 * pkg_conflicts_abstract returns 1 if pkg->conflicts contains conflictee and 0
 * otherwise.
 */
int pkg_conflicts_abstract(pkg_t *pkg, abstract_pkg_t *conflictee)
{
     compound_depend_t *conflicts = pkg->conflicts;
     int conflicts_count = pkg->conflicts_count;
     int i, j;
     for (i = 0; i < conflicts_count; i++) {
	  int possibility_count = conflicts[i].possibility_count;
	  struct depend **possibilities = conflicts[i].possibilities;
	  for (j = 0; j < possibility_count; j++) {
	       if (possibilities[j]->pkg == conflictee) {
		    return 1;
	       }
	  }
     }
     return 0;
}

/**
 * pkg_conflicts returns 1 if pkg->conflicts contains one of
 * conflictee's provides and 0 otherwise.
 */
int pkg_conflicts(pkg_t *pkg, pkg_t *conflictee)
{
     compound_depend_t *conflicts = pkg->conflicts;
     int conflicts_count = pkg->conflicts_count;
     abstract_pkg_t **conflictee_provides = conflictee->provides;
     int conflictee_provides_count = conflictee->provides_count;
     int i, j, k;
     int possibility_count;
     struct depend **possibilities;
     abstract_pkg_t *possibility ;

     for (i = 0; i < conflicts_count; i++) {
	  possibility_count = conflicts[i].possibility_count;
	  possibilities = conflicts[i].possibilities;
	  for (j = 0; j < possibility_count; j++) {
	       possibility = possibilities[j]->pkg;
	       for (k = 0; k < conflictee_provides_count; k++) {
		    if (possibility == conflictee_provides[k]) {
			 return 1;
		    }
	       }
	  }
     }
     return 0;
}

static char ** merge_unresolved(char ** oldstuff, char ** newstuff)
{
    int oldlen = 0, newlen = 0;
    char ** result;
    register int i, j;

    if(!newstuff)
	return oldstuff;
    
    while(oldstuff && oldstuff[oldlen]) oldlen++;
    while(newstuff && newstuff[newlen]) newlen++;
    
    result = (char **)realloc(oldstuff, sizeof(char *) * (oldlen + newlen + 1));
    if (result == NULL) {
        fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
        return NULL;
    }
    
    for(i = oldlen, j = 0; i < (oldlen + newlen); i++, j++)
	*(result + i) = *(newstuff + j);
    
    *(result + i) = NULL;

    return result;
}
    
/* 
 * a kinda kludgy way to back out depends str from two different arrays (reg'l'r 'n pre)
 * this is null terminated, no count is carried around 
 */
char ** add_unresolved_dep(pkg_t * pkg, char ** the_lost, int ref_ndx)
{
    int count;
    char ** resized;
    char *depend_str = pkg_depend_str(pkg, ref_ndx);

    count = 0;
    while(the_lost && the_lost[count]) count++;

    count++;  /* need one to hold the null */
    resized = (char **)realloc(the_lost, sizeof(char *) * (count + 1));
    if (resized == NULL) {
       fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
       return NULL;
    }
    resized[count - 1] = strdup(depend_str);
    resized[count] = NULL;
    
    return resized;
}
	
void printDepends(pkg_t * pkg)
{
    register int i, j;
    compound_depend_t * depend;
    int count;
    
    count = pkg->pre_depends_count + pkg->depends_count;
    
    depend = pkg->depends;
    if(!depend){
	fprintf(stderr, "Depends pointer is NULL\n");
	return;
    }
    for(i = 0; i < count; i++){
	fprintf(stderr, "%s has %d possibilities:\n", 
		(depend->type == GREEDY_DEPEND) ? "Greedy-Depend" : ((depend->type == DEPEND) ? "Depend" : "Pre-Depend"),
		depend->possibility_count);
	for(j = 0; j < depend->possibility_count; j++)
	    fprintf(stderr, "\t%s version %s (%d)\n",
		    depend->possibilities[j]->pkg->name,
		    depend->possibilities[j]->version,
		    depend->possibilities[j]->constraint);
	depend++;
    }
}

int buildProvides(hash_table_t * hash, abstract_pkg_t * ab_pkg, pkg_t * pkg)
{
    register int i, j;

    /* every pkg provides itself */
    abstract_pkg_vec_insert(ab_pkg->provided_by, ab_pkg);

    if (!pkg->provides_count)
      return 0;

    pkg->provides = (abstract_pkg_t **)malloc(sizeof(abstract_pkg_t *) * (pkg->provides_count + 1));
    if (pkg->provides == NULL) {
       fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
       return -1 ;
    }
    pkg->provides[0] = ab_pkg;

    // if (strcmp(ab_pkg->name, pkg->name))
    //     fprintf(stderr, __FUNCTION__ ": ab_pkg=%s pkg=%s\n", ab_pkg->name, pkg->name);

    for(i = 0; i < pkg->provides_count; i++){
	 abstract_pkg_t *provided_abpkg = ensure_abstract_pkg_by_name(hash, pkg->provides_str[i]);

	pkg->provides[i+1] = provided_abpkg;

	j = 0;
	abstract_pkg_vec_insert(provided_abpkg->provided_by, ab_pkg);
    }
    return 0;
}

/* Abhaya: added conflicts support */
int buildConflicts(hash_table_t * hash, abstract_pkg_t * ab_pkg, pkg_t * pkg)
{
    register int i;
    compound_depend_t * conflicts;

    if (!pkg->conflicts_count)
	return 0;

    conflicts = pkg->conflicts = malloc(sizeof(compound_depend_t) *
					pkg->conflicts_count);
    if (conflicts == NULL) {
       fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
       return  -1;
    }
    for (i = 0; i < pkg->conflicts_count; i++) {
	 conflicts->type = CONFLICTS;
	 parseDepends(conflicts, hash,
		      pkg->conflicts_str[i]);
#if 0
	 for (j = 0; j < conflicts->possibility_count; j++) {
	      depend_t *possibility = conflicts->possibilities[j];
	      abstract_pkg_t *conflicting_apkg = possibility->pkg;
	      pkg_add_conflict_pair(ab_pkg, conflicting_apkg);
	 }
#endif
	 conflicts++;
    }
    return 0;
}

int buildReplaces(hash_table_t * hash, abstract_pkg_t * ab_pkg, pkg_t * pkg)
{
     register int i, j;

     if (!pkg->replaces_count)
	  return 0;

     pkg->replaces = (abstract_pkg_t **)malloc(sizeof(abstract_pkg_t *) * pkg->replaces_count);
     if (pkg->replaces == NULL) {
        fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
        return  -1;
     }

     // if (strcmp(ab_pkg->name, pkg->name))
     //     fprintf(stderr, __FUNCTION__ ": ab_pkg=%s pkg=%s\n", ab_pkg->name, pkg->name);

     for(i = 0; i < pkg->replaces_count; i++){
	  abstract_pkg_t *old_abpkg = ensure_abstract_pkg_by_name(hash, pkg->replaces_str[i]);

	  pkg->replaces[i] = old_abpkg;

	  j = 0;
	  if (!old_abpkg->replaced_by)
	       old_abpkg->replaced_by = abstract_pkg_vec_alloc();
               if ( old_abpkg->replaced_by == NULL ){
                  return -1;
               }
	  /* if a package pkg both replaces and conflicts old_abpkg,
	   * then add it to the replaced_by vector so that old_abpkg
	   * will be upgraded to ab_pkg automatically */
	  if (pkg_conflicts_abstract(pkg, old_abpkg))
	       abstract_pkg_vec_insert(old_abpkg->replaced_by, ab_pkg);
     }
     return 0;
}

int buildDepends(hash_table_t * hash, pkg_t * pkg)
{
     int count;
     register int i;
     compound_depend_t * depends;

     if(!(count = pkg->pre_depends_count + pkg->depends_count + pkg->recommends_count + pkg->suggests_count))
	  return 0;

     if (0 && pkg->pre_depends_count)
	  fprintf(stderr, "pkg=%s pre_depends_count=%d depends_count=%d\n", 
		  pkg->name, pkg->pre_depends_count, pkg->depends_count);
     depends = pkg->depends = malloc(sizeof(compound_depend_t) * count);
     if (depends == NULL) {
        fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
        return  -1;
     }
     

     for(i = 0; i < pkg->pre_depends_count; i++){
	  parseDepends(depends, hash, pkg->pre_depends_str[i]);
	  if (0 && pkg->pre_depends_count)
	       fprintf(stderr, " pre_depends_str=%s depends=%p possibility_count=%x\n", 
		       pkg->pre_depends_str[i], depends, depends->possibility_count);
	  depends->type = PREDEPEND;
	  depends++;
     }

     for(i = 0; i < pkg->recommends_count; i++){
	  parseDepends(depends, hash, pkg->recommends_str[i]);
	  if (0 && pkg->recommends_count)
	       fprintf(stderr, " recommends_str=%s depends=%p possibility_count=%x\n", 
		       pkg->recommends_str[i], depends, depends->possibility_count);
	  depends->type = RECOMMEND;
	  depends++;
     }

     for(i = 0; i < pkg->suggests_count; i++){
	  parseDepends(depends, hash, pkg->suggests_str[i]);
	  if (0 && pkg->suggests_count)
	       fprintf(stderr, " suggests_str=%s depends=%p possibility_count=%x\n", 
		       pkg->suggests_str[i], depends, depends->possibility_count);
	  depends->type = SUGGEST;
	  depends++;
     }

     for(i = 0; i < pkg->depends_count; i++){
	  parseDepends(depends, hash, pkg->depends_str[i]);
	  if (0 && pkg->depends_count)
	       fprintf(stderr, " depends_str=%s depends=%p possibility_count=%x\n",
		       pkg->depends_str[i], depends, depends->possibility_count);
	  depends++;
     }
     return 0;
}    

/*
 * pkg_depend_string: returns the depends string specified by index.
 *   All 4 kinds of dependences: dependence, pre-dependence, recommend, and suggest are number starting from 0.
 *   [0,npredepends) -> returns pre_depends_str[index]
 *   [npredepends,npredepends+nrecommends) -> returns recommends_str[index]
 *   [npredepends+nrecommends,npredepends+nrecommends+nsuggests) -> returns recommends_str[index]
 *   [npredepends+nrecommends+nsuggests,npredepends+nrecommends+nsuggests+ndepends) -> returns depends_str[index]
 */
char *pkg_depend_str(pkg_t *pkg, int pkg_index)
{
     if (pkg_index < pkg->pre_depends_count) {
	  return pkg->pre_depends_str[pkg_index];
     }
     pkg_index -= pkg->pre_depends_count;

     if (pkg_index < pkg->recommends_count) {
	  return pkg->recommends_str[pkg_index];
     }
     pkg_index -= pkg->recommends_count;

     if (pkg_index < pkg->suggests_count) {
	  return pkg->suggests_str[pkg_index];
     }
     pkg_index -= pkg->suggests_count;

     if (pkg_index < pkg->depends_count) {
	  return pkg->depends_str[pkg_index];
     }
     fprintf(stderr, "pkg_depend_str: index %d out of range for pkg=%s\n", pkg_index, pkg->name);
     return NULL;
}

void freeDepends(pkg_t *pkg)
{
    int i;

    if (pkg == NULL || pkg->depends == NULL) {
	return;
    }

    fprintf(stderr, "Freeing depends=%p\n", pkg->depends);
    for (i=0; i < pkg->depends->possibility_count; i++) {
	depend_deinit(pkg->depends->possibilities[i]);
    }
    free(pkg->depends->possibilities);
    free(pkg->depends);
    pkg->depends = NULL;
}

void buildDependedUponBy(pkg_t * pkg, abstract_pkg_t * ab_pkg)
{
     compound_depend_t * depends;
     int count, othercount;
     register int i, j;
     abstract_pkg_t * ab_depend;
     abstract_pkg_t ** temp;

     count = pkg->pre_depends_count + pkg->depends_count;
     depends = pkg->depends;

	  if (0 && pkg->pre_depends_count)
	       fprintf(stderr, "pkg=%s pre_depends_count=%d depends_count=%d\n",
		       pkg->name, pkg->pre_depends_count, pkg->depends_count);
     for (i = 0; i < count; i++) {
	  if (0 && pkg->pre_depends_count)
	       fprintf(stderr, "  i=%d possibility_count=%x depends=%p\n", i, depends->possibility_count, depends);
	  for (j = 0; j < depends->possibility_count; j++){
	       ab_depend = depends->possibilities[j]->pkg;
	       if(!ab_depend->depended_upon_by)
		    ab_depend->depended_upon_by = (abstract_pkg_t **)calloc(1, sizeof(abstract_pkg_t *));

	       temp = ab_depend->depended_upon_by;
	       othercount = 1;
	       while(*temp){
		    temp++;
		    othercount++;
	       }
	       *temp = ab_pkg;

	       ab_depend->depended_upon_by = (abstract_pkg_t **)realloc(ab_depend->depended_upon_by, 
									(othercount + 1) * sizeof(abstract_pkg_t *));
	       /* the array may have moved */
	       temp = ab_depend->depended_upon_by + othercount;
	       *temp = NULL;
	  }
	  depends++;
     }
}

static depend_t * depend_init(void)
{
    depend_t * d = (depend_t *)malloc(sizeof(depend_t));    
    if ( d==NULL ){
        fprintf(stderr, "%s: out of memory\n", __FUNCTION__);
        return NULL; 
     }
    d->constraint = NONE;
    d->version = NULL;
    d->pkg = NULL;
    
    return d;
}

static void depend_deinit(depend_t *d)
{
    free(d);
}

static int parseDepends(compound_depend_t *compound_depend, 
			hash_table_t * hash, char * depend_str)
{
     char * pkg_name, buffer[2048];
     int num_of_ors = 0;
     register int i;
     register char * src, * dest;
     depend_t ** possibilities;

     /* first count the number of ored possibilities for satisfying dependency */
     src = depend_str;
     while(*src)
	  if(*src++ == '|')
	       num_of_ors++;

     compound_depend->type = DEPEND;

     compound_depend->possibility_count = num_of_ors + 1;
     possibilities = (depend_t **)malloc(sizeof(depend_t *) * (num_of_ors + 1));
     if (!possibilities)
	  return -ENOMEM;
     compound_depend->possibilities = possibilities;

     src = depend_str;
     for(i = 0; i < num_of_ors + 1; i++){
	  possibilities[i] = depend_init();
          if (!possibilities[i])
	       return -ENOMEM;
	  /* gobble up just the name first */
	  dest = buffer;
	  while(*src &&
		!isspace(*src) &&
		(*src != '(') &&
		(*src != '*') &&
		(*src != '|'))
	       *dest++ = *src++;
	  *dest = '\0';
	  pkg_name = trim_alloc(buffer);
          if (pkg_name == NULL )
	       return -ENOMEM;
	
	  /* now look at possible version info */
	
	  /* skip to next chars */
	  if(isspace(*src))
	       while(*src && isspace(*src)) src++;

	  /* extract constraint and version */
	  if(*src == '('){
	       src++;
	       if(!strncmp(src, "<<", 2)){
		    possibilities[i]->constraint = EARLIER;
		    src += 2;
	       }
	       else if(!strncmp(src, "<=", 2)){
		    possibilities[i]->constraint = EARLIER_EQUAL;
		    src += 2;
	       }
	       else if(!strncmp(src, ">=", 2)){
		    possibilities[i]->constraint = LATER_EQUAL;
		    src += 2;
	       }
	       else if(!strncmp(src, ">>", 2)){
		    possibilities[i]->constraint = LATER;
		    src += 2;
	       }
	       else if(!strncmp(src, "=", 1)){
		    possibilities[i]->constraint = EQUAL;
		    src++;
	       }
	       /* should these be here to support deprecated designations; dpkg does */
	       else if(!strncmp(src, "<", 1)){
		    possibilities[i]->constraint = EARLIER_EQUAL;
		    src++;
	       }
	       else if(!strncmp(src, ">", 1)){
		    possibilities[i]->constraint = LATER_EQUAL;
		    src++; 
	       }

	       /* now we have any constraint, pass space to version string */
	       while(isspace(*src)) src++;

	       /* this would be the version string */
	       dest = buffer;
	       while(*src && *src != ')')
		    *dest++ = *src++;
	       *dest = '\0';
	    
	       possibilities[i]->version = trim_alloc(buffer);
	       /*	   	    fprintf(stderr, "let's print the depends version string:");
				    fprintf(stderr, "version %s\n", possibilities[i]->version);*/
               if (possibilities[i]->version == NULL )
	            return -ENOMEM;

	 
	  }
	  /* hook up the dependency to its abstract pkg */
	  possibilities[i]->pkg = ensure_abstract_pkg_by_name(hash, pkg_name);

	  free(pkg_name);
	
	  /* now get past the ) and any possible | chars */
	  while(*src &&
		(isspace(*src) ||
		 (*src == ')') ||
		 (*src == '|')))
	       src++;
	  if (*src == '*')
	  {
	       compound_depend->type = GREEDY_DEPEND;
	       src++;
	  }
     }

     return 0;
}
