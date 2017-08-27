/*
  Copyright (c) 2013 Qualcomm Atheros, Inc..

  All Rights Reserved.
  Qualcomm Atheros Confidential and Proprietary
*/

function dbOverSub()
{
    var time = null;
    var strata = null;
}

function dbItem()
{
    var strName = "";
    var nUpload = 0;
    var nDownload = 0;

    var timeOpen = null;
    var timeClose = null;

    var strata;

    var vOverSub = null;
}

//=========================================================================
//
// Page that displays the Bandwidth graphs
//
//=========================================================================
function CPage_AppEvents()
{
    //=======================================================================
    //
    //
    //
    //=======================================================================
    this.Base   = CPageBase;
    this.Base();

    var self = this;

    //=======================================================================
    //
    //
    //
    //=======================================================================
    this.m_nPreviousTime = 0;
    this.m_iTimerInterval   = 1;
    this.m_sName            = "App Events";
    this.m_sHTML            = "graph_pages/appevents.html";

    this.m_aPrevTX=[];
    this.m_aPrevRX=[];

    this.bCreatedGraph = false;

    //=======================================================================
    //
    //
    //
    //=======================================================================
    this.Init = function()
    {
        //init the from date range control
        $( "#aeFrom" ).datepicker({
            //defaultDate: "-1m",
            changeMonth: true,
            //numberOfMonths: 3,
            onSelect: function( selectedDate ) {
                $( "#aeFrom" ).datepicker( "option", "minDate", selectedDate );
                self.drawGraph();
            }
        });

        //init the to date range control
        $( "#aeTo" ).datepicker({
            //defaultDate: "+1w",
            changeMonth: true,
            //numberOfMonths: 3,
            onSelect: function( selectedDate ) {
                $( "#aeTo" ).datepicker( "option", "maxDate", selectedDate );
                self.drawGraph();
            }
        });


        self.drawGraph();
    }

    this.drawGraph = function()
    {

        var vData = self.getData();

        var vLabelArray = [];

        //create the label array for the y axis ticks
        for(var n = 0;n<self.vLabels.length;n++)
        {
            vLabelArray.push([n,self.vLabels[n]]);
        }

        // set the default dates on the to and from controls
        //$("#aeFrom").datepicker("setDate",self.startTime);
        //$("#aeTo").datepicker("setDate",self.endTime);

        //create the otion for the chart
        var options = { series: { gantt: { active: true, show: true, barHeight: .5 } }
                       ,xaxis:  {   min: self.startTime.getTime(),
                                    max: self.endTime.getTime(),
                                    mode: "time",
                                    color:"#000000",
                                    axisLabel: 'Time',
                                    axisLabelUseCanvas: false,
                                    axisLabelFontSizePixels: 20,
                                    axisLabelFontFamily: 'Arial'}
                       ,yaxis:  {   min: -1,
                                    max: vLabelArray.length,
                                    ticks: vLabelArray,
                                    labelWidth:75,
                                    color:"#000000",
                                    axisLabel: 'Applications',
                                    axisLabelUseCanvas: false,
                                    axisLabelFontSizePixels: 20,
                                    axisLabelFontFamily: 'Arial'}
                       ,grid:   { hoverable: true, clickable: true}
                      };

        //get the width of the work pane
        var nWidth = Math.round(($(window).width()-$("#L").width()));

        //make the right side of the graph 100px from the browser edge
        nWidth -= 100;

        //set graph width
        $("#aeBandwidthUpload").width(nWidth);

        //get the hight of the work pane
        var nHeight = Math.round(($(window).height() - 330));

        //set graph height
        $("#aeBandwidthUpload").height(nHeight);

        //center the date range in the middle of the screen
        $("#aeSpacer").css("margin-left",(nWidth/2)-(400/2));

        //create the graph
        $.plot($("#aeBandwidthUpload"), [
                                          { label: "Best", data: vData[0], color: "#00FF00", gantt: { connectSteps: { show: false }, drawstep: addHover } },
                                          { label: "Better", data: vData[1], color: "#FFFF00", gantt: { connectSteps: { show: false }, drawstep: addHover } },
                                          { label: "Best Effort", data:  vData[2], color: "#FF0000", gantt: { connectSteps: { show: false }, drawstep: addHover }  }
                                        ],
               options);

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

        //remember the last point to fade it
        var previousPoint = null;

        //this is the on hover binder for each bar used by the above graph
        $("#aeBandwidthUpload").bind("plothover", function (event, pos, item)
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

        //move the y axis up so its align with our font size
        $("#aeBandwidthUpload").find(".yAxis").find(".tickLabel").css("margin-top","-10px");

        //set the legend up to have black borders and fonts
        $("#aeBandwidthUpload").find("table:first").children().css("color","black");
        $("#aeBandwidthUpload").find("table:first").children().find("legendColoBox:first").css("border","1px solid black");


        $("#aeBandwidthUpload").find("#xaxisLabel").css("margin-bottom","-30px");
        $("#aeBandwidthUpload").find("#xaxisLabel").css("font-size","15px");

        $("#aeBandwidthUpload").find("#yaxisLabel").css("margin-left","-40px");
        $("#aeBandwidthUpload").find("#yaxisLabel").css("font-size","15px");
        $("#aeBandwidthUpload").find("#yaxisLabel").css("-webkit-transform","rotate(-90deg)");
        $("#aeBandwidthUpload").find("#yaxisLabel").css("-moz-transform","rotate(-90deg)");
        $("#aeBandwidthUpload").find("#yaxisLabel").css("-ms-transform","rotate(-90deg)");
        $("#aeBandwidthUpload").find("#yaxisLabel").css("-o-transform","rotate(-90deg)");

        $("#aeBandwidthUpload").find(".yAxis").find(".tickLabel").css("margin-right","15px");
        $("#aeBandwidthUpload").find(".xAxis").find(".tickLabel").css("font-size","12px");

    }

    //=======================================================================
    //
    //
    //
    //=======================================================================
    this.OnDataReady = function(_sKey, _kData)
    {
        //when the data comes in construct this page
        if (_sKey == "tc")
        {
        }
    }

    //=======================================================================
    //
    // Pause or activate data polling
    //
    //=======================================================================
    this.OnActivated = function(_bActive)
    {
        /*
        if(_bActive == false)
        {
            $( "#aeTo" ).datepicker("destroy");
            $( "#aeFrom" ).datepicker("destroy");
        }
        else
        {
            $( "#aeTo" ).datepicker("destroy");
            $( "#aeFrom" ).datepicker("destroy");

            //init the from date range control
            $( "#aeFrom" ).datepicker({
                defaultDate: "+1w",
                changeMonth: true,
                numberOfMonths: 3,
                onSelect: function( selectedDate ) {
                    $( "#aeFrom" ).datepicker( "option", "minDate", selectedDate );
                }
            });

            //init the to date range control
            $( "#aeTo" ).datepicker({
                defaultDate: "+1w",
                changeMonth: true,
                numberOfMonths: 3,
                onSelect: function( selectedDate ) {
                    $( "#aeTo" ).datepicker( "option", "maxDate", selectedDate );
                }
            });

            $( "#aeTo" ).datepicker("refresh");
            $( "#aeFrom" ).datepicker("refresh");
        }*/
        self.drawGraph();
    }

    //get the labels for the y axis applications list
    this.getLabels = function(vData)
    {
        var vLabels = [];

        var nIndex = 1;

        for(var item in vData)
        {
            var bFound = false;

            for(test in vLabels)
            {
                if(vLabels[test] ==  vData[item].strName)
                {
                    bFound = true;
                    break;
                }
            }

            if(bFound == false)
            {
                vLabels.push(vData[item].strName);
            }
        }

        return vLabels;
    }

    //make fake data for testing purposes
    this.makeData = function()
    {
        var strData = "";

        var nMax = (Math.random()*100);

        if(nMax<5)
        {
            nMax = 5;
        }

        nMax = nMax.toFixed(0);

        var nCurrentTime=1348004743;
        var nAppNumber = 0;

        self.startTime = new Date();

        var nVal = parseInt(nCurrentTime)*1000;

        self.startTime.setTime(nVal);

        self.endTime = new Date();

        for(var n = 0;n<nMax;n++)
        {

            var nOverSub = (Math.random()*3);

            nOverSub = nOverSub.toFixed(0);

            var strOpenLine = nCurrentTime+".463107"+",open,"+nAppNumber+",App"+nAppNumber+",00:1b:21:5a:7d:b2,"+nOverSub+","+nOverSub+",776,0,0,0,0,0,0\r\n";
            var nCloseTime = (Math.random()*60);

            nCloseTime = nCloseTime.toFixed(0);

            if(nCloseTime<1)
            {
                nCloseTime = 1;
            }

            var nEndTime = parseInt(nCurrentTime)+parseInt(nCloseTime);

            var strCloseLine = nEndTime+".466117,close,"+nAppNumber+",0,0\r\n";

            var nTimeInc = (Math.random()*10);

            nTimeInc = nTimeInc.toFixed(0);

            if(nTimeInc<1)
            {
                nTimeInc = 1;
            }

            nCurrentTime = parseInt(nCurrentTime)+parseInt(nTimeInc);

            strData+=strOpenLine;
            strData+=strCloseLine;

            nAppNumber = parseInt(nAppNumber) + 1;


            var nVal = (parseInt(nEndTime)+60)*1000;

            self.endTime.setTime(nVal);
        }

        return strData;
    }

    //process the data and return it to the graph
    this.getData = function()
    {
        //get the data (currently the fake data)
        //var strData = self.makeData();

        //this is the real data
        var strData = "";

         var sDate = $( "#aeFrom" ).datepicker( "getDate" );
         var eDate = $( "#aeTo" ).datepicker( "getDate" );

         var strUrl = "/api/events";

         if(sDate != null && eDate != null)
         {
            strUrl += "?start=\""+sDate.getFullYear()+"-"+(sDate.getMonth()+1)+"-"+sDate.getDate()+"%200:0:0"+"\"&end=\""+eDate.getFullYear()+"-"+(eDate.getMonth()+1)+"-"+eDate.getDate()+"%2023:59:59\"";
         }

        //get the data from the router
        $.ajax(
            {
                type: "GET",
                async: false,
                url: strUrl,
                dataType: "text",
                //-------------------------------------------------------------------
                // Issue callback with result data
                success: function(data, status, request)
                {
                    strData = data;
                }
            }
        );


//      strData = "=========== /usr/sbin/dumpelog -c   /var/run/appflow/events.db ===========\r\n\
// 1350055439.729640,close,5,250,0\r\n\
// 1350055439.762978,open,8,default,00:0a:cd:1e:43:5c,0,0,520,0,0,0,0\r\n\
// 1350055462.317277,open,9,default,00:e0:4c:68:00:01,0,0,520,0,0,0,0\r\n\
// 1350055474.644269,open,10,default,00:90:a9:0f:11:fb,0,0,520,0,0,0,0\r\n\
// 1350055474.819936,close,10,0,0\r\n\
// 1350055483.071816,open,11,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350055483.264432,close,11,0,0\r\n\
// 1350055492.634495,open,12,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350055492.694013,close,12,0,0\r\n\
// 1350055552.718027,open,13,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350055552.773320,close,13,0,0\r\n\
// 1350055612.871077,open,14,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350055612.926321,close,14,0,0\r\n\
// 1350055672.954090,open,15,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350055673.002453,close,15,0,0\r\n\
// 1350055733.045534,open,16,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350055733.113216,close,16,0,0\r\n\
// 1350055793.182245,open,17,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350055793.230725,close,17,0,0\r\n\
// 1350055853.360443,open,18,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350055853.416202,close,18,0,0\r\n\
// 1350055913.445731,open,19,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350055914.334947,close,19,0,0\r\n\
// 1350055934.327779,open,20,default,00:0a:cd:1e:43:89,0,0,520,0,0,0,0\r\n\
// 1350055934.489218,close,20,0,0\r\n\
// 1350055954.621561,open,21,default,00:90:a9:0f:11:fb,0,0,520,0,0,0,0\r\n\
// 1350055954.804067,close,21,0,0\r\n\
// 1350055974.167867,open,22,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350055974.223631,close,22,0,0\r\n\
// 1350056009.851183,open,23,default,00:25:64:f0:73:79,0,0,520,0,0,0,0\r\n\
// 1350056034.251096,open,24,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056034.312685,close,24,0,0\r\n\
// 1350056083.281868,open,25,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056083.467683,close,25,0,0\r\n\
// 1350056094.429860,open,26,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056094.485247,close,26,0,0\r\n\
// 1350056137.487054,open,27,default,1c:6f:65:72:68:b4,0,0,520,0,0,0,0\r\n\
// 1350056154.621569,open,28,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056154.676424,close,28,0,0\r\n\
// 1350056201.068607,open,29,default,28:e7:cf:d6:af:72,0,0,520,0,0,0,0\r\n\
// 1350056214.702775,open,30,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056214.756205,close,30,0,0\r\n\
// 1350056250.279291,close,23,330,330\r\n\
// 1350056258.009990,close,27,0,0\r\n\
// 1350056274.782074,open,31,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056274.837225,close,31,0,0\r\n\
// 1350056335.489156,open,32,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056335.535582,close,32,0,0\r\n\
// 1350056389.014399,close,29,4206,5231\r\n\
// 1350056395.567782,open,33,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056395.614656,close,33,0,0\r\n\
// 1350056455.645375,open,34,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056455.703115,close,34,0,0\r\n\
// 1350056515.785212,open,35,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056515.844261,close,35,0,0\r\n\
// 1350056554.652862,open,36,default,00:90:a9:0f:11:fb,0,0,520,0,0,0,0\r\n\
// 1350056554.844557,close,36,0,0\r\n\
// 1350056558.335205,open,37,default,00:25:64:f0:73:79,0,0,519,0,0,0,0\r\n\
// 1350056558.866741,close,37,0,0\r\n\
// 1350056575.871856,open,38,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056575.933355,close,38,0,0\r\n\
// 1350056635.964492,open,39,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056636.015951,close,39,0,0\r\n\
// 1350056683.508165,open,40,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056683.684020,close,40,0,0\r\n\
// 1350056696.135459,open,41,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056696.183253,close,41,0,0\r\n\
// 1350056756.215391,open,42,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056756.266884,close,42,0,0\r\n\
// 1350056816.447137,open,43,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056816.504293,close,43,0,0\r\n\
// 1350056876.621010,open,44,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056876.681518,close,44,0,0\r\n\
// 1350056936.713494,open,45,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056936.759736,close,45,0,0\r\n\
// 1350056945.928585,open,46,default,00:25:64:f0:73:79,0,0,519,0,0,0,0\r\n\
// 1350056996.925028,open,47,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350056996.980235,close,47,0,0\r\n\
// 1350057057.118782,open,48,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350057057.173947,close,48,0,0\r\n\
// 1350057117.679756,open,49,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350057117.737131,close,49,0,0\r\n\
// 1350057154.617108,open,50,default,00:90:a9:0f:11:fb,0,0,520,0,0,0,0\r\n\
// 1350057154.797978,close,50,0,0\r\n\
// 1350057177.767072,open,51,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350057177.824236,close,51,0,0\r\n\
// 1350057186.115443,close,46,330,330\r\n\
// 1350057191.199097,open,52,default,00:0a:cd:1e:43:89,0,0,520,0,0,0,0\r\n\
// 1350057196.635530,open,53,youtube_360,00:0a:cd:1e:43:89,0,0,264,8000,8000,92000,92000\r\n\
// 1350057198.089613,open,54,youtube_480,00:0a:cd:1e:43:89,0,0,264,8000,8000,150000,150000\r\n\
// 1350057198.953648,close,54,0,0\r\n\
// 1350057210.310915,open,55,youtube_1080,00:0a:cd:1e:43:89,0,0,264,8000,8000,550000,550000\r\n\
// 1350057237.948087,open,56,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350057238.010683,close,56,0,0\r\n\
// 1350057283.821587,open,57,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350057284.020051,close,57,0,0\r\n\
// 1350057298.143687,open,58,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350057298.203163,close,58,0,0\r\n\
// 1350057358.323718,open,59,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350057358.393388,close,59,0,0\r\n\
// 1350057369.251992,close,55,4925322,279355710\r\n\
// 1350057401.381048,open,60,default,00:25:64:f0:73:79,0,0,519,0,0,0,0\r\n\
// 1350057401.795852,close,60,0,0\r\n\
// 1350057418.499561,open,61,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350057418.557358,close,61,0,0\r\n\
// 1350057444.519175,close,53,60477,3096560\r\n\
// 1350057478.581086,open,62,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350057478.630085,close,62,0,0\r\n\
// 1350057538.754771,open,63,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350057538.802005,close,63,0,0\r\n\
// 1350057598.928621,open,64,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350057598.986402,close,64,0,0\r\n\
// 1350057659.011808,open,65,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350057659.070189,close,65,0,0\r\n\
// 1350057719.093095,open,66,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350057719.149507,close,66,0,0\r\n\
// 1350057754.645455,open,67,default,00:90:a9:0f:11:fb,0,0,520,0,0,0,0\r\n\
// 1350057754.943760,close,67,0,0\r\n\
// 1350057779.306690,open,68,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350057779.366476,close,68,0,0\r\n\
// 1350057839.390196,open,69,default,5c:d9:98:80:3e:5c,0,0,520,0,0,0,0\r\n\
// 1350057839.445488,close,69,0,0\r\n\
// 1350057847.929976,open,70,default,00:25:64:f0:73:79,0,0,519,0,0,0,0\r\n\
// 1350057852.871375,close,70,66,0\r\n\
// 1350057853.045915,open,71,default,00:25:64:f0:73:79,0,0,519,0,0,0,0\r\n";


        //convert the CSV to a walkable array
        var arData = CSVToArray(strData);
        var vData =[];

        var id2index = [];

        //walk the array of arrays
        for(var index = 0;index<arData.length;index++)
        {
            //get the uid
            var nRealID = arData[index][2];

            var nID = id2index[nRealID];

            if(nID == undefined || nID == null)
            {
                nID = id2index[nRealID] = vData.length;
            }

            //if this is an open
            if(arData[index][1]=="open")
            {
                var strName = arData[index][3];
                var strTime = (arData[index][0]).replace(/\s+/g, ' ').replace(/\s+/g, '\n').replace(/\s+/g, '\r')
                var vTime = strTime.split(".");
                var strata = arData[index][5];


                //if this doesnt exist
                if(vData[nID] == undefined)
                {
                    vData[nID] = new dbItem;
                    vData[nID].strName = strName;

                    vData[nID].timeOpen = new Date();

                    var nVal = parseInt(vTime[0])*1000;

                    vData[nID].timeOpen.setTime(nVal);

                    vData[nID].strata = strata;

                    if(self.startTime == null)
                    {
                        self.startTime = vData[nID].timeOpen;
                    }
                    else if(self.startTime > vData[nID].timeOpen)
                    {
                        self.startTime = vData[nID].timeOpen;
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
                if(vData[nID] != undefined)
                {

                    var strTime = (arData[index][0]).replace(/\s+/g, ' ').replace(/\s+/g, '\n').replace(/\s+/g, '\r')
                    var vTime = strTime.split(".");

                    vData[nID].nUpload   = parseInt(arData[index][3]);
                    vData[nID].nDownload = parseInt(arData[index][4]);

                    vData[nID].timeClose = new Date();

                    var nVal = parseInt(vTime[0])*1000;

                    vData[nID].timeClose.setTime(nVal);


                    if(self.endTime == null)
                    {
                        self.endTime = vData[nID].timeClose;
                    }
                    else if(self.endTime < vData[nID].timeClose)
                    {
                        self.endTime = vData[nID].timeClose;
                    }

                    id2index[nRealID]=null;
                    //id2index.splice(nRealID,1);
                }
                else
                {
                    //alert("close no open");
                }
            }
            else if(arData[index][1]=="oversub")
            {
                //if this doesnt exist
                if(vData[nID] != undefined)
                {

                    var osub = new dbOverSub;
                    osub.strata = arData[index][4];


                    var strTime = (arData[index][0]).replace(/\s+/g, ' ').replace(/\s+/g, '\n').replace(/\s+/g, '\r')
                    var vTime = strTime.split(".");

                    osub.time = new Date();

                    var nVal = parseInt(vTime[0])*1000;

                    osub.time.setTime(nVal);

                    if(vData[nID].vOverSub == undefined || vData[nID].vOverSub == null)
                    {
                        vData[nID].vOverSub = [];
                    }

                    vData[nID].vOverSub.push(osub);
                }
                else
                {
                    //alert("oversub no open");
                }
            }
        }

        var vReturn = [];

        self.vLabels = self.getLabels(vData);

        vReturn[0] = [];
        vReturn[1] = [];
        vReturn[2] = [];
        vReturn[3] = [];

        //
        var vName2Index = [];

        //create a name to index lookup for the data routine
        //so we can tell each piece of data what row to be in
        for(var n in self.vLabels)
        {
            vName2Index[self.vLabels[n]] = n;
        }

        for(var item in vData)
        {
            //vReturn.push({label: item,  data: vPre[item]});
            var timeOpen = vData[item].timeOpen;
            var timeClose = undefined;

            if(vData[item].timeClose == undefined)
            {
                timeClose = new Date();//vData[item].timeOpen;
            }
            else
            {
                timeClose = vData[item].timeClose;
            }

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

        if(self.startTime == null)
        {
            self.startTime = new Date();

        }

        if(self.endTime == null)
        {
            self.endTime = new Date();

        }

        if(sDate == null)
        {
            // set the default dates on the to and from controls
            $("#aeFrom").datepicker("setDate",self.startTime);
            $("#aeTo").datepicker("setDate",self.endTime);
        }


        return vReturn;
    }


    //!!!this is stateful and should be fixed
    //the labels populate by getdata()
    this.vLabels = [];

    //!!!this is stateful and should be fixed
    //start time and end time for this graph
    //populated by getdata()
    this.startTime = null;
    this.endTime = null;
}
