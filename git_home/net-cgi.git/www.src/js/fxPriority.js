/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
////////////////////////////////////////////////////////////////////////
/*
    Class: fxPriorty

    Version:   1.0

    the visual display for node priorities

    Parameters:
            strDivID        - id of the div to draw into
            pSaveFunction   - custome save function to call when fx.Save() is called
            bShowButtons    - boolean to show save/cancel
            pSaveClick      - save button click handler
            pCancelClick    - cancel button click handler

        Returns:
            nothing.

        See also:
            nothing.
*/
////////////////////////////////////////////////////////////////////////

function fxPriority(strDivID,
                    pSaveFunction,
                    bShowButtons,
                    pSaveClick,
                    pCancelClick,
                    pLoadIcon)
{
    var self = this;
    var showNonActive = true;

     var m_strMenu = "Device";
     this.m_sDiv = null;

    /////////////////////////////////////////////////////////////////////////
    /*
        The magic to load our pre-requisite scripts in javascript
    */
    /////////////////////////////////////////////////////////////////////////
    loadScript("utility-2.0.js");
    loadScript("raphael-min.js");
    loadScript("PRV/PRItem-2.0.js");
    loadScript("PRV/PRView-2.0.js");
    loadScript("jquery.jscrollpane.min.js");
    loadScript("jquery-swapsies.js");
    loadScript("jquery-ui-1.8.21.custom.min.js");
    loadScript("fxTranslate-2.0.js");


    /*
        Variable: self

        Easy access to the this of the class, to keep it separate
        from other this scopes
    */
    var self = this;

    /*
        Variable: bAmIDirty

        class member to store list modified state
    */
    this.bAmIDirty          = false;

    /*
        Variable: pSaveClick

        Passed in save click  callback function
    */
    this.pSaveClick         = null;

    /*
        Variable: pCancelClick

        Passed in cancel click  callback function
    */
    this.pCancelClick       = null;

    /*
        Variable: pLoadIcon

        Passed in cancel click  callback function
    */
    this.pLoadIcon          = pLoadIcon;

    /*
        Variable: nHeightAdjuster

        Adjuster to change height to make up for the button box/title
    */
    this.nHeightAdjuster = 350;

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: initialize

        Initialize the view list and do the 1st draw

        Parameters:
            strDivID        - id of the div to draw into
            pSaveFunction   - custome save function to call when fx.Save() is called
            bShowButtons    - boolean to show save/cancel
            pSaveClick      - save button click handler
            pCancelClick    - cancel button click handler

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.initialize = function( strDivID,
                                pSaveFunction,
                                bShowButtons,
                                pSaveClick,
                                pCancelClick)
    {
        self.m_sDiv = strDivID;

        //save click handlers
        if(typeof pSaveClick === 'function')
        {
            self.pSaveClick = pSaveClick;
        }
        else if(pSaveClick != null)
        {
            self.pSaveClick = window[pSaveClick];
        }

        //cancel click handlers
        if(typeof pCancelClick === 'function')
        {
            self.pCancelClick = pCancelClick;
        }
        else if(pCancelClick != null)
        {
            self.pCancelClick = window[pCancelClick];
        }

        // first prepend the header line into the display
        var hdrhtml =
            '<div class="nodeselHeader" style="overflow:hidden">'+
            '    <span style="height:16px;vertical-align:middle;margin-top:5px; margin-left:20px">Priority</span>'+
            '    <span style="height:16px;vertical-align:middle;margin-left:40px">Device</span>'+
            '    <span style="height:16px;vertical-align:middle;margin-left:270px">Type</span>'+
            '    <span style="height:16px;vertical-align:middle;margin-left:180px">MAC Address</span>'+
            '    <span class="nodeselComboSpan">' +
            _t("Show:") + '&nbsp;<select id="deviceselect" class="nodeselSelect"><option>' +
            _t("All Devices") + '</option><option>' +
            _t("Active Devices") + '</option></select></span>'+
            '</div>';

        $("#"+self.m_sDiv).prepend(hdrhtml);

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

        //self.pPriority = pPriority;
        self.pSaveFunction = pSaveFunction;

        //external save function when the model doesn't work
        if(typeof pSaveFunction === 'function')
        {
            self.pSaveFunction = pSaveFunction;
        }
        else if(pSaveFunction != null)
        {
            self.pSaveFunction = window[pSaveFunction];
        }

        self.initPriority();

        if(bShowButtons != undefined && bShowButtons == true)
        {
            var btBox =
            '<div id="pageactions" class="cbi-page-actions">' +
            '<div class="priorityHeaderLine">Priorities</div>' +
            '<div class="priorityBox">' +
            '    <div class="priorityText">' +
            _t("Priorities allows you to change the importance of one device over another. This is useful when the devices are competing for limited bandwidth with applications of the same classification.") +
            '</div>' +
            '    <div style="margin-left:10px;">' +
            '    <button id="prSave" type="button" style="width:120px">' + _t("Save Settings") + '</button>' +
            '    <button id="prCancel"type="button" style="width:135px">' + _t("Don't Save Settings") + '</button>' +
            '    </div>' +
            '</div>' +
            '</div>';
            $("#"+self.m_sDiv).prepend(btBox);

            $("#prSave").button();
            $("#prCancel").button();

            //set visibility of the two buttons
            $("#prSave").hide();
            $("#prCancel").hide();

            //hook up save button
            $("#prSave").click(function()
            {

                //if we have a handler
                if(self.pSaveClick != null)
                {
                    self.pSaveClick();
                }

                //save the data
                self.save();

                //set visibility of the two buttons
                $("#prSave").hide();
                $("#prCancel").hide();

            });

            //hook up cancel button
            $("#prCancel").click(function()
            {
                //if we have a handler
                if(self.pCancelClick != null)
                {
                    self.pCancelClick();
                }

                //set visibility of the two buttons
                $("#prSave").hide();
                $("#prCancel").hide();

                //clear the list which also force a reload
                self.prview.clearAll();
            });
        }

    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: initPriority

        Completely clear the priority view and get it ready to draw.

        Parameters:
            none.
        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.initPriority = function()
    {
        if ($("#PanelPriority").css("display") != "none")
        {
            var data = [];

            //make an exact copy of the data
            self.devices = clone(data);

            //remove pre existing elements
            $("#lvScrollbox").children().remove();
            $("#lvScrollbox").remove();

            //create elements to add
            var nHeight = $(window).height()*.98;
            var nNewHeight = $(window).height()-self.nHeightAdjuster;
            var listView = "<div id=\"lvRoundBox\" style=\"clear: right;border: 2px solid #555;margin-right: 15px;margin-left: 15px;height:"+nNewHeight+";\"> \
                            <div id=\"lvScrollbox\" style=\"-webkit-user-select: none;-moz-user-select: none; height:"+nNewHeight+"; overflow-y: auto;overflow-x: auto;margin-left: 15px;\"></div> \
                            </div>";

            //Add the panel to our right pane
            $(listView).appendTo("#"+self.m_sDiv);

            //create a a bwview parented to the scrollview
            self.prview = new PRView("lvScrollbox",self.onMod);

            //if we created the object
            if(self.prview != null)
            {
                //clean up everything
                self.prview.clearAll();
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: updateData

        Completely clear the priority view and get it ready to draw.

        Parameters:
            nodes - the list of nodes in the format of the angular nodes
                    model
        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.updateData = function(nodes)
    {

        //add the prioritized
        for(var nPrio = 0;nPrio<=257;nPrio++)
        {
            //loop through all the nodes for this index value
            for(var nIndex = 0;nIndex<nodes.length;nIndex++)
            {
                //hand node accesor
                var node = nodes[nIndex];

                //show/hide active nodes
                if (!showNonActive && !isActiveNode(node))
                    continue;

                //make a readable node name
                var strName = makeNodeName(node);

                //default type
                var strType = "Unknown";

                //set the os type if not use the above
                if(node.os.type != undefined && node.os.type != null && node.os.type != "undefined")
                {
                    strType = node.os.type;
                }

                //get the icon path
                var icon = pLoadIcon(strType);

                //if we have aview add nodes
                if( self.prview!= null &&
                    (
                        (typeof node.user_priority != 'undefined' && node.user_priority == nPrio) ||    //do 0 to 256 prioritized
                        (typeof node.user_priority == 'undefined' && nPrio == 257)                      //on 257 add unprioritized (dirty, but no code repitition)
                    )
                  )
                {
                    //put the item in the list
                    //this doesn't add a duplicate item uid
                    self.prview.addItem(nIndex,
                                        self.makeUID(node.mac),    //unique identifier of flow/node
                                        icon,                               //png icon in /images
                                        makeLabel(strName,20),              //label of item
                                        node.mac,
                                        strType);
                }
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: onMod

        When the list is modified this is called to set dirty and button
        states

        Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.onMod = function()
    {
        self.bAmIDirty = true;

        $("#prSave").show();
        $("#prCancel").show();
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: resize

        call to size this to the Display area of the windows

        Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.resize = function()
    {
        //calc width and heigh
        var nNewHeight = $(window).height()-self.nHeightAdjuster;
        var nNewWidth = ($(window).width()-$("#L").width())-100;

        //set the widths and heights
        $("#"+self.m_sDiv).css("width",nNewWidth);
        $("#lvScrollbox").css("height",nNewHeight);
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: save

        Save the priority change list to the router

        Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.save = function()
    {
        //get our nodes to save
        var lsPri = self.prview.enumItems();

        //loop through the keys
        for(var i = 0;i<lsPri.length;i++)
        {
            //get the item
            var prItem = lsPri[i];

            if (self.pSaveFunction != null)
            {
                //create a prirority based on visual order
                var nPriority = i+1;

                //make a mac out of this uid
                var strMAC = self.makeMAC(prItem.uid);

                //call the save functino and write this back to tho model
                self.pSaveFunction(strMAC, nPriority);
            }
        }

        //turn off saving
        self.bAmIDirty = false;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: makeMAC

        make a mac from the uid of a PRView item

        Parameters:
            strUID - UID from a PRView Item

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.makeMAC = function(strUID)
    {
        var strMAC =strUID.replace(/\_/g,':');

        return strMAC;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: makeUID

        make a UID from a MAC address

        Parameters:
            strMAC - mac address a router device node

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.makeUID = function(strMAC)
    {
        var strSpace  = "";
        var strPeriod = "";
        var strColon = "";
        var strComma = "";
        var strApostrophe = "";
        var strResult = "";

        if(strMAC != undefined)
        {
            strSpace = strMAC.replace(/\ /g,'_');
            strPeriod = strSpace.replace(/\./g,'_');
            strColon = strPeriod.replace(/\:/g,'_');
            strComma = strColon.replace(/\,/g,'_');
            strApostrophe = strComma.replace(/\'/g,'_');

            strResult = strApostrophe;
        }

        return strResult;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: setup

        when the class is constructied this is the 1st call
        to get everything going

        Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.setup = function() {
        //the list of nodes from api/nodes
        this.devices    = null;

        //priority list view
        this.prview     = null;

        //jscroll api handy pointer
        this.api        = null;

        this.pSaveFunction = null;

        //if they passed in parameters on the construction of this
        //objec then call initialize
        if( strDivID != undefined)
        {
            //initialize ourself
            self.initialize(strDivID,
                            pSaveFunction,
                            bShowButtons,
                            pSaveClick,
                            pCancelClick,
                            bShowButtons);
        }
    }

    //call the setup to get the constuction moving
    this.setup();

    //start a time for the resizer
    setInterval(function()
        {
            self.resize();
        },
    1000);
}
