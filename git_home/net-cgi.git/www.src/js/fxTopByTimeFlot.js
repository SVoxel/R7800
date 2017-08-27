/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
//
// fxTopByTime for flotgraphs
//
////////////////////////////////////////////////////////////////////////
/*
    Class: fxTopByTimeFlot

    Version:   1.0

    the visual display for my network graph
*/
////////////////////////////////////////////////////////////////////////

function fxTopByTime(strDiv,
                    vData,
                    stats,
                    transFunction)
{
    var self = this;

    /////////////////////////////////////////////////////////////////////vOptions////
    /*
        The magic to load our pre-requisite scripts in javascript
    */
    /////////////////////////////////////////////////////////////////////////
    loadScript("utility-2.0.js");
    loadScript("jquery.flot.js");
    loadScript("jquery.flot.stack.js");
    loadScript("jquery.flot.categories.js");
    loadScript("jquery.flot.axislabels.js");

    this.strDiv = strDiv;
    this.vData = vData;
    this.stats = stats;
    this.trans = transFunction;

    this.initialize = function() {
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

    this.draw = function(topData) {
        //$(self.strDiv).css({height: $(window).height() - $(self.strDiv).offset().top});
        // scaling -- stats.maxFlowTime is in ms
        var maxmins = stats.maxFlowTime / 60000;
        var scaling = 1;
        var scalename = _t("Minutes")
        if (maxmins > 100) {
            var maxhours = maxmins / 60;
            scaling /= 60;
            scalename = _t("Hours");
            if (maxhours > 35) {
                var maxdays = maxhours / 24;
                scaling /= 24;
                scalename = _t("Days");
            }
        }

        // reformat the data for d3
        var byTimeData = [];
        var nMax = 1.0;
        // skip over the headers
       for (var i = 1; i < topData.length; i++)
       {
            var strLabel = self.trans(topData[i][0]);
            byTimeData.push([strLabel, topData[i][1] * scaling]);

            //set the max value
            if(topData[i][1]>nMax)
            {
                nMax = topData[i][1];
            }
        }
        // show only top 5
        byTimeData.sort(function(a, b) {return b[1] - a[1]});
        byTimeData = byTimeData.slice(0, 5);

        // setup the chart
        var margin = {top: 20, right: 0, bottom: 30, left: 40},
            width = 840 - margin.left - margin.right,
            height = 500 - margin.top - margin.bottom;

        var plotdata = []
        $.each(byTimeData, function(n,d) {
            plotdata.push({data: [d]});
        });
        //var plotdata = [{data: byTimeData}];

        $.plot(self.strDiv, plotdata, {
            series: {
                lines: { show: false },
                bars: {
                    show: true,
                    fill: 1,
                    barWidth: 0.8,
                    align: "center",
                    horizontal: false
                },
                shadowSize: 4
            },
            yaxis: {
                min: 0,
                axisLabel: scalename,
                max: nMax*1.1
            },
            xaxis: {
                mode: "categories",
                autoscaleMargin: 0.05,
                font : {size: 12, color: '#000000' },
            }
        });
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

    //if they passed in parameters on the construction of this
    //object then call initialize
    if(strDiv != undefined && vData != undefined)
    {
        //initialize ourself
        self.initialize(strDiv);

        //draw the graph
        self.draw(vData,true);
    }
  }
