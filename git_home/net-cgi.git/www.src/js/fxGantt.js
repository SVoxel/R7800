/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
////////////////////////////////////////////////////////////////////////
/*
    Class: fxGantt

    Version:   1.0

    Simplified display wrapper for the flot gantt chart
*/
////////////////////////////////////////////////////////////////////////

function fxGantt(strDiv,
                vData,
                vOptions)
{
    var self = this;

    var evplot = null;
    var plotOptions;

    /////////////////////////////////////////////////////////////////////////
    /*
        The magic to load our pre-requisite scripts in javascript
    */
    /////////////////////////////////////////////////////////////////////////
    loadScript("utility-2.0.js");
    loadScript("jquery.flot.js");
    //loadScript("jquery.flot.resize.js");
    loadScript("jquery.flot.stack.js");
    loadScript("jquery.flot.pie.js");
    loadScript("jquery.flot.gant.js");
    loadScript("jquery.flot.axislabels.js");
    loadScript("jquery.flot.time.js");
    loadScript("fxTranslate-2.0.js");

    //tell the trans library to look here and load the appropriate files.
    loadTranslation();


    this.strDiv = strDiv;
    this.vOptions = vOptions;
    this.vData = vData;

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: initialize

        Initialize the class. Called when the mynetwork module loads you.

        Parameters:
            strDiv      - div to draw the gant into
            vOptions    - custom flot gantt chart option see flot for details

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.initialize = function( strDiv,
                                vOptions)
    {
        $(window).resize(function(ev) {

            //on a window resize wait for things to settle
            //for 400 ms
            setTimeout(function()
            {
                //draw the graph
                self.draw(vData,true);

                //this is for maximize as it can
                //take 2 attempts to redraw correctly
                //its not cool, but always to works
                setTimeout(function()
                {
                    //draw the graph
                    self.draw(vData,true);
                },
                400);
            },
            400);
        });
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: draw

        Draw the graph. Internally this also parses the raw data from the
        fxStats.getEvents function.

        Parameters:
            arData - array data from the raw fxStats.getEvents function

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.draw = function(arData, dobind)
    {
        //on a redraws clear the display
        $('#Display').html('');

        //figure out how big we want our graph to be
        var nWidth =  $(self.strDiv).parent().width() - 100;
        var nHeight =  $(self.strDiv).parent().height() - $("TopRight").height() - $("SubMenu").height() - 300;

        //set the display the size of our graph
        $("#Display").width(nWidth);
        $("#Display").height(nHeight);

        //turn the raw data into something the graph can handle
        var vData = arData; // self.parseData(arData);
        var vLabelArray = [];

        //create the label array for the y axis ticks
        for(var n = 0;n<self.vLabels.length;n++)
        {
            vLabelArray.push([n,self.vLabels[n]]);
        }

        //default options
        if (!plotOptions) {
            plotOptions = {
                series: { gantt: { active: true, show: true, barHeight: .5 } }
                ,xaxis:  {   min: self.startTime,
                            max: self.endTime,
                            mode: "time",
                            timezone: "browser",
                            color:"#000000",
                            axisLabel: _t('Time'),
                            axisLabelUseCanvas: true,
                            axisLabelFontSize: 20,
                            axisLabelFontFamily: 'Arial'}
                ,yaxis:  {  min: -1,
                            max: vLabelArray.length,
                            ticks: vLabelArray,
                            color:"#000000",
                            axisLabel: _t('Applications'),
                            axisLabelUseCanvas: true,
                            axisLabelFontSize: 20,
                            axisLabelFontFamily: 'Arial'}
                ,grid: {
                    hoverable: true,
                    clickable: true,
                    axisMargin: 0
                }
                ,legend: {
                    show: true,
                    margin: [80,10]
               }
        };
        }

        //create the graph
        evplot = $.plot($(self.strDiv),  [
                                    { label: _t("Optimal"), data: vData[0], color: "#00FF00", gantt: { connectSteps: { show: false }, drawstep: addHover } },
                                    { label: _t("Good"), data: vData[1], color: "#FFFF00", gantt: { connectSteps: { show: false }, drawstep: addHover } },
                                    { label: _t("Best Effort"), data:  vData[2], color: "#FF9F00", gantt: { connectSteps: { show: false }, drawstep: addHover } }
                                ],
               plotOptions);

        //internal function for the hover
        function addHover(ctx,series,data,x,y,x2,color, isHighlight)
        {
                ctx.beginPath();
                ctx.lineWidth = series.gantt.barheight;
                ctx.strokeStyle = data[4];
                ctx.moveTo(x, y);
                ctx.lineTo(x2, y);
                ctx.stroke();
        }

        //remember the last point to fade it
        var previousPoint = null;

        if (dobind) {
            //tooltip display used by the hover routine
            function showTooltip(x, y, contents)
            {
                $('<div id="tooltip">' + contents + '</div>').css(
                {
                    position: 'absolute',
                    display: 'none',
                    top: y + 5,
                    left: x + 5,
                    border: '1px solid #fdd',
                    padding: '2px',
                    'background-color': '#fee',
                    opacity: 0.80,
                    'z-index':75000
                }).appendTo("body").fadeIn(200);
            }

            //this is the on hover binder for each bar used by the above graph
            $(self.strDiv).bind("plothover", function (event, pos, item)
            {
                if (item)
                {
                    if (previousPoint != item.datapoint)
                    {
                        previousPoint = item.datapoint;

                        $("#tooltip").remove();

                        showTooltip(pos.pageX+16,
                                    pos.pageY-8,
                                    item.series.label + "(" + item.datapoint[3] + ")");
                    }
                }
                else
                {
                    $("#tooltip").remove();
                    previousPoint = null;
                }
            });
        }

        //adjust the labels to not bump into our graph
        $(self.strDiv).find("#yaxisLabel").css("left","-40px");
        $(self.strDiv).find("#xaxisLabel").css("padding-top","20px");

        //set the legend up to have black borders and fonts
        $(self.strDiv).find("table:first").children().css("color","black");
        $(self.strDiv).find("table:first").children().find("legendColorBox:first").css("border","1px solid black");
        $(".legend").css("width","150px");
        $(".legend").find("div:first").css("width","150px");
        $(".legend").find("table:first").css("width","150px");
        $(".legendColorBox").css("width","10px");
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: parseData

        Process the data and for the graph and convert it into a flot format.

        Parameters:
            arData - array data from the raw fxStats.getEvents function

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.parseData = function(arData)
    {
        var vData =[];
        //temp data ie opened but not closed
        var vTemp = [];

        //walk the array of arrays
        for(var index = 0;index<arData.length;index++)
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
                    var strTime = (arData[index][0]).replace(/\s+/g, ' ').replace(/\s+/g, '\n').replace(/\s+/g, '\r');
                    var vTime = strTime.split(".");
                    var strata = Math.max(arData[index][5],arData[index][6]);


                    //if this doesnt exist
                    if(vTemp[nID] == undefined)
                    {
                        vTemp[nID] = new dbItem;
                        vTemp[nID].strName = flowName(strName);

                        vTemp[nID].timeOpen = new Date(parseInt(vTime[0])*1000 + g_routerDeltaGMT);

                        vTemp[nID].strata = strata;

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

                        var strTime = (arData[index][0]).replace(/\s+/g, ' ').replace(/\s+/g, '\n').replace(/\s+/g, '\r');
                        var vTime = strTime.split(".");

                        vTemp[nID].nUpload   = parseInt(arData[index][3]);
                        vTemp[nID].nDownload = parseInt(arData[index][4]);

                        vTemp[nID].timeClose = new Date(parseInt(vTime[0])*1000 + g_routerDeltaGMT);

                        if(self.endTime == null)
                        {
                            self.endTime = vTemp[nID].timeClose;
                        }
                        else if(self.endTime < vTemp[nID].timeClose)
                        {
                            self.endTime = vTemp[nID].timeClose;
                        }

                        //put this is the done category
                        vData.push(vTemp[nID]);

                        //delete it out the temp data
                        delete vTemp[nID];
                    }
                    else
                    {
                        //alert("close no open");
                    }
                }
                else if(arData[index][1]=="oversub")
                {
                    //if this doesnt exist
                    if(vTemp[nID] != undefined)
                    {

                        var osub = new dbOverSub;
                        osub.strata = arData[index][4];


                        var strTime = (arData[index][0]).replace(/\s+/g, ' ').replace(/\s+/g, '\n').replace(/\s+/g, '\r')
                        var vTime = strTime.split(".");

                        osub.time = new Date(parseInt(vTime[0])*1000 + g_routerDeltaGMT);

                        if(vTemp[nID].vOverSub == undefined || vTemp[nID].vOverSub == null)
                        {
                            vTemp[nID].vOverSub = [];
                        }

                        vTemp[nID].vOverSub.push(osub);
                    }
                    else
                    {
                        //alert("oversub no open");
                    }
                }
            }
        }

        var vReturn = [];

        self.vLabels = self.getLabels(vData);

        vReturn[0] = [];
        vReturn[1] = [];
        vReturn[2] = [];
        vReturn[3] = [];

        //name 2 index of the display
        var vName2Index = [];

        //create a name to index lookup for the data routine
        //so we can tell each piece of data what row to be in
        for(var n in self.vLabels)
        {
            vName2Index[self.vLabels[n]] = n;
        }

        for(var item in vData)
        {
            var timeOpen = vData[item].timeOpen;
            var timeClose = vData[item].timeClose;

            //add the guy to the end of our return
            vReturn[parseInt(vData[item].strata)].push([timeOpen,parseInt(vName2Index[vData[item].strName]),timeClose,vData[item].strName]);

            //if we have oversub data we need to subdivide this
            if(vData[item].vOverSub!=undefined)
            {
                for(var n = 0;n<vData[item].vOverSub.length;n++)
                {
                    var close = null;
                    var open = vData[item].vOverSub[n].time;

                    if(vData[item].vOverSub[n+1] != undefined)
                    {
                        close = vData[item].vOverSub[n+1].time
                    }
                    else
                    {
                        close = vData[item].timeClose;
                    }


                    //add the guy to the end of our return
                    vReturn[parseInt(vData[item].vOverSub[n].strata)].push([open,parseInt(vName2Index[vData[item].strName]),close,vData[item].strName]);
                }

                vData[item].timeClose = vData[item].vOverSub[0].time;
            }

        }

        return vReturn;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getLabels

        Get the labels for the y axis applications list from the parsed data
        from parseData. This allows the graph to have readable names for
        flows.

        Parameters:
            vData - array data from the above parseData

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.getLabels = function(vData)
    {
        var vLabels = [];

        var nIndex = 1;

        for(var item in vData)
        {
            var bFound = false;

            for(test in vLabels)
            {
                if(vLabels[test] ==  flowName(vData[item].strName))
                {
                    bFound = true;
                    break;
                }
            }

            if(bFound == false)
            {
                vLabels.push(flowName(vData[item].strName));
            }
        }

        return vLabels;
    }

    //!!!this is stateful and should be fixed
    /*
        Variable: vLabels
        Labels on the y axis for the gant chart
        the labels populate by getdata()
    */
    this.vLabels = [];

    //!!!this is stateful and should be fixed
    /*
        Variable: startTime
        start time for this graph
    */
    this.startTime = null;

    /*
        Variable: endTime
        end time for this graph
    */
    this.endTime = null;

    //if we dont have options set some defaults
    if(vOptions == undefined)
    {
       //set some default options
       vOptions = self.defaults;
    }

    //if they passed in parameters on the construction of this
    //objec then call initialize
    if(strDiv != undefined && vData != undefined)
    {
        //we have to load the flow table id list before
        //we can draw
        loadFlowTable(function()
        {
            //initialize ourself
            self.initialize(strDiv,vOptions);

            //draw the graph
            self.draw(vData,true);
        });
    }
}
