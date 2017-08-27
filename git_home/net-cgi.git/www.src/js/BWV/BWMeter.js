/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
////////////////////////////////////////////////////////////////////////
/*
    Class: BWMeter
    Version:   1.0

    Bandwidth View meter for the Dual graph node view and subview
*/
////////////////////////////////////////////////////////////////////////

function BWMeter(   x,
                    y,
                    width,
                    height,
                    value,
                    raphael,
                    color1,
                    color2)
{
    var self = this;

    this.TestValue = 0;

    this.nX = x;
    this.nY = y;
    this.nWidth = width;
    this.nHeight = height;
    this.nValue =value;
    this.pR = raphael;
    this.previousValue = 0;

    if(color1 != undefined)
    {
        this.strColor1 = color1;
        this.strColor2 = color2
    }
    else
    {
        //blue bars by default
        this.strColor1 = "#006493";//"#3682b3";
        this.strColor2 = "#33bfff";//"#89cde0";
    }

    var rgbTrackFill    = "#AAAAAA";
    var rgbTrackStroke  = "#888888";

    var rgbText         = "#000000";
    var rgbTextShadow   = "#FFFFFF";

    var inset = 1;
    var offX = x + inset;
    var offY = y + inset;
    var offW = width - (inset*2);
    var offH = height - (inset*2);

    var track = null;
    var meter = null;

    var previousTime = 0;
    var label = null;
    var labelshadow = null;
    var shadowoffset = 1;

    var labelOffest = 12;


    this.destroy = function()
    {
        if(track != null)
        track.remove();
        if(meter != null)
        meter.remove();
        if(label != null)
        label.remove();
        if(labelshadow != null)
        labelshadow.remove();
        track = null;
        meter = null;
        label = null;
        labelshadow = null;
    };

    this.update = function( x,
                            y,
                            width,
                            height,
                            nPercent,
                            nBytesPerSec)
    {
        inset = 1;
        offX = x + inset;
        offY = y + inset;
        offW = width - (inset*2);
        offH = height - (inset*2);
        self.nWidth = width;

        var forceRedraw = false;
        if (arguments.length == 7) {
            forceRedraw = arguments[6];
        }

        if(forceRedraw || track == null || track == undefined || meter == null || meter == undefined || self.nX != x || self.nY != y)
        {
            self.nX = x;
            self.nY = y;
            self.nWidth = width;
            self.nHeight = height;

            //self.destroy();

            if(track == null || track == undefined)
            {
                track = self.pR.rect(x, y, width,height,height/2).attr({fill: rgbTrackFill,stroke: rgbTrackStroke});
                /*track = self.pR.image(    "images/big_sqmeter_track.png",
                                        x,
                                        y,
                                        width,
                                        height);*/
            }
            else
            {
                track.attr({x: x, y: y, 'width': width, 'height': height, 'r': height/2, fill: rgbTrackFill,stroke: rgbTrackStroke});
            }

            var percent = nPercent;// self.TestValue; // test code to test the meter

            if(percent>100)
                percent = 100;

            if(percent<0)
                percent = 0;

            percent /=100;

            if(meter == null || meter == undefined)
            {
                meter = this.pR.rect(offX,offY,offW*0,offH).attr({fill: "90-"+this.strColor1+":20-"+this.strColor2,stroke: "90-#89cde0:20-#3682b3"});
                /* meter = self.pR.image(   "images/big_sqmeter_gray.png",
                                        offX,
                                        offY,
                                        offW,
                                        offH);*/
            }
            else
            {
                meter.attr({x: offX, y: offY, 'width':0, 'height': offH, fill: "90-"+this.strColor1+":20-"+this.strColor2,stroke: "90-#89cde0:20-#3682b3"});
            }

            var labeltext = bytesToSize(nBytesPerSec, 1) + "ps" ;

            if(label == null || label == undefined)
            {
                //select he font
               var attr         = {font: '11px Century Gothic, Arial', opacity: 1.0};

                //create the text label for the button
                labelshadow     = this.pR.text(offX+(width/2)+shadowoffset,offY+labelOffest+shadowoffset, labeltext).attr(attr).attr({fill: rgbTextShadow}).attr({"text-anchor": "start"});
                label   = this.pR.text(offX+(width/2),offY+labelOffest, labeltext).attr(attr).attr({fill: rgbText}).attr({"text-anchor": "start"});//.attr({"stroke": "#000000","stroke-opacity":.5});
                label.attr('text', labeltext);
                labelshadow.toFront();
                label.toFront();
            }
            else
            {
                labelshadow.attr({x: offX+(width/2)+shadowoffset, y: offY+labelOffest+shadowoffset});
                label.attr({x: offX+(width/2), y: offY+labelOffest});
                labelshadow.toFront();
                label.toFront();
            }
        }
        else
        {
            this.setValue(  nPercent,
                            nBytesPerSec);
        }
    };

    this.strLabel = "";

    this.setValue = function(   nPercent,
                                nBytesPerSec)
    {
        //if we have a new value
        if(self.nValue != nPercent)
        {
            self.nValue = nPercent;
            var percent = nPercent;

            //are they in the valid range
            if(percent>100)
                percent = 100;

            if(percent<0)
                percent = 0;

            //make are decimal
            percent /=100;

            //put in the bottom layer
            track.attr({width: self.nWidth});

            //if we are on weird browser
            if ($.browser.msie && $.browser.version < 9) meter.attr({r: nPercent > 2 ? self.nHeight/2 : 0})
            else meter.attr({r:self.nHeight/2})

            //draw the meter
            meter.attr({width: offW*self.previousValue});
            meter.stop();
            meter.animate({width: offW*percent},1000,"<>");

            //cache off the state
            self.previousValue=percent;
        }
        else // just make sure the meter stays the same size
        {
            meter.attr({width: offW*self.previousValue});
        }

        var labeltext = bytesToSize(nBytesPerSec, 1) + "ps" ;

        //if(self.strLabel!=labeltext)
        {
            self.strLabel=labeltext;

            //if the label doesn't exist create it.
            if(label == null || label == undefined)
            {
                //select he font
               var attr         = {font: '11px Century Gothic, Arial', opacity: 1.0};

                //create the text label for the button
                labelshadow     = this.pR.text(offX+(width/2)+shadowoffset,offY+labelOffest+shadowoffset, labeltext).attr(attr).attr({fill: rgbTextShadow}).attr({"text-anchor": "start"});
                label   = this.pR.text(offX+(width/2),offY+labelOffest, labeltext).attr(attr).attr({fill: rgbText}).attr({"text-anchor": "start"});
            }
            else
            {
                label.attr('text', labeltext);
                labelshadow.attr('text', labeltext);
                var position =offX+((self.nWidth/2)-(label.getBBox().width/2));
                label.attr('x',position);
                labelshadow.attr('x',position+shadowoffset);

                //there seems to be a bug in raphael that causes the internal children to get
                //an invalid dy object on certain text labels
                $(label.node).children().removeAttr('dy');
                $(labelshadow.node).children().removeAttr('dy');
            }
        }
    };
}
