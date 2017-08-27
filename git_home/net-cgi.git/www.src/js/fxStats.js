/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
/*
    Variable: fxType
    Filter type enum for the getFlows routine

    bytesDL - bytes downloaded
    bytesUL - bytes uploaded
    time    - open time of flows
    bpsDL   - bytes per second downloaded
    bpsUL   - bytes per second uploaded
    events  - all events
*/
var fxType = {
  bytesDL : {value: 1 },
  bytesUL:  {value: 2 },
  time:     {value: 3 },
  bpsDL:    {value: 4 },
  bpsUL:    {value: 5 },
  events:   {value: 6 }
};

////////////////////////////////////////////////////////////////////////
/*
    Class: dbOverSub

    Version:   1.0

    Oversub data inside the vOversub array in a dbItem class
*/
////////////////////////////////////////////////////////////////////////
function dbOverSub()
{
    /*
        Variable: time
        time of strata change
    */
    var time = null;

    /*
        Variable: strata
        current strata #
    */
    var strata = null;
}


////////////////////////////////////////////////////////////////////////
/*
    Class: dbItem

    Version:   1.0

    Item returned from a stat
*/
////////////////////////////////////////////////////////////////////////
function dbItem()
{

    /*
        Variable: strName
        Name of the item
    */
    var strName = "";

    /*
        Variable: strName
        Name of the item
    */
    var strDisplayName = "";

    /*
        Variable: nUpload
        bytes uploaded
    */
    var nUpload = 0;

    /*
        Variable: nDownload
        bytes downloaded
    */
    var nDownload = 0;

    /*
        Variable: timeOpen
        time the connection opened
    */
    var timeOpen = null;

    /*
        Variable: timeClose
        time the connection closed
    */
    var timeClose = null;

    /*
        Variable: strata
        current strata of the item
    */
    var strata;

    /*
        Variable: vOverSub
        array of dbOverSub changes
    */
    var vOverSub = null;
}

////////////////////////////////////////////////////////////////////////
/*
    Class: fxStats

    Version:   1.0

    get stats from the router and parse them into a usable format
*/
////////////////////////////////////////////////////////////////////////

function fxStats()
{
    /////////////////////////////////////////////////////////////////////////
    /*
        The magic to load our pre-requisite scripts in javascript
    */
    /////////////////////////////////////////////////////////////////////////
    loadScript("utility-2.0.js");
    loadScript("Page_AppEvents-2.0.js");

    var self = this;

    //global end time of the graphs
    //usually time that the sample is taken
    self.routerEndTime = 0;

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getFlows

        Get events filtered by  flows.

        Parameters:
        nType       - what type of flow data? bytesDL/bytesUL/time/bpsDL/bpsUL
        vFilter     - flow id or node mac to filter on
        dateStart   - beginning time to filter on
        dateEnd     - end time to filter on

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.getFlows = function(   nType,
                                vFilter,
                                dateStart,
                                dateEnd)
    {
        var vReturn = [];

        //set the filter for this
        self.setFilter( dateStart,
                        dateEnd,
                        vFilter,
                        nType);

        var vData = self.getData();

        //if we got back valid data from the routine
        if(vData != undefined && vData != null &&  getArrayLength(vData) > 0)
        {
            if(nType == fxType.bytesDL)         //if this is a bytes download type
            {
                vReturn = self.getBytesDL(vData);
            }
            else if(nType == fxType.bytesUL)    //if this is a bytes download type
            {
                vReturn = self.getBytesUL(vData);
            }
            else if(nType == fxType.time)
            {
                vReturn = self.getByTime(vData);
            }
        }
        else
        {
            vData = [];
        }

        return vReturn;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getNodes

        Get events filtered by  nodes. <doesnt work yet>

        Parameters:
        nType       - what type of flow data? bytesDL/bytesUL/time/bpsDL/bpsUL
        vFilter     - flow id or node mac to filter on
        dateStart   - beginning time to filter on
        dateEnd     - end time to filter on

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.getNodes = function(   nType,
                                vFilter,
                                dateStart,
                                dateEnd)
    {
        //set the filter for this
        self.setFilter( dateStart,
                        dateEnd,
                        vFilter,
                        nType);
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getEvents

        get the raw events (node/flow/times/bytes/all)

        Parameters:
        vFilter     - flow id or node mac to filter on
        dateStart   - beginning time to filter on
        dateEnd     - end time to filter on

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    var saveflows = {};
    this.getEvents = function(  vFilter,
                                dateStart,
                                dateEnd)
    {
        var vReturn = [];

        var strData = "";

        //set the filter for this
        self.setFilter( dateStart,
                        dateEnd,
                        vFilter,
                        fxType.events); // type is not used in the filter functions!

         var sDate = new Date();
         var eDate = new Date();

         var strUrl = g_path.ozker+"/api/events";
         saveflows = {};

         $.ajax({
                type: "GET",
                async: false,
                cache: false,
                url: strUrl,
                dataType: "text",
                timeout: 3000,
                //-------------------------------------------------------------------
                // Issue callback with result data
                success: function(data, status, xhr)
                {
                    strData = data;

                    //calc offset start and stop for
                    //the moment of this sample
                    self.routerEndTime = calculateTime();

                    //convert the data to a usable format
                    var vData = CSVToArray(strData);
                    if (dateStart || dateEnd || vFilter) {
                        vReturn = self.getFilteredEvents(vData);
                    }
                    else vReturn = vData;

                },
                error: function(data,status)
                {
                    alert(_t("Unable to get requested data. Please check your connection to your router."));
                }
         });

        return vReturn;

    }

    ///////////////////////////////////////
    //internal used on calls

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: setFilter

        internal used on calls to set the filter for the getData function

        Parameters:
        dateStart   - startime of events to get
        dateEnd     - endtime of events to get
        vFilter     - list of nodes macs/flows ids to get
        nType       - see fxType for stat types

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.setFilter = function(  dateStart,
                                dateEnd,
                                vFilter,
                                nType)
    {
        self.dateStart = dateStart;
        self.dateEnd = dateEnd;
        self.vFilter = vFilter;
        self.nType = nType;
        //if we don't have a start time
        if(dateStart == undefined)
        {
            //make an old date
            self.dateStart = new Date(2000, //year
                                        1,      //month
                                        1,      //day
                                        1,      //hour
                                        0,      //minutes
                                        0,      //seconds
                                        0);     //milliseconds
        }

        //if we don't have a filter
        if(vFilter == undefined )
        {
            //create an empty filter
            self.vFilter = [];
        }

        //if we don't have a type
        if(nType == undefined)
        {
            //default to all events
            self.nType = fxType.events;
        }
    }

    /*
        Variable: dateStart
        start date for this stat
    */
    this.dateStart = null;

    /*
        Variable: dateEnd
        end date for this stat
    */
    this.dateEnd = null;

    /*
        Variable: vFilter
        list of filters for this stat
        device macs/flows id's
    */
    this.vFilter = null;

    /*
        Variable: nType
        type of filter
        see fxType for event types
    */
    this.nType = null;

    this.isRawFilteredOut = function(arData) {
        if (saveflows[arData[2]]) {
            // if enddate is > self.dateEnd then truncate?
            // if startdate is < self.dateStart then trucate?
            delete saveflows[arData[2]];
            return false;
        }
        var strTime = (arData[0]).replace(/\s+/g, ' ').replace(/\s+/g, '\n').replace(/\s+/g, '\r')
        var vTime = strTime.split(".");
        var dattime = new Date(parseInt(vTime)*1000 + g_routerDelta);
        var datmac = arData[4];
        if (self.dateStart && (dattime < self.dateStart)) return true;
        if (self.dateEnd && (dattime > self.dateEnd)) return true;
        if (self.vFilter && self.vFilter.length) {
            var fmac;
            for (var f in self.vFilter) {
                fmac = self.vFilter[f];
                if (fmac == datmac) {
                    saveflows[arData[2]] = true;
                    break;
                }
            }
            if (fmac != datmac) return true;
        }
        return false;
    }

    // check the filters (self.vFilter, self.dateStart, self.dateEnd, self.nType)
    this.isFilteredOut = function(dat) {
        if (self.dateStart && (dat.timeOpen < self.dateStart)) return true;
        if (self.dateEnd && (dat.timeClose > self.dateEnd)) return true;
        if (self.vFilter && self.vFilter.length) {
            var fmac;
            for (var f in self.vFilter) {
                fmac = self.vFilter[f];
                if (fmac == dat.mac) break;
            }
            if (fmac != dat.mac) return true;
        }
        return false;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getData

        internal function to get flows data

        Parameters:
          none.

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.getData = function()
    {
         var strData = "";

         var sDate = new Date();//$( "#topFrom" ).datepicker( "getDate" );
         var eDate = new Date();//$( "#topTo" ).datepicker( "getDate" );

         var strUrl = g_path.ozker+"/api/events";

         $.ajax({
                type: "GET",
                async: false,
                cache: false,
                url: strUrl,
                dataType: "text",
                timeout: 3000,
                //-------------------------------------------------------------------
                // Issue callback with result data
                success: function(data, status, xhr)
                {
                    strData = data;

                    //calc offset start and stop for
                    //the moment of this sample
                    self.routerEndTime = calculateTime();
                },
                error: function(data, status)
                {
                    alert(_t("Unable to get requested data. Please check your connection to your router."));
                }
         });

        //if we didnt get any data
        //return nothing
        if(strData == "")
            return [];

        var arData = CSVToArray(strData);

        //temp data ie opened but not closed
        var vTemp = [];

        //finished data closed all done up
        var vDone = [];

        self.maxFlowTime = 0;

        // arData is by time
        // vTemp is by id
        // vDone is a queue
        // vReturn is by flow id

        //walk the array of arrays
        // skip the header line
        for(var index = 1;index<arData.length;index++)
        {
            //get the uid
            var nRealID = arData[index][2];

            var nID = "id_"+nRealID;

            if(arData[index][3]!="default")
            {
                //if this is an open
                if(arData[index][1]=="open")
                {
                    var strName = arData[index][3];
                    var strTime = (arData[index][0]).replace(/\s+/g, ' ').replace(/\s+/g, '\n').replace(/\s+/g, '\r')
                    var vTime = strTime.split(".");

                    //if this doesnt exist
                    if(vTemp[nID] == undefined)
                    {
                        //create and add the new entry
                        vTemp[nID] = new dbItem;
                        vTemp[nID].strName = strName;
                        vTemp[nID].strDisplayName = flowName(strName);
                        vTemp[nID].mac = arData[index][4];

                        vTemp[nID].uprate = arData[index][8];
                        vTemp[nID].downrate = arData[index][10];

                        vTemp[nID].timeOpen = new Date(parseInt(vTime[0])*1000 + g_routerDelta);

                        //set the earliest starttime
                        if(self.startTime == null)
                        {
                            self.startTime = vTemp[nID].timeOpen;
                        }
                        else if(self.startTime > vTemp[nID].timeOpen)
                        {
                            self.startTime = vTemp[nID].timeOpen;
                        }
                    }
                    else
                    {
                        //alert("double ID!!");

                    }
                }
                else if(arData[index][1]=="close") // if this is closed
                {
                    //if this doesnt exist
                    if(vTemp[nID] != undefined)
                    {
                        var strTime = (arData[index][0]).replace(/\s+/g, ' ').replace(/\s+/g, '\n').replace(/\s+/g, '\r')
                        var vTime = strTime.split(".");

                        vTemp[nID].nUpload   = parseInt(arData[index][3]);
                        vTemp[nID].nDownload = parseInt(arData[index][4]);

                        vTemp[nID].timeClose = new Date(parseInt(vTime[0])*1000 + g_routerDelta);

                        // overlapped open and close for the same uid is not possible
                        if(self.endTime == null)
                        {
                            self.endTime = vTemp[nID].timeClose;
                        }
                        else if(self.endTime < vTemp[nID].timeClose)
                        {
                            self.endTime = vTemp[nID].timeClose;
                        }

                        //put this is the done category
                        vDone.push(vTemp[nID]);

                        //delete it out the temp data
                        delete vTemp[nID];
                    }
                    else
                    {
                        //alert("close no open");
                    }
                }
            }
        }

        //make sure all our flows/nodes have end times
        for(var item in vTemp)
        {
            vTemp[item].timeClose = new Date();
            var seconds = (vTemp[item].timeClose.getTime() - vTemp[item].timeOpen.getTime()) / 1000;
            // estimated data!
            vTemp[item].nDownload = Math.floor(vTemp[item].downrate * seconds);
            vTemp[item].nUpload = Math.floor(vTemp[item].uprate * seconds);

            //put this is the done category
            //vDone.push(vTemp[item]); don't add unclosed events!!!
        }

        //kill our temp objects
        vTemp = [];

        //combine all the events togather
        var vReturn = self.combineEvents(vDone);

        if(self.startTime == null)
        {
            self.startTime = new Date();

        }

        if(self.endTime == null)
        {
            self.endTime = new Date();

        }

        return vReturn;
    }


    //new data time for hand time ranges
    function timeRange()
    {
        timeOpen = null;
        timeClose = null;
    }

    this.getFilteredEvents = function(vData)
    {
        var vReturn = [];

        //1st extract the sequence for the give id
        for(var item in vData)
        {
            //if we don't filter this event
            if (vData[item].length && (!self.isRawFilteredOut(vData[item])))
            {
                vReturn.push(vData[item]);
            }
        }
        return vReturn;
    }
    ////////////////////////////////////////////////////////////////////////
    /*
        Function: combineEvents

        Take a list of Events with multiple instances of each flow id.
        Collapse them into one entry per flow id. Accummulate time
        totals, and up/down byte counts.

        Parameters:
          vData - list of events to collapse

        Returns:
            return a list of dbItems by indexed flow id

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.combineEvents = function(vData)
    {
        var vReturn = [];

        //1st extract the sequence for the give id
        for(var item in vData)
        {
            //if we don't filter this event
            if(!self.isFilteredOut(vData[item]))
            {
                //get the name of the flow
                var app = vData[item].strDisplayName;

                //if the flow isn't in our list
                if (!vReturn[app])
                {
                    vReturn[app] = vData[item];

                    vReturn[app].times = [];

                    vReturn[app].totalTime = 0;
                }
                else //if it is in our ist
                {
                    //get up and down bytes of current item
                    var nD = parseInt(vData[item].nDownload);
                    var nU = parseInt(vData[item].nUpload);

                    //if a valid value
                    if(isNaN(nD)==false)
                    {
                        //add down to our current item
                        vReturn[app].nDownload+=nD;
                    }

                    //if a valid value
                    if(isNaN(nU)==false)
                    {
                        //add up to our current item
                        vReturn[app].nUpload+=nU;
                    }
                }

                //create our time range object
                var range = new timeRange;

                //set time range
                range.timeOpen = vData[item].timeOpen.getTime();
                range.timeClose = vData[item].timeClose.getTime();

                //add the time range to our list
                vReturn[app].times = self.pushRange(vReturn[app].times,range);
            }
        }

        //now that the data is sequenctiar compute total times on return data
        for(var item in vReturn)
        {
            //add up the times for this app
            for(var n in vReturn[item].times)
            {
                vReturn[item].totalTime+=(vReturn[item].times[n].timeClose - vReturn[item].times[n].timeOpen);
            }
        }

        return vReturn;

    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: pushRange

        Add a new time range to the list of time ranges

        Parameters:
          vList     - list of timeRange class values to append
          newItem   - timeRange class value range

        Returns:
            a list of time ranges with the value added
            and overlapping time ranges merged in

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.pushRange = function(vList,newItem)
    {
        var vReturn = [];

        for(var item in vList)
        {
            var oldItem = vList[item];

            //newItem open before oldItem && newItem close inside of oldItem
            if( (newItem.timeOpen < oldItem.timeOpen) &&
                (newItem.timeClose < oldItem.timeClose && newItem.timeClose <= oldItem.timeClose))
            {
                //adjust our close to account
                newItem.timeClose = oldItem.timeClose;
            }
            //newItem open inside oldItem && newItem close after oldItem
            else if((newItem.timeOpen >= oldItem.timeOpen && newItem.timeOpen <= oldItem.timeClose) &&
                    (newItem.timeClose > oldItem.timeClose))
            {
                //adjust our open to account
                newItem.timeOpen = oldItem.timeOpen;
            }
            //newItem open inside of oldItem && 1 close inside of oldItem
            else if((newItem.timeOpen >= oldItem.timeOpen && newItem.timeOpen <= oldItem.timeClose) &&
                    (newItem.timeClose >= oldItem.timeOpen && newItem.timeClose <= oldItem.timeClose))
            {
                //adjust our open and close
                newItem.timeOpen = oldItem.timeOpen;
                newItem.timeClose = oldItem.timeClose;
            }
            //oldItem open inside of newItem && oldItem close inside of newItem
            else if((oldItem.timeOpen >= newItem.timeOpen && oldItem.timeOpen <= newItem.timeClose) &&
                    (oldItem.timeClose >= newItem.timeOpen && oldItem.timeClose <= newItem.timeClose))
            {
                continue;
            }
            else //the value doesn't interfere with anything
            {
                //just add it
                vReturn.push(oldItem);
            }
        }

        //finally after all add the new item
        vReturn.push(newItem);

        return vReturn;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getBytesDL

        internal function to get flows data of download bytes

        Parameters:
          none.

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.getBytesDL = function(vData)
    {
        var vReturn = [];

        //vReturn.push(["Flow Name", "Bytes Downloaded","Tool Tip"]);

        //walk the list
        for(var i in vData)
        {
            //get name
            var strName = flowName(vData[i].strName);
            var nDL = 0;

            //if we have a valid download (don't show zeros)
            if(vData[i].nDownload)
            {
                nDL = vData[i].nDownload;
                //make our list
                vReturn.push([strName,B2Megabytes(nDL)]);
            }
        }

        return vReturn;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getBytesUL

        internal function to get flows data of upload bytes

        Parameters:
          none.

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.getBytesUL = function(vData)
    {
        var vReturn = [];

        vReturn.push([_t("Flow Name"), _t("Bytes Downloaded")]);

        //walk the flows
        for(var i in vData)
        {
            //get name
            var strName = flowName(vData[i].strName);
            var nUL = 0;

            //if we have a valid upload (don't show zeros)
            if(vData[i].nUpload)
            {
                nUL = vData[i].nUpload;
                //make our list
                vReturn.push([strName,nUL]);
            }

        }
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getByTime

        internal function to get flows by length of time opens

        Parameters:
          none.

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.getByTime = function(vData)
    {
        var vReturn = [];

        vReturn.push([_t("Flow Name"), _t("Minutes")]);

        //walk the list and get the times
        for(var i in vData)
        {
            var strName = flowName(i);

            // pre-calculated by adding all times for the flowName
            var nTime = (vData[i].totalTime/1000)/60; // these are all in minutes
/*
            //get start and end time
            var nStart  = (vData[i].timeOpen.getTime()/1000)/60;
            var nEnd    = (vData[i].timeClose.getTime()/1000)/60;

            //calculate length of flows time
            var nTime = nEnd - nStart;
*/

            //make our list
            vReturn.push([strName,nTime]);
        }

        return vReturn;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: init

        Initialize the data class and prime it to do work

        Parameters:
          callback - the function to call when this is ready

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.init = function(callback)
    {
        //we have to load the flow table id list before
        //we can draw
        loadFlowTable(function()
        {
            if(callback != undefined && callback != null)
            {
                callback();
            }
        });
    }
}
