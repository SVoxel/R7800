/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
function CreateElement(tag, id, text) {
    var e = document.createElement(tag);
    e.id = id;
    if (text) {
        e.appendChild(document.createTextNode(text));
    }
    return e;
}

////////////////////////////////////////////////////////////////////////
/*
    Function: node

    Internal function used by clonad to clone a node

    Parameters:
        RealName                - Readable name of the Node
        mac                     - mac address for the device
        name                    - uid name of the device
        os_type                 - type of device example: "Windows"
        os_flavor               - flavor of device example: "8.1"
        timestamp               - last time updated
        up                      - up bytes sent
        down                    - down bytes received
        user_priority           - priority of the device
        connected               - is the device connected to the network?
        epoch                   - current update time
        up_rate                 - current mbps up
        down_rate               - current mbps down
        epoch_last_change       - last time the device was update
        type                    - simple device type not os type
        ip                      - IP address of the device

    Returns:
        cloned node

    See also:
        nothing.
*/
////////////////////////////////////////////////////////////////////////
function node(  RealName,
                mac,
                name,
                os_type,
                os_flavor,
                timestamp,
                up,
                down,
                user_priority,
                static,
                connected,
                epoch,
                up_rate,
                down_rate,
                epoch_last_change,
                type,
                ip)
{
    var add = {};

    add.RealName        = RealName;
    add.mac             = mac;
    add.name            = name;

    add.os = {};
    add.os.type         = os_type;
    add.os.flaver       = os_flavor;
    add.timestamp       = timestamp;
    add.up              = up;
    add.down            = down;
    add.user_priority   = user_priority;
    add.static          = static;
    add.connected       = connected;
    add.epoch           = epoch;
    add.up_rate         = up_rate;
    add.down_rate       = down_rate;
    add.epoch_last_change  = epoch_last_change;
    add.type = type;
    add.ip = ip;

    return add;
}

////////////////////////////////////////////////////////////////////////
/*
    Function: flow

    Internal function used by clonad to clone a flow

    Parameters:
        uid                     - unique identifer of a flo
        mac                     - mac address for the flow
        name                    - name of the flow
        group                   - group id of the flow
        protocol                - protocol used by the flo
        up                      - up bytes sent
        down                    - down bytes received
        connection_count        - how many connection with the flow?
        timestamp               - last time updated
        node_priority           - priority of the flow
        pol_type                - policy type of the flo
        pol_up_active           - active up policy
        pol_up_optimal_rate     - optimal up rate
        pol_up_nominal_rate     - nominal up rate
        pol_dn_active           - active down policy
        pol_dn_optimal_rate     - optimal down rate
        pol_dn_nominal_rate     - nominal down rate
        up_rate                 - current mbps up
        down_rate               - current mbps down

    Returns:
        cloned node

    See also:
        nothing.
*/
////////////////////////////////////////////////////////////////////////

function flow(  uid,
                mac,
                name,
                group,
                protocol,
                up,
                down,
                connection_count,
                timestamp,
                node_priority,
                pol_type,
                pol_up_active,
                pol_up_optimal_rate,
                pol_up_nominal_rate,
                pol_dn_active,
                pol_dn_optimal_rate,
                pol_dn_nominal_rate,
                up_rate,
                down_rate,
                RealName)
{
    var add = {};

    add.uid                     = uid;
    add.mac                     = mac;
    add.name                    = name;
    add.group                   = group;
    add.protocol                = protocol;
    add.up                      = up;
    add.down                    = down;
    add.connection_count        = connection_count;
    add.timestamp               = timestamp;
    add.node_priority           = node_priority;

    add.policies = {
          up: {
            optimal: {},
            nominal: {}
          },
          down: {
            optimal: {},
            nominal: {}
          }
        };

    add.policies.type               = pol_type;
    add.policies.up.active          = pol_up_active;
    add.policies.up.optimal.rate    = pol_up_optimal_rate;
    add.policies.up.nominal.rate    = pol_up_nominal_rate;
    add.policies.down.active        = pol_dn_active;
    add.policies.down.optimal.rate  = pol_dn_optimal_rate;
    add.policies.down.nominal.rate  = pol_dn_nominal_rate;

    add.up_rate = up_rate;
    add.down_rate = down_rate;
    add.RealName  = RealName;

    return add;
}


////////////////////////////////////////////////////////////////////////
/*
    Function: clonad

    slow stack size clone function for flows and nodes

    Parameters:
        nodes - a list or indidual flow/node to clone

    Returns:
        cloned version of the original

    See also:
        nothing.
*/
////////////////////////////////////////////////////////////////////////
function clonad(nodes)
{
    //our cloned return
    var lsResult = []

    //if this is an element not an array
    if(typeof nodes.length == 'undefined')
    {
        //if this is a node device
        if(typeof nodes.uid == 'undefined')
        {
            lsResult = node(nodes.RealName,
                            nodes.mac,
                            nodes.name,
                            nodes.os.type,
                            nodes.os.flavor,
                            nodes.timestamp,
                            nodes.up,
                            nodes.down,
                            nodes.user_priority,
                            nodes.static,
                            nodes.connected,
                            nodes.epoch,
                            nodes.up_rate,
                            nodes.down_rate,
                            nodes.epoch_last_change,
                            nodes.type,
                            nodes.ip);
        }
        else //else we are a flow
        {
            lsResult =   flow(  nodes.uid,
                                nodes.mac,
                                nodes.name,
                                nodes.group,
                                nodes.protocol,
                                nodes.up,
                                nodes.down,
                                nodes.connection_count,
                                nodes.timestamp,
                                nodes.node_priority,
                                nodes.policies.type,
                                nodes.policies.up.active,
                                nodes.policies.up.optimal.rate,
                                nodes.policies.up.nominal.rate,
                                nodes.policies.down.active,
                                nodes.policies.down.optimal.rate,
                                nodes.policies.down.nominal.rate,
                                nodes.up_rate,
                                nodes.down_rate,
                                nodes.RealName);
        }
    }
    else //we are an array
    {
        //1st loop through and duplicate all the items
        for(var x = 0;x<nodes.length;x++)
        {
            var add = null;

            //if this is a node device
            if(typeof nodes[x].uid == 'undefined')
            {
                add = node( nodes[x].RealName,
                            nodes[x].mac,
                            nodes[x].name,
                            nodes[x].os.type,
                            nodes[x].os.flavor,
                            nodes[x].timestamp,
                            nodes[x].up,
                            nodes[x].down,
                            nodes[x].user_priority,
                            nodes[x].static,
                            nodes[x].connected,
                            nodes[x].epoch,
                            nodes[x].up_rate,
                            nodes[x].down_rate,
                            nodes[x].epoch_last_change,
                            nodes[x].type,
                            nodes[x].ip);
            }
            else //if not we are a flow
            {
                add = flow( nodes[x].uid,
                            nodes[x].mac,
                            nodes[x].name,
                            nodes[x].group,
                            nodes[x].protocol,
                            nodes[x].up,
                            nodes[x].down,
                            nodes[x].connection_count,
                            nodes[x].timestamp,
                            nodes[x].node_priority,
                            nodes[x].policies.type,
                            nodes[x].policies.up.active,
                            nodes[x].policies.up.optimal.rate,
                            nodes[x].policies.up.nominal.rate,
                            nodes[x].policies.down.active,
                            nodes[x].policies.down.optimal.rate,
                            nodes[x].policies.down.nominal.rate,
                            nodes[x].up_rate,
                            nodes[x].down_rate,
                            nodes[x].RealName);
            }

            //put result in our return cloned list
            lsResult.push(add);
        }
    }

    return lsResult;
}

////////////////////////////////////////////////////////////////////////
/*
    Class: fxNode

    Version:   1.0

    the visual display for nodes

    Parameters:
            strDivID    - id of the div to draw into
            pFlows      - fxData object for the flows
            pDevices    - fxData object for the devices
            strNodeID   - mac of the node or "router" for all

        Returns:
            nothing.

        See also:
            nothing.
*/
////////////////////////////////////////////////////////////////////////

function fxNode(strDivID,
                pFlows,
                pDevices,
                strNodeID,
                pDeviceIconLoader,
                pFlowIconLoader)
{
    var self = this;

    var showNonActive = false;

     var m_strMenu = "Device";
     this.m_sDiv = null;

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
    loadScript("BWV/BWView-2.0.js");
    loadScript("BWV/BWItem-2.0.js");
    loadScript("BWV/BWMeter-2.0.js");
    loadScript("BWV/BWSubView-2.0.js");
    loadScript("BWV/BWSubItem-2.0.js");
    loadScript("raphael-min.js");
    loadScript("GraphItem-2.0.js");
    loadScript("NodeInfo-2.0.js");
    loadScript("Line-2.0.js");
    //loadScript("Arrows-2.0.js");
    loadScript("jquery.jscrollpane.min.js");
    loadScript("jquery-ui-1.8.21.custom.min.js");

    //load the stuff to draw the graphs on the bottom
    loadScript("jquery.flot.js");
    loadScript("jquery.flot.resize.js");
    loadScript("jquery.flot.stack.js");
    loadScript("jquery.flot.pie.js");
    loadScript("jquery.flot.gant.js");
    loadScript("jquery.flot.axislabels.js");

    ////////////////////////////////////////////////////////////////////////
    //                     Base Implementation Features
    ////////////////////////////////////////////////////////////////////////

    /*
        Variable: self

        Easy access to the this of the class, to keep it separate
        from other this scopes
    */
    var self = this;

    //image loaders
    self.pDeviceIconLoader  = pDeviceIconLoader;
    self.pFlowIconLoader    = pFlowIconLoader;

    this.nodeinfo = null;

    /*
        Variable: m_sName
        English readable name of the view
    */
    this.m_sName            = "Top List Bottom Graph";


    /*
        Variable: bwview
        the view list class for the main display
    */
    this.bwview             = null;

    /*
        Variable: api
        class level access to the jscrollpane api
    */
    this.api                = null;

    /*
        Variable: nHeadRoom
        Headroom for graphs to go over sample.
        This is multiplied with the max bw of a graph.
    */
    var nHeadRoom = 1.2;


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: initialize

        Initialize the class. Called when the mynetwork module loads you.

        Parameters:
            mainApp  - Reference back to the main application

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.initialize = function( strDivID,
                                pFlows,
                                pDevices,
                                strNodeID)
    {
        self.m_sDiv = strDivID;

        self.pFlows = pFlows;

        self.pDevices = pDevices;

        self.strNodeID = strNodeID;


        if (strNodeID == "router") {
            // first prepend the header line into the display
            var hdrhtml =
                '<div class="nodeselHeader">'+
                '    <span class="nodeselComboSpan">' +
                _t("Show:") + '&nbsp;<select id="deviceselect" class="nodeselSelect"><option>' +
                _t("All Devices") + '</option><option>' +
                _t("Active Devices") + '</option></select></span>'+
                '</div>' +
                '<div id="_display"></div>';

            $("#"+self.m_sDiv).prepend(hdrhtml);

            $("#deviceselect").change(function(ev) {
                var newsel = $('#deviceselect :selected').text();
                if (newsel == _t("All Devices")) {
                    showNonActive = true;
                }
                else showNonActive = false;

                $('#deviceselect').val(newsel);
            });

            $('#deviceselect').val(showNonActive ? _t("All Devices") : _t("Active Devices"));

            self.m_sDiv = "_display";
        }

        //get the node to show
        self.node = self.createNode(strNodeID);

        // prepare the status dialog
        var strDLG = '<div style="display: none; font-size:16px;">\
            <div id="dialog-hover" title="Device Information" style = "-webkit-user-select:none;-moz-user-select:none;">\
                <div style="display: inline-table;width: 40px;margin-top: 6px;">'+_t('Status:')
                + ' </div><div style="display: inline-table;margin-top: 6px;" id="hover-name"></div><br>\
            </div>\
        </div>';

        $("#"+self.m_sDiv).append(strDLG);
        self.nodeinfo = new NodeInfo("#dialog-hover",40,120);

        //show that node
        self.show(self.node);

        self.resizeNode();

        var mywidth = self.calcWidth();             //width of  #Display
        var nWindowHeight = $(window).height();     //height of visibl browser window

        g_WorkHeightAdjuster = 210;

        if(typeof g_WorkHeightAdjuster != "undefined")
        {
            nWindowHeight -= g_WorkHeightAdjuster;
        }

        var nHeightDiff = $("#SubMenu").height()*3; //height of the top bars in our layout

        //visible height of the work panel
        var nVisibleH = nWindowHeight - nHeightDiff;

        //set the width and height of the window
        $("#DevicePanel").width(mywidth);
        $("#DevicePanel").height(nVisibleH);

        //height of the flows/nodes list 60% of visible
        var nListHeight = (nVisibleH * .6).toFixed(0);

        //set the height of the list box
        $("#lvRoundBox").height(nListHeight);
        $("#lvScrollbox").height(nListHeight);

        //calc graph sizes
        var newWidth = mywidth*.47;                         //graphs are a little less that half window width
        var newLabelWidth = (mywidth*.45) - 45;             //graph labels are a littel smaller than graphs
        var nSpaceWidth = mywidth * .05;                    //the width of the space between graph
        var nGraphHeight =  (nVisibleH * .4).toFixed(0)-35;    //height of the graphs aer 40% of the screen after 60% top list

        self.pULGraph = $.plot( $("#"+strUpDiv),self.ulData, {
                        yaxis: {min: 0, max:maxUp(), // labelWidth:nLabelWidth,
                                    tickFormatter: function(val,axis) {
                                    return (val/1000000).toFixed(2);
                                }
                            },
                            xaxes: [{
                                axisLabel: _t("seconds"),
                            }],
                                yaxes: [{
                                axisLabel: _t("Mb/s"),
                            }],
                            series: {
                                stack: stack,
                                lines: { show: lines, fill: true, steps: steps },
                                bars: { show: bars, barWidth: 0.6 }
                            },
                            height:nGraphHeight
                            });

        self.pDLGraph = $.plot( $("#"+strDnDiv),self.dlData, {
                            yaxis: {min: 0, max: maxDn(), // labelWidth:nLabelWidth,
                                tickFormatter: function(val,axis) {
                                    return (val/1000000).toFixed(2);
                                }
                            },
                            xaxes: [{
                                axisLabel: _t("seconds"),
                            }],
                            yaxes: [{
                                axisLabel: _t("Mb/s"),
                            }],
                                series: {
                                stack: stack,
                                lines: { show: lines, fill: true, steps: steps },
                                bars: { show: bars, barWidth: 0.6 }
                            },
                            height:nGraphHeight
                        });

        //hack: shoot me please.
        //this delays the 1st display until 1st draw is done
        setTimeout(function() {
            //fade in the screen
            $("#lvScrollbox").fadeIn(3000);
        },1000);
    }

    //default viewport width is 640px
    this.nMinWidth = 640;

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
        var nWidth = $("#"+self.m_sDiv).width()-g_ViewPortAdjuster;

        //if the width is less than the min
        if(nWidth<this.nMinWidth)
        {
            //set the min
            nWidth = this.nMinWidth;
        }

        return nWidth;
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: resizeNode

        When the screen resizes this is the routine called to recalc the
        layout of the window.

        Currently hooked into $(window).resize callback.

        Parameters:
            none

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.resizeNode = function()
    {
        //window width & hieght
        var mywidth = self.calcWidth();             //width of  #Display
        var nWindowHeight = $(window).height();     //height of visibl browser window

        //g_WorkHeightAdjuster = 210;

        if(typeof g_WorkHeightAdjuster != "undefined")
        {
            nWindowHeight -= g_WorkHeightAdjuster;
        }

        //height of the top bars in our layout
        var nHeightDiff = $("#SubMenu").height()*3;

        //initial percent of the screen for the graph width
        var nGraphWidth = .47;

        //if the top bar width == 0 we  are not using the steel blue theme
        //so we need to recalc width of the graphs
        //and heights of the screen
        if(nHeightDiff == 0)
        {
            nHeightDiff = 180;
            nGraphWidth = .43;
        }

        //visible height of the work panel
        var nVisibleH = nWindowHeight - nHeightDiff;

        //set the width and height of the window
        $("#DevicePanel").width(mywidth);
        $("#DevicePanel").height(nVisibleH);

        //height of the flows/nodes list 60% of visible
        var nListHeight = (nVisibleH * .87).toFixed(0);

        //set min for display list
        //set a global for this
        if(nListHeight<350)
        {
            nListHeight = 350;
        }

        //set the height of the list box
        $("#lvRoundBox").height(nListHeight);
        $("#lvScrollbox").height(nListHeight);

        //calc graph sizes
        var newWidth = mywidth*nGraphWidth;                         //graphs are a little less that half window width
        var newLabelWidth = (mywidth*.45) - 45;             //graph labels are a littel smaller than graphs
        var nSpaceWidth = mywidth * .05;                    //the width of the space between graph
        var nGraphHeight =  (nVisibleH * .4).toFixed(0)-35;    //height of the graphs aer 40% of the screen after 60% top list

        //set min of graphs
        //set a global for this
        if(nGraphHeight < 200)
        {
            nGraphHeight = 200;
        }

        //on webkit slightly smaller labels
        if( $.browser.webkit)
        {
            newLabelWidth = newWidth - 54;
        }

        var nSpacerHeight = nGraphHeight;

        //this draws very differently on msie
        if(!($.browser.webkit))
        {
            nSpacerHeight = 0;
        }

        //adjust the graph sizes
        $("#"+strDnDiv).css({'width':newWidth+"px", 'height': nGraphHeight + 'px', 'display':'inline-table'});
        $("#"+strUpDiv).css({'width':newWidth+"px", 'height': nGraphHeight + 'px', 'display':'inline-table'});

        //the space between the graphs is slightly smaller
        $("#spacer").css({'width':nSpaceWidth+"px", 'height': nSpacerHeight*.8 + 'px', 'display':'inline-table'});

        //top graph labels
        $("#GLabel1").css({'width':newLabelWidth+"px", 'display':'inline-table','margin-top':'30px','text-align':'center', 'padding-left': '50px','font-size': '14px','font-weight': 'bold'});
        $("#GLabel2").css({'width':newLabelWidth+"px", 'display':'inline-table','margin-top':'30px','text-align':'center', 'padding-left': (nSpaceWidth + 50) + 'px','font-size': '14px','font-weight': 'bold'});

        //adjust graph inside div size
        if(self.pULGraph != null)
        {
            $("#"+strUpDiv).children().css({"width":newWidth+"px"});
            $("#"+strUpDiv).children().css({"height":nGraphHeight+"px"});
        }
        if(self.pDLGraph != null)
        {
            $("#"+strDnDiv).children().css({"width":newWidth+"px"});
            $("#"+strDnDiv).children().css({"height":nGraphHeight+"px"});
        }

        $("#DevicePanel").css("display","block");
    }

    /*
        Variable: m_r
        class level raphael intstance for drawing
    */
    var m_r = null;

    /*
        Variable: routerNode
        fake router node for router viewing
    */
    this.routerNode ={
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
    /*
        Variable: computerNode
        fake router node for router viewing
    */
    this.computerNode ={
                        "uid": "Unknown",
                        "ip": "N/A",
                        "mac": "N/A",
                        "os":
                        {
                        "type": "Windows",
                        "flavor": "7 or 8",
                        },
                        "wireless": false,
                        "name": "Computer",
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
    /*
        Variable: strNodeID
        class level storage for the id of the currently displayed node
    */
    this.strNodeID = null;

    /*
        Variable: node
        full api/node info for the currently displayed node
    */
    this.node = null;

    /*
        Variable: pFlows
        fxData object for the flows info (originally: api/flows)
    */
    this.pFlows = null;

    /*
        Variable: pDevices
        fxData object for the devices info (originally: api/flows)
    */
    this.pDevices = null;

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: createNode

        Create object the node we are going to display. If it isn't the
        router itself, api/nodes is called and the info for the device
        is inserted into the node. If not a router object is created.

        Parameters:
            mainApp  - Reference back to the main application

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.createNode = function(strNodeID)
    {
        //create our svg surface in this div only
        m_r = Raphael("Display",10,10);

        var node = null;

        //are we the router??
        if(strNodeID == "router")
        {
            //make a router node and put the fake router data into it
            node = new CGraphItem(  _t("Router"),
                                    _t("The Killer 9100 Intelligent networking for the home."),
                                    "Killer.png",
                                    -1,
                                    m_nIconSize,
                                    m_nIconSize,
                                    -100,
                                    -100,
                                    1.0,
                                    m_r,
                                    self.selectNode,
                                    self.onHover,
                                    self.routerNode,
                                    null,
                                    false);
        }
        else
        {
            //make a router node and put the fake router data into it
            node = new CGraphItem(  _t("Computer"),
                                    _t("Computer"),
                                    "Unknown.png",
                                    -1,
                                    m_nIconSize,
                                    m_nIconSize,
                                    -100,
                                    -100,
                                    1.0,
                                    m_r,
                                    self.selectNode,
                                    self.onHover,
                                    self.computerNode,
                                    null,
                                    false);

        }

        //return the node
        return node;
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: kill

        Kill the class. When this is called do anything you need to and
        free everything you have.

        Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.kill = function()
    {

    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: show

        When this is called you are supposed to show the node that is given
        to this class. It should open its display and start any necessary
        timers to run.

        Parameters:
            node  - the which you are supposed to tdisplay

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.show = function(node)
    {
        //self.m_btCurrentSelect = self.m_pApp.getCurrentSelect();

        if(node.m_pNode.os.type == "Router")
        {
            self.bIsRouter = true;
        }
        else
        {
            self.bIsRouter = false;
        }

        //call the show node specific implementation
        self.showNode(node);
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: hide

        Turn off the display and shut down any unnecessary timers to
        relinquish control back to the main program.

        Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.hide = function()
    {
        self.pULGraph = null;
        self.pDLGraph = null;

        if(self.bwview!=undefined && self.bwview!=null)
        {
            self.bwview.destroy();
            delete self.bwview;
        }

        bwview = null;

        $("#"+strDiv).remove();

        $("#"+self.m_sDiv).css('z-index', 32768);

        self.selectedNode = null;
        //self.pParent.m_btCurrentSelect = null;
    }

    /////////////////////////////////////////////////////////////////////////
    //                      custom implementations
    /////////////////////////////////////////////////////////////////////////

    var labelWidth = 45;

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: showNode

        This function creates the page to display when a node is selected.
        The graph, graphkey, menu etc. Called in the selectNode() handler
        function that is registered in addDevice.

        Parameters:
            node    - the CGraphItem that was clicked

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////


    this.showNode = function(node)
    {
        var Panel = '<div id="'+strDiv+'" class="PageStyle"></div>';

        //Add the panel to our right pane
        $("#" + self.m_sDiv).append(Panel);
        var strdiv = $("#" + strDiv);

        var nListHeight = ($(window).height() * .45).toFixed(0);
        var listView = '<div id="lvRoundBox" style="clear: right;border: 2px solid #555;width:100%;margin-right: 15px;margin-left: 15px;"> <div id="lvScrollbox" style="-webkit-user-select: none;-moz-user-select: none; width: 100%;height:'+(nListHeight)+'px; overflow-y: auto;overflow-x: hidden; display:none;"></div></div>';

        //Add the panel to our right pane
        $(listView).appendTo("#"+strDiv);

        var newWidth = self.calcWidth();//;$(window).width()-$("#L").width() - 48;
        $("#DevicePanel").width(newWidth);
        $(window).height($("#L").height()-256);

        var nGraphWidth = ($("#DevicePanel").width() * .47); // .toFixed(0);
        var nSpaceWidth = ($("#DevicePanel").width() * .05); // .toFixed(0);
        var nGraphHeight =  ($("#DevicePanel").height() * .33); // .toFixed(0);

        if( $.browser.webkit)
        {
            labelWidth = 45;//49.5;
        }

        var upLabel = CreateElement("div","GLabel1",_t("Bandwidth from Devices"));
        var dnLabel = CreateElement("div","GLabel2",_t("Bandwidth to Devices"));
        var spacer = CreateElement("div","spacer",null);
        var upGraph = CreateElement("div",strUpDiv,null);
        var dnGraph = CreateElement("div",strDnDiv,null);

        strdiv.append(upLabel);
        strdiv.append(dnLabel);
        strdiv.append(upGraph);
        strdiv.append(spacer);
        strdiv.append(dnGraph);

        //create a a bwview parented to the scrollview
        this.bwview =new BWView("lvScrollbox",true,self.onSelectItem);

        $(window).unbind('resize');

        $(window).resize(function() { self.resizeNode(); } );

        //get the height and the width of the parent div
        var rX = parseInt(nGraphWidth) + parseInt($("#L").width()) + 32;

        self.pCurrentSelect = null;
        self.selectedNode = node;

        setGlobalUpDown();

        //initialize upload graph to zero
        self.ulData = [];
        for (var i = 0; i <= 59; i += 1)
            self.ulData.push([i, 0]);


        //initialize download graph to zero
        self.dlData = [];
        for (var i = 0; i <= 59; i += 1)
            self.dlData.push([i, 0]);

        setTimeout(function()
        {
            self.resizeNode();
        },100);
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getPercent

        thresholded 0-100 get percent function for download meters

        Parameters:
            max     - make value in the meter
            value   - current value of the meter

        Returns:
            percent of the max that the value is

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

        return nPercent;
    }


    this.nTestOverSub = 4;


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: addNodes

        Add the given nodes to the view. This is called by the updateRouter
        and updateData routines.

        Parameters:
            nodes   - nodes to add to the view

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.addNodes = function(nodes)
    {
        if(self.bwview!=undefined && self.bwview!=null)
        {
            //get the current list of items
            var lsCurrent = self.bwview.enumItems();


            //1st loop through and add the new items
            for(var nNode = 0;nNode<nodes.length;nNode++)
            {
                var bFound = false;

                //loop through the old items and see if its there
                for (var item in lsCurrent)
                {
                    if (item == 'indxOf')
                        continue;
                    //if this is the same item
                    if( lsCurrent[item].uid == nodes[nNode].name)
                    {
                        //if its not add it
                        bFound = true;
                        break;
                    }
                }

                self.updateTime = 1000;


                //if this isnt found then add the item
                if(bFound == false && ((this.strNodeID == nodes[nNode].mac)||(this.strNodeID == "router")))
                {
                    //re init the scroll view
                    //self.api.reinitialise();

                    self.bwview.draw();

                    var strimg = self.pFlowIconLoader(nodes[nNode.name]);


                    var nOverSub = Math.max(nodes[nNode].policies.up_profile_active,nodes[nNode].policies.down_profile_active);

                    self.bwview.addItem(nodes[nNode].name,              //unique identifier of flow/node
                                        nodes[nNode].RealName,
                                        strimg,                         //png icon in /images
                                        0,                              //perent of meter
                                        0,                              //perent of meter
                                        "0.0 mbps",                     //bw up label
                                        "0.0 mpbs",                     //bw dn label
                                        nOverSub,
                                        self);
                }

            }

            self.bwview.draw();
        }

        //re init the scroll view
        //self.api.reinitialise();

    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: delNodes

        Given the current list of nodes this routine will delete nodes that
        arent in the list.

        Parameters:
            nodes   - current list of nodes to compare the view with

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.delNodes = function(nodes)
    {
        if(self.bwview!=undefined && self.bwview!=null)
        {
            //get the current list of items
            var lsCurrent = self.bwview.enumItems();

            //loop through the view
            for (var item in lsCurrent)
            {
              if (item == 'indexOf')
                  continue;

                var bFound = false;

                //see if the item is in the nodes
                for(var nNode = 0;nNode<nodes.length;nNode++)
                {
                    //if this is the same item
                    if(lsCurrent[item].uid == nodes[nNode].name)
                    {
                        //if its not add it
                        bFound = true;
                        break;
                    }
                }

                //if not thes delete it!
                if(bFound == false)
                {
                    self.bwview.delItem(lsCurrent[item].uid);

                    //re init the scroll view
                    //self.api.reinitialise();

                }

            }
        }
    }

    this.nDLNumber = 0;
    this.nULNumber = 0;


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: updateGraphs

        Draw a historical upload / download graph for the given nodes.

        Parameters:
            nodes   - current list of nodes to graph

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.updateGraphs = function(nodes)
    {
        //test var nodeTime = new Date();

        if(self.bwview!=undefined && self.bwview!=null)
        {
            //var new node list
            var myNodes = [];

            //concat the nodes
            for(var x = 0;x<nodes.length;x++)
            {
                var id = nodes[x].name;

                if(myNodes[id] == undefined)
                {
                    //copy the node
                    myNodes[id] = clonad(nodes[x]);
                }
                else
                {
                    //add the up down values
                    myNodes[id].down_rate += nodes[x].down_rate;
                    myNodes[id].up_rate += nodes[x].up_rate;
                }
            }

            //loop the concat'ed nodes
            for(var nNode in myNodes)
            {
                var nodeTime = null;

                if (nNode == 'indexOf')
                    continue;


                if(self.bIsRouter == false)
                {
                    //create a date object of the time of the nodes last updat
                    nodeTime = new Date(0); //set to currenttime and zone
                    nodeTime.setUTCSeconds(myNodes[nNode].epoch);
                }
                else
                {
                    var node =  clonad(myNodes[nNode]);

                    //create a date object of the time of the nodes last updat
                    nodeTime = new Date(0); //set to currenttime and zone
                    nodeTime.setUTCSeconds(myNodes[nNode].epoch);
                }
                //test self.nDLNumber += Math.random()*100000;
                //test self.nULNumber += Math.random()*10000;

                var strUID = "";
                var nUP = 0;
                var nDN = 0;
                var nOverSub = undefined;
                var iconPath = "";

                //create a previous value and default to the
                //current rate if its a new flow
                var prevUp = myNodes[nNode].up_rate;
                var prevDn = myNodes[nNode].down_rate;

                if(self.bIsRouter)
                {
                    strUID = self.makeUID(myNodes[nNode].mac);
                    nUP = myNodes[nNode].up;
                    nDN = myNodes[nNode].down;
                    iconPath = self.pDeviceIconLoader(myNodes[nNode].os.type);

                    //1st loop through nodes
                    for(var x in self.prevNodes)
                    {
                        if (x == 'indexOf')
                          continue;

                        //if this is our device and flow
                        if( strUID == self.makeUID(self.prevNodes[x].mac) &&
                            myNodes[nNode].name == self.prevNodes[x].name)
                        {
                            //copy the previous values for averaging
                            prevUp = self.prevNodes[x].up_rate;
                            prevDn = self.prevNodes[x].down_rate;
                            break;
                        }
                    }
                }
                else
                {
                    strUID = myNodes[nNode].name;
                    nUP = myNodes[nNode].up;
                    nDN = myNodes[nNode].down;

                    var nOverSubUp = myNodes[nNode].policies.up.active;
                    var nOverSubDn = myNodes[nNode].policies.down.active;

                    nOverSub = Math.max(nOverSubUp,nOverSubDn);

                    iconPath = self.pFlowIconLoader(myNodes[nNode].name);

                    //1st loop through flows
                    for(var x in self.prevFlows)
                    {
                        if (x == 'indexOf')
                          continue;

                        //if this is our device and flow
                        if( strUID == self.prevFlows[x].name)
                        {
                            //copy the previous values for averaging
                            prevUp = self.prevFlows[x].up_rate;
                            prevDn = self.prevFlows[x].down_rate;
                            break;
                        }
                    }
                }

                if(self.bwview!=undefined && self.bwview!=null)
                {
                    if(isNaN(nUP))
                    {
                        nUP = 0;
                    }
                    if(isNaN(nDN))
                    {
                        nDN = 0;
                    }

                    //If this item not selected update him here. If he is selected this updates in updatesubgraphs
                    if(self.pCurrentSelect == undefined ||self.pCurrentSelect == null || self.pCurrentSelect.uid != strUID || self.bIsRouter == false)
                    {
                        //now calculate the current up/down average
                        var curUp = (myNodes[nNode].up_rate + prevUp)/2;
                        var curDn = (myNodes[nNode].down_rate + prevDn)/2;

                        //if we suprass max
                        if(curUp>maxUp(true))
                        {
                            curUp = maxUp(true);
                        }
                        //if we suprass max
                        if(curDn>maxDn(true))
                        {
                            curDn = maxDn(true);
                        }

                        var nDnPerSec = curDn * 8;
                        var nUpPerSec = curUp * 8;

                        var nUpPercent = self.getPercent(maxUp(),nUpPerSec);
                        var nDnPercent = self.getPercent(maxDn(),nDnPerSec);;

                        //update this node by info
                        this.bwview.updateItem( strUID,
                                                iconPath,
                                                nUpPercent,
                                                nDnPercent,
                                                nUpPerSec,
                                                nDnPerSec,
                                                nOverSub,
                                                myNodes[nNode].RealName);
                    }
                }
            }

            var nGraphHeight = (($("#"+this.m_sDiv).height() - 130) * .33).toFixed(0);

            var selectUID = "";

            if(self.pCurrentSelect != null && self.pCurrentSelect != undefined)
            {
                selectUID = self.pCurrentSelect.uid;
            }

            var qDataUL = self.bwview.getTotalUL(selectUID);
            var selectedUL = self.bwview.getItemUL(selectUID);

            var qDataDL = self.bwview.getTotalDL(selectUID);
            var selectedDL = self.bwview.getItemDL(selectUID);


            var dataUL = [];
            var dataDL = [];


            if(selectedUL.length == 0 && selectedDL.length == 0 )
            {
                selectUID = null;
            }

            if(selectUID != "" && selectUID != null && selectUID != undefined)
            {
                if(selectedUL.length == 0 || selectedDL.length == 0 )
                {
                    dataUL = [{data:qDataUL,color:'#888888'}];
                    dataDL = [{data:qDataDL,color:'#015b86'}];
                }
                else
                {
                    dataUL = [{data:selectedUL,color:'#ffffff'},{data:qDataUL,color:'#888888'}];
                    dataDL = [{data:selectedDL,color:'#ffffff'},{data:qDataDL,color:'#015b86'}];
                }
            }
            else
            {
                dataUL = [{data:qDataUL,color:'#888888'}];
                dataDL = [{data:qDataDL,color:'#015b86'}];
            }

            self.pULGraph = $.plot( $("#"+strUpDiv),dataUL, {
                            yaxis: {min: 0, max:maxUp()*nHeadRoom, // labelWidth:nLabelWidth,
                                        tickFormatter: function(val,axis) {
                                        return (val/1000000).toFixed(2);
                                    }
                                },
                                xaxes: [{
                                    axisLabel: _t("seconds"),
                                }],
                                    yaxes: [{
                                    axisLabel: _t("Mb/s"),
                                }],
                                series: {
                                    stack: stack,
                                    lines: { show: lines, fill: true, steps: steps },
                                    bars: { show: bars, barWidth: 0.6 }
                                },
                                height:nGraphHeight
                                });

            self.pDLGraph = $.plot( $("#"+strDnDiv),dataDL, {
                                yaxis: {min: 0, max: maxDn()*nHeadRoom, // labelWidth:nLabelWidth,
                                    tickFormatter: function(val,axis) {
                                        return (val/1000000).toFixed(2);
                                    }
                                },
                                xaxes: [{
                                    axisLabel: _t("seconds"),
                                }],
                                yaxes: [{
                                    axisLabel: _t("Mb/s"),
                                }],
                                    series: {
                                    stack: stack,
                                    lines: { show: lines, fill: true, steps: steps },
                                    bars: { show: bars, barWidth: 0.6 }
                                },
                                height:nGraphHeight
                            });
        }
    }

    this.isActiveNode = function(n) {

        if (showNonActive)
            return true;
        else
            return isActiveNode(n);
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: updateData

        Gets the latest list of nodes/flows and draw all the graphs and
        meters

        Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.updateData = function()
    {
        $.ajaxSetup({ cache: false });

        //if(self.m_btCurrentSelect!=null)
        {
            if(self.bIsRouter)
            {
                self.updateRouter();
            }
            else
            {
                self.updateNode();
            }

        }

    }

    //original flows and nodes data
    this.flows = [];
    this.nodes = [];

    //secondary flows and nodes data for averaging values
    this.prevNodes = [];
    this.prevFlows = [];

    this.updateFlows = function(flows)
    {
        //copy the current to keep for averaging
        self.prevFlows = clonad(self.flows);

        //copy the new
        self.flows = clonad(flows);
    }

    this.updateNodes = function(nodes)
    {
        //copy the current to keep for averaging
        self.prevNodes = clonad(self.nodes);

        //copy the new
        self.nodes = clonad(nodes);

        //do the drawing
        self.updateData();
    }

    this.updateTime = g_nShortTimerM;


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: updateNode

        Gets the latest list of flows and draw all the graphs and
        meters. Used internally by updateData() for when a specific
        device is selected.

        Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.updateNode = function()
    {
        //var data = self.pFlows.getData();

        //add the new nodes
        self.addNodes(self.flows);

        //delete the missing nodes
        self.delNodes(self.flows);

        //now update the graphs and meters
        self.updateGraphs(self.flows);

        //if we are still selected
        //setTimeout(self.updateData,self.updateTime);
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: updateRouter

        Gets the latest list of flows and draw all the graphs and
        meters. Used internally by updateData() for when the router
        is selected.

        Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.updateRouter = function()
    {
        //var data = self.pDevices.getData();//g_kRouter.GetData("devices");

        if(self.nodes!=undefined)
        {
            //add the new nodes
            self.addDevices(self.nodes);

            //delete the missing nodes
            self.delDevices(self.nodes);

            //add subItems
            self.updateSubItems(self.nodes);

            //now update the graphs and meters
            self.updateGraphs(self.nodes);

        }

        //if we are still selected
        //setTimeout(self.updateData,self.updateTime );
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: updateSubItems

        Gets the latest list of flows and draw all the graphs and
        meters. Used internally by updateData() for when the router
        is selected. This is specifically just the subview flows meters.

        Parameters:
            nodes - the nodes to update  flows for

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.updateSubItems = function(nodes)
    {
        //get the current list of items
        var lsCurrent = self.bwview.enumItems();

        //var data = self.pFlows.getData();

        //1st loop through and add the new items
        for(var nNode = 0;nNode<nodes.length;nNode++)
        {
            //if the node is active add its subitems
            if(self.isActiveNode(nodes[nNode]))
            {
                //create the variable we need to hold our subset
                var flows = [];

                //loop through the flows
                for(var index in self.flows)
                {
                    if (index == 'indexOf')
                      continue;

                    //make sure the mac is defined in both
                    if(self.flows[index].mac != undefined && nodes[nNode].mac != undefined)
                    {
                        //if the mac matches the flow and node then add to our data set
                        if(self.flows[index].mac == nodes[nNode].mac)
                        {
                            flows.push(self.flows[index]);
                        }
                    }
                }

                //add the new nodes
                self.addSubItems(nodes[nNode],flows);

                //delete the missing nodes
                self.delSubItems(nodes[nNode],flows);

                self.updateSubGraphs(nodes[nNode],flows);
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: updateSubGraphs

        Used internally by update subitems when the router is selected.
        Draws the meteres and bottom graphs.

        Parameters:
            nodes - the nodes to update  flows for

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.updateSubGraphs = function(node,flows)
    {
        var strUID = self.makeUID(node.mac);

        //add variables to run totals
        var nTotalUp = 0;
        var nTotalDn = 0;

        //node time from last flow
        var lastTime = null;

        if(self.pCurrentSelect!= undefined && self.pCurrentSelect!= null && self.pCurrentSelect.uid  == strUID)
        {
            //var new node list
            var myFlows = [];

            //concat the flows
            for(var x = 0;x<flows.length;x++)
            {
                var id = flows[x].name;

                if(myFlows[id] == undefined)
                {
                    //copy the node
                    myFlows[id] = clone(flows[x]);
                }
                else
                {
                    myFlows[id].down_rate += flows[x].down_rate;
                    myFlows[id].up_rate += flows[x].up_rate;
                }
            }

            //1st loop through flows
            for(var nFlow in myFlows)
            {
                if (nFlow == 'indexOf')
                  continue;

                var flowTime = null;

                //create a date object of the time of the nodes last updat
                flowTime = new Date(0); //set to currenttime and zone
                flowTime.setUTCSeconds(myFlows[nFlow].epoch);

                var strUID = self.makeUID(node.mac);

                if(self.bwview != undefined && self.bwview!=null)
                {
                    var nOverSubUp = myFlows[nFlow].policies.up.active;
                    var nOverSubDn = myFlows[nFlow].policies.down.active;

                    nOverSub = Math.max(nOverSubUp,nOverSubDn);

                    //create a previous value and default to the
                    //current rate if its a new flow
                    var prevUp = myFlows[nFlow].up_rate;
                    var prevDn = myFlows[nFlow].down_rate;

                    //1st loop through flows
                    for(var x in self.prevFlows)
                    {
                        if (x == 'indexOf')
                          continue;

                        //if this is our device and flow
                        if( strUID == self.makeUID(self.prevFlows[x].mac) &&
                            myFlows[nFlow].name == self.prevFlows[x].name)
                        {
                            //copy the previous values for averaging
                            prevUp = self.prevFlows[x].up_rate;
                            prevDn = self.prevFlows[x].down_rate;

                            break;
                        }
                    }

                    //now calculate the current up/down average
                    var curUp = (myFlows[nFlow].up_rate + prevUp)/2;
                    var curDn = (myFlows[nFlow].down_rate + prevDn)/2;

                    //if we suprass max
                    if(curUp>maxUp(true))
                    {
                        curUp = maxUp(true);
                    }
                    //if we suprass max
                    if(curDn>maxDn(true))
                    {
                        curDn = maxDn(true);
                    }

                    var nUpPercent = self.getPercent(maxUp(),(curUp*8));
                    var nDnPercent = self.getPercent(maxDn(),(curDn*8));

                    //update this node by info
                    this.bwview.updateSubItem(  strUID,
                                                myFlows[nFlow].name,
                                                self.pFlowIconLoader(myFlows[nFlow].name),
                                                nUpPercent,
                                                nDnPercent,
                                                curUp * 8,
                                                curDn * 8,
                                                nOverSub,
                                                myFlows[nFlow].RealName);

                    //sum up the totals for the selected item
                    nTotalUp += curUp;
                    nTotalDn += curDn;

                    //do we have valid up?
                    if(isNaN(nTotalUp))
                    {
                        nTotalUp = 0;
                    }

                    //do we have valid down?
                    if(isNaN(nTotalDn))
                    {
                        nTotalDn = 0;
                    }

                    //record the last time sample for this node
                    lastTime = flowTime;
                }
            }

            //if we suprass max
            if(nTotalUp>maxUp())
            {
                nTotalUp = maxUp();
            }
            //if we suprass max
            if(nTotalDn>maxDn())
            {
                nTotalDn = maxDn();
            }

            var nUpPercent = self.getPercent(maxUp(),(nTotalUp*8));
            var nDnPercent = self.getPercent(maxDn(),(nTotalDn*8));

            var iconPath = self.pDeviceIconLoader(node.os.type);

            //update this node by info
            this.bwview.updateItem( strUID,
                                    iconPath,
                                    nUpPercent,
                                    nDnPercent,
                                    nTotalUp * 8,
                                    nTotalDn * 8,
                                    nOverSub);
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: addSubItems

        Used internally by update subitems when the router is selected.
        Adds new items to the subviews of selected nodes.

        Parameters:
            node  - the selected node
            flows - flows for the given node

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.addSubItems = function(node,flows)
    {

        var strUID = self.makeUID(node.mac);

        if(self.bwview != null)
        {
            //get the current list of items
            var lsCurrent = self.bwview.enumSubItems(strUID);


            //1st loop through and add the new items
            for(var nFlow = 0;nFlow<flows.length;nFlow++)
            {
                var bFound = false;

                //loop through the old items and see if its there
                for (var item in lsCurrent)
                {
                    //if this is the same item
                    if(lsCurrent[item].uid == flows[nFlow].name)
                    {
                        //if its not add it
                        bFound = true;
                        break;
                    }
                }


                //if this isnt found then add the item
                if(bFound == false)
                {
                    var strimg = self.pFlowIconLoader(flows[nFlow].name);;

                    if(self.bwview != null)
                    {

                        var nOverSub = Math.max(flows[nFlow].policies.up_profile_active,flows[nFlow].policies.down_profile_active);

                        self.bwview.addSubItem( strUID,
                                                flows[nFlow].name,   //unique identifier of flow/node
                                                flows[nFlow].RealName,   //translated name
                                                strimg,                 //png icon in /images
                                                0,                      //perent of meter
                                                0,                      //perent of meter
                                                "0.0 mbps",                     //bw up label
                                                "0.0 mpbs",                     //bw dn label
                                                nOverSub,
                                                self);
                    }
                }
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: delSubItems

        Used internally by update subitems when the router is selected.
        Deletes old items in the subviews of selected nodes.

        Parameters:
            node  - the selected node
            flows - flows for the given node

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.delSubItems = function(node,flows)
    {

        var strUID = self.makeUID(node.mac);


        if(self.bwview != null)
        {
            //get the current list of items
            var lsCurrent = self.bwview.enumSubItems(strUID);

            //loop through the view
            for (var item in lsCurrent)
            {
                var bFound = false;

                //see if the item is in the nodes
                for(var nFlow = 0;nFlow<flows.length;nFlow++)
                {
                    //if this is the same item
                    if(lsCurrent[item].uid == flows[nFlow].name)
                    {
                        //if its not add it
                        bFound = true;
                        break;
                    }
                }

                //if not thes delete it!
                if(bFound == false)
                {

                    if(self.bwview != null)
                    {
                        self.bwview.delSubItem( strUID,
                                                lsCurrent[item].uid);
                    }

                    //re init the scroll view
                    //self.api.reinitialise();
                }

        }
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: addDevices

        Used internally by update nodes when the router is selected.
        Adds the top level devices to the list

        Parameters:
            nodes  - nodes to look for new items in

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.addDevices = function(nodes)
    {
        if(self.bwview!=undefined && self.bwview!=null)
        {
            //get the current list of items
            var lsCurrent = self.bwview.enumItems();


            //1st loop through and add the new items
            for(var nNode = 0;nNode<nodes.length;nNode++)
            {
                var bFound = false;

                //loop through the old items and see if its there
                for (var item in lsCurrent)
                {
                    var strTestID = self.makeUID(nodes[nNode].mac);

                    var bShow = true;

                    //if this is the same item
                    if(lsCurrent[item].mac == strTestID || bShow == false)
                    {
                        //if its not add it
                        bFound = true;
                        break;
                    }
                }

                self.updateTime = 1000;

                //if this isnt found then add the item and the node is active
                if(bFound == false && self.isActiveNode(nodes[nNode]))
                {
                    var icon = self.pDeviceIconLoader(nodes[nNode].os.type);

                    var strUID = self.makeUID(nodes[nNode].mac);

                    var strLabelName = "Unknown";

                    if( self.bIsRouter == true)
                    {
                        strLabelName = makeNodeName(nodes[nNode]);
                    }
                    else
                    {
                        strLabelName = nodes[nNode].name;
                    }


                    self.bwview.addItem(strUID,     //unique identifier of flow/node
                                        strLabelName,                   //label of item
                                        icon,                               //png icon in /images
                                        0,                                  //perent of meter
                                        0,                                  //perent of meter
                                        "0.0 mbps",                         //bw up label
                                        "0.0 mpbs",
                                        null,
                                        self);                        //bw dn label
                }
            }

            self.bwview.draw();
        }
    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: delDevices

        Used internally by update nodes when the router is selected.
        Deletse old top level devices in the list

        Parameters:
            nodes  - nodes to look for old items in

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.delDevices = function(nodes)
    {
        if(self.bwview!=undefined && self.bwview!=null)
        {
            //get the current list of items
            var lsCurrent = self.bwview.enumItems();

            //loop through the view
            for(var item in lsCurrent)
            {
                if (item == 'indexOf')
                    continue;

                var bFound = false;

                //see if the item is in the nodes
                for(var nNode = 0;nNode<nodes.length;nNode++)
                {
                    var strUID = self.makeUID(nodes[nNode].mac);

                    //if this is the same item
                    if(lsCurrent[item].mac == strUID)
                    {
                        //if its not add it
                        bFound = true;
                        break;
                    }
                }

                //if not thes delete it!
                if(bFound == false)
                {
                    self.bwview.delItem(lsCurrent[item].mac);

                    //re init the scroll view
                    //self.api.reinitialise();
                }

            }
        }
    }

    //this is teh function to test the graphs
    this.testUpdateData = function()
    {
        var nGlobalUL = 0;
        var nGlobalDL = 0;

        //walk the items in the arrayw
        for(var item in self.bwview.lsItem)
        {
            if (item == 'indexOf')
                continue;

            var upRange = 1;
            var dnRange = 1;

            var upDigits = (Math.random()*7);

            upRange = Math.pow(10,upDigits);

            var dnDigits = (Math.random()*8);

            var dnRange = Math.pow(10,dnDigits);

            var Up = (Math.min(upRange,maxUp()/10)).toFixed(0);//self.randomInRange(1,upRange);
            var Dn = (Math.min(dnRange,maxDn()/10)).toFixed(0);//self.randomInRange(1,dnRange);

            nGlobalUL += parseInt(Up);
            nGlobalDL += parseInt(Dn);

            var UpPercent = self.getPercent(maxUp(),Up);
            var DnPercent = self.getPercent(maxDn(),Dn);

            self.bwview.updateItem( self.bwview.lsItem[item].mac,
                                    UpPercent,
                                    DnPercent,
                                    Up,
                                    Dn);
        }

        self.ulData.splice(0,1);
        self.dlData.splice(0,1);

        for (var i = self.dlData.length - 1; i >= 0; i--) {
            self.dlData[i][0] = i;
        };
        for (var i = self.dlData.length - 1; i >= 0; i--) {
            self.ulData[i][0] = i;
        };


        if(nGlobalUL>=maxUp())
            nGlobalUL = maxUp()*.95;

        if(nGlobalDL>=maxDn())
            nGlobalDL=maxDn()*.95;;

        self.ulData.push([59, nGlobalUL]);
        self.dlData.push([59, nGlobalDL]);


        var nGraphHeight = (($("#"+this.m_sDiv).height() - 130) * .33).toFixed(0);

        var selectUID = "";

        if(self.pCurrentSelect != null && self.pCurrentSelect != undefined)
        {
            selectUID = self.pCurrentSelect.uid;
        }

        var qDataUL = self.bwview.getTotalUL(selectUID);
        var selectedUL = self.bwview.getItemUL(selectUID);

        var qDataDL = self.bwview.getTotalDL(selectUID);
        var selectedDL = self.bwview.getItemDL(selectUID);


        var dataUL = [];
        var dataDL = [];

        if(selectedUL.length == 0 && selectedDL.length == 0 )
        {
            selectUID = null;
        }

        if(selectUID != "" && selectUID != null && selectUID != undefined)
        {
            dataUL = [{data:selectedUL,color:'#ffffff'},{data:qDataUL,color:'#888888'}];
            dataDL = [{data:selectedDL,color:'#ffffff'},{data:qDataDL,color:'#015b86'}];
        }
        else
        {
            dataUL = [{data:qDataUL,color:'#888888'}];
            dataDL = [{data:qDataDL,color:'#015b86'}];
        }


        self.pULGraph = $.plot( $("#"+strUpDiv),dataUL, {
                            yaxis: {min: 0, max:maxUp(),labelWidth:nLabelWidth},
                            series: {
                                stack: stack,
                                lines: { show: lines, fill: true, steps: steps },
                                bars: { show: bars, barWidth: 0.6 }
                            }
                        });

        self.pDLGraph = $.plot( $("#"+strDnDiv),dataDL, {
                            yaxis: {min: 0, max: maxDn(),labelWidth:nLabelWidth},
                            series: {
                                stack: stack,
                                lines: { show: lines, fill: true, steps: steps },
                                bars: { show: bars, barWidth: 0.6 }
                            }
                        });

        //setTimeout(self.testUpdateData,g_nShortTimerM);
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: onSelectItem

        When an item in the list is clicked on this is the function called.

        Parameters:
            pCurrentSelect - item that was clicked on

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.onSelectItem = function(pCurrentSelect)
    {
        //re init the scroll view
        //self.api.reinitialise();

        self.pCurrentSelect = pCurrentSelect;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: makeUID

        Make an id for a div given a string name for something.

        Parameters:
          strName - string name for the label of a button to start with

        Returns:
          returns a unique div id

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.makeUID = function(strName)
    {
        var strSpace  = "";
        var strPeriod = "";
        var strColon = "";

        if(strName != undefined)
        {
            strSpace = strName.replace(/\ /g,'_');
            strPeriod = strSpace.replace(/\./g,'_');
            strColon = strPeriod.replace(/\:/g,'_');
        }

        return strColon;
    }

    /////////////////////////////////////////////////////////////////////////
    //                          local variables
    /////////////////////////////////////////////////////////////////////////

    /*
        Variable: strUpDiv

        div id for the scroll view of upload graph
    */
    var strUpDiv    = "devUpload";

    /*
        Variable: strDnDiv

        div id for the scroll view of download graph
    */
    var strDnDiv    = "devDownload";

    /*
        Variable: strDiv

        div id for the scroll view of devices/flows
    */
    var strListDiv  = "devListView";

    /*
        Variable: strDiv

        name of the div for this page
    */
    var strDiv = "DevicePanel";

    /*
        Variable: selectedNode

        the node that was clicked on to create this page
    */
    this.selectedNode = null;

    /*
        Variable: pCurrentSelect

        this is a the currently selected flow or subvie of this item
        used for highlighting the graph to show the bandwidth
        of the selected item
    */
    this.pCurrentSelect = null

    /*
        Variable: stack

        Stack data in the graph true/false
    */
    var stack = true;

    /*
        Variable: bars

        bar display in the graph true/false
    */
    var bars = false;

    /*
        Variable: lines

        line display in the graph true/false
    */
    var lines = true;

    /*
        Variable: steps

        step display in the graph true/false
    */
    var steps = false;

    /*
        Variable: ulData

        Data array for the upload graph
    */
    this.ulData = [];

    /*
        Variable: dlData

        Data array for the download graph
    */
    this.dlData = [];

    /*
        Variable: dlData

        Data array for the download graph
    */
    this.ulData = [];

    /*
        Variable: nLabelWidth

        The width of the y axis labels on the graph and the padding offset for
        the graph labels
    */
    var nLabelWidth = 50;

    /*
        Variable: pULGraph

        Upload graph at bottom of page
    */
    this.pULGraph = null;

    /*
        Variable: pDLGraph

        Download graph at bottom of page
    */
    this.pDLGraph = null;

    /*
        Variable: bIsRouter

        is the currently selected device the router??
    */
    this.bIsRouter = false;

    this.setup = function() {
        // initialize everything -- so we can restart this object/page
        m_strMenu = "Device";
        self.m_sDiv = null;
        self.m_sName            = "Top List Bottom Graph";
        self.bwview             = null;
        self.api                = null;

        self.nMinWidth = 640;
        m_r = null;
        self.strNodeID = null;
        self.node = null;
        self.pFlows = null;
        self.pDevices = null;

        labelWidth = 45;

        self.nTestOverSub = 4;
        self.nDLNumber = 0;
        self.nULNumber = 0;
        self.updateTime = g_nShortTimerM;
        self.lsForwardDeletes = [];

        lastsel = null;
        lastedit = null;
        changedRows = new Array();
        strUpDiv    = "devUpload";
        strDnDiv    = "devDownload";
        strListDiv  = "devListView";
        strDiv = "DevicePanel";
        self.selectedNode = null;
        self.pCurrentSelect = null
        stack = true;
        bars = false;
        lines = true;
        steps = false;
        self.ulData = [];
        self.dlData = [];
        self.ulData = [];
        nLabelWidth = 50;
        self.pULGraph = null;
        self.pDLGraph = null;

        self.bIsRouter = false;

        //if they passed in parameters on the construction of this
        //objec then call initialize
        if( strDivID != undefined && strNodeID != undefined)
        {
            //initialize ourself
            self.initialize(strDivID,pFlows,pDevices,strNodeID);
        }
    }
    this.setup();
}
