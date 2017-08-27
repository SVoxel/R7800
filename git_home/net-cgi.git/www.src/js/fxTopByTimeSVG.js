/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
//
// fxTopByTime for browsers supporting SVG
//
////////////////////////////////////////////////////////////////////////
/*
    Class: fxTopByTimeSVG

    Version:   1.0

    the visual display for my network graph
*/
////////////////////////////////////////////////////////////////////////

function fxTopByTime( strDivID,
                    topData,
                    stats)
{

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
    loadScript("d3.v3.min.js");
    loadScript("tresbar.js");

    var chart_div = d3.select(strDivID);
    chart_div.html("");

    //if we don't get back any events lets fake a chart so we don't get any errors
    if(topData == undefined || topData.length == undefined || topData.length == 0)
    {
       $(".chartTitle").hide();
       $(".chartOptions").hide();
       postAlert("#Display","No Data Available. Please come back to this page after running some network traffic.");
    }
    else
      {

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
      var byTimeData = Array();
      // skip over the headers
      for (var i = 1; i < topData.length; i++) {
        byTimeData.push({'tag': topData[i][0], 'minutes': topData[i][1] * scaling});
      }
      // show only top 5
      byTimeData.sort(function(a, b) {return b['minutes'] - a['minutes']});
      byTimeData = byTimeData.slice(0, 5);

      // setup the chart
      var margin = {top: 20, right: 0, bottom: 30, left: 40},
          width = 840 - margin.left - margin.right,
          height = 500 - margin.top - margin.bottom;

      var x = d3.scale.ordinal()
          .rangeRoundBands([0, width], 0.17);

      var y = d3.scale.linear()
          .range([height, 0]);

      var xAxis = d3.svg.axis()
         .scale(x)
          .orient("bottom");

      var yAxis = d3.svg.axis()
          .scale(y)
          .orient("left");

      var chart = d3.tresbar()
          .width(width)
          .height(height)
          .x(x)
          .y(y);

      var svg = chart_div.append("svg")
          .attr("width", width + margin.left + margin.right)
          .attr("height", height + margin.top + margin.bottom)
        .append("g")
          .attr("transform", "translate(" + margin.left + "," + margin.top + ")");


      byTimeData.forEach(function(d) {
        d.minutes = +d.minutes;
      });

      x.domain(byTimeData.map(function(d) { return d.tag; }));
      y.domain([0, d3.max(byTimeData, function(d) { return d.minutes; })]);

      svg.append("g")
          .attr("class", "x axis")
          .attr("transform", "translate(0," + height + ")")
          .call(xAxis);

      svg.append("g")
          .attr("class", "y axis")
          .call(yAxis)
        .append("text")
          .attr("class", "yaxislabel")
          .attr("transform", "rotate(-90)")
          .attr("y", 6)
          .attr("dy", ".71em")
          .style("text-anchor", "end")
          .text(scalename);

      svg.selectAll(".bar")
          .data(byTimeData)
          .call(chart.duration(1000));
    }
}
