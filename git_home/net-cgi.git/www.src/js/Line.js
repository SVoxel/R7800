/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */

function Line(startX, startY, endX, endY, raphael,time,callback) {
    var start = {
        x: startX,
        y: startY
    };
    var end = {
        x: endX,
        y: endY
    };
    var dash=".";

    //var rgb = "#cacaca"
    var rgb = "#777777";
    //the is the fina path of the line
    var getPath = function() {
        return "M" + start.x + " " + start.y + " L" + end.x + " " + end.y;
    };

    //this is the start frame of the animation
    var getStart = function() {
        return "M" + start.x + " " + start.y + " L" + start.x + " " + start.y;
    };

    var redraw = function() {
        node.attr("path", getPath())/*.attr("stroke-dasharray",dash)*/.attr("stroke-width",2).attr("stroke",rgb);
    };

    var killme = function() {
        node.remove();
        node = null;
    };

    var node = null;

    //are we supposed to animite over time?
    if(time != undefined && time != 0)
    {
        node = raphael.path(getStart())/*.attr("stroke-dasharray",dash)*/.attr("stroke-width",2).attr("stroke",rgb).animate({path: getPath()},time,"linear",function(){

            if(callback!=undefined)
            {
                callback();
            }
        }).toBack();
    }
    else //if not just draw the line
    {
        node = raphael.path(getPath())/*.attr("stroke-dasharray",dash)*/.attr("stroke-width",2).attr("stroke",rgb).toBack();
    }

    return {
        updateStart: function(x, y) {
            start.x = x;
            start.y = y;
            redraw();
            return this;
        },
        updateEnd: function(x, y) {
            end.x = x;
            end.y = y;
            redraw();
            return this;
        },
        animate: function(x1, y1, x2, y2,opacity,time) {

            var strNewPath =  "M" + x1 + " " + y1 + " L" + x2 + " " + y2;
            /*
            node.animate({path: strNewPath,"opacity": opacity},time,"linear",function(){
                        start.x = x1;
                        start.y = y1;
                        end.x = x2;
                        end.y = y2;
                    }).toBack();*/

            this.updateStart(x1,y1);
            this.updateEnd(x2,y2);

            return this;
        },
        kill: function()
        {
            killme();
            return this;
        }
    };
};
