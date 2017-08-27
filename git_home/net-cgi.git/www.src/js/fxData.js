/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
////////////////////////////////////////////////////////////////////////
/*
    Class: fxData

    Version:   1.0

    get data from a url with or without a poll routine
*/
////////////////////////////////////////////////////////////////////////

function fxData(strURL)
{
    var self = this;

    /////////////////////////////////////////////////////////////////////////
    /*
        The magic to load our pre-requisite scripts in javascript
    */
    /////////////////////////////////////////////////////////////////////////
    //load the translation library
    loadScript("fxTranslate-2.0.js");

    //tell the trans library to look here and load the appropriate files.
    loadTranslation();

    loadScript("utility-2.0.js");

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: initialize

        initialize the data object with a url to get the data from

        Parameters:
          strURL - url to get data from

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.initialize = function(strURL)
    {
        //init any variables incase this is double called
        self.strURL = "";
        self.bPolling = false;
        self.nPollDelay = 1000;
        self.pCallback = null;

        //set the url
        self.setURL(strURL);

        //stop polling just in case
        self.stopPoll();
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: setURL

        set the url the data will come from

        Parameters:
          strURL - url to get data from

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.setURL = function(strURL)
    {
        self.strURL = strURL;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: setURL

        set the url the data will come from

        Parameters:
          strURL - url to get data from

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.getURL = function()
    {
        return self.strURL;
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: destroy

        stop this class from doing everything and get in ready to be deleted

        Parameters:
          none.

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.destroy = function()
    {
        self.stopPoll();

        //init any other fun stuff
        self.strURL = "";
        self.bPolling = false;
        self.nPollDelay = 1000;
        self.pCallback = null;
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getData

        get data from the previously set data source

        Parameters:
          bShowAlert - if true errors will show a user level alert

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.getData = function(bShowAlert)
    {
        var results = null;

        var strURL = self.strURL;

        if(self.strFilter != "")
        {
            strURL += "?"+self.strFilter;
        }
        //get the data syncronously
        $.ajax(
                {
                    type: "GET",
                    async: false,
                    url: strURL,
                    dataType: "json",
                    cache: false,
                    timeout: 3000,
                    //-------------------------------------------------------------------
                    // Issue callback with result data
                    success: function(data, status, xhr)
                    {
                        results = data;

                        //calc offset start and stop for
                        //the moment of this sample
                        calculateTime();
                    },
                    error: function(data,status)
                    {
                        //if they want us to show an alert
                        if(bShowAlert != undefined && bShowAlert == true)
                        {
                            alert(_t("Unable to get requested data. Please check your connection to your router."));
                        }
                    }
                });

        //return results to the user
        return results;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: startPoll

        get data from the previously set data source

        Parameters:
        nTimeout    - time out for the poll
        pCallback   - callback to give data to for the polling

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.startPoll = function(  nTimeout,
                                pCallback)
    {
        var bReturn = false;

        //if this hasnt started
        if(self.bPolling == false)
        {
            //tell the poll routine to start
            self.bPolling = true;

            //set timeout
            self.nTimeout = nTimeout;

            //set callback
            self.pCallback = pCallback;

            //start the timeout routine itself now
            window.setTimeout(self.pollRoutine,self.nTimeout);

            //set the return to good
            bReturn = true;
        }

        return bReturn;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: stopPoll

        stop a previously running poll timer

        Parameters:
        none.

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.stopPoll = function()
    {
        var bReturn = false;

        //if this hasnt started
        if(self.bPolling == true)
        {
            //tell the poll routine to stop
            self.bPolling = false;

            //set the return to good
            bReturn = true;
        }

        return bReturn;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: pollRoutine

        Internal poll routine that does the update of the data and call
        the user supplied callback with the results.

        Parameters:
        none.

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.pollRoutine = function()
    {
        var results = null;

        //get the data
        results = self.getData();

        //if we have a call back
        if(self.pCallback != undefined && self.pCallback != null)
        {
            //call requested user function with the data
            self.pCallback(results);
        }

        //if we are still running
        if(self.bPolling == true)
        {
            //rearm
            window.setTimeout(self.pollRoutine,self.nTimeout);
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: isPolling

        Is a poll timer running?

        Parameters:
        none.

        Returns:
          true  - running
          false - not running

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.isPolling = function()
    {
        return self.bPolling;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: setFilter

        Internal use: sets a filter that gets appended to an API data call

        Parameters:
        strFilter - string based filter to append to  api call

        Returns:
          nothing.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.setFilter = function(strFilter)
    {
        self.strFilter = strFilter;
    }

    /*
        Variable: strURL
        url to get data from
    */
    this.strURL = "";

    /*
        Variable: bPolling
        Are we polling? Global boolean of whether or not the
        poll routine is running.
    */
    this.bPolling = false;

    /*
        Variable: nPollDelay
        Poll time default to 1 sec. This is the delay between
        poll calls.
    */
    this.nPollDelay = 1000;

    /*
        Variable: pCallback
        callback for the update poll so the user can process the data
    */
    this.pCallback = null;

    /*
        Variable: strFilter
        filter to append to the end of the get data calls
    */
    this.strFilter = "";


    //if they passed in parameters on the construction of this
    //objec then call initialize
    if( strURL != undefined)
    {
        //initialize ourself
        self.initialize(strURL);
    }
}
