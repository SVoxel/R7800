/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */

//do we have to draw ie style
function isMS()
{
    var bReturn = false;
    var ua = window.navigator.userAgent;
    var msie = ua.indexOf("MSIE ");

    if (msie > 0 || !!navigator.userAgent.match(/Trident.*rv\:11\./))
    {
        bReturn = true;
    }

    return bReturn;
}

/*
    color range used for pie charts
*/
var colorrange =    [   '#40699c',
                        '#9e413e',
                        '#7f9a48',
                        '#695185',
                        '#3c8da3',
                        '#cc7b38',
                        '#4f81bd',
                        '#c0504d',
                        '#9bbb59',
                        '#8064a2',
                        '#4bacc6',
                        '#f79646'
                    ];
/*
    Function: addPieKey

    add a key with icon/name for a pie element and draw a line
    to the pie wedge

    Parameters:
    strIcon     - path and icon to show on key
    strName     - name to display after icon on the key
    strMac      - mac of the node for click thru to details
    nItemX      - x postiton of key
    nItemY      - y positino of key
    strSlice    - pie wedge to draw line too
    chart       - chart class for drawing
    rgb         - rgb color of the key background puck
    handler     - Click callback handler
    bSelected   - Draw the item selected true/false

    returns:
    nothing
*/
function addPieKey( strIcon,
                    strName,
                    strMac,
                    nItemX,
                    nItemY,
                    slice,
                    chart,
                    rgb,
                    handler,
                    bSelected)
{

    //the chart query interface
    var cli = chart.getChartLayoutInterface();

    //get the bounding box of the target wedge
    var bbox = cli.getBoundingBox("slice#"+slice);

    //if the bounding box is real
    if(bbox != null)
    {
        //get center of bounding box
        var nPieX = Math.round(bbox.left + (bbox.width / 2));
        var nPieY = Math.round(bbox.top + (bbox.height / 2));

        // Handler for .ready() called.
        var graph = d3.select('svg');

        var defs = graph.append("defs");

        // create filter with id #drop-shadow
        // height=130% so that the shadow is not clipped
        var filter = defs.append("filter")
            .attr("id", "drop-shadow")
            .attr("height", "130%");

        // SourceAlpha refers to opacity of graphic that this filter will be applied to
        // convolve that with a Gaussian with standard deviation 3 and store result
        // in blur
        filter.append("feGaussianBlur")
            .attr("in", "SourceAlpha")
            .attr("stdDeviation", 5)
            .attr("result", "blur");

        // translate output of Gaussian blur to the right and downwards with 2px
        // store result in offsetBlur
        filter.append("feOffset")
            .attr("in", "blur")
            .attr("dx", 5)
            .attr("dy", 5)
            .attr("result", "offsetBlur");

        // overlay original SourceGraphic over translated blurred opacity by using
        // feMerge filter. Order of specifying inputs is important!
        var feMerge = filter.append("feMerge");

        feMerge.append("feMergeNode")
            .attr("in", "offsetBlur")
        feMerge.append("feMergeNode")
            .attr("in", "SourceGraphic");

        var rect = graph.append("rect")
            .attr("rx", 16)
            .attr("ry", 16)
            .attr("x", nItemX)
            .attr("y", nItemY)
            .attr("width", 256)
            .attr("height", 72)
            .style("fill", rgb);

        if(isMS() == false)
        {
            rect.style("filter", "url(#drop-shadow)");
        }

        //make the image for the device/flow icon
        var img = graph.append("svg:image")
            .attr('xlink:href', strIcon)
            .attr("x", nItemX + 8)
            .attr("y", nItemY + 4)
            .attr("width", 64)
            .attr("height", 64);

        var nStartX = nItemX + 240;
        var nStartY = nItemY + 38;

        var nEndX = nPieX;
        var nEndY = nPieY;

        var nMidX = nEndX; //nStartX+((nEndX-nStartX)/2);

        //draw the line to the pie wedgt
        var line1 = graph.append('line')
            .attr({
                'x1': nStartX,
                'y1': nStartY,
                'x2': nMidX,
                'y2': nStartY
            })
            .attr('class', 'unselect_line');

        //draw the end of the line
        var line2 = graph.append('line')
            .attr({
                'x1': nStartX,
                'y1': nStartY-1,
                'x2': nMidX,
                'y2': nStartY-1
            })
            .attr('class', 'unselect_line');

        //draw a line highlight/shadow
        var line3 = graph.append('line')
            .attr({
                'x1': nMidX,
                'y1': nStartY,
                'x2': nMidX,
                'y2': nEndY
            })
            .attr('class', 'unselect_line');

        //draw end line highlight/shadow
        var line4 = graph.append('line')
            .attr({
                'x1': nMidX-1,
                'y1': nStartY,
                'x2': nMidX-1,
                'y2': nEndY
            })
            .attr('class', 'unselect_line');

        //make a circle to star tthe line at
        var circle1 = graph.append("circle")
            .attr("cx", nStartX)
            .attr("cy", nStartY)
            .attr("r", 3)
            .style("fill", "black");

        //circle end point
        var circle2 = graph.append("circle")
            .attr("cx", nPieX)
            .attr("cy", nPieY)
            .attr("r", 3)
            .style("fill", "black");

        //make the label for the item flow/device
        var text = graph.append("text")
            .attr("x", nItemX + 80)
            .attr("y", nItemY + 41)
            .text(strName)
            .style("fill", "white")
            .style("font-size", "14")
            .style("font-family", 'sans-serif');

        //is this item selected/selectable?
        if(bSelected == false)
        {
            //mouse hover handler
            var hMouseIn = function()
            {
                //is this item unselected?
                if(selected==null)
                {
                    //draw the selected state
                    options.slices={};
                    options.slices[parseInt(slice)] = {};
                    options.slices[parseInt(slice)].offset = .1;
                    drawChart(false,parseInt(slice));
                    $(this).css('cursor', 'pointer');
                    rect.style("fill", "#d8ad45");
                    text.style("fill", "black");
                    line1.attr('class', 'select_line');
                    line2.attr('class', 'select_line');
                    line3.attr('class', 'select_line');
                    line4.attr('class', 'select_line');
                    selected = slice;
                }
            }

            //mouse leave hover handler
            var hMouseOut = function()
            {
                //if we are currently selected
                if(selected!=null)
                {
                    //draw the unselected state
                    options.slices={};
                    //drawChart(false);
                    $(this).css('cursor', 'pointer');
                    rect.style("fill", rgb);
                    text.style("fill", "white");
                    line1.attr('class', 'unselect_line');
                    line2.attr('class', 'unselect_line');
                    line3.attr('class', 'unselect_line');
                    line4.attr('class', 'unselect_line');
                    selected = null;
                }
            }

            //hook up hover handlers on image
            img.on("mouseover",hMouseIn);
            img.on("mouseout",hMouseOut);

            //hook up hover handlers on puck rect
            rect.on("mouseover",hMouseIn);
            rect.on("mouseout",hMouseOut);

            //hook up hover handlers on the label
            text.on("mouseover",hMouseIn);
            text.on("mouseout",hMouseOut);

            //if we are selected and its not from a hover event
            if(typeof bSelected != 'undefined' && bSelected == true)
            {
                //draw the select state for the puck and text
                rect.style("fill", "#d8ad45");
                text.style("fill", "black");

                //select the line
                line1.attr('class', 'select_line');
                line2.attr('class', 'select_line');
                line3.attr('class', 'select_line');
                line4.attr('class', 'select_line');
            }
        }
        else //we are not selected
        {
            //make sure the pointer is the selection finger on hover
            $(this).css('cursor', 'pointer');

            //draw the unselected state for the puck and text
            rect.style("fill", "#d8ad45");
            text.style("fill", "black");

            //draw unselected lines
            line1.attr('class', 'select_line');
            line2.attr('class', 'select_line');
            line3.attr('class', 'select_line');
            line4.attr('class', 'select_line');
        }
        if(handler!='undefined')
        {
            //hook handler to icon
            img.on("click", handler);

            //hook handler to rounded rect
            rect.on("click", handler);

            //hook handler to text
            text.on("click", handler);
        }
    }
}

//curent selected item (geez a nasty global!)
var selected = null;

// Returns path data for a rectangle with rounded right corners.
// The top-left corner is ⟨x,y⟩.
/*function rightRoundedRect(x, y, width, height, radius)
{
    return "M" + x + "," + y + "h" + (width - radius) + "a" + radius + "," +
            radius + " 0 0 1 " + radius + "," + radius + "v" + (height - 2 * radius) +
            "a" + radius + "," + radius + " 0 0 1 " + -radius + "," + radius + "h" +
            (radius - width) + "z";
}*/

//Global dates for things
var g_dateCurrent  = new Date();

//get the currint date integers
var g_nCurrentYear  = g_dateCurrent.getFullYear();
var g_nCurrentMonth = g_dateCurrent.getMonth();
var g_nCurrentDay   = g_dateCurrent.getDate();

//calc end of month for default end date 
var g_dateEnd = new Date(g_nCurrentYear,g_nCurrentMonth+1,0);

//calc four months back for default start
var g_nStartYear    = g_nCurrentYear;
var g_nStartMonth   = g_nCurrentMonth;

//if the start - 3 months is in the previous year
if(g_nCurrentMonth <= 2)
{
    g_nStartMonth = 12+(g_nCurrentMonth - 3);
    g_nStartYear = g_nCurrentYear - 1;
}
else
{
    g_nStartMonth -= 3;
}

var g_dateStart = new Date(g_nStartYear,g_nStartMonth,1);

/*
    Function: getEvents

    get the long term events from the ozker api
    start and end are in utcsecond not date objects

    Parameters:
    start   - start time of the events you want
    end     - end time of the events you want

    returns:
    all the event between start and end
*/
function getEvents(start,end)
{
    var rtData = null;

    $.ajax({
        url: '/cgi-bin/ozker/api/events/flows',
        type: "GET",
        async: false,
        dataType: "json",
        cache: false,
        timeout: 3000,
        success: function(data, status, xhr) {

            if(typeof start == 'undefined' || typeof end == 'undefined')
            {
                rtData = data.events;
            }
            else
            {
                rtData = [];

                var myTime = 0;

                if( typeof data.events != 'undefined' &&
                    typeof data.events[0] != 'undefined' &&
                    typeof data.events[0].time != 'undefined')
                {
                    myTime = parseInt(data.events[0].time);
                }

                g_dateStart = new Date(0);
                g_dateStart.setUTCSeconds(myTime);

                for(var x in data.events)
                {
                    var myTime = parseInt(data.events[x].time);

                    if(myTime >= start && myTime <= end)
                    {
                        rtData.push(data.events[x]);
                    }
                }

                g_dateEnd = new Date(0);
                g_dateEnd.setUTCSeconds(myTime);

            }
        },
        error: function(data, status) {
            rtData = status;
        }
    });

    return rtData;
}

/*
    cruddy global mutex for asyncEvents to stop stacking
*/
var g_bUpdating = false;

/*
    Function: asyncEvents

    get the long term events from the ozker api
    start and end are in utcsecond not date objects
    in an asynchronous mannet with a callback

    Parameters:
    start    - start time of the events you want
    end      - end time of the events you want
    callback - functino to call with data when done

    returns:
    nothing
*/
function asyncEvents(start,end,callback)
{
    var rtData = null;

    //is this routine already running?
    if(g_bUpdating == false)
    {
        //if not set it to running
        g_bUpdating = true;

        $.ajax({
            url: '/cgi-bin/ozker/api/events/flows',
            type: "GET",
            async: true,
            dataType: "json",
            cache: false,
            timeout: 5000,
            success: function(data, status, xhr) {

                //clear the cruddy mutex
                g_bUpdating = false;

                //do we null range?
                if(typeof start == 'undefined' || typeof end == 'undefined')
                {
                    rtData = data.events;
                }
                else
                {
                    rtData = [];

                    var myTime = 0;

                    if( typeof data.events != 'undefined' &&
                        typeof data.events[0] != 'undefined' &&
                        typeof data.events[0].time != 'undefined')
                    {
                        myTime = parseInt(data.events[0].time);
                    }

                    g_dateStart = new Date(0);
                    g_dateStart.setUTCSeconds(myTime);

                    for(var x in data.events)
                    {
                        var myTime = parseInt(data.events[x].time);

                        if(myTime >= start && myTime <= end)
                        {
                            rtData.push(data.events[x]);
                        }
                    }

                    g_dateEnd = new Date(0);
                    g_dateEnd.setUTCSeconds(myTime);

                }

                callback(rtData);
            },
            error: function(data, status) {
                rtData = status;
                callback(rtData);
            }
        });
    }
}

/*
    Function: getDeviceData

    take the event data and split by each device

    Parameters:
    rtData - raw data from getEvents()

    returns:
    api/events data broken down by device
*/
function getDeviceData(rtData)
{
    //combine events in her for processing before return
    var eData = [];

    //walk the raw data
    for(var x in rtData)
    {
        var uuid = rtData[x].uuid;

        //does the event have a UUID?
        if(typeof eData[uuid] == 'undefined')
        {
            //if not create a blank one
            eData[uuid] = {};
        }

        //add part to regogize flow and device
        eData[uuid].uuid        = rtData[x].uuid;
        eData[uuid].flow_id     = rtData[x].flow_id;

        //was this an open event?
        if(rtData[x].event == "open")
        {
            //copy the open event it to or presort data
            eData[uuid].openTime    = rtData[x].time;
            eData[uuid].policy_id   = rtData[x].details.policy_id;
            eData[uuid].closeTime   = null;

            //set the device
            eData[uuid].mac          = rtData[x].mac;

            //open has no byte counts so init them to 0
            eData[uuid].rx_bytes = 0;
            eData[uuid].tx_bytes = 0;
        }
        else if(rtData[x].event == "milestone" || rtData[x].event == "close") //was this a close or milestone update?
        {
            //set the close time to the last update
            eData[uuid].closeTime = rtData[x].time;

            //update the byte counts
            eData[uuid].rx_bytes = parseInt(rtData[x].details.rx_bytes);
            eData[uuid].tx_bytes = parseInt(rtData[x].details.tx_bytes);
        }
    }

    //create our return array
    var dData = [];

    //init the device array inside our return
    dData.devices = [];

    //global byte counts across all devices
    dData.rx_total = 0;
    dData.tx_total = 0;

    //pre calculate the totals
    for(var x in eData)
    {
        var mac = eData[x].mac;

        //if this is a valid device
        if(typeof mac != 'undefined')
        {
            //get up down (for debuggin purposes)
            var nRx = eData[x].rx_bytes;
            var nTx = eData[x].tx_bytes;

            //do global byte counts
            dData.rx_total += nRx;
            dData.tx_total += nTx;
        }

    }

    //combine by device and flow
    for(var x in eData)
    {
        //get the device mac
        var mac = eData[x].mac;

        //if we have a valid device
        if(typeof mac != 'undefined')
        {
            //get up down (for debuggin purposes)
            var nRx = eData[x].rx_bytes;
            var nTx = eData[x].tx_bytes;

            //if this is the 1st time we've seen this device
            if(typeof dData.devices[mac] == 'undefined')
            {
                //create the device entry in our array
                dData.devices[mac] = {};
                dData.devices[mac].flows = {};
                dData.devices[mac].rx_total         = 0;
                dData.devices[mac].tx_total         = 0;
            }

            //get the flow
            var flow = eData[x].policy_id;

            //if this is the 1st time we've seen this flow
            if(typeof dData.devices[mac].flows[flow] == 'undefined')
            {
                //init te flow
                dData.devices[mac].flows[flow] = {};

                //init flow byte counts
                dData.devices[mac].flows[flow].rx_bytes   = 0;
                dData.devices[mac].flows[flow].tx_bytes   = 0;
            }

            //do byte count
            dData.devices[mac].flows[flow].rx_bytes += nRx;
            dData.devices[mac].flows[flow].tx_bytes += nTx;

            //do device byte counts
            dData.devices[mac].rx_total += nRx;
            dData.devices[mac].tx_total += nTx;

            //do flow percent
            dData.devices[mac].flows[flow].rx_percent = ((dData.devices[mac].flows[flow].rx_bytes/dData.rx_total)*100).toFixed(2);
            dData.devices[mac].flows[flow].tx_percent = ((dData.devices[mac].flows[flow].tx_bytes/dData.tx_total)*100).toFixed(2);



            //do device  percent
            dData.devices[mac].rx_percent = ((dData.devices[mac].rx_total/dData.rx_total)*100).toFixed(2);
            dData.devices[mac].tx_percent = ((dData.devices[mac].tx_total/dData.tx_total)*100).toFixed(2);
        }
    }

    return dData;
}

/*
    Function: getMonthData

    get the by month flow data for the given device

    Parameters:
    rtData  - raw data from getevents()
    mac     - mac id of device to get data for

    returns:
    event data broken down by month
*/
function getMonthData(  rtData,
                        mac)
{
    //combine events
    var eData = [];


    var now = new Date().getTime();

    //walk the raw data and combine for futher processing
    for(var x in rtData)
    {
        var uuid = rtData[x].uuid;

        //if this is the 1st time we've seen this event
        if(typeof eData[uuid] == 'undefined')
        {
            eData[uuid] = {};
        }

        //create our event entry
        eData[uuid].uuid        = rtData[x].uuid;
        eData[uuid].flow_id     = rtData[x].flow_id;

        //if this is an open event
        if(rtData[x].event == "open")
        {
            eData[uuid].openTime = new Date(0); // The 0 there is the key, which sets the date to the epoch
            eData[uuid].openTime.setUTCSeconds(parseInt(rtData[x].time));

            eData[uuid].policy_id   = rtData[x].details.policy_id;
            eData[uuid].closeTime   = new Date(now); // The 0 there is the key, which sets the date to the epoch

            //set the device
            eData[uuid].mac         = rtData[x].mac;

            eData[uuid].rx_bytes = 0;
            eData[uuid].tx_bytes = 0;
        }
        else if(rtData[x].event == "milestone" || rtData[x].event == "close") // if this is a close or milestone update
        {
            eData[uuid].closeTime = new Date(0); // The 0 there is the key, which sets the date to the epoch
            eData[uuid].closeTime.setUTCSeconds(parseInt(rtData[x].time));

            eData[uuid].rx_bytes = parseInt(rtData[x].details.rx_bytes);
            eData[uuid].tx_bytes = parseInt(rtData[x].details.tx_bytes);
        }
    }

    //create return
    var months = [];

    //add an array for each month
    for(var m = 0; m<=12;m++)
    {
        months[m] = {};
        months[m].flows = [];
        months[m].rx_total   = 0;
        months[m].tx_total   = 0;
    }

    //walk the combined data and add to month buckets
    //filtered by the given mac
    for(var x in eData)
    {
        //if not our mac next!
        if(eData[x].mac != mac)
            continue;

        if(typeof eData[x].openTime == 'undefined')
        {
            eData[x].openTime = new Date(0);
        }

        //get the month of the event
        var month = eData[x].openTime.getMonth();

        //get the flow
        var flow = eData[x].policy_id;

        //get up down (for debuggin purposes)
        var nRx = eData[x].rx_bytes;
        var nTx = eData[x].tx_bytes;

        //if this is the 1st time we've seen this flow
        if(typeof months[month].flows[flow] == 'undefined')
        {
            months[month].flows[flow] = {};

            months[month].flows[flow].rx_bytes   = 0;
            months[month].flows[flow].tx_bytes   = 0;
        }

        //do byte count
        months[month].flows[flow].rx_bytes += nRx;
        months[month].flows[flow].tx_bytes += nTx;

        //do device byte counts
        months[month].rx_total += nRx;
        months[month].tx_total += nTx;

        //do flow percent
        months[month].flows[flow].rx_percent = ((months[month].flows[flow].rx_bytes/months[month].rx_total)*100).toFixed(2);
        months[month].flows[flow].tx_percent = ((months[month].flows[flow].tx_bytes/months[month].tx_total)*100).toFixed(2);
    }

    return months;
}

/*
    Function: getDayData

    get the by daily flow data for the given device

    Parameters:
    rtData  - raw data from getevents()
    mac     - mac id of device to get data for
    month   - month to break down by day

    returns:
    given month of flows broken down by daily activity
*/
function getDayData(  rtData,
                      mac,
                      nMonth)
{
    //combine events
    var eData = [];

    //get the current time
    var now = new Date().getTime();

    //walk the raw data and combine
    for(var x in rtData)
    {
        //event id
        var uuid = rtData[x].uuid;

        //if we haven't seen this event
        if(typeof eData[uuid] == 'undefined')
        {
            eData[uuid] = {};
        }

        //add event to our data
        eData[uuid].uuid        = rtData[x].uuid;
        eData[uuid].flow_id     = rtData[x].flow_id;

        //if this is an open event
        if(rtData[x].event == "open")
        {
            eData[uuid].openTime = new Date(0); // The 0 there is the key, which sets the date to the epoch
            eData[uuid].openTime.setUTCSeconds(parseInt(rtData[x].time));

            eData[uuid].policy_id   = rtData[x].details.policy_id;
            eData[uuid].closeTime   = new Date(now); // The 0 there is the key, which sets the date to the epoch

            //set the device
            eData[uuid].mac         = rtData[x].mac;
            //init byte counts
            eData[uuid].rx_bytes = 0;
            eData[uuid].tx_bytes = 0;
        }
        else if(rtData[x].event == "milestone" || rtData[x].event == "close") //if this is a close or milestone update event
        {
            eData[uuid].closeTime = new Date(0); // The 0 there is the key, which sets the date to the epoch
            eData[uuid].closeTime.setUTCSeconds(parseInt(rtData[x].time));

            //update byte counts
            eData[uuid].rx_bytes = parseInt(rtData[x].details.rx_bytes);
            eData[uuid].tx_bytes = parseInt(rtData[x].details.tx_bytes);
        }
    }

    //create return
    var days = [];
    var nMaxDay = 31;

    //add an array for each day
    for(var d = 0; d<=nMaxDay;d++)
    {
        days[d] = {};
        days[d].flows = [];
        days[d].rx_total   = 0;
        days[d].tx_total   = 0;
    }

    //walk the combined data and add to day buckets
    //filtered by the given mac
    for(var x in eData)
    {
        //if not our mac next!
        if(eData[x].mac != mac)
            continue;

        if(typeof eData[x].openTime == 'undefined')
        {
            eData[x].openTime = new Date(0);
        }

        var month   = eData[x].openTime.getMonth();
        var day     = eData[x].openTime.getDate();

        //if not our month
        if(month != nMonth)
            continue;

        var flow = eData[x].policy_id;

        //get up down (for debuggin purposes)
        var nRx = eData[x].rx_bytes;
        var nTx = eData[x].tx_bytes;

        //if this is the 1st time we've seen this flow
        if(typeof days[day].flows[flow] == 'undefined')
        {
            days[day].flows[flow] = {};

            days[day].flows[flow].rx_bytes   = 0;
            days[day].flows[flow].tx_bytes   = 0;
        }

        //do byte count
        days[day].flows[flow].rx_bytes += nRx;
        days[day].flows[flow].tx_bytes += nTx;

        //do device byte counts
        days[day].rx_total += nRx;
        days[day].tx_total += nTx;
    }

    return days;
}

/*
    Function: getHourData

    get the by month flow data for the given device

    Parameters:
    rtData  - raw data from getevents()
    mac     - mac id of device to get data for
    nMonth  - month to use for data
    nDay    - day from nMonth to use for data

    returns:
    event data broke down by given month/day into hours of that day
*/
function getHourData( rtData,
                      mac,
                      nMonth,
                      nDay)
{
    //combine events
    var eData = [];

    //get current time for unknown flow closes
    var now = new Date().getTime();

    //walk the raw data and combine
    for(var x in rtData)
    {
        //get the evnt id
        var uuid = rtData[x].uuid;

        //is this the 1st time we've seen the id?
        if(typeof eData[uuid] == 'undefined')
        {
            eData[uuid] = {};
        }

        //add our event info
        eData[uuid].uuid        = rtData[x].uuid;
        eData[uuid].flow_id     = rtData[x].flow_id;

        //is this an open event?
        if(rtData[x].event == "open")
        {
            eData[uuid].openTime = new Date(0); // The 0 there is the key, which sets the date to the epoch
            eData[uuid].openTime.setUTCSeconds(parseInt(rtData[x].time));

            eData[uuid].policy_id   = rtData[x].details.policy_id;
            eData[uuid].closeTime   = new Date(now); // The 0 there is the key, which sets the date to the epoch

            //set the device
            eData[uuid].mac         = rtData[x].mac;

            //init byte counts
            eData[uuid].rx_bytes = 0;
            eData[uuid].tx_bytes = 0;
        }
        else if(rtData[x].event == "milestone" || rtData[x].event == "close") //is this a close / milestone update event?
        {
            eData[uuid].closeTime = new Date(0); // The 0 there is the key, which sets the date to the epoch
            eData[uuid].closeTime.setUTCSeconds(parseInt(rtData[x].time));

            //update the ul/dl byte counts
            eData[uuid].rx_bytes = parseInt(rtData[x].details.rx_bytes);
            eData[uuid].tx_bytes = parseInt(rtData[x].details.tx_bytes);
        }
    }

    //create return
    var hours = [];
    var nMaxHours = 23;

    //add an array for each houre
    for(var d = 0; d<=23;d++)
    {
        hours[d] = {};
        hours[d].flows = [];
        hours[d].rx_total   = 0;
        hours[d].tx_total   = 0;
    }

    //walk the combined data and add to day buckets
    //filtered by the given mac
    for(var x in eData)
    {
        //if not our mac next!
        if(eData[x].mac != mac)
            continue;

        if(typeof eData[x].openTime == 'undefined')
        {
            eData[x].openTime = new Date(0);
        }

        //get the exact month/day/hour of the event
        var month   = eData[x].openTime.getMonth();
        var day     = eData[x].closeTime.getDate();
        var hour     = eData[x].closeTime.getHours();

        //if not our month
        if(month != nMonth || day != nDay)
            continue;

        //get the flow id
        var flow = eData[x].policy_id;

        //get up down (for debuggin purposes)
        var nRx = eData[x].rx_bytes;
        var nTx = eData[x].tx_bytes;

        //is this the 1st time we've seen this flow in this hour?
        if(typeof hours[hour].flows[flow] == 'undefined')
        {
            //init the flow in this hour
            hours[hour].flows[flow] = {};
            hours[hour].flows[flow].rx_bytes   = 0;
            hours[hour].flows[flow].tx_bytes   = 0;
        }

        //do byte count
        hours[hour].flows[flow].rx_bytes += nRx;
        hours[hour].flows[flow].tx_bytes += nTx;

        //do device byte counts
        hours[hour].rx_total += nRx;
        hours[hour].tx_total += nTx;
    }

    return hours;
}

/*
    Function: getDevices

    get all the devices this router has seen

    Parameters:
    none

    returns:
    return raw json from /api/ozker/nodes
*/
function getDevices()
{
    var rtDevices = {};

    $.ajax({
        url: '/cgi-bin/ozker/api/nodes',
        type: "GET",
        async: false,
        dataType: "json",
        cache: false,
        timeout: 3000,
        success: function(data, status, xhr) {
            rtDevices = data.nodes;
        },
        error: function(data, status) {
            rtDevices = status;
        }
    });

    return rtDevices;
}

//nasty global storage for devices so we dont have to keep refetching this every update
var rtDevices = {};
//nasty global storage for policies so we dont have to keep refetching this every update
var rtPolicy = {};

/*
    Function: getPolicy

    get all the policies this router knows about

    none

    returns:
    return raw json from /api/ozker/policies
*/
function getPolicy()
{
    var dPolicy = {};

    $.ajax({
        url: '/cgi-bin/ozker/api/policies',
        type: "GET",
        async: false,
        dataType: "json",
        cache: false,
        timeout: 3000,
        success: function(data, status, xhr) {
            rtPolicy = dPolicy = data;
        },
        error: function(data, status) {
            dPolicy = status;
        }
    });

    return dPolicy;
}

/*
    Function: flowname

    take a flow id and get is readable name

    none

    returns:
    human readable flow name for the flow_id as given in
    events or /api/flows
*/
function flowname(strFlowID)
{
    //call the flowName in utility.js and get the readable name
    return flowName(policyname(strFlowID));
}


/*
    Function: policyname

    get the policy id for a given flow id

    ie:

    flowname = policydb:policies:counterstrikego
    policyid = counterstrikego

    none

    returns:
    a policy name for the flowid
*/
function policyname(strFlowID)
{
    //set the string given to return by default
    var strReturn = strFlowID;

    //if we have a table at all
    if(rtPolicy!=undefined && rtPolicy != null)
    {
        //to get the language string
        var strTemp = rtPolicy[strFlowID];

        //if we got back a valid string
        if(strTemp != undefined)
        {
            //set the return
            strReturn = strTemp.emit;
        }
    }

    //call the flowName in utility.js and get the readable name
    return strReturn;
}

/*
    Function: mac2name

    look up the given mac in /api/flow and find the
    human readable name

    Parameters:
    mac - mac address to look up

    returns:
    return the name for the given mac
*/
function mac2name(mac)
{
    //default to sending the mac back
    var name = mac;

    //walk the global rtDevices array
    for(var x in rtDevices)
    {
        //if we find the mac
        if(rtDevices[x].Pipeline.mac_addr == mac)
        {
            //if the device has a name
            if(rtDevices[x].Pipeline.name != undefined)
            {
                name = rtDevices[x].Pipeline.name.substring(0,13);
            }
            else if(rtDevices[x].Pipeline.type != undefined) // if the device has a device type
            {
                name = rtDevices[x].Pipeline.type;
            }
            else if(rtDevices[x].Pipeline.ip_addr != undefined) // if the device has an ip address
            {
                name = rtDevices[x].Pipeline.ip_addr;
            }

            break;
        }
    }

    return name;
}

/*
    Function: mac2type

    look up the given mac in /api/flow and find the
    human readable type of the device

    Parameters:
    mac - mac address to look up

    returns:
    return the device type for the given mac
*/
function mac2type(mac)
{
    //default to sending back nothing
    var type = "Unknown";

    //walk the global device list
    for(var x in rtDevices)
    {
        //if this is our mac
        if(rtDevices[x].Pipeline.mac_addr == mac)
        {
            //if this mac has a type
            if(rtDevices[x].Pipeline.type != undefined)
            {
                type = rtDevices[x].Pipeline.type;
            }
            break;
        }
    }

    return type;
}

/*
    Function: mac2ip

    look up the given mac in /api/flow and find the
    ip of the device

    Parameters:
    mac - mac address to look up

    returns:
    return the ip string for the given mac
*/
function mac2ip(mac)
{
    //return null if no ip found
    var ip = null;

    //wark the global device list
    for(var x in rtDevices)
    {
        //if this is our mac
        if(rtDevices[x].Pipeline.mac_addr == mac)
        {
            //get the mac if any
            if(rtDevices[x].Pipeline.ip_addr != undefined)
            {
                ip = rtDevices[x].Pipeline.ip_addr;
            }

            break;
        }
    }

    return ip;
}


/*
    Function: nodeIcon

    look up the give node id and find its icon and path

    Parameters:
    strNodeID - node id to look up

    returns:
    return the icon and path for the icon
*/
function nodeIcon(strNodeID)
{
    //set the string given to return by default
    var strReturn = "/images/UnknownDevice.png";

    //if we have a table at all
    if(g_strIcons!=undefined && g_strIcons != null)
    {
        //to get the icon
        var strTemp = g_strIcons[strNodeID];

        //if we got back a valid string
        if(strTemp != undefined)
        {
            //set the return
            strReturn = strTemp;
        }
    }

    return strReturn;
}

//global table of devices
var devicetable = [];


/*
    Function: getFlowData

    get the data split by each flow and device info removed

    Parameters:
    rtData - raw event data from getEvents()

    returns:
    api/events data broken down by flow
*/
function getFlowData(rtData)
{
    //combine events
    var eData = [];

    //walk the raw data
    for(var x in rtData)
    {
        //get the event id
        var uuid = rtData[x].uuid;

        //if this is the 1st time we have seen this event
        if(typeof eData[uuid] == 'undefined')
        {
            eData[uuid] = {};
        }

        //add event to our collection
        eData[uuid].uuid        = rtData[x].uuid;
        eData[uuid].flow_id     = rtData[x].flow_id;

        //if this is an open event
        if(rtData[x].event == "open")
        {
            //init our open data
            eData[uuid].openTime    = rtData[x].time;
            eData[uuid].policy_id   = rtData[x].details.policy_id;
            eData[uuid].closeTime   = null;

            eData[uuid].mac         = rtData[x].mac;

            //setup our bytes
            eData[uuid].rx_bytes = 0;
            eData[uuid].tx_bytes = 0;
        }
        else if(rtData[x].event == "milestone" || rtData[x].event == "close") //if this is a close or milestone update
        {
            //set the time
            eData[uuid].closeTime = rtData[x].time;

            //update the ul/dl byte counts
            eData[uuid].rx_bytes = parseInt(rtData[x].details.rx_bytes);
            eData[uuid].tx_bytes = parseInt(rtData[x].details.tx_bytes);
        }
    }

    //create our return objecw
    var fData = {};

    //init the flow array in our return
    fData.flows = [];

    //init total byte counts
    fData.rx_total = 0;
    fData.tx_total = 0;

    //pre calculate the totals
    for(var x in eData)
    {
        //get the flow id
        var policy_id = eData[x].policy_id;

        //if this is the 1st time we've seen this id
        if(typeof policy_id != 'undefined')
        {
            //get up down (for debuggin purposes)
            var nRx = eData[x].rx_bytes;
            var nTx = eData[x].tx_bytes;

            //do global byte counts
            fData.rx_total += nRx;
            fData.tx_total += nTx;
        }

    }

    //combine by flow
    for(var x in eData)
    {
        //get the flow id
        var policy_id = eData[x].policy_id;

        //if this is a valid id
        if(typeof policy_id != 'undefined')
        {
            //get up down (for debuggin purposes)
            var nRx = eData[x].rx_bytes;
            var nTx = eData[x].tx_bytes;

            //if this is the 1st time we've seen this flow
            if(typeof fData.flows[policy_id] == 'undefined')
            {
                //init our new flow
                fData.flows[policy_id] = {};
                fData.flows[policy_id].flows = {};
                fData.flows[policy_id].rx_total         = 0;
                fData.flows[policy_id].tx_total         = 0;
            }

            //do flow byte counts
            fData.flows[policy_id].rx_total += nRx;
            fData.flows[policy_id].tx_total += nTx;


            //do flow percent
            fData.flows[policy_id].rx_percent = ((fData.flows[policy_id].rx_total/fData.rx_total)*100).toFixed(2);
            fData.flows[policy_id].tx_percent = ((fData.flows[policy_id].tx_total/fData.tx_total)*100).toFixed(2);
        }
    }

    return fData;
}


/*
    Function: flowTable

    get the flow table from the cloud for icons

    Parameters:
    callback - when we have grabbed the table this callback is called to signal done!

    returns:
    nothing but callback gets data for table on complete
*/
function flowTable(callback)
{
    //1st get the local language
    var strFile = "flows_"+getLanguage()+".js";

    //create the url by looking at the global path for this model of router
    var strURL = g_path.strings + strFile;

    //load the table
    $.ajax({
                url: strURL,
                dataType: "jsonp",
                jsonpCallback: "jsonpFlowsCallback",
                cache: false,
                timeout: 3000,
                success: function(data, status, request)
                {
                    //set the global language to the returned data
                    g_FlowTable = data;

                    //if the user gave us a callback tell
                    //them we are done
                    if(callback != undefined)
                    {
                        callback();
                    }
                },
                error: function(data,status)
                {
                    //we timed out, but still need to call the user
                    if(callback != undefined)
                    {
                        callback();
                    }
                }
    });
}

//get what quarter the month is in
function getQuarter(nMonth)
{
    //set up arrays with quarter ranges
    var nQuarter = 0;

    //set the current quarter
    if(nMonth >=1 && nMonth <= 3)
    {
        nQuarter = 1;
    }
    else if(nMonth >=4 && nMonth <= 6)
    {
        nQuarter = 2;
    }
    else if(nMonth >=7 && nMonth <= 9)
    {
        nQuarter = 3;
    }
    else
    {
        nQuarter = 4;
    }

    return nQuarter;
}

//get the range of dates for the quarters of the given year
function quarterRange(nQuarter, nYear)
{
    var range = {nQuarter: 1, strStart: "1-1-15", strEnd: "3-31-15"};

    range.nQuarter = nQuarter;
    //set the current quarter
    if(nQuarter == 1)
    {
        range.strStart = "1/1/"+nYear;
        range.strEnd = "3/31/"+nYear;
    }
    else if(nQuarter == 2)
    {
        range.strStart = "4/1/"+nYear;
        range.strEnd = "6/31/"+nYear;
    }
    else if(nQuarter == 3)
    {
        range.strStart = "7/1/"+nYear;
        range.strEnd = "9/31/"+nYear;
    }
    else // we are 4
    {
        range.strStart = "10/1/"+nYear;
        range.strEnd = "12/31/"+nYear;
    }

    return range;
}