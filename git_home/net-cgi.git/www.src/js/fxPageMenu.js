/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
////////////////////////////////////////////////////////////////////////
/*
    Class: nhButton

    Version:   1.0

    Button Data Structure. Holds the info about button on the right
    menu pane bar on the main page.

    - .id
    - .panel
    - .plugin
    - .parent           <--- For submenu items
    - .onActivate
    - .subMenuNode  <--- Sub menu div ID
*/
////////////////////////////////////////////////////////////////////////
function nhButton()
{
    this.myname=null;

    //icons for this guy
    this.icoNormal=null;
    this.icoHover=null;
    this.icoSelect=null;

    this.label=null;

    this.strLabelText="";

    this.plugin=null;
    this.div2index = [];

    this.yRestore = null;

    this.strMenuDiv = "";

    this.strCurrentDiv = "";

    this.nIndex = 0;

    this.bDeleteOnUnselect = false;

    this.handler = null;
}

var useRaphael = true;
var PoweredMinPos = 490;

var sbpages = {'mynetwork.asp':1,'selectnode.asp':1,'priorities.asp':1,'toptimes.asp':1,'topdownloads.asp':1,'allevents.asp':1};
// for mobile browsers
// android browser 3.0 or higher
// android chrome ???? 18?

// should check for SVG support on "older or unknown" browsers
// return -1  -- browser recognized, but too old
// return 0 -- browser unrecognized
// return 1 -- browser is recognized and good version
function CheckBrowserList(ua) {
    for (var b in browser_agents) {
        var br = browser_agents[b];
        var matchret;
        if ((matchret = ua.match(br[0])) == null) continue;
        if (matchret.length > 1) {
            if (parseFloat(matchret[1]) < br[1]) return -1; // too old
            else return 1; // good match
        }
    }
    return 0; // not found
}

////////////////////////////////////////////////////////////////////////
/*
    Function: isStreamboostPage

    Check and see if the page that is currently loaded is a streamboost
    page. This work by having the HTML/XHTML have a javascript section
    that globally defines g_bIsStreamboost and sets it to true.

    //add this to the file to set its dependency on streamboost
    var g_bIsStreamboost = true;

    Parameters:
      none.

    Returns:
      true  - if this is a streamboost dependent page
      false - if streamboost isn't needed for the page to work

    See also:
      nothing.
*/
    ////////////////////////////////////////////////////////////////////////
function isStreamboostPage() {

    var bReturn = false;

    //is the global streamboost defined?
    if(typeof g_bIsStreamboost != 'undefined')
    {
        //if so this is set in our page so use its value
        bReturn = g_bIsStreamboost;
    }

    return bReturn;
}


////////////////////////////////////////////////////////////////////////
/*
    Class: nhAnim

    Version:   1.0

    This is the item that goes into an animation queue for buttons.
*/
////////////////////////////////////////////////////////////////////////
function nhAnim()
{
    this.item=null;
    this.animation=null;
    this.time=null;
    this.type=null;
}

////////////////////////////////////////////////////////////////////////
/*
    Class: fxPageMenu

    Version:   1.0

    Left Page selection Top Menu system
*/
////////////////////////////////////////////////////////////////////////
//global pagemenu variable
var g_pm = null;

//if (navigator.userAgent.indexOf("MSIE 9") != -1)
//    document.documentElement.style.visibility = "hidden";

var prevWindowWidth = -1; // keep reisize from doing anything unless the window width changes
var prevWindowHeight = -1;

var g_nMenuReadyCallback = null;

//set a callback to call when the menu is fully drawn
var fxMenuReady = function(callback)
{
    //set the global callback for the on ready
    g_nMenuReadyCallback=callback;
}

function fxPageMenu(strDiv,bInsertMenu)
{
    var self = this;

    /////////////////////////////////////////////////////////////////////////
    /*
        The magic to load our pre-requisite scripts in javascript
    */
    /////////////////////////////////////////////////////////////////////////

    //load the translation library
    loadScript("fxTranslate-2.0.js");
    loadScript("utility-2.0.js");
    loadScript("raphael-min.js");
    loadScript("GraphItem-2.0.js");
    loadScript("NodeInfo-2.0.js");
    loadScript("Line-2.0.js");
    loadScript("fxHelp.js");



    ////////////////////////////////////////////////////////////////////////
    /*
        Function: initialize

        Ininitalize the Full page menu setup and show the menu.
        Note this is intended to take up the whole browser page.

        Parameters:
          strDiv - Div to draw the page in

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.initialize = function(strDiv,bInsertMenu)
    {
        loadDeviceTable(function()
        {
            for (var n = 0; n < 100; n++)
            {
                m_animList[n] = new Array();
            }

            var strPre = "<div id=\"L\" class=\"Left\"> \
                        <div id=\"TL\" class=\"TopLeft\"> \
                              <div class=\"KillerLogo\"></div> \
                          </div> \
                          <div id=\"ML\" class=\"LeftMenu\"></div> \
                          <div id=\"BL\" class=\"BottomLeft\" > \
                          <div id=\"PoweredBy\" class=\"LogoLeft\"></div> \
                           </div> \
                      </div> \
                      <div id=\"R\" class=\"Right\"> \
                        <div class=\"TopRight\"></div> \
                        <div id=\"SubMenu\" class=\"RightMenu\"></div> \
                        <div id=\"Work\" class=\"BottomlRight\" >";

            var strApp =  "</div></div>";

            var strDLG =  "<div style=\"display: none; font-size:12px;\">";
                strDLG += "<div id=\"dialog-bwtest\" title=\"Testing Bandwidth\" style=\"overflow: hidden;\">";
                strDLG += "<h3> Progress: </h3>"
                strDLG += "<div class=\"adminline\" style=\"height: 24px;margin-top: 10px;\">"
                strDLG += "<div class=\"admincontrol\">"
                strDLG += "<div id=\"bwprogress\" style=\"width:465px;height:24px;\"></div>"
                strDLG += "</div>"
                strDLG += "</div>"
                strDLG += "</div>"


                strDLG += "<div id=\"dialog-alert\" title=\"Device Information\" style = \"-webkit-user-select:none;-moz-user-select:none;overflow:hidden\">";
                strDLG += "<div style=\"display: inline-table;margin-top: 3px;\" id=\"alert-message\">Message</div><br>";
                strDLG += "</div>";

                strDLG += "</div>";


            $('body').css({'position':'relative', top: '0px', left: '0px',height: "100%", 'overflow':'visible'}); // , min-height: "2000px"});

            //if this is a streamboost page
            if (isStreamboostPage() && !isSBEnabled())
            {
                var streamboostOffMsg = '<div class="inpageError">' +
                    _t("This page is used for StreamBoost. Click ") +
                    "<a href=\""+getSBLink()+"\">" + _t("here") + '</a>' +
                    _t(" to set up StreamBoost.") +
                    '</div>';
                //if we are supposed to add the menu programaticlly
                if(bInsertMenu != undefined && bInsertMenu == true)
                {
                    document.getElementsByTagName("body")[0].innerHTML = strPre + streamboostOffMsg + strApp + strDLG;
                }
                else
                {
                    $("#Work").append(streamboostOffMsg);
                }
            }
            else
            {
                //if we are supposed to add the menu programaticlly
                if(bInsertMenu != undefined && bInsertMenu == true)
                {
                    document.getElementsByTagName("body")[0].innerHTML = strPre + $('body').html() + strApp + strDLG;
                }
            }

            AlertUnsupportedBrowser();

            document.getElementsByTagName("body")[0].style.minHeight = "1000px";

            if (useRaphael) {
                m_r = Raphael(0, 0, "100%", "100%");
            }

            var mtop = Math.max($(window).height()-256, PoweredMinPos);
            $("#PoweredBy").css('margin-top',mtop);
            $("#PoweredBy").css("background-image", "url("+g_path.images+"QualcommStreamBoost.png)");

            //do alert test
            self.doAlert();

            //if we have a callback for menu ready
            if(g_nMenuReadyCallback!=undefined && g_nMenuReadyCallback!=null)
            {
                //tell the callback that we have drawn
                g_nMenuReadyCallback();
            }

            //set all the sizes by calling resize after 1st draw
            self.resize();
        });
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: doAlert

        See if we are supposed to show an alert on the QC SB logo

        Parameters:
          none.

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.doAlert = function()
    {
        var bAlert = false;
        var strLink = "none.";
        var strMessage = "none.";

        $.ajax({
                type: "GET",
                async: false,
                cache: true,
                url: "notification.json",
                dataType: "json",
                //-------------------------------------------------------------------
                // Issue callback with result data
                success: function(data, status, xhr)
                {
                    if(data.link != undefined && data.message != undefined && data.link != null && data.message != null)
                    {
                        bAlert = true;
                        strMessage = data.message;
                        strLink = data.link;
                    }
                }
         });


        //if we mave an alert status
        if(bAlert == true)
        {
            //change out the image
            $("#PoweredBy").removeClass("LogoLeft");
            $("#PoweredBy").addClass("AlertLeft");
            $("#PoweredBy").css("background-image", "url("+g_path.images+"Alert_QualcommStreamBoost1.png)");

            //show the message
            var info = new AlertInfo();

            //hook up the mouseover message
            $("#PoweredBy").hover(function()
            {
                info.show(strMessage);
            },
            function()
            {
                info.hide();
            });

            $("#PoweredBy").click(function()
            {
                window.location = strLink;
            });

            //start a time for the flash value
            setInterval(function()
                {
                    //get the current image
                    var strCurrentImage = $("#PoweredBy").css("background-image");

                    //flip the image based on current image
                    if(strCurrentImage.indexOf("Alert_QualcommStreamBoost2") != -1)
                    {
                        $("#PoweredBy").css("background-image", "url("+g_path.images+"Alert_QualcommStreamBoost1.png)");
                    }
                    else
                    {
                        $("#PoweredBy").css("background-image", "url("+g_path.images+"Alert_QualcommStreamBoost2.png)");
                    }
                },
            1000);//timer of 1 sec
        }
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: destroy

        Kill this class and clear the page

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
        //kill the timers if any

        //destroy the drawing
    }

    this.handler = null;

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: addPage

        This is the function called to create a left menu button. The Icon
        parameters are the image name in the htdocs/images foloer for the
        icon. Such as "button.png"

        Parameters:
          strName       - Name of the button this is the label
          strIcon       - Regular icon for the tab button.
          strIconHover  - Hover Icon state
          strIconSelect - Selected Icon state
          bTabPage      - is this a tab page or a single page
        Returns:
          returns UUID for the button you will need this to add menus

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.addPage = function(strName,
                            strIcon,
                            strIconHover,
                            strIconSelect,
                            bTabPage,
                            handler)
    {
        //add the button
        this.addButton( strName,
                        strIcon,
                        strIconHover,
                        strIconSelect,
                        handler);

        //currently just return the name as the id
        return strName;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: delPage

        Delete a previously added page

        Parameters:
          pageID        - ID of the page to delete (returned by addPage)

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.delPage = function(pageID)
    {

    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: selectPage

        Delete a previously added page

        Parameters:
          pageID        - ID of the page to select (returned by addPage)

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.selectPage = function(pageID,bCallHandler)
    {
        var bt = m_lsButtons[pageID];

        if(bt != undefined)
        {
            self.setCurrentSelect(bt,bCallHandler);
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: addSubButton

        Add a submenu to the parent plugin. This is used by the admin module
        and others to add their submenus.

        Parameters:
          parent  - parent ID : returned by a call to addPage()
          strName - name of the submenu to add
          handler - function to call on a click

        Returns:
          returns ID of sub button

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.addSubButton = function(parentID,
                                 strName,
                                 handler)
    {

    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: delSubButton

        Delete a previously added subbutton

        Parameters:
          parentID      - parent ID : returned by a call to addPage()
          subButtonID   - button ID : returned by a call to addSubButton()

        Returns:
          returns ID of sub button

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.delSubButton = function(   parentID,
                                    subButtonID)
    {

    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: addSubTab

        Add a submenu to the parent plugin. This is used by the admin module
        and others to add their submenus.

        Parameters:
          parent  - parent ID : returned by a call to addPage()
          strName - name of the submenu to add
          handler - function to call on a click

        Returns:
          returns ID of sub button

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.addSubTab = function(  parentID,
                                strName,
                                handler)
    {

    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: delSubTab

        Delete a previously added subbutton

        Parameters:
          parentID      - parent ID : returned by a call to addPage()
          subButtonID   - button ID : returned by a call to addSubButton()

        Returns:
          returns ID of sub button

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.delSubTab = function(  parentID,
                                subTabID)
    {

    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: delSubTab

        Delete a previously added subbutton

        Parameters:
          parentID      - parent ID : returned by a call to addPage()
          subButtonID   - button ID : returned by a call to addSubButton()

        Returns:
          returns DIV you can draw into for the subtab

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.getSubTabDiv = function( parentID,
                                  subTabID)
    {
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: delSubTab

        Delete a previously added subbutton

        Parameters:
          parentID      - parent ID : returned by a call to addPage()
          subButtonID   - button ID : returned by a call to addSubButton()

        Returns:
          returns DIV you can draw into for the subtab

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.getPageDiv = function(pageID)
    {
        return m_lsID2Div[pageID];
    }

    this.nOldWidth = 0;

    setInterval(function()
    {
        var newWidth = $(window).width()-$("#L").width();

        if($("#R").width() != newWidth)
        {
            self.resize();
        }

    },500);

    this.resize = function()
    {
        var mainWindow = $(window).width();

        if (prevWindowWidth != mainWindow) {
            prevWindowWidth = mainWindow;

            if(mainWindow<1030)
            {
                mainWindow = 1030;

                $('html').css("overflow-x","scroll");
            }
            else
            {
                $('html').css("overflow-x","hidden");
            }

            var newWidth = mainWindow-$("#L").width()+g_WorkWidthAdjuster;

            self.nOldWidth = newWidth;

            $("#R").width(newWidth);
            $("#Display").width(newWidth-g_ViewPortAdjuster);

            //body main width
            $('body').width(mainWindow);
            $('html').width(mainWindow);
            $(window).width(mainWindow);
        }

        //set the BL to the same height as the work
        $("#BL").height($("#Work").height());
        //resize the tabs/pages here
        var nHeight=$(window).height();
        if (prevWindowHeight != nHeight) {
            prevWindowHeight = nHeight;

            $("#BL").height($("#Work").height());

            var mtop = Math.max($(window).height()-256,PoweredMinPos);
            $("#PoweredBy").css('margin-top',mtop);
        }
    }



    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getID

        Generate an ID for a button. This is called when adding buttons
        that are programaticcly generated.

        Parameters:
          none.

        Returns:
          returns an incremented integar id

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.getID = function()
    {
        return m_nGenID++;
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: addPlugin

        Add a plugin module to the main interface. This adds the icon and
        the page. It then generates a left tab bar button to select it.

        Parameters:
          none.

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.addPlugin = function(_kPlugin)
    {
        //create the button and add it
        var button = self.addButton(_kPlugin.GetName(),
                                        _kPlugin.GetIcon(),
                                        _kPlugin.GetIconHover(),
                                        _kPlugin.GetIconSelected());

        //hook up the plugin
        button.plugin = _kPlugin;
        button.plugin.Init(self);
        button.plugin.UpdateDataSources();

        //add div for html of plugin
        button.plugin.SetDiv(self.makeID("Panel" + _kPlugin.GetName()));


        //if the plugin has pages
        var kPages = button.plugin.GetPages();

        if (kPages != undefined && kPages.length > 0)
        {
            self.addSubmenu(button,
                            _kPlugin.GetName());//"Pages");

            for (var n = 0; n < kPages.length; n++)
            {
                //add div for html of plugin
                var strDiv = self.makeID("Panel" + kPages[n].GetName());

                kPages[n].SetDiv(strDiv);

                var sHtml = g_kRouter.GetHTML(kPages[n].GetHTML());
                var Panel = "<div id=\"" + strDiv + "\" class=\"PageStyle\">" + sHtml + "</div>"; ;

                //Add the panel to our right pane
                $(Panel).appendTo('#Work');

                $("#" + strDiv).hide();

                self.addSMTab(button,
                                kPages[n].GetName(),
                                n);
            }

            for (var n = 0; n < kPages.length; n++)
            {
                kPages[n].Init();
                kPages[n].UpdateDataSources();
            }

        }
        else // since this is a test make a fake page if we have no pages
        {
            //add div for html of plugin
            var strDiv = self.makeID("Panel" + button.plugin.GetName());

            button.plugin.SetDiv(strDiv);

            var Panel = "<div id=\"" + strDiv + "\" class=\"PageStyle\">" + g_kRouter.GetHTML(_kPlugin.GetHTML()) + "</div>"; ;

            //Add the panel to our right pane
            $(Panel).appendTo('#Work');

            $("#" + strDiv).hide();
        }

        return button;
    }

    this.findPlugin=function(strName)
    {
        var kButton=m_lsButtons[strName];
        if(kButton==null || kButton==undefined)
            return null;
        else
            return kButton.plugin;
    }


    this.makeButtonID = function(strName)
    {
        var strSpace  = "";
        var strPeriod = "";
        var strColon = "";
        var strComma = "";
        var strApostrophe = "";
        var strResult = "";

        if(strName != undefined)
        {
            strSpace = strName.replace(/\ /g,'_');
            strPeriod = strSpace.replace(/\./g,'_');
            strColon = strPeriod.replace(/\:/g,'_');
            strComma = strColon.replace(/\,/g,'_');
            strApostrophe = strComma.replace(/\'/g,'_');

            strResult = "button_id_"+strApostrophe;
        }

        return strResult;
    }

    function CreateMenuImage(srcurl, x, y, width, height, extraAttrs) {
        if (useRaphael) {
            return m_r.image(srcurl, x, y, width, height).attr(extraAttrs);
        }
        else {
            var e = document.createElement('img');
            e.src = srcurl;
            e.left = x;
            e.top = y;
            e.width = width;
            e.height = height;
            return {'node': e};
        }

    }

    function CreateMenuText(left, top, text, attrs) {
        if (useRaphael) {
            return m_r.text(left, top, text).attr(attrs);
        }
        else {
            var e = document.createElement('span');
            e.textContent = text;
            //e.style.position = 'absolute';
            e.left = left;
            e.top = top;
            return {'node': e };
        }
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: addButton

        This is the function called by addPlugin to create the button in
        the tab bar for each new module that is loaded. The Icon parameters
        are the image name in the htdocs/images foloer for the icon. Such
        as "monkey.png"

        Parameters:
          strName       - Name of the button this is the label
          strIcon       - Regular icon for the tab button.
          strIconHover  - Hover Icon state
          strIconSelect - Selected Icon state
        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.addButton = function(  strName,
                                strIcon,
                                strIconHover,
                                strIconSelect,
                                handler)
    {
        //create the container for this button
        var btNew = new nhButton;
        if (useRaphael) {
            btNew.st = m_r.set();
        } else {
            btNew.st = document.createElement("div");
        }

        btNew.handler = handler;

        //store the index in the menu
        btNew.nIndex = m_nIndex++;

        //add this button to our array
        m_lsButtons[strName] = btNew;

        //put paths in filenames
        var strImage1   = g_path.images + strIcon;
        var strImage2   = g_path.images + strIconHover;
        var strImage3   = g_path.images + strIconSelect;

        //calculate the position
        var w = $("#L").width();
        var left        = w * m_LeftMargin;
        var top         = m_posMenuTop + (m_posNextItem*m_nCurrent);

        //create the icons
        btNew.icoNormal = CreateMenuImage(strImage1, left , top, m_MenuIconSize, m_MenuIconSize,{opacity: eShow});
        btNew.icoNormal.node.myname = strName;
        btNew.icoNormal.myid = self.getID()
        btNew.icoHover  = CreateMenuImage(strImage2, left , top, m_MenuIconSize, m_MenuIconSize,{opacity: eHide});
        btNew.icoHover.myid = self.getID();
        btNew.icoHover.node.myname = strName;

        btNew.icoSelect = CreateMenuImage(strImage3, left , top, m_MenuIconSize, m_MenuIconSize,{opacity: eHide});
        btNew.icoSelect.myid = self.getID();
        btNew.icoSelect.node.myname = strName;

        //chrome workaround
        fixAlpha(btNew.icoNormal,eShow);
        fixAlpha(btNew.icoHover,eHide);
        fixAlpha(btNew.icoSelect,eHide);

        btNew.yRestore = top;


        //add an id to this element
        $(btNew.icoNormal.node).attr('id',self.makeButtonID(strName));


        //get the icon width so we can calculate the label placement
        var nWidth      = btNew.icoNormal.getBBox().width;

        //create the text label for the button
        btNew.label     = CreateMenuText(left+(m_MenuIconSize*1.5), top+24, strName, {font: '16px Century Gothic, Arial', opacity: eShow, fill: m_ColorNormal, "text-anchor": "start"}); ;
        btNew.label.myid = self.getID();
        btNew.label.node.myname = strName;

        //chrome hack work around
        fixAlpha(btNew.label,eShow);

         $(btNew.label.node).attr('unselectable', 'on').css('-moz-user-select', 'none').css('-webkit-user-select', 'none');

         $(btNew.icoNormal.node).mouseover(function() {
            $(this).css('cursor', 'pointer');});

         $(btNew.icoHover.node).mouseover(function() {
            $(this).css('cursor', 'pointer');});

         $(btNew.label.node).mouseover(function() {
            $(this).css('cursor', 'pointer');});

        //hook up the callbacks
        self.buttonCallbacks(btNew);

        btNew.icoNormal.toFront();

        //increment the count
        m_nCurrent++;

        var strDivID = self.makeID("Panel" + strName);

        btNew.strCurrentDiv = strDivID;

        m_lsID2Div[strName] = strDivID;

        return btNew;
    }

    this.fixButtons = function(current)
    {
        if(g_bDoChromeHack == true)
        {
            for(var n in m_lsButtons)
            {
                if(current!=n)
                {
                    self.fadeOutButton(m_lsButtons[n]);
                }
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////
    /*
        Function: buttonCallbacks

        Hook up the callbacks for a new button. THis function is called by
        addButton() to hook up the callback handlers. That allows the icon
        transitions and animation states. It also allows the click handler.

        Parameters:
          button - this is the element for the button

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.buttonCallbacks = function(button)
    {

        //create a target for the label that is based on its bounding box
        var label = m_r.rect().attr(button.label.getBBox()).attr({  fill: "#000",
                                                                    opacity: eHide,
                                                                    cursor: "pointer"});
        //chrome hack work around
        fixAlpha(label,eHide);

        //create the handlor for    the text
        label.hover(function(){
            self.fixButtons(this.myname);
            self.fadeInButton(button);
        },
        function ()
        {
            self.fixButtons(this.myname);;
            self.fadeOutButton(button);
        });

        $(button.icoNormal.node).mouseover(
        function(){
            self.fixButtons(this.myname);
            self.fadeInButton(m_lsButtons[this.myname]);
        }).mouseout( function () {
            self.fixButtons(this.myname);
            if(g_bDoChromeHack == false)
            {
                self.fadeOutButton(m_lsButtons[this.myname]);
            }
        });

        //chrome fixes for bad version
        if(g_bDoChromeHack == true)
        {

            $(button.icoHover.node).mouseover(
            function(){
                self.fixButtons(this.myname);
            }).mouseout( function () {
                self.fixButtons(this.myname);
                self.fadeOutButton(m_lsButtons[this.myname]);
            });

            $(button.label.node).mouseover(function() {
                self.fadeInButton(m_lsButtons[this.myname]);

            self.fixButtons(this.myname);;
            }).mouseout( function () {
                self.fadeOutButton(m_lsButtons[this.myname]);
                self.fixButtons(this.myname);
            });

            $(button.label.node).mousedown(
                function(e){
                self.fixButtons(this.myname);
                self.mouseDnButton(button);
            });

            $(button.label.node).mousedown(
                function(e){
                self.fixButtons(this.myname);
                self.mouseUpButton(button);
            });


        }

        //create the handler for the icon
        $(label.node).mousedown(
        function(e){
            self.fixButtons(this.myname);;
            //only do this if we are the left button
            //if (e.which == 1)
            {
                self.mouseDnButton(button);
            }
        });

        $(label).mousedown(
        function(e){
            self.fixButtons(this.myname);
            //only do this if we are the left button
            //if (e.which == 1)
            {
                self.mouseDnButton(button);
            }
        });

        //create the handler for the icon
        $(label.node).mouseup(
        function(e){
            self.fixButtons(this.myname);
            //only do this if we are the left button
            //if (e.which == 1)
            {
                self.mouseUpButton(button);
            }
        });

        //create the handler for the icon
        $(button.icoNormal.node).mousedown(
        function(){
            self.fixButtons(this.myname);
            self.mouseDnButton(m_lsButtons[this.myname]);
        });


        //create the handler for the icon
        $(button.icoNormal.node).mouseup(
        function(e){
            self.fixButtons(this.myname);
            //only do this if we are the left button
            if (e.which == 1)
            {
                self.mouseUpButton(m_lsButtons[this.myname]);
            }
        });

        //create the handler for the icon
        $(button.icoHover.node).mousedown(
        function(){
            self.fixButtons(this.myname);
            self.mouseDnButton(m_lsButtons[this.myname]);
        });

        //create the handler for the icon
        $(button.icoHover.node).mouseup(
        function(e){
            self.fixButtons(this.myname);
            //only do this if we are the left button
            if (e.which == 1)
            {
                self.mouseUpButton(m_lsButtons[this.myname]);
            }
        });
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: selectionMove

        Move the selected button to the selected box. When a button is
        clicked this moves the icon and label to the top box so the
        user knows it is selected. It sets up and plays the animation.

        Parameters:
          pButton - this is the element for the button

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.selectionMove = function(pButton)
    {
        //this is the stop method for movement on the selection
        var strType = "bounce"

        //this is the stop movement for the previous selection
        var strType2 = "backOut"

        var nPrevSpeed = 100;

        //move the previous selection back
        if(m_btCurrent != null)
        {
            //move the previous item back to its original spot
            self.doAnimation(m_btCurrent.icoNormal,
                        {y: m_btCurrent.yRestore},
                        nPrevSpeed,
                        strType2);

            self.doAnimation(m_btCurrent.icoHover,
                        {y: m_btCurrent.yRestore},
                        nPrevSpeed,
                        strType2);

            self.doAnimation(m_btCurrent.icoSelect,
                        {y: m_btCurrent.yRestore},
                        nPrevSpeed,
                        strType2);

            self.doAnimation(m_btCurrent.label,
                        {y: m_btCurrent.yRestore+24},
                        nPrevSpeed,
                        strType2);

            //set the correct state for the previous item
            self.doAnimation(m_btCurrent.icoNormal,
                        {"opacity": eShow},
                        nPrevSpeed);

            self.doAnimation(m_btCurrent.icoHover,
                        {"opacity": eHide},
                        nPrevSpeed);

            self.doAnimation(m_btCurrent.icoSelect,
                        {"opacity": eHide},
                        nPrevSpeed);

            self.doAnimation(m_btCurrent.label,
                        {"fill": m_ColorNormal},
                        nPrevSpeed);

            m_btCurrent.icoNormal.toFront();
            m_btCurrent.label.toFront();
        }

        //change the current item to selected
        self.doAnimation(pButton.icoNormal,
                    {"opacity": eHide},
                    0);

        self.doAnimation(pButton.icoHover,
                    {"opacity": eHide},
                    0);

        self.doAnimation(pButton.icoSelect,
                    {"opacity": eShow},
                    0);

        self.doAnimation(pButton.label,
                    {"fill": m_ColorSelect},
                    0);

        //create a speed based on distance so that further item move
        //slow
        var speed = 500 + (200 * pButton.nIndex);

        self.doAnimation(pButton.icoSelect,
                        {y: m_posSelectTop},
                        speed,
                        strType);

        self.doAnimation(pButton.label,
                    {y: m_posSelectTop+24},
                    speed,
                    strType);
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: fadeInButton

        Fade in the selected button. This is called when you click or
        hover over a tab menu button.

        Parameters:
          pButton - this is the element for the button

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.fadeInButton = function(pButton)
    {

        //if we don't have a current selection there is nothing to fade in
        if(m_btCurrent != pButton)
        {
            self.doAnimation(pButton.icoNormal,
                        {"opacity": eHide});

            self.doAnimation(pButton.icoHover,
                        {"opacity": eShow});

            self.doAnimation(pButton.icoSelect,
                        {"opacity": eHide});

            self.doAnimation(pButton.label,
                        {"fill": m_ColorHover});
        }
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: fadeOutButton

        Fade out the selected button. This is called when you click or
        hover over a tab menu button. To create the transition from
        one state to another.

        Parameters:
          pButton - this is the element for the button

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.fadeOutButton = function(pButton)
    {
        //if this is the current button
        if(m_btCurrent == pButton)
        {
            self.doAnimation(pButton.icoNormal,
                        {"opacity": eHide});

            self.doAnimation(pButton.icoHover,
                        {"opacity": eHide});

            self.doAnimation(pButton.icoSelect,
                        {"opacity": eShow});

            self.doAnimation(pButton.label,
                        {"fill": m_ColorSelect});
        }
        else //if not the current button
        {
            self.doAnimation(pButton.icoNormal,
                        {"opacity": eShow});

            self.doAnimation(pButton.icoHover,
                        {"opacity": eHide});

            self.doAnimation(pButton.icoSelect,
                        {"opacity": eHide});

            self.doAnimation(pButton.label,
                        {"fill": m_ColorNormal});
        }

    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: mouseDnButton
        call on mouse down (do we need this?)

        Parameters:
          pButton - this is the element for the button

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.mouseDnButton = function(pButton)
    {
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: mouseUpButton
        Called after click on mouse up. This is the handler that selects
        the button. Its mostly unused now.

        Parameters:
          pButton - this is the element for the button

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.mouseUpButton = function(pButton)
    {
        //set the current select without a callaback and no animation
        self.setCurrentSelect(pButton,null,false);
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: selectNetworkItem

        When the user clicks a network item on the MyNetworkModule this is
        the function that is called to animate the icon to the selected
        box and sets up the tab bar to the appropriate stat..

        Parameters:
          strIcon   - image name of the icon for the network ite
          strName   - text name of the selected icon
          nX        - start x position
          nY        - start y position
          startSize - start size of the icon

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.selectNetworkItem = function(  strIcon,
                                        strName,
                                        nX,
                                        nY,
                                        startSize)
    {

        //create the container for this button
        var btNew = new nhButton;

        var zindex = $(m_r.canvas.parentNode).css("z-index");

        $(m_r.canvas.parentNode).css("z-index",65535);

        btNew.st = m_r.set();

        //store the index in the menu
        btNew.nIndex = 0;

        //add this button to our array
        m_lsButtons[strName] = btNew;

        //init path to icon
        var strIconPath=iconPath(strIcon);

        //if we fail
        if(strIconPath == "")
        {
            //set default
            strIconPath =  g_path.images+"UnknownDevice.png";
        }

        //put paths in filenames
        var strImage1   = strIconPath;
        var strImage2   = strIconPath;  // don't need grey
        var strImage3   = strIconPath;  // don't need selected

        //calculate the position
        var w = $("#L").width();
        var left        = nX;//w * m_LeftMargin;
        var top         = nY;//m_posMenuTop + (m_posNextItem*m_nCurrent);

        //create the icons
        btNew.icoNormal = m_r.image(strImage1, left , top, startSize, startSize).attr({opacity: eShow});
        btNew.icoNormal.node.myname = strName;
        btNew.icoNormal.myid = self.getID()
        btNew.icoHover  = m_r.image(strImage2, left , top, startSize, startSize).attr({opacity: eHide});
        btNew.icoHover.myid = self.getID();

        btNew.icoSelect = m_r.image(strImage3, left , top, startSize, startSize).attr({opacity: eHide});
        btNew.icoSelect.myid = self.getID();

        //chrome hack workaround
        fixAlpha(btNew.icoNormal,eShow);
        fixAlpha(btNew.icoHover,eHide);
        fixAlpha(btNew.icoSelect,eHide);

        btNew.yRestore = top;

        //get the icon width so we can calculate the label placement
        var nWidth      = btNew.icoNormal.getBBox().width;

        //select he font
        var attr        = {font: '16px Century Gothic, Arial', opacity: eHide};


        var strLabelSized = strName;

        if(strLabelSized.length > 20)
        {
            strLabelSized = strLabelSized.substring(0,20);
        }

        //create the text label for the button
        btNew.label     = m_r.text(left+(m_MenuIconSize*1.5), top+24, strLabelSized).attr(attr).attr({fill: m_ColorSelect}).attr({"text-anchor": "start"});

        //chrome hack workaround
        fixAlpha(btNew.label,attr.opacity);

        btNew.label.myid = self.getID();

         $(btNew.label.node).attr('unselectable', 'on').css('-moz-user-select', 'none').css('-webkit-user-select', 'none');

         $(btNew.icoNormal.node).mouseover(function() {
            $(this).css('cursor', 'pointer');});

         $(btNew.label.node).mouseover(function() {
            $(this).css('cursor', 'pointer');});

        //create the handlor for the text
        $(btNew.label.node).hover(
        function(){
            self.fadeInButton(m_lsButtons[this.textContent]);
        },
        function ()
        {
            self.fadeOutButton(m_lsButtons[this.textContent]);
        });

        //hook up the callbacks
        //self.buttonCallbacks(btNew);

        btNew.icoNormal.toFront();
        btNew.icoSelect.toFront();
        btNew.label.toFront();

        btNew.bDeleteOnUnselect = true;

        var realX = w * m_LeftMargin

        self.doAnimation(btNew.icoSelect,
                        {x: realX,y: m_posSelectTop,opacity:eShow,width:m_MenuIconSize,height:m_MenuIconSize},
                        1000,
                        "bounce");

        self.doAnimation(btNew.label,
                        {x: realX+(m_MenuIconSize*1.5),y: m_posSelectTop+24,opacity:eShow},
                        1000,
                        "bounce");

        self.setCurrentSelect(btNew);

        return btNew;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: selectNetworkItem

        Set the current button that is selected in the tab bar. This also
        plays on the correct animation. This is called when the user
        clicks on a button.

        Parameters:
          pButton   - pointer to the button class

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.setCurrentSelect = function(pButton,bCallHandler,bAnimate)
    {
        if (pButton == m_btCurrent) return;

        if (selectedTab != "")
        {
            $("#" + selectedTab).css({ color: "#FFFFFF"});

            $("#" + selectedTab).css("text-decoration", "none");
            $("#" + selectedTab).css("font-weight", "normal");
        }

        //if we have no animate variable we need to animate
        //if not check for animation
        if(bAnimate == undefined || bAnimate == true)
        {
            self.selectionMove(pButton);
        }

         $(pButton.label.node).attr('unselectable', 'on').css('-moz-user-select', 'none').css('-webkit-user-select', 'none');

         $(pButton.icoNormal.node).mouseover(function() {
            $(this).css('cursor', '');});

         $(pButton.label.node).mouseover(function() {
            $(this).css('cursor', '');});

        var Out = "";
        var In = "";
        var menuOut = "";
        var menuIn = "";

        //if there is a current selection
        if (m_btCurrent != null)
        {
            selectedTab  = "";
            var kPages = 0;
            if(m_btCurrent.plugin != undefined && m_btCurrent.plugin != null)
            {
                kPages=m_btCurrent.plugin.GetPages();
            }

            if (kPages != undefined && kPages.length > 0)
            {
                //add div for html of plugin
                for(var n = 0;n<kPages.length;n++)
                {
                    self.fadeOutIn( "#"+kPages[n].GetDiv(),"",time);
                }
            }
            else
            {
                //if we have a div
                if(m_btCurrent.StrCurrentDiv != "")
                {
                    Out = "#" + m_btCurrent.StrCurrentDiv;
                }
            }


            //if we have a menu
            if (m_btCurrent.strMenuDiv != "")
            {

                menuOut = "#" + m_btCurrent.strMenuDiv;
            }

            if(m_btCurrent.bDeleteOnUnselect==true)
            {
                m_btCurrent.icoNormal.remove();
                m_btCurrent.icoHover.remove();
                m_btCurrent.icoSelect.remove();
                m_btCurrent.label.remove();

                delete m_btCurrent;
                m_btCurrent = null;
            }
            else
            {
                //tell the plugin to deactivate
                //m_btCurrent.plugin.SetActive(false);
                $("#"+m_btCurrent.strCurrentDiv).hide();

                if(m_btCurrent.handler != undefined && m_btCurrent.handler != null)
                {
                    m_btCurrent.handler(false);
                }
            }

        }


            //if we have a menu
            if (m_strButtonMenu != "")
            {

                menuOut = "#Panel" + m_strButtonMenu;
                m_strButtonMenu = "";
            }

        //set current
        m_btCurrent = pButton;

        if(m_btCurrent.plugin!=null)
        {
            var kPages =m_btCurrent.plugin.GetPages();


            if (kPages != undefined && kPages.length > 0)
            {
                //add div for html of plugin
                var strDiv = self.makeID(kPages[0].GetDiv());

                //store the dive for later
                m_btCurrent.StrCurrentDiv = strDiv;

                In = "#" + strDiv;

                var strTabDiv = self.makeID("SubMenu" + kPages[0].GetName());

                self.selectSMTab(   pButton,
                                    strTabDiv,
                                    0);
            }
            else
            {
                //swap the div
                In = "#" + m_btCurrent.plugin.GetDiv();

                //store the dive for later
                m_btCurrent.StrCurrentDiv =  m_btCurrent.plugin.GetDiv();;
            }

            self.setTabState(pButton.plugin.m_sName);
        }
        else
        {
            if(m_btCurrent.strCurrentDiv != undefined && m_btCurrent.strCurrentDiv != "")
            In = "#" + m_btCurrent.strCurrentDiv;
        }

        //if we hav a menu
        if (m_btCurrent.strMenuDiv != "")
        {

            menuIn = "#" + m_btCurrent.strMenuDiv;

        }

        var time = 250;

        self.fadeOutIn( menuOut,menuIn,time);

        self.fadeOutIn( Out,In,time);

        //hack!!! this is blocked for d-link
        //if(m_btCurrent.strCurrentDiv != "")
        //$("#"+m_btCurrent.strCurrentDiv).show();
        //hack!!!!!

        if((bCallHandler == undefined || bCallHandler == true) && (m_btCurrent.handler != undefined && m_btCurrent.handler != null))
        {
            m_btCurrent.handler(true);
        }
    }

    this.getCurrentSelect = function()
    {
        return m_btCurrent;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: doAnimation

        Animate the selected element. This adds to an animation io queue.
        The queue animates from front to back to make sure sequence occur
        in the right order.

        Parameters:
          item      - pointer to the raphael item to animate
          animation - thing to animate. example : "{y:200}"" see raphael for details
          time      - time length for the animation
          type      - raphael animation type example "linear" or "bounce"

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.doAnimation = function(item,
                                animation,
                                time,
                                type)
    {
        var bPlay = false;
        var anim = new nhAnim;

        if(time==undefined)
        {
            time = 100;
        }

        if(type==undefined)
        {
            type = "linear";
        }

        anim.item = item;
        anim.animation = animation;
        anim.time = time;
        anim.type = type;

        if(m_animList[item.myid].length == 0)
        {
            bPlay = true;
        }

        //push the item
        m_animList[item.myid].push(anim);

        //if we have an itme in the list start the play
        if(bPlay == true)
        {
            self.animate(item.myid);
        }
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: animate

        Iterates through the buffer of an object and does each step of
        an animation sequence until the queue is empty. This is called
        and does the work for the doAnimation() function.

        Parameters:
          id - id of the item to animate in the queue

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.animate = function(id)
    {
        if(m_animList.length > 0)
        {
            var ar = m_animList[id];

            if(ar!=undefined && ar!=null && ar.length > 0)
            {
                ar[0].item.animate(ar[0].animation,ar[0].time, ar[0].type, function()
                {

                    ar[0].item.animate(ar[0].animation,0);

                    //chrome hack work around
                    //if we have opacity to the fix
                    if(typeof ar[0].animation.opacity != 'undefined')
                    {
                        fixAlpha(   ar[0].item,
                                    ar[0].animation.opacity);
                    }

                    ar.shift();

                    if(ar.length > 0)
                    {
                        self.animate(ar[0].item.myid);
                    }
                });
            }
        }
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: makeID

        Make an id for a div given a string name for something.

        Parameters:
          strName - string name for the label of a button to start with

        Returns:
          returns a unique div id

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.makeID = function(strName)
    {
        return strName.replace(/\ /g,'_').replace(/\//g,'_').replace(/:/g,'_');
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: addSubmenu

        Add a submenu to the parent plugin. This is used by the admin module
        and others to add their submenus.

        Parameters:
          parent  - parent module to add the submenu too
          strName - name of the submenu to add

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.addSubmenu = function( parent,
                                strName)
    {
        //add div for html of plugin
        var strDiv = self.makeID("SubMenu" + strName);

        var Panel = "<div id=\""+strDiv+"\" class=\"SubMenuTitle\">"+strName+": </div>";;

        //Add the panel to our right pane
        $(Panel).appendTo('#SubMenu');

        parent.strMenuDiv = strDiv;

        $("#"+strDiv).hide();
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: showSMMenu

        When a module/plugin is selected this is called to show the submenu.

        Parameters:
          strMenu  - id of the menu to show
          bShow    - true = show, false = hide

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.showSMMenu = function(strMenu,
                                bShow)
    {
        //add div for html of plugin
        var strDivMenu = self.makeID("SubMenu" + strMenu.split(' ').join('_'));

        if(bShow == true)
        {
            $("#"+strDivMenu).show();
            m_strButtonMenu = strDivMenu;
        }
        else
        {
            $("#"+strDivMenu).hide();
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: addSMButton

        add a subbutton on the top bar for the current plugin

        Parameters:
          strMenu  - id of the menu add the button to
          strName  - name of the menu button
          callback - callback to handle the button click

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.addSMButton = function(strMenu,
                                strName,
                                callback,
                                bHideTitle)
    {

    //add div for html of plugin
        var strDivMenu = self.makeID("SubMenu" + strMenu.split(' ').join('_'));

        //show the menu titles by default
        var bShowTitle = true;

        if(bHideTitle != undefined && bHideTitle == true)
        {
            bShowTitle = false;
        }

        //add div for html of plugin
        var strDiv = self.makeID("SubMenu" + strName);

        var PanelMenu = "<div id=\""+strDivMenu+"\" class=\"SubMenuTitle\">";

        //if we are supposed to show the title
        if(bShowTitle == true)
        {
            PanelMenu += strMenu+": ";
        }

        PanelMenu += "</div>";


        if($("#"+strDivMenu).length <= 0)
        {
            //Add the panel to our right pane
            $(PanelMenu).appendTo('#SubMenu');

            $("#"+strDivMenu).hide();
        }

        //add div for html of plugin
        var strDiv = self.makeID("SubItem" + strName.split(' ').join('_'));

        var Panel = "<div id=\""+strDiv+"\" class=\"SubMenuTab\">"+strName+"</div>";;

        //Add the panel to our right pane
        $(Panel).appendTo("#"+strDivMenu);

        $("#"+strDiv).mouseup(
        function(e)
        {
            //only do this if we are the left button
            if (e.which == 1)
            {
                if(callback != undefined && callback != null)
                {
                    callback();
                }
            }
            return false;
        });

        $("#"+strDiv).hover(
        function()
        {
            $( "#"+strDiv ).animate({   color: "#d8ad44"
                                    }, 200 );
        },
        function()
        {
            //set default color to white
            var strColor = "#FFFFFF";

            //get link decoration to see selection state
            var strState = $("#"+strDiv).css("text-decoration");

            //if it is underlined
            if( strState == "underline")
            {
                //restore the color to select
                strColor = "#63ADCD";
            }

            $( "#"+strDiv ).animate({   color: strColor
                                    }, 200 );
        });

    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: addSMTab

        Add a tabbutton on the top bar for the current plugin. This is
        used by the admin panel to use the submenu bar as a tab control.

        Parameters:
          parent     - id of the parent to add the tab to
          strName    - name of the menu button
          nPageIndex - index of the tab
          callback   - callback to handle the click

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.addSMTab = function(   parent,
                                strName,
                                nPageIndex,
                                callback)
    {
        //add div for html of plugin
        var strDiv = self.makeID("SubMenu" + strName);

        var Panel = "<div id=\""+strDiv+"\" class=\"SubMenuTab\">"+strName+"</div>";;

        //Add the panel to our right pane
        $(Panel).appendTo("#"+parent.strMenuDiv);

        parent.div2index[strDiv] = nPageIndex;

        $("#"+strDiv).mouseup(
        function()
        {
            self.onTab(parent,strDiv);
            return false;
        });

        $("#"+strDiv).hover(
        function()
        {
            if(selectedTab != strDiv)
            {
                $( "#"+strDiv ).animate({   color: "#d8ad44"
                                        }, 200 );
            }
        },
        function()
        {
            if(selectedTab != strDiv)
            {
                $( "#"+strDiv ).animate({   color: "#FFFFFF"
                                        }, 200 );
            }
        });
    }

    var selectedTab = "";

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: onTab

        AddSMTab click handle. This is used internall to the addSMTab function

        Parameters:
          parent     - id of the parent to add the tab to
          strDiv     - div of the button

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.onTab = function(  parent,
                            strDiv)
    {
        self.selectSMTab(   parent,
                            strDiv);
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: onTab

        When a tab button is clicked this handles the fade of the panes.
        It will animate the fade out of the old pane and fade in of the new
        one.

        Parameters:
          parent     - id of the parent to add the tab to
          strDiv     - div of the button
          bFade      - true = do fade animation for panel, false = just show

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.selectSMTab = function(parent,
                                strDiv,
                                bFade)
    {
            var strDivName = self.makeID("SubItem" + strDiv);
            $("#"+strDivName).css( "color","#63ADCD");

            $("#"+strDivName).css("font-weight", "bold");
            $("#"+strDivName).css("text-decoration", "underline");
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: addSMCombo

        Add a combobox on the top bar for the current plugin.
        Not yet implemented.

        Parameters:
         <not implemented>

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.addSMCombo = function( parent,
                                items,
                                select,
                                callback)
    {

    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: fadeOutIn

        Transition two divs. Fade one out and the other in.

        Parameters:
            Out     - the div to fade out
            In      - the div to fade in
            msTime  - the time to do the animation

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.fadeOutIn = function(  Out,
                                In,
                                msTime)
    {
        /*if(msTime != 0)
        {
            var time = msTime/2;
            if(Out != "")
            {
                $( Out ).fadeOut( time, function()
                {
                    if(In!="")
                    {
                        $( In ).fadeIn( time);
                    }
                } );
            }
            else
            {
                if(In!="")
                {
                    $( In ).fadeIn( time);
                }
            }
        }
        else*/
        {
            if (Out && Out != "") $(Out).hide();
            if (In && In != "") $(In).show();
        }

    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: initHelp

        Setup and show the help menu supply path handler

        Parameters:
          pCallback - callback to get the help to show this should return
                      the key in the tranlation js/json file

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.initHelp = function(pCallback)
    {
        //we don't already have a help menu
        if(self.help == null)
        {
            //create it and put it in the current
            self.help = new fxHelp(self.strDiv,pCallback);
        }
    };

    ///////////////////////////////////////////////////////////////////
    // member variables
    ///////////////////////////////////////////////////////////////////

    /*
        Variable: pHelp
        help menu
    */
    this.help = null;

    /*
        Variable: strDiv
        The div to draw in
    */
    this.strDiv = "";

    /*
        Variable: m_lsButtons
        The base variable where all the plugins are kept
    */
    var m_lsButtons = [];

    /*
        Variable: m_lsButtons
        The base variable where all the plugins are kept
    */
    var m_lsID2Div = [];

    /*
        Variable: m_nIndex
        The current index to insert in the array
    */
    var m_nIndex = 0;

    /*
        Variable: m_btCurrent
        The currently selected control
    */
    var m_btCurrent = null;

    /*
        Variable: m_r
        The raphael API pointer
    */
    var m_r = 0;

    /*
        Variable: m_animList
        list of items to animate
    */
    var m_animList =  new Array();

    //positioning variables
    var m_LeftMargin = .22
    var m_MenuIconSize = 48;
    var m_ColorHover = "rgb(216,173,69)";
    var m_ColorNormal = "rgb(192,192,192)";
    var m_ColorSelect = "rgb(255,255,255)";

    var m_posNextItem   = 84;
    var m_posSelectTop  = 94;
    var m_posMenuTop        = 188;

    var m_nCurrent = 0;

    var m_nGenID=0;

    var m_nIndex = 0;

    /*
        Variable: m_strButtonMenu
        the currently selected button menu if any
    */
    var m_strButtonMenu = "";


    //if they passed in parameters on the construction of this
    //objec then call initialize
    if( strDiv != undefined)
    {
        //initialize ourself
        self.initialize(strDiv,bInsertMenu);
    }
}
