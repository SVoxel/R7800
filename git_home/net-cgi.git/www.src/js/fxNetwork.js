/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
// fix IE8!
// implemented from ecma spec 15.4.4.14 http://ecma262-5.com/ELS5_HTML.htm#Section_15.4.4.14

/*
When the indexOf method is called with one or two arguments, the following steps are taken:
Let O be the result of calling ToObject passing the this value as the argument.
Let lenValue be the result of calling the [[Get]] internal method of O with the argument "length".
Let len be ToUint32(lenValue).
If len is 0, return -1.
If argument fromIndex was passed let n be ToInteger(fromIndex); else let n be 0.
If n ≥ len, return -1.
If n ≥ 0, then
Let k be n.
Else, n<0
Let k be len - abs(n).
If k is less than 0, then let k be 0.
Repeat, while k<len
Let kPresent be the result of calling the [[HasProperty]] internal method of O with argument ToString(k).
If kPresent is true, then
Let elementK be the result of calling the [[Get]] internal method of O with the argument ToString(k).
Let same be the result of applying the Strict Equality Comparison Algorithm to searchElement and elementK.
If same is true, return k.
Increase k by 1.
Return -1.
*/


if ($.browser.msie && $.browser.version <= 8) {
    Array.prototype.indexOf = function(elt /* , start */) {
        if (this.length === 0) return -1;
        var len = this.length;
        var n = Number(arguments[1]) ? Number(arguments[1]) : 0;
        if (Math.floor(n) >= len) return -1;
        var k = (n >=0) ? Math.floor(n) : len - Math.abs(Math.ceil(n));
        for (k = Math.max(k,0); k < len; k++) {
            var kstring = k.toString();
            if (this[kstring] && elt === this[kstring]) return k;
        }
        return -1;
    };
}

////////////////////////////////////////////////////////////////////////
/*
    Class: fxNetwork

    Version:   1.0

    the visual display for my network graph
*/
////////////////////////////////////////////////////////////////////////

function fxNetwork( strDivID,
                    pHandler,
                    pLoadIcon)
{
    var showNonActive = false;
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
    loadScript("raphael-min.js");
    loadScript("GraphItem-2.0.js");
    loadScript("NodeInfo-2.0.js");
    loadScript("Line-2.0.js");
    loadScript("Arrows-2.0.js");
    loadScript("IEArrows-2.0.js");

    this.pLoadIcon = pLoadIcon;

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: calcWidth

        Get / calculate the width of the display

        Parameters:
          none.

        Returns:
          new width of display

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.calcWidth = function()
    {
        return $("#"+self.m_sDiv).width()-64;
    }
    ////////////////////////////////////////////////////////////////////////
    /*
        Function: initialize

        initialize our network graph and be ready to display it

        Parameters:
          none.

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.initialize = function( strDivID,
                                pHandler)
    {
        self.m_sDiv = strDivID;

        self.pHandler = pHandler;

        setGlobalUpDown();

        // first prepend the header line into the display
        var hdrhtml =
                '<div class="nodeselHeader">'+
                '    <span class="nodeselComboSpan">' +
                _t("Show:") + '&nbsp;<select id="deviceselect" class="nodeselSelect"><option>' +
                _t("All Devices") + '</option><option>' +
                _t("Active Devices") + '</option></select></span>'+
                '</div>' +
                '<div id="_display"></div>';

        $("#"+strDivID).prepend(hdrhtml);

        $("#deviceselect").change(function(ev) {
            var newsel = $('#deviceselect :selected').text();
            if (newsel == _t("All Devices")) {
                showNonActive = true;
            }
            else showNonActive = false;

            $('#deviceselect').val(newsel);

            // now wipe the whole thing and redisplay!
            $("#"+strDivID).empty();
            self.setup();
        });

        $('#deviceselect').val(showNonActive ? _t("All Devices") : _t("Active Devices"));

        self.m_sDiv = "_display";

        //get the height and the width of the parent div
        g_nWidth = m_nWidth = self.calcWidth();//$(window).width()-$("#L").width() - 48;

        m_nHeight = $(window).height() - 130;

        //set up the columns
        m_nLeftX = 10;
        m_nRightX = m_nWidth-m_nIconSize-m_nLeftX;

        //create our svg surface in this div only
        m_r = Raphael(self.m_sDiv,m_nWidth,m_nHeight);

        var strDLG = "<div style=\"display: none; font-size:12px;\">\
        <div id=\"dialog-hover\" title=\"Device Information\" style = \"-webkit-user-select:none;-moz-user-select:none;\">\
        <div style=\"display: inline-table;width: 100px;margin-top: 8px;margin-left: 8px;\">"+_t("Name:")+" </div><div style=\"display: inline-table;margin-top: 8px;\" id=\"hover-name\">name</div><br>\
        <div style=\"display: inline-table;width: 100px;margin-left: 8px;\">"+_t("IP:")+" </div><div style=\"display: inline-table;\"id=\"hover-ip\"><!--# echo lan_ipaddr --></div><br>\
        <div id=\"iptype\"><div style=\"display: inline-table;width: 100px;margin-left: 8px;\">"+_t("IP Type:")+" </div><div style=\"display: inline-table;\"id=\"hover-ip-type\">DHCP</div><br></div>\
        <div style=\"display: inline-table;width: 100px;margin-left: 8px;\">"+_t("MAC:")+" </div><div style=\"display: inline-table;\"id=\"hover-mac\"><!--# echo lan_mac --></div><br>\
        <div style=\"display: inline-table;width: 100px;margin-left: 8px;\">"+_t("Type:")+" </div><div style=\"display: inline-table;\"id=\"hover-type\">MS Windows PC</div><br>\
        <div style=\"display: inline-table;width: 100px;margin-left: 8px;\">"+_t("Connection:")+" </div><div style=\"display: inline-table;\"id=\"hover-contype\">Wired</div><br>\
        <div style=\"display: none;\"id=\"router-ip\"><!--# echo lan_ipaddr --></div>\
        <div style=\"display: none;\"id=\"router-mac\"><!--# echo lan_mac --></div>\
        </div>\
        </div>";

        $("#"+self.m_sDiv).append(strDLG);

        //create the router
        self.createRouter();

        self.m_bActivated = true;

        //make an empty data set
        m_graphData = [];

        //update the datasync
        m_graphTimeSync = 0;

        //start the draw routine which draws continuously until program exit
        self.doDraw();

        //create the node info class for popup displays
        self.nodeinfo = new NodeInfo("#dialog-hover",150,300);
    }

    this.isActiveNode = function (n) {
        return showNonActive ? true : isActiveNode(n);
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: updateData

        called by the user to update nodes data

        Parameters:
          data - the nodes list from api/nodes

        Returns:
          none.

        See also:
          none.
    */
    ////////////////////////////////////////////////////////////////////////
    this.updateData = function(data)
    {
        //make an exact copy of the data
        m_graphData = clone(data);

        //update the datasync
        m_graphTimeSync++;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: updateRouter

        called by the user to update router data

        Parameters:
          router - the router data

        Returns:
          none.

        See also:
          none.
    */
    ////////////////////////////////////////////////////////////////////////
    this.updateRouter = function(router)
    {
        //make an exact copy of the data
        m_routerData = clone(router);

        if(typeof m_routerData.down_limit != 'undefined' && typeof m_routerData.up_limit != 'undefined')
        {
            g_nMaxDownload  = m_routerData.down_limit*8;
            g_nMaxUpload    = m_routerData.up_limit*8;
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: destroy

        destroy and stop the insides of this class. get it ready to delete

        Parameters:
          none.

        Returns:
          none.

        See also:
          none.
    */
    ////////////////////////////////////////////////////////////////////////
    this.destroy = function()
    {
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: isVisible

        call to set the  the  Flux display

        Parameters:
          none.

        Returns:
          none.

        See also:
          none.
    */
    ////////////////////////////////////////////////////////////////////////
    this.isVisible = function()
    {
        return self.bVisible;
    }


    /*
        Variable: self
        internal scope pointer to stay clean in callbacks.
    */
    var self = this;

    /*
        Variable: bVisible
        quick access to the current visible state so we don't have to walk
        the tree of dependencies
    */
    this.bVisible = false;


        /*
        Variable: m_sHTML
        The HTML file used to display the module
    */
    this.m_sHTML            = "mynetwork.html";

    /*
        Variable: m_iTimerInterval
        the update interval of the subscribed data retrieved from the webserver
    */
    this.m_iTimerInterval   = 1;

    var m_nHeight = 0;

    //special case objecs for the graph
    var m_pInternet = 0;
    var m_pRouter = 0;


    //scale of the graph items
    this.m_nScale = 1.0;

    this.m_bActivated = false;

    /*
        Variable: m_pNodeView
        reference to the class that views node items
    */
    this.m_pNodeView = null;

    /*
        Variable: m_pRouterView
        reference to the class that views the router

        can this be removed and replaced with pnodeview?
    */
    this.m_pRouterView = null;



    var m_routerData = null;
    var m_graphData = null;
    var m_graphTimeSync = 0;
    var m_drawTimeSync = 0;

    this.getLatestTime = function(lsNodes)
    {
        //get the previous time
        var dateNew = null;

        for(var n=0;n<lsNodes.length;n++)
        {
            var node = null;

            if(lsNodes[n].m_pNode == undefined || lsNodes[n].m_pNode == null)
            {
                node = lsNodes[n];
            }
            else
            {
                node = lsNodes[n].m_pNode;
            }

            // Record time of current node.
            var dateTest = new Date(0); //set to currenttime and zone
            dateTest.setUTCSeconds(node.timestamp);

            //if we don't have a newest we are the newest
            //if we are newer than the newest we win
            if(dateNew == null || dateTest.getTime()>dateNew.getTime())
            {
                dateNew = dateTest;
            }
        }

        return new Date(dateNew.getTime() + g_routerDeltaGMT);
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: doDraw

        This function does the drawing of the network map when a device isn't
        selected.

        doDraw() will recall itself on a 200 ms time out as long as MyNetwork
        is selected.

        This is the for all intensive purposes mainloop of this module.

        Parameters:
          none.

        Returns:
          none.

        See also:
          none.
    */
    ////////////////////////////////////////////////////////////////////////
    this.doDraw = function()
    {
        if(self.m_bActivated == true)
        {
            //update router and internet icons
            m_pInternet.updateIcon(self.pLoadIcon("Internet"));
            m_pRouter.updateIcon(self.pLoadIcon("Router"));

            //if we arent in the select item
            //and we have graph data
            //and the sync hasn't already been draw
            if(self.m_btCurrentSelect == null && m_graphData != null && m_graphTimeSync != m_drawTimeSync)
            {
                //increment the global sync counter to keep our list up to date.
                m_nGlobalSyncCounter++;

                //did we update any items
                var bUpdatedItem = false;

                var nGlobalUL = 0;
                var nGlobalDL = 0;

                var lsPrevious = [];
                lsPrevious = m_lsDevices.slice(0); // copy all

                //set the global current time
                self.curTime = new Date();
                self.preTime = new Date();

                //get the current time setup
                if(m_graphData.length > 0 )
                {
                    self.curTime = self.getLatestTime(m_graphData);
                }

                //make sure we have a valid previous time
                if(m_lsDevices.length > 0)
                {
                    self.preTime = self.getLatestTime(m_lsDevices);
                }

                //loop over the nodes
                for(var nNodes = 0;nNodes<m_graphData.length;nNodes++)
                {
                    //var strWrapperID = "mnItemLabel"+nNodes;sel

                    //replace all instances of "." with "" to insure we have good HTML ID's
                    //data.nodes[nNodes].mac = data.nodes[nNodes].mac.replace(/\./g, "");

                    //if the item isnt in the list then add it (this also increments the sync counter)
                    if(self.incrementItem(m_graphData[nNodes].mac) == false)
                    {
                        var bAdd = true;

                        if(bAdd && self.isActiveNode(m_graphData[nNodes]))
                        {
                            //add the item to our list
                            self.addDevice( m_graphData[nNodes],
                                            m_nGlobalSyncCounter);
                        }
                    }
                    else //update the item
                    {
                        self.updateItem(m_graphData[nNodes]);
                        bUpdatedItem = true;
                    }

                    nGlobalUL += m_graphData[nNodes].up;
                    nGlobalDL += m_graphData[nNodes].down;
                }

                //remove out of sync elements from m_lsDevices
                for(var device in m_lsDevices)
                {
                    if (device == 'indexOf')
                      continue;
                    if (!m_lsDevices[device].hasOwnProperty()) continue;
                    if(m_lsDevices[device].Sync != m_nGlobalSyncCounter)
                    {
                        m_lsDevices.splice(device,1);
                    }
                }

                if(m_graphData.length > 0 && m_lsDevices.length > 0)
                {
                    var nGlobalDnPS = m_routerData.down_rate*8;
                    var nGlobalUpPS = m_routerData.up_rate*8;

                    //if we have a previous sample
                    if(nPrevUpGlobal != -1 && nPrevDnGlobal !=-1)
                    {
                        var nUP = self.getPercent(g_nMaxUpload,nGlobalUpPS);
                        var nDN = self.getPercent(g_nMaxDownload,nGlobalDnPS);

                        m_pInternetBW.setUpDown(nGlobalDnPS,
                                                nGlobalUpPS,
                                                nDN,
                                                nUP);
                    }

                    nPrevUpGlobal = nGlobalUL;
                    nPrevDnGlobal = nGlobalDL;
                }

                self.draw(true,lsPrevious);
            }

            //sychronize the draw sync with the current data set
            m_drawTimeSync = m_graphTimeSync;
        }

        self.timer = window.setTimeout(self.doDraw,333);//g_nShortTimerM);
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getPercent

        Calculate the percent of <max> that <value> is.

        Parameters:
          max   - the number that we are calculating the percentage in.
          value - the number that is a percentage of the max

        Returns:

        returns a locked percent for display.

        min value of 0 max value of 100

        See also:

         nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.getPercent = function( max,
                                value)
    {
        var nPercent = (value/max) * 100;

        if(nPercent > 0)
            nPercent = (nPercent).toFixed(0);

        if(nPercent > 100)
            nPercent = 100;

        if(nPercent < 0)
            nPercent = 0;


        //console.log("Get Percent - max: "+max+" value: "+value+" return: "+nPercent);

        return nPercent;
    }



     var m_strMenu = "Device";


    //node info class
    this.nodeinfo = null;

    this.progress1 = null;
    this.progress2 = null;

    this.val1=0;
    this.val2=50;


    var m_nTransition = 0;//500;


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: killOld

        This routine compares the previous devices with the current m_lsDevices
        and deletes any of the ones that are no longer there. After completion it
        calls the calback routine.

        Parameters:
          lsPrevious    - Previous valid set of devices in the network map
          callback      - when this routine exits call this function

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.killOld = function(lsPrevious,
                            callback)
    {
        var lsDeleteMe = lsPrevious.slice(0);

        for(var nX = 0;nX<m_lsDevices.length;nX++)
        {
            var nDelMe = -1;

            // stupid IE8
            if (lsDeleteMe.length != undefined && lsDeleteMe.length > 0)
            {
                nDelMe = lsDeleteMe.indexOf(m_lsDevices[nX]);
            }

            //if the item is in the list and the node is active delete it
            if( nDelMe != -1 && self.isActiveNode(lsDeleteMe[nDelMe].m_pNode) == true)
            {
                lsDeleteMe.splice(nDelMe,1);
            }
        }
        //now fade the remaining items
        for(var nX = 0;nX<lsDeleteMe.length;nX++)
        {
            //remove the bandwidth of this from the global on the next pass
            //so that we don't spike
            nPrevUpGlobal -= lsDeleteMe[nX].m_pNode.up;
            nPrevDnGlobal -= lsDeleteMe[nX].m_pNode.down;

            //delete the device
            self.delDevice(lsDeleteMe[nX]);
            lsDeleteMe[nX].Kill();

            //call the callback for deletion if defined
            if(callback!=undefined)
            {
                callback();
            }
        }

        if(lsDeleteMe.length == 0 && callback!=undefined)
        {
            callback();
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: moveRemaining

        After a handful of the devices have been delete calling this routine
        with the previous device list will rebalance the current devices by
        looking at the differntial.

        Parameters:
          lsPrevious    - Previous valid set of devices in the network map
          callback      - when this routine exits call this function

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////

    this.moveRemaining = function(lsPrevious,callback)
    {
        var lsRemaining = [];

        for(var nX = 0;nX<m_lsDevices.length;nX++)
        {
            var nDelMe = -1;

            // stupid IE8
            if (lsPrevious.length != undefined && lsPrevious.length > 0)
            {
                nDelMe = lsPrevious.indexOf(m_lsDevices[nX]);
            }

            // //if we are to be hidden delete this node
            // if(nDelMe != -1 && lsPrevious[nDelMe].UI != undefined && lsPrevious[nDelMe].m_pNode.UI.show != undefined && lsPrevious[nDelMe].m_pNode.UI.show == false)
            // {
            //     nDelMe = -1;
            // }

            //if the item is in the list delete it
            if( nDelMe != -1)
            {
                lsRemaining.push(m_lsDevices[nX]);
            }
        }

        var bDoFullRepaint = self.m_bDidResize;

        //draw the elements
        for(var nIndex = 0;nIndex<lsRemaining.length;nIndex++)
        {
            //get the y position
            var nY = self.indexY(m_lsDevices.indexOf(lsRemaining[nIndex]));

            var nX = m_nRightX;

            if(self.isEven(m_lsDevices.indexOf(lsRemaining[nIndex])) == true)
            {
                nX = m_nLeftX;
            }
            //if((nX+1)<lsRemaining.length)
            //{

            if(nX != lsRemaining[nIndex].nXPos || nY != lsRemaining[nIndex].nYPos || bDoFullRepaint == true)
            {
                lsRemaining[nIndex].draw(   nX,
                                            nY,
                                            1.0,
                                            1.0,
                                            m_nTransition,
                                            self.m_nScale);
            }
        }

        if(bDoFullRepaint == true)
        {
            //reset the redraw after this move
            self.m_bDidResize = false;
        }

        if(/*lsRemaining.length == 0 &&*/ callback!=undefined)
        {
            callback();
        }
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: addNew

        After a handful of the devices have been deleted with killOld() and
        the moveRemaining has been called to rebalance the network map. Call addNew
        and the new network nodes will be added to the display in a nice animated
        manner.

        Parameters:
          lsPrevious    - Previous valid set of devices in the network map

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////

    this.addNew = function(lsPrevious)
    {
        var lsNew = m_lsDevices.slice(0);

        /* optimize drawing by checking for differences btwn lsnew and lsPrevious */
        var redraw = (lsNew.length != lsPrevious.length);

        if (!redraw) {
            for(var nX = 0;nX<lsPrevious.length;nX++) {
                if (lsNew.indexOf(lsPrevious[nX]) == -1) { // this item (nx) was deleted
                    redraw = true;
                    break;
                }
            }
        }

        if (!redraw) {
            for(var nX = 0;nX<lsNew.length;nX++) {
                if (lsPrevious.indexOf(lsNew[nX]) == -1) { // this item (nx) was deleted
                    redraw = true;
                    break;
                }
            }
        }

        if (!redraw) return;

        //draw the elements
        for(var nIndex = 0;nIndex<lsNew.length;nIndex++)
        {
            //get the y position
            var nY = self.indexY(m_lsDevices.indexOf(lsNew[nIndex]));

            var nX = m_nRightX;

            if(self.isEven(m_lsDevices.indexOf(lsNew[nIndex])) == true)
            {
                nX = m_nLeftX;
            }

            //lsNew[nIndex].m_nXPos = nX;
            //lsNew[nIndex].m_nYPos = nY;

            lsNew[nIndex].move(nX,nY);

            var startOpacity = lsNew[nIndex].getOpacity();

            lsNew[nIndex].draw( nX,
                                nY,
                                startOpacity,
                                1.0,
                                m_nTransition,
                                this.m_nScale);

        }
    }

    var m_lsDevicePool = [];
    var m_pGR = null;

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: delDevice

        Parameters:
            pDevice - The device to add to the network graph.

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.delDevice = function(pDevice)
    {
        //remove from our device list
        for(var nIndex=0; nIndex<m_lsDevices.length;nIndex++ )
        {
            if(m_lsDevices[nIndex]==pDevice)
            {
                m_lsDevices.splice(nIndex,1);
                break;
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getPosition

        This function gets the middle of an icon of a given size. This is
        mainly used for drawing pipes to the middle of network graph items.

        Parameters:
            nSize       - the size of the icon to get the middle of
            nPosition   - the position to start the middle calculation from

        Returns:
            returns the position minus half the given size

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.getPosition = function(    nSize,
                                    nPosition)
    {
        return nPosition-(nSize/2);
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: setUpDn

        unimplemented seems to have no use.

        Parameters:
            none.

        Returns:
            returns the position minus half the given size

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.setUpDn = function(nID,
                            nUpBPS,
                            nDnBPS)
    {
    }

    //array of all the lines in the program
    var m_lsLines = [];
    var m_lindex = 0;
    var m_TbarX = m_nIconSize*.66;
    var m_tbarLeft = null;
    var m_tbarRight = null;

    var m_tLeft = null;
    var m_tRight = null;

    var m_topT = -1;
    var m_botT = -1;
    var m_botTR = -1;

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: drawTBar

        This function draws the vertical line on the network graph when
        there are more than one network node on the side of a graph.

        This function is called internally by addLine

        Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.drawTBar = function()
    {
        var orgX = m_nWidth/2;
        var orgY = self.getPosition(m_nIconSize,(m_nHeight/2)-16);

        var leftX = orgX - m_TbarX;
        var rightX = orgX + m_TbarX;

        var windowH = $(window).height() - 130;

        if(windowH<370)
            windowH=370;

        var winHalf = ((m_nIconSize*self.m_nScale)*3)+ (m_nHalfSize*self.m_nScale);//(windowH/2) + m_nHalfSize - 16;


        //if we need to make a left bar
        if(m_tbarLeft==null)
        {
            m_tbarLeft = Line(  orgX,
                                winHalf,//orgY,
                                leftX,
                                winHalf,//orgY,
                                m_r);
        }
        else //update the left bar position
        {
            m_tbarLeft.updateStart(orgX,winHalf);
            m_tbarLeft.updateEnd(leftX,winHalf);
        }

        //do we need a right bar?
        if(m_lsDevices.length > 1)
        {
            //create the right bar if necessary
            if(m_tbarRight==null)
            {
                m_tbarRight = Line(     orgX,
                                    winHalf,//orgY,
                                    rightX,
                                    winHalf,//orgY,
                                    m_r);
            }
            else //update the position
            {
                //m_tbarRight.animate(orgX,winHalf,rightX,winHalf,1.0,m_nTransition);
                m_tbarRight.updateStart(orgX,winHalf);
                m_tbarRight.updateEnd(rightX,winHalf);
            }
        }
        else //if we have 1 device
        {
            //if we have a valid right bar
            if(m_tbarRight!=null)
            {
                //fade it out
                //m_tbarRight.animate(orgX,winHalf,rightX,winHalf,0.0,m_nTransition);
                //m_tbarRight.updateStart(orgX,winHalf);
                //m_tbarRight.updateEnd(rightX,winHalf);
                m_tbarRight.kill();
                m_tbarRight = null;
            }
        }

        var leftX = orgX - m_TbarX;
        var rightX = orgX + m_TbarX;
        var topY = m_topT +(m_nIconSize/2);
        var botY = m_botT +(m_nIconSize/2);
        var botYR = botY;

        if(m_lsDevices.length > 8 && self.isEven(m_lsDevices.length) == false)
        {
            botYR = m_botTR - 42;
        }

        //draw the left t
        if(m_lsDevices.length > 2)
        {
            //create the right bar if necessary
            if(m_tLeft==null)
            {
                m_tLeft = Line(     leftX,
                                    topY,
                                    leftX,
                                    botY,
                                    m_r);
            }
            else //update the position
            {
                /*m_tLeft.animate(leftX,
                                topY,
                                leftX,
                                botY,
                                1.0,
                                m_nTransition);*/

                m_tLeft.updateStart(leftX,topY);
                m_tLeft.updateEnd(leftX,botY);

            }
        }
        else
        {
            //create the right bar if necessary
            if(m_tLeft!=null)
            {
                /*m_tLeft.animate(leftX,
                                topY,
                                leftX,
                                botY,
                                0.0,
                                m_nTransition);*/

                //m_tLeft.updateStart(leftX,topY);
                //m_tLeft.updateEnd(leftX,botY);
                m_tLeft.kill();
                m_tLeft = null;
            }
        }

        //draw the left t
        if(m_lsDevices.length > 3)
        {
            //create the right bar if necessary
            if(m_tRight==null)
            {
                m_tRight = Line(    rightX,
                                    topY,
                                    rightX,
                                    botYR,
                                    m_r);
            }
            else //update the position
            {
                // m_tRight.animate(rightX,
                //              topY,
                //              rightX,
                //              botYR,
                //              1.0,
                //              m_nTransition);

                m_tRight.updateStart(rightX,topY);
                m_tRight.updateEnd(rightX,botYR);
            }
        }
        else
        {
            //create the right bar if necessary
            if(m_tRight!=null)
            {
                // m_tRight.animate(rightX,
                //              topY,
                //              rightX,
                //              botY,
                //              0.0,
                //              m_nTransition);

                //m_tRight.updateStart(rightX,topY);
                //m_tRight.updateEnd(rightX,botY);
                m_tRight.kill();
                m_tRight = null;
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: addLine

        Add a line in the network graph for the node at the given
        coordinates.

        Parameters:
            nX - horizontal position to draw a line to
            nY - vertical position to draw a line to

        Returns:
            returns the UID of the newly created line

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.addLine = function(nX,
                            nY)
    {
        var nID = m_lindex++;

        var myY = nY+(m_nIconSize/2);
        var myX = nX+(m_nIconSize/2);

        var orgX = m_nWidth/2;

        if(orgX > myX)
        {
            orgX = orgX - m_TbarX;
        }
        else
        {

            orgX = orgX + m_TbarX;
        }

        m_lsLines[nID] = Line(  orgX,
                                myY,
                                myX,
                                myY,
                                m_r);

        self.drawTBar();

        return nID;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: moveLine

        Move an existing line to a new coordinate.

        Parameters:
            nID     - UID of the line to move give by the addLine function
            nX      - horizontal position to draw a line to
            nY      - vertical position to draw a line to
            opacity - ending opacity of the line after the move
            nTime   - time span of the animation from the current position to the new postion

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.moveLine = function(   nID,
                                nX,
                                nY,
                                opacity,
                                nTime)
    {
        var myY = nY+(m_nIconSize/2);
        var myX = nX+(m_nIconSize/2);

        var orgX = m_nWidth/2;

        if(orgX > myX)
        {
            orgX = orgX - m_TbarX;
        }
        else
        {

            orgX = orgX + m_TbarX;
        }



/*      if(opacity != undefined && nTime != undefined)
        {
            m_lsLines[nID].animate(orgX, myY, myX, myY,opacity,nTime);
        }
        else*/
        {
            m_lsLines[nID].updateStart(orgX,myY);
            m_lsLines[nID].updateEnd(myX,myY);
        }

        self.drawTBar(false);
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: killLine

        unimplemented.

        Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.killLine = function(nID)
    {
        m_lsLines[nID].kill();
        //m_lsLines.splice(nID,1);
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getLine

        Get the RaphaelJS line object for the given UID

        Parameters:
            nID - UID of the line that is wanted

        Returns:
            Returns a RaphaelJS line object

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.getLine = function(nID)
    {
        return m_lsLines[nID];
    }

    var m_pInternetBW = null;


    this.routerNode = {
                        "uid": "router",
                        "ip": "N/A",
                        "mac": "N/A",
                        "os":
                        {
                        "type": "Router",
                        "flavor": "Router",
                        },
                        "wireless": false,
                        "name": "Router",
                        "timestamp": {
                        "millisecond": 915,
                        "hour": 0,
                        "month": 1,
                        "second": 33,
                        "year": 1970,
                        "day": 1,
                        "minute": 10
                        },
                        "down": 0,
                        "up": 0
                    };

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: createRouter

        Creates the Killer Node, the Internet Node, and the global internet
        bandwidth arrows then adds them to the network graph.

        This is one of the first function called when the my network page
        is initialized.

        Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.createRouter = function()
    {
        m_pGR = new CGraphRoutines( self.setUpDn,
                                    self.addLine,
                                    self.moveLine,
                                    self.killLine,
                                    m_r,
                                    self.getLine);

        m_pRouter = new CGraphItem(     "Router",
                                        "The Killer 9100 Intelligent networking for the home.",
                                        self.pLoadIcon("Router"), // the router icon key
                                        -1,
                                        m_nIconSize,
                                        m_nIconSize,
                                        self.getPosition(m_nIconSize,m_nWidth/2),
                                        self.getPosition(m_nIconSize,m_nHeight/2+m_nIconSize),
                                        1.0,
                                        m_pGR,
                                        self.selectNode,
                                        self.onHover,
                                        self.routerNode,
                                        null,
                                        false);

        m_pRouter.setScale(self.m_nScale,self.m_nScale);

        m_pInternet = new CGraphItem(   "Internet",
                                        "The Internet connection for your Killer 9100.",
                                        self.pLoadIcon("Internet"), //the internet icon key
                                        -1,
                                        m_nIconSize,
                                        m_nIconSize,
                                        self.getPosition(m_nIconSize,m_nWidth/2),
                                        m_nHeight - (m_nIconSize * 1.7),
                                        1.0,
                                        m_pGR,
                                        null,
                                        null,
                                        null,
                                        null,
                                        false);

        m_pInternet.setScale(self.m_nScale,self.m_nScale);
    }

    var RouterLine = null;
    var nStartScaleWidth    = 1090;
    var nMinScaleWidth      = 690;
    var nStepPercent        = .5/ (nStartScaleWidth - nMinScaleWidth);


    this.m_bDidResize = false;

    //hook up the resize window function
    $(window).resize(function()
    {
        self.doResize();
    });


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: doResize

        Called when the resize window routine is fired by the browser.

        Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.doResize = function()
    {
        if(self.m_bActivated == true)
        {

            //g_nh.resize();

            if( m_pRouter != undefined && m_pRouter != null && m_pRouter != 0 &&
                m_pInternet != undefined && m_pInternet != null && m_pInternet != 0)
            {
                var newWidth = self.calcWidth();//$(window).width()-$("#L").width() - 48;

                if(newWidth<g_nMinWidth)
                {
                    newWidth = g_nMinWidth-100;
                }

                newHeight = m_botT + 256;

                if(newHeight<$(window).height() - 130)
                    newHeight = $(window).height() - 130;

                var nRows = m_lsDevices.length/2;
                var nCalcMinHeight = nRows*getIconSize(self.m_nScale);

                if(newHeight<nCalcMinHeight)
                {
                    newHeight = nCalcMinHeight;
                }

                if(newHeight != m_nHeight || newWidth != m_nWidth)
                {
                    if(m_r != undefined && m_r != null && m_r != 0)
                    {
                        m_nHeight = newHeight;
                        g_nWidth =  m_nWidth = newWidth;

                        m_r.setSize(newWidth,newHeight);
                    }
                }

                var windowH = $(window).height() - 130;

                if(windowH<nCalcMinHeight)
                    windowH=nCalcMinHeight;

                //draw the elements
                for(var nIndex = 0;nIndex<m_lsDevices.length;nIndex++)
                {
                    //get the y position
                    var nY = self.indexY(m_lsDevices.indexOf(m_lsDevices[nIndex]));

                    var nX = m_nRightX;

                    if(self.isEven(m_lsDevices.indexOf(m_lsDevices[nIndex])) == true)
                    {
                        nX = m_nLeftX;
                    }

                    m_lsDevices[nIndex].draw(   nX,
                                                nY,
                                                1.0,
                                                1.0,
                                                m_nTransition,
                                                self.m_nScale);

                }

                m_pRouter.draw(self.getPosition(m_nIconSize,m_nWidth/2),
                              ((m_nIconSize*self.m_nScale)*3)-(m_nHalfSize-(m_nHalfSize*self.m_nScale)),//self.getPosition(m_nIconSize,windowH/2+(m_nHalfSize*.7)),
                                1.0,
                                1.0,
                                m_nTransition,
                                self.m_nScale);

                var nInternetY = 0;

                //draw the internet
                m_pInternet.draw(   self.getPosition(m_nIconSize,m_nWidth/2),
                                    nInternetY,//windowH - (m_nIconSize * 1.7),
                                    1.0,
                                    1.0,
                                    m_nTransition,
                                    self.m_nScale);

                var nHalfHalf = ((windowH - (m_nIconSize * 1.7))- self.getPosition(m_nIconSize,windowH/2 - m_nHalfSize))/2;
                var nHalfWay =  self.getPosition(m_nIconSize,windowH/2 - m_nHalfSize) + nHalfHalf;

                if( m_pInternetBW == null)
                {
                    if(isIE8())
                    {
                        m_pInternetBW = new IEArrows(    self.getPosition(m_nIconSize,m_nWidth/2) - 312,
                                                    (m_nIconSize*self.m_nScale)*1.2,
                                                    3,
                                                    m_r,
                                                    self.m_nScale);
                    }
                    else
                    {
                        m_pInternetBW = new CArrows(    self.getPosition(m_nIconSize,m_nWidth/2) - 312,
                                                    (m_nIconSize*self.m_nScale)*1.2,
                                                    3,
                                                    m_r,
                                                    self.m_nScale);
                    }

                    m_pInternetBW.setScale(self.m_nScale,self.m_nScale);
                }
                m_pInternetBW.setPos(   self.getPosition(m_nIconSize,m_nWidth/2) - 284,
                                        (m_nIconSize*self.m_nScale)*1.2);//(nHalfWay *.95)/*- (m_nIconSize * self.m_nScale)*/);

                m_pInternetBW.setScale(self.m_nScale,self.m_nScale);

                //RouterLine.animate(m_nWidth/2,self.getPosition(m_nIconSize,windowH/2+(m_nIconSize*.8)),m_nWidth/2,nInternetY + (m_nIconSize/2),1.0,m_nTransition);
                if(RouterLine != null)
                {
                    RouterLine.updateStart(m_nWidth/2,((m_nIconSize*self.m_nScale)*3.5));//self.getPosition(m_nIconSize,windowH/2+(m_nIconSize*.8)));
                    RouterLine.updateEnd(m_nWidth/2,nInternetY + (m_nIconSize/2));
                }
                //tell the draw routine to do an extra pass
                self.m_bDidResize = true;

                self.doDraw();
            }
        }
    }

    /*
        Variable: m_nMinScale
        minimal scale allowed for the icons and network map
    */
    this.m_nMinScale = .5;

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: draw

        Draw the My Network Graph.

        This is the main loop that does the Node Graph by calling

        - killOld()
        - moveRemaining()
        - addNew()

        After recieving the new data.

        Parameters:
            bAnimate    - is this draw to be animated? false means instant
            lsPrevious  - pevious state of the graph for comparison of the draw state

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.draw = function(bAnimate,lsPrevious)
    {

        m_nRightX = m_nWidth-m_nIconSize-m_nLeftX;

        //get the height and the width of the parent div
        var newWidth = self.calcWidth();//$(window).width()-$("#L").width() - 48;

        if(newWidth<g_nMinWidth)
        {
            newWidth = g_nMinWidth;
        }

        if(newWidth<=nStartScaleWidth)
        {
            self.m_nScale = 1.0-((nStartScaleWidth - newWidth)*nStepPercent);
        }
        else
        {
            self.m_nScale = 1.0;
        }

        if(self.m_nScale < self.m_nMinScale)
        {
            self.m_nScale = self.m_nMinScale;
        }

        var newHeight = $(window).height() - 130;

        var nRows = m_lsDevices.length/2;
        var nCalcMinHeight = nRows*getIconSize(self.m_nScale);

        if(newHeight<nCalcMinHeight)
        {
            newHeight = nCalcMinHeight;
        }


        if(RouterLine == null)
        {
            //draw the line from the router to the internet
            RouterLine = Line(  m_nWidth/2,
                                self.getPosition(m_nIconSize,m_nHeight/2),
                                m_nWidth/2,
                                m_nHeight - m_nIconSize,
                                m_r);
        }

        if(lsPrevious == undefined || lsPrevious.length == 0)
        {
            lsPrevious = [];
        }

        self.killOld(   lsPrevious,
                        function()
                        {
                            self.moveRemaining( lsPrevious,
                                                function()
                                                {
                                                    self.addNew(lsPrevious);


                                                    newHeight = m_botT + 256;//$(window).height() - 130;

                                                    if(newHeight<$(window).height() - 130)
                                                        newHeight = $(window).height() - 130;

                                                    var nRows = m_lsDevices.length/2;
                                                    var nCalcMinHeight = nRows*getIconSize(self.m_nScale);

                                                    if(newHeight<nCalcMinHeight)
                                                    {
                                                        newHeight = nCalcMinHeight;
                                                    }

                                                    if(newHeight != m_nHeight || newWidth != m_nWidth)
                                                    {
                                                        if(m_r != null)
                                                        {
                                                            m_nHeight = newHeight;
                                                            g_nWidth =  m_nWidth = newWidth;

                                                            m_r.setSize(newWidth,newHeight);
                                                            self.m_bDidResize = true;

                                                            //console.log("width: "+m_nWidth+" height: "+m_nHeight);
                                                        }
                                                    }

                                                    var windowH = $(window).height() - 130;

                                                    if(windowH<370)
                                                        windowH=370;

                                                    m_pRouter.draw(self.getPosition(m_nIconSize,m_nWidth/2),
                                                                   ((m_nIconSize*self.m_nScale)*3)-(m_nHalfSize-(m_nHalfSize*self.m_nScale)),//self.getPosition(m_nIconSize,windowH/2+(m_nHalfSize*.7)),
                                                                    1.0,
                                                                    1.0,
                                                                    m_nTransition,
                                                                    self.m_nScale);

                                                    var nInternetY = 0;

                                                    //draw the internet
                                                    m_pInternet.draw(   self.getPosition(m_nIconSize,m_nWidth/2),
                                                                        nInternetY,//windowH - (m_nIconSize * 1.7),
                                                                        1.0,
                                                                        1.0,
                                                                        m_nTransition,
                                                                        self.m_nScale);

                                                    var nHalfHalf = ((windowH - (m_nIconSize * 1.7))- self.getPosition(m_nIconSize,windowH/2 - m_nHalfSize))/2;
                                                    var nHalfWay =  self.getPosition(m_nIconSize,windowH/2 - m_nHalfSize) + nHalfHalf;

                                                    if( m_pInternetBW == null)
                                                    {
                                                        if(isIE8())
                                                        {
                                                            m_pInternetBW = new IEArrows(   self.getPosition(m_nIconSize,m_nWidth/2) + 284,
                                                                                            (m_nIconSize*self.m_nScale)*1.2,
                                                                                            3,
                                                                                            m_r,
                                                                                            self.m_nScale);
                                                        }
                                                        else
                                                        {
                                                            m_pInternetBW = new CArrows(    self.getPosition(m_nIconSize,m_nWidth/2) + 284,
                                                                                        (m_nIconSize*self.m_nScale)*1.2,
                                                                                        3,
                                                                                        m_r,
                                                                                        self.m_nScale);
                                                        }

                                                        m_pInternetBW.setScale(self.m_nScale,self.m_nScale);
                                                    }
                                                    m_pInternetBW.setPos(   self.getPosition(m_nIconSize,m_nWidth/2) - 284,
                                                                            (m_nIconSize*self.m_nScale)*1.2);//(nHalfWay *.95)/*- (m_nIconSize * self.m_nScale)*/);

                                                    m_pInternetBW.setScale(self.m_nScale,self.m_nScale);

                                                    //RouterLine.animate(m_nWidth/2,self.getPosition(m_nIconSize,windowH/2+(m_nIconSize*.8)),m_nWidth/2,nInternetY + (m_nIconSize/2),1.0,m_nTransition);

                                                    RouterLine.updateStart(m_nWidth/2,((m_nIconSize*self.m_nScale)*3.5));//self.getPosition(m_nIconSize,windowH/2+(m_nIconSize*.8)));
                                                    RouterLine.updateEnd(m_nWidth/2,nInternetY + (m_nIconSize/2));
                                                });
                        });

    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: isEven

        Is the given number even?

        Parameters:
            nNumber - number to check for even

        Returns:
            -true   even
            -false  odd

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.isEven = function(nNumber)
    {
        return (nNumber%2 == 0) ? true : false;
    };

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: indexY

        given the index # of a node what is the y position

        Parameters:
            nIndex - index # of the node to finde the y position of

        Returns:
            returns the y position of the node

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.indexY = function(nIndex)
    {
        var rt = -1;

        var nCount = m_lsDevices.length;

        var nHeight = ($("#Display").height() - 130) - m_nIconSize;

        var nRows = m_lsDevices.length/2;

        //calculate the height to a min of 6 icons high, which is
        //the height of the router
        var nCalcMinHeight = 6*getIconSize(self.m_nScale);

        //nHeight = nCalcMinHeight;

        if(nHeight<nCalcMinHeight)
        {
            nHeight = nCalcMinHeight;
        }


        if(nIndex < nCount)
        {
            //set the display graph config
            if(nCount == 1 || nCount == 2)
            {
                rt = ((m_nIconSize*self.m_nScale)*3) - (m_nHalfSize-(m_nHalfSize*self.m_nScale));//self.getPosition(m_nIconSize,nHeight/2 - m_nHalfSize + m_nIconSize);
                m_topT = 0;
                m_botTR = m_botT = 0;
            }
            else if(nCount == 3)
            {
                var ar = []

                m_topT = ar[0] = rt = self.getPosition(m_nIconSize,m_nHalfSize);
                ar[1] = rt = ((m_nIconSize*self.m_nScale)*3) - (m_nHalfSize-(m_nHalfSize*self.m_nScale));//self.getPosition(m_nIconSize,nHeight/2 - m_nHalfSize+m_nIconSize);
                m_botTR =m_botT = ar[2] = rt = self.getPosition(m_nIconSize,nHeight-m_nIconSize);

                rt = ar[nIndex];
            }
            else if(nCount == 4)
            {
                var ar = []

                m_topT = ar[0] = rt = self.getPosition(m_nIconSize,m_nHalfSize);
                ar[1] = rt = self.getPosition(m_nIconSize,m_nHalfSize);
                m_botTR =m_botT = ar[2] = rt = self.getPosition(m_nIconSize,nHeight-m_nIconSize);
                ar[3] = rt = self.getPosition(m_nIconSize,nHeight-m_nIconSize);

                rt = ar[nIndex];
            }
            else if(nCount == 5)
            {
                var ar = []

                m_topT = ar[0] = rt = self.getPosition(m_nIconSize,m_nHalfSize);
                ar[1] = rt = self.getPosition(m_nIconSize,m_nHalfSize);
                ar[2] = rt = self.getPosition(m_nIconSize,nHeight/2 - m_nHalfSize);
                m_botTR =m_botT = ar[3] = rt = self.getPosition(m_nIconSize,nHeight-m_nIconSize);
                ar[4] = rt = self.getPosition(m_nIconSize,nHeight-m_nIconSize);

                rt = ar[nIndex];
            }
            else if(nCount == 6)
            {
                var ar = []

                m_topT = ar[0] = rt = self.getPosition(m_nIconSize,m_nHalfSize);
                ar[1] = rt = self.getPosition(m_nIconSize,m_nHalfSize);
                ar[2] = rt = self.getPosition(m_nIconSize,nHeight/2 - m_nHalfSize);
                ar[3] = rt = self.getPosition(m_nIconSize,nHeight/2 - m_nHalfSize);
                m_botTR = m_botT = ar[4] = rt = self.getPosition(m_nIconSize,nHeight-m_nIconSize);
                ar[5] = rt = self.getPosition(m_nIconSize,nHeight-m_nIconSize);

                rt = ar[nIndex];
            }
            else if(nCount == 7)
            {
                var ar = []

                var top = m_nHalfSize;
                var bottom = nHeight-m_nIconSize;

                var inc = nHeight / 4;

                m_topT = ar[0] = rt = self.getPosition(m_nIconSize,top);
                ar[1] = rt = self.getPosition(m_nIconSize,top);
                ar[2] = rt = self.getPosition(m_nIconSize,top+inc);;
                ar[3] = rt = self.getPosition(m_nIconSize,top+(inc*1.5));
                ar[4] = rt = self.getPosition(m_nIconSize,top+(inc*2));
                ar[5] = rt = self.getPosition(m_nIconSize,top+(inc*3));
                m_botTR = m_botT = ar[6] = rt = self.getPosition(m_nIconSize,top+(inc*3));

                rt = ar[nIndex];
            }
            else if(nCount == 8)
            {
                var ar = []

                var top = m_nHalfSize;
                var bottom = nHeight-m_nIconSize;

                var inc = nHeight / 4;

                m_topT = ar[0] = rt = self.getPosition(m_nIconSize,top);
                ar[1] = rt = self.getPosition(m_nIconSize,top);
                ar[2] = rt = self.getPosition(m_nIconSize,top+inc);;
                ar[3] = rt = self.getPosition(m_nIconSize,top+inc);
                ar[4] = rt = self.getPosition(m_nIconSize,top+(inc*2));
                ar[5] = rt = self.getPosition(m_nIconSize,top+(inc*2));
                m_botTR = m_botT = ar[6] = rt = self.getPosition(m_nIconSize,top+(inc*3));
                ar[7] = rt = self.getPosition(m_nIconSize,top+(inc*3));

                rt = ar[nIndex];
            }
            else
            {
                var ar = [];

                var top = m_nHalfSize;
                var bottom = nHeight-m_nIconSize;

                var inc = getIconSize(self.m_nScale);//nHeight / 8;

                //m_topT = ar[0] = rt = self.getPosition(m_nIconSize,top);

                for(var n = 0;n<nCount;n+=2)
                {
                    var nRow = top + (inc*n);

                    ar[n] = rt = self.getPosition(m_nIconSize,nRow);
                    ar[n+1] = rt = self.getPosition(m_nIconSize,nRow);
                }

                m_topT = ar[0];
                m_botT = ar[ar.length-1];
                rt = ar[nIndex];

                if(self.isEven(nCount) == true)
                {
                    m_botTR = m_botT;
                }
                else
                {
                    m_botTR = m_botT - inc;
                }
            }

            //calculate the width for this # of nodes
            var nGraphWidth = ($("#"+self.m_sDiv).width() * .66).toFixed(0);
            //get the height and the width of the parent div
            var rX = parseInt(nGraphWidth) + parseInt($("#L").width());// + 32;
        }


        return rt;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getIndex

        Given the index # of a node what is the y position

        Parameters:
            nIndex - index # of the node to finde the y position of

        Returns:
            returns the y position of the node

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.getIndex = function(nUID)
    {
        var nIndex = -1;

        for(var n = 0;n<m_lsDevices.length;n++)
        {
            if(m_lsDevices[n].m_pNode.mac == nUID)
            {
                nIndex = n;
                break;
            }
        }

        return nIndex;
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: hasItem

        does the device exist in this m_lsDevices list

        Parameters:
            nUID - unique ID of the device to try and find

        Returns:
            -true  if it exists
            -false if nonexistent

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.hasItem = function(nUID)
    {
        var bReturn = false;
        var index = self.getIndex(nUID);
        //if the item is undefined its not in the list
        if(m_lsDevices[index] != undefined)
        {
            bReturn = true;
        }

        return bReturn;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: incrementItem

        This is a synchronization routine used by the data update functions.
        When a UID is handed to this function it increments the sync to the
        current time so that it isn't delete. Items with an old sync number
        are deleted.

        Parameters:
            nUID - unique ID of the device to set as current

        Returns:
            -true  if it exists and is implemented
            -false if nonexistent

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.incrementItem = function(nUID)
    {
        //does this have the item
        var bReturn = self.hasItem(nUID);

        //if it does
        if(bReturn == true)
        {
            var index = self.getIndex(nUID);
            //keep its sync count up to date
            m_lsDevices[index].Sync = m_nGlobalSyncCounter;
        }

        return bReturn;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: addDevice

        Add a device to the network graph and set the sync counter to the
        current time so it gets to stay.

        Parameters:
            node            - CGraphicItem to add to the network graph
            nSyncCounter    - current time sync for the device


        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.addDevice = function(  node,
                                nSyncCounter)
    {
        //pick  a random icon image for this device
        var nIndex = (Math.random() * (g_strIcons.length-1)).toFixed(0);

        //get the image path
        var icon = self.pLoadIcon(node.os.type);

        //default the name
        var strName = makeNodeName(node);

        var pDevice =new CGraphItem(strName,
                                    "noDescription",
                                    icon,
                                    node.wireless,
                                    m_nIconSize,
                                    m_nIconSize,
                                    -1,
                                    -1,
                                    0.0,
                                    m_pGR,
                                    self.selectNode,
                                    self.onHover,
                                    node,
                                    self.onDeleteNode);

        //test online/offlien
        //pDevice.setOnline(false);

        pDevice.Sync = m_nGlobalSyncCounter;
        m_lsDevices.push(pDevice);
    }

    this.m_btCurrentSelect = null;

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: selectNode

        This is the handler called when a CGraphItem is clicked on. It is
        automatically set in the above addDevice when a new device is put in.
        Sets the m_btCurrentSelect for easy use throughout the class.

        Parameters:
            icon    - Icon to select too
            name    - of the selected device
            x       - x position of the node
            y       - y position of the node
            node    - the CGraphItem itself


        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.selectNode = function(icon,name,x,y,node)
    {
        if(self.pHandler != null)
        {
            var id = "";

            if(node.m_pNode.os.type == "Router")
            {
                id = node.m_pNode.os.type.toLowerCase();
            }
            else
            {
                id = node.m_pNode.mac;
            }

            self.pHandler(icon,name,x,y,node,id);
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: onDeleteNode

        This is the handler called when a CGraphItem is clicked on to delete

        Parameters:
            graphitem   - the CGraphItem itself


        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.onDeleteNode = function(graphitem)
    {
        if(graphitem != undefined)
        {
                var nodedata = clone(graphitem.m_pNode);

                //nodedata.UI.show = false;
        }
    }
    ////////////////////////////////////////////////////////////////////////
    /*
        Function: updateItem

        Update a node on the front My Network graph view. Does a deleta
        time and calculates the correct bandwdith. Updates the Arrows and
        starts off the animations for the bandwadth arrows.

        Parameters:
            node - the node we are updating the bandwidth of

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.updateItem = function( node)
    {
        var index = self.getIndex(node.mac);

        nUpPerSec = node.up_rate*8;
        nDnPerSec = node.down_rate*8;

        var UpPercent = self.getPercent(g_nMaxUpload,nUpPerSec);
        var DnPercent = self.getPercent(g_nMaxDownload,nDnPerSec);

        m_lsDevices[index].setUpDown(   nUpPerSec,
                                        nDnPerSec,
                                        UpPercent,
                                        DnPercent);

        //get the icon path
        var icon = self.pLoadIcon(node.os.type);

        //call the graphitem if the path is different it will update
        m_lsDevices[index].updateIcon(icon);

        //store this off for future use
        m_lsDevices[index].m_pNode.timestamp = node.timestamp;
        m_lsDevices[index].m_pNode.up = node.up;
        m_lsDevices[index].m_pNode.down = node.down;
        m_lsDevices[index].m_pNode.ip = node.ip;
        m_lsDevices[index].m_pNode.os.type = node.os.type;
        m_lsDevices[index].m_pNode.name = node.name;
        m_lsDevices[index].m_pNode.connected = node.connected;
    }


    /*
        Variable: nPrevUpGlobal
        previous global upload byte count
    */
    var nPrevUpGlobal;

    /*
        Variable: nPrevDnGlobal
        previous global download byte count
    */
    var nPrevDnGlobal;

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: onHover

        Handle hover event

        Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
    */


    this.openInfo = function(node) {
        var name    = "none";
        var ip      = "none";
        var iptype  = "none";
        var lease   = "false";
        var mac     = "none";
        var dmz     = "false";
        var type    = "unknown";
        var contype = "none";

        if(node.name!=null)
        {
            //make an appropriate length label for the node
            name = makeLabel(node.name,20);
        }

        if(node.ip!=null)
        {
            ip = node.ip;
        }

        if(node.mac!=null)
        {
            mac = node.mac;
        }

        if(node.os.type!=null)
        {
            type = node.os.type;
        }

        if(node.os.flavor!=null)
        {
            type += " "+node.os.flavor;
        }

        if(node.static!=null)
        {
            if(node.static == false)
            {
                iptype = _t("DHCP");

                //show dhcp field
                $("#iptype").css("display","block");
            }
            else
            {
                iptype = _t("Static");

                //hide dhcp field
                $("#iptype").css("display","none");
            }
        }
        if(node.wireless!=null)
        {
            if(node.wireless == false)
            {
                contype = _t("Wired");
            }
            else
            {
                contype = _t("Wireless");
            }
        }


        //set the node text
        $("#hover-name").text(_t(name));

        if(name!="Router")
        {
            $("#hover-ip").text(ip);
            $("#hover-mac").text(mac);
            $("#hover-ip-type").text(iptype);
            $("#hover-contype").text(contype);
        }
        else
        {
            if( typeof RouterIPAddr != 'undefined' &&
                typeof RouterMacId != 'undefined')
            {
                $("#hover-ip").text(RouterIPAddr);
                $("#hover-mac").text(RouterMacId);
            }

            //always show router field
            $("#iptype").css("display","block");

            if( typeof RouterIPType != 'undefined' &&
                RouterIPType.toLowerCase().indexOf("dhcp") != -1)
            {
                iptype = _t("DHCP");
            }
            else
            {
                iptype = _t("Static");
            }

            $("#hover-ip-type").text(iptype);
            $("#hover-contype").text(_t("Wired"));
        }

        $("#hover-type").text(type);
    }

    ////////////////////////////////////////////////////////////////////////
    this.onHover = function(bShow,pNode)
    {
        //if show the node
        if(bShow == true)
        {
            self.nodeinfo.show(pNode.popupX(), pNode.popupY(), self.openInfo(pNode.m_pNode));
        }
        else //hide the node
        {
            self.nodeinfo.hide();
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: onClick

        handle click event

        Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.onClick = function(pDevice)
    {

    }

    ////////////////////////////////////////////////////////////////////////
    // Locals
    ////////////////////////////////////////////////////////////////////////

    /*
        Variable: m_pCurrentSelect
        Deprecated?
    */
    var m_pCurrentSelect;

    /*
        Variable: m_nLeftX
        Deprecated?
    */
    var m_nLeftX;

    /*
        Variable: m_nRightX
        Deprecated?
    */
    var m_nRightX;

    /*
        Variable: m_nWidth
        Current width of this display.
    */
    var m_nWidth;

    /*
        Variable: m_r
        raphael instance variable
    */
    var m_r;

    /*
        Variable: m_lsDevices
        Current list of network devices for the network map
    */
    var m_lsDevices;

    /*
        Variable: m_nGlobalSyncCounter
        Var to keep objects in sync my have a global update
        counter to compare to a per node update counter if
        they are out of sync we delete or update
    */
    var m_nGlobalSyncCounter;


    this.setup = function() {
        // reset initial states of EVERYTHING

        self.bVisible = false;

        self.m_sHTML            = "mynetwork.html";

        self.m_iTimerInterval   = 1;

        m_nHeight = 0;
        m_pInternet = 0;
        m_pRouter = 0;
        self.m_nScale = 1.0;
        self.m_bActivated = false;
        self.m_pNodeView = null;
        self.m_pRouterView = null;

        m_graphData = null;
        m_graphTimeSync = 0;
        m_drawTimeSync = 0;
        m_strMenu = "Device";

        self.nodeinfo = null;
        self.progress1 = null;
        self.progress2 = null;

        self.val1=0;
        self.val2=50;

        m_nTransition = 0;//500;

        m_lsLines = [];
        m_lindex = 0;
        m_TbarX = m_nIconSize*.66;
        m_tbarLeft = null;
        m_tbarRight = null;

        m_tLeft = null;
        m_tRight = null;

        m_topT = -1;
        m_botT = -1;
        m_botTR = -1;

        m_pInternetBW = null
        RouterLine = null;
        nStartScaleWidth    = 1090;
        nMinScaleWidth      = 690;
        nStepPercent        = .5/ (nStartScaleWidth - nMinScaleWidth);

        self.m_bDidResize = false;
        self.m_nMinScale = .5;

        self.m_btCurrentSelect = null;
        nPrevUpGlobal = -1;
        nPrevDnGlobal = -1;

        m_pCurrentSelect = null;
        m_nLeftX = 0;
        m_nRightX = 0;
        m_nWidth = 0;
        m_r = 0;
        m_lsDevices = [];
        m_nGlobalSyncCounter = 0;
        /*
            Variable: pData
            Returned data for the last updated network list
        */
        this.pData = null;

        /*
            Variable: pHandler
            mouse handler to call back when events occur
        */
        this.pHandler = null;

        //if they passed in parameters on the construction of this
        //objec then call initialize
        if( strDivID != undefined)
        {
            //initialize ourself
            self.initialize(strDivID,pHandler);
        }

        this.curTime = null;
        this.preTime = null;
    }

    this.setup();
}
