/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */

////////////////////////////////////////////////////////////////////////
/*
    Class: fxDownloadPie

    Version:   1.0

    show top download bytes by flow
*/
////////////////////////////////////////////////////////////////////////

function fxDownloadPie(strDiv,
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
    loadScript("jquery.flot.pie.js");
//    loadScript("jquery.flot.stack.js");
//    loadScript("jquery.flot.categories.js");
//    loadScript("jquery.flot.axislabels.js");

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

    // take top 5 data elements and draw a pie
    this.draw = function(topData) {
        $(self.strDiv).css({height: $(window).height() - $(self.strDiv).offset().top});

        var byDLData = topData;

        // show only top 5
        byDLData.sort(function(a, b) {return b[1] - a[1]});
        byDLData = byDLData.slice(0, 5);

        // setup the chart
        var margin = {top: 20, right: 0, bottom: 30, left: 90},
            width = 640 - margin.left - margin.right,
            height = 480 - margin.top - margin.bottom;

        var plotdata = []
        $.each(byDLData, function(n,d) {
            plotdata.push({label: d[0], data: d[1]});
        });
        //var plotdata = [{data: byDLData}];

        function labelFormatter(label, series) {
            return "<div style='font-size:8pt; text-align:center; padding:2px; color:white;'>" + self.trans(label) + "<br/>"+series.data[0][1].toFixed(1)+"Mb - " + Math.round(series.percent) + "%</div>";
        }

        //quick hover display window
        function hover(x, y, contents, z) 
        {
            $('<div id="hovertip">' + contents + '</div>').css(
            {
                'top': y - 30,
                'left': x,
                'position': 'absolute',
                'display': 'none',
                'border': '1px solid rgb(0, 0, 0)',
                'padding': '2px',
                'background-color': 'rgb(255,255,255)',
                'opacity': '0.75',
                'z-index': '100000'
            }).appendTo("body").show();
         };

        $.plot(self.strDiv, plotdata, {
            series: {
                pie: {
                    show: true,
                    radius: .9,
                    label: {
                        show: true,
                        radius: 1,
                        threshold: .03,
                      formatter: labelFormatter,
                        background: {
                            opacity: 0.5,
                            color: '#000'
                        }
                    }
                }
            },
            grid: {
                hoverable: true
            },
            legend: {
                show:false
            }
        });

        //connect the hover mouse over
        $(self.strDiv).bind("plothover", function(event, pos, item)
        {
            $("#hovertip").remove();

            var x = item.datapoint[0];
            var y = item.datapoint[1] - item.datapoint[2];

            hover(pos.pageX, pos.pageY, self.trans(item.series.label)+" - "+item.series.data[0][1].toFixed(1)+"Mb - "+item.series.percent.toFixed(0)+"%", item.series.color);

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
