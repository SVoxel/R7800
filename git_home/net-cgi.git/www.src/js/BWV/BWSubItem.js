/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
////////////////////////////////////////////////////////////////////////
/*
    Class: BWSubItem
    Version:   1.0

    Bandwidth Sub Item for the Dual graph node view sub-view
*/
////////////////////////////////////////////////////////////////////////

function BWSubItem( uid,                //unique identifier of flow/node
                    strName,            //label of item
                    icon,               //png icon in /images
                    nUpPercent,         //perent of meter
                    nDnPercent,         //perent of meter
                    nUpBytesPerSec,     //bw up label
                    nDnBytesPerSec,     //bw dn label
                    pRaphael,           //the div of the parent to insert ourself into
                    x,
                    y,
                    width,              //width of item
                    height,             //height of item
                    pClickHandler,      //callback to call upon selection
                    nOverSub)
{
    //set or local ref
    var self = this;

    //set the oversub undefined = don't show icon
    this.nOverSub = nOverSub;

    this.overGreen      = "90-#004400:20-#00DD00";
    this.overYellow     = "90-#444400:20-#dddd00";
    this.overOrange     = "90-#443e00:20-#dd7f00";
    this.overRed        = "90-#440000:20-#dd0000";

    this.overStroke     = "#111111";

    this.nSubOffX = 110;

    // this.setColor = function()
    // {
    //  if(self.nOverSub == 2)
    //  {
    //      self.overColor = this.overOrange;
    //  }
    //  else if(self.nOverSub == 1)
    //  {
    //      self.overColor = this.overYellow;
    //  }
    //  else if(self.nOverSub == 0)
    //  {
    //      self.overColor = this.overGreen;
    //  }
    //  else
    //  {
    //      self.overColor = this.overOrange;
    //  }
    // }

    this.strOverSub = "";

    this.nIconSize = 48;
    this.nOverSubY = 8;
    this.nOverSubX = width-(self.nIconSize * 3.05);


    this.nPrevOverSub = -1;

    this.setColor = function()
    {
        var bReturn = false;

        if(self.nOverSub != self.nPrevOverSub)
        {
            self.nPrevOverSub = self.nOverSub;
            bReturn = true;

            if(self.nOverSub == 2)
            {
                self.strOverSub = g_path.images+"3-triangle.png";
            }
            else if(self.nOverSub == 1)
            {
                self.strOverSub = g_path.images+"2-square.png";
            }
            else if(self.nOverSub == 0)
            {
                self.strOverSub = g_path.images+"1-circle.png";
            }
            else
            {
                self.strOverSub = g_path.images+"3-triangle.png";
            }
        }

        return bReturn;
    }

    this.init = function()
    {
        self.initGraphics();
        self.initHandlers();
    }
    this.initGraphics = function()
    {
        //self.strParentDiv = strParentDiv;

        self.r = pRaphael;

        self.nX = x+self.nIconXY*1.25;//x;
        self.nY = y;//+self.nIconXY*.1;//y;
        self.nOverSubY = self.nY + 8;
        self.nWidth = width;
        self.nHeight =self.nIconXY*1.2;

        //meter bars
        self.meterUp = null;
        self.meterDn = null;

        //text label object
        self.txtLabel = null;

        //offsets and dimensions of meter bar
        self.nProgOffX = 278;
        self.nProgWidth = self.nWidth - self.nProgOffX - 50;
        self.nProgHeight = self.nRowHeight/2;
        self.nProgOffY = self.nProgHeight+2;

        self.imgHover = self.r.rect(0, 0, self.nWidth*.98,self.nIconXY*1.2, 10).attr({fill: "#3d464e",stroke: "#3d464e",opacity: 0});

        //create the router image
        self.strImagePath   = self.icon;

        //create the image object
        self.imgIcon = self.r.image(self.strImagePath,
                                    self.nX,
                                    self.nY,
                                    self.nIconXY,
                                    self.nIconXY);

        var fadetime = 200;

        //text xy position
        self.txtX = self.nIconXY + 4;
        self.txtY = self.nIconXY/2 + 6;

        //the font shouldn't be fixed it should be in a changable place
        self.font       = {font: '20px Century Gothic, Arial', opacity: 1.0};
        self.txtLabel = self.r.text(self.nX+self.txtX,self.nY+self.txtY,self.strName).attr(self.font).attr({fill: "#000000"}).attr({"text-anchor": "start",width: 256});

        //if we are to show the new oversub
        if(self.nOverSub != undefined)
        {
            //shorten the progress bars
            self.nProgWidth -= 50;

            if(self.setColor())
            {
                self.imgOverSub = self.r.image( self.strOverSub,
                                                self.nOverSubX,
                                                self.nOverSubY,
                                                self.nIconSize,
                                                self.nIconSize);
            }
        }

        //create our meter bars
        self.meterUp = new BWMeter( self.nX + self.nProgOffX,
                                    self.nY + ((self.nIconXY - self.nRowHeight)/2),
                                    self.nProgWidth,
                                    self.nProgHeight,
                                    self.nUp,
                                    self.r,
                                    "#555555",
                                    "#DDDDDD");

        self.meterDn = new BWMeter( self.nX + self.nProgOffX,
                                    self.nY + self.nProgOffY + ((self.nIconXY - self.nRowHeight)/2),
                                    self.nProgWidth,
                                    self.nProgHeight,
                                    self.nDown,
                                    self.r);
    }

    this.initHandlers = function()
    {
        //add a hover handler to every element
        self.addHover(self.meterUp);
        self.addHover(self.meterDn);
        self.addHover(self.txtLabel);
        self.addHover(self.imgIcon);
        //self.addHover("#"+self.strDivID);

        //add a click handler to every element
        self.addClick(self.meterUp);
        self.addClick(self.meterDn);
        self.addClick(self.txtLabel);
        self.addClick(self.imgIcon);
        //self.addClick("#"+self.strDivID);

    }

    //add a hover handler to the given control
    this.addHover = function(ctrl)
    {
        $(ctrl).hover(
        function(){
            if(self.selected() == false)
            {
                self.highlight(true);
            }
        },
        function ()
        {
            if(self.selected() == false)
            {
                self.highlight(false);
            }
        });
    }

    //add a click handler to the given control
    this.addClick = function(ctrl)
    {
        $(ctrl).click(
        function(){

            //default to unselect
            var bValue = true;

            //this allows us to select and unselect
            if(self.selected()==true)
            {
                bValue = false;
            }

            //set the selection state
            self.select(bValue);

            //if there is a defined click handler call it
            if(self.pClickHandler != null && self.pClickHandler != undefined)
            {
                var pReturn = null;

                if(bValue == true)
                {
                    pReturn = self;
                }

                self.pClickHandler(pReturn);
            }
        });
    }

    //select this item true/false
    this.select = function(bSelect)
    {
        //tell the item to highlight
        self.highlight(bSelect);

        //set the selected state
        self.bSelected = bSelect;
    }

    //tell the item to highlight or unhighlight
    this.highlight = function(bHighlight)
    {
        var nFull = 1.0;
        var nPrev = 1.0;

        //if we are selected / unselect the start and stop animations are different
        if(self.selected() == true)
        {
            nPrev = nFull;
        }

        if(bHighlight == true)
        {
            self.imgHover.attr({opacity: nPrev}).animate({"opacity": nFull},fadetime,"linear");
        }
        else
        {
            self.imgHover.attr({opacity: nPrev}).animate({"opacity": 0.0},fadetime,"linear");
        }
    }

    //get the current selected state
    this.selected = function()
    {
        return self.bSelected;
    }

    //draw th BWItem
    this.draw = function(x,y,newWidth,newHeight)
    {
        //set new positions
        self.nX = x+self.nIconXY*1.25;
        self.nY = y;

        self.nWidth     = newWidth;
        self.imgIcon.attr({"x":self.nX,"y":self.nY});
        self.nOverSubY = self.nY + 8;
        self.nOverSubX = newWidth-(self.nIconSize*3);
        self.imgOverSub.attr({"x":self.nOverSubX,"y":self.nOverSubY});

//        if (self.imgOverSub) self.imgOverSub.attr({"x":self.nOverSubX,"y":self.nOverSubY});
        self.txtLabel.attr({"x":self.nX+self.txtX,"y":self.nY+self.txtY});//self.txtY

        //there seems to be a bug in raphael that causes the internal children to get
        //an invalid dy object on certain text labels
        $(self.txtLabel.node).children().removeAttr('dy');

        //calculate the width and height
        self.nProgWidth     = self.nWidth - self.nProgOffX - 50 - (self.nIconXY*2);
        self.nProgHeight    = self.nRowHeight/2;
        self.nProgOffY      = self.nProgHeight+2;

        self.imgHover.attr({"width":newWidth-5});

        if(self.nOverSub != undefined)
        {
            //shorten the progress bars
            self.nProgWidth -= 50;

            if(self.setColor())
            {
                if(self.imgOverSub != undefined && self.imgOverSub != null)
                {
                    self.imgOverSub.remove();
                    self.imgOverSub = null;
                }

                self.imgOverSub = self.r.image( self.strOverSub,
                                                self.nOverSubX,
                                                self.nOverSubY,
                                                self.nIconSize,
                                                self.nIconSize);
            }
        }


        //fix bad values on initial draw
        if(self.nProgWidth < 0)
        {
            self.nProgWidth = 1;
        }

        //draw the up meter
        self.meterUp.update(self.nX+self.nProgOffX,
                            self.nY + ((this.nIconXY - this.nRowHeight)/2),
                            self.nProgWidth,
                            this.nProgHeight,
                            self.meterUp.nValue,
                            0, true);

        //draw the dn meter
        self.meterDn.update(self.nX+self.nProgOffX,
                            self.nY+self.nProgOffY+((this.nIconXY - this.nRowHeight)/2),
                            self.nProgWidth,
                            this.nProgHeight,
                            self.meterDn.nValue,
                            0, true);

        //make sure the data is #'s not text
        var updata = parseFloat(self.meterUp.nValue, 10).toFixed(0);
        var dndata = parseFloat(self.meterDn.nValue, 10).toFixed(0);
         //set the meters
        self.meterUp.setValue(  self.nUp,
                                updata);

        self.meterDn.setValue(  self.nDown,
                                dndata);
    };

    //set up down meter and label of the item
    this.setUpDn = function(nUpPercent,
                            nDnPercent,
                            nUpBytesPS,
                            nDnBytesPS,
                            nOverSub)
    {
        self.nUp = nUpPercent;
        self.nDown = nDnPercent;
        var updata = parseFloat(nUpBytesPS, 10).toFixed(0);
        var dndata = parseFloat(nDnBytesPS, 10).toFixed(0);

        self.meterUp.setValue(  self.nUp,
                                updata);

        self.meterDn.setValue(  self.nDown,
                                dndata);

        if(self.nOverSub != undefined)
        {
            this.nOverSub = nOverSub;

            //set the fill color
            if(self.setColor())
            {
                if(self.imgOverSub != undefined && self.imgOverSub != null)
                {
                    self.imgOverSub.remove();
                }
                self.imgOverSub = self.r.image( self.strOverSub,
                                                self.nOverSubX,
                                                self.nOverSubY,
                                                self.nIconSize,
                                                self.nIconSize);
            }
        }
    }

    //destroy this item
    this.destroy = function()
    {
        self.txtLabel.remove();
        self.imgIcon.remove();

        if(self.imgOverSub != undefined && self.imgOverSub != null)
        {
            self.imgOverSub.remove();
            self.imgOverSub = null;
        }

        self.meterUp.destroy();
        delete self.meterUp;
        self.meterDn.destroy();
        delete self.meterDn;
    };

    //change image if different
    this.updateIcon = function(strIconPath)
    {
        //if the icon is different
        if(self.strImagePath != strIconPath)
        {
            //create the router image
            self.strImagePath = strIconPath;

            // //kill the old icon
            // self.imgIcon.remove();

            // //create the image object
            // self.imgIcon =  self.r.image(   self.strImagePath,
            //                                 self.nX,
            //                                 self.nY,
            //                                 self.nIconXY,
            //                                 self.nIconXY);

            var a = self.imgIcon.attr({src: self.strImagePath});
        }

    }


    //change label
    this.updateName = function(strName)
    {
        //if the icon is different
        if(self.strName != strName && typeof self.txtLabel != 'undefined' && self.txtLabel != null)
        {
            //create the router image
            self.strName = strName;

            self.txtLabel.attr("text", self.strName);
        }

    }

    //time of the last sample
    this.lastTime = null;

    //last bytes up
    this.nBytesUp = 0;

    //last bytes down
    this.nBytesDn = 0;

    //get the time of the last setBytesByTime sample
    this.getLastTime = function()
    {
        return self.lastTime;
    }

    //set the bytes that went by and the time of the sample
    //after the 2nd call the item will show accurate numbers
    this.setBytesByTime = function( nBytesUp,       // bytes that went up
                                    nBytesDn,       // bytes that went down
                                    dtTime,         // this is a Date Object
                                    nOverSub)
    {
        var nDeltaTime=0;

        var nDeltaUp = 0;
        var nDeltaDn = 0;

        //if this is the 1st call we can to a meter change
        if(self.lastTime != null)
        {

            //calculate the delta timie
            nDeltaTime = (dtTime.getTime()-self.lastTime.getTime()).toFixed(3)/1000.0;

            if(nDeltaTime >= 1)
            {

            //if we havent wrapped
            if(nBytesUp >= self.nBytesUp)
            {
                nDeltaUp = nBytesUp - self.nBytesUp;

            }
            else // if we have fudge the value
            {
                nDeltaUp = nBytesUp;
            }

            //if we havent wrappped
            if(nBytesDn >= self.nBytesDn)
            {
                nDeltaDn = nBytesDn - self.nBytesDn;

            }
            else // if we have fudge the value
            {
                nDeltaDn = nBytesDn;
            }

            //calcualte bytes per second
            var nUpPerSec = nDeltaUp/nDeltaTime;
            var nDnPerSec = nDeltaDn/nDeltaTime;

            //if we have irrational values set the m to 0
            if(isNaN(nUpPerSec))
            {
                nUpPerSec = 0;
            }
            if(isNaN(nDnPerSec))
            {
                nDnPerSec = 0;
            }

            //turn bytes to bits
            nUpPerSec *= 8;
            nDnPerSec *= 8;

            var nMaxUp = g_nMaxUpload;
            var nMaxDn = g_nMaxDownload;
            var precision = 2;

            //calculate the percentage of the peter this is
            var UpPercent = ((nUpPerSec/nMaxUp).toFixed(precision) * 100).toFixed(0);
            var DnPercent = ((nDnPerSec/nMaxDn).toFixed(precision) * 100).toFixed(0);

            //set the meter itself now
            self.setUpDn(   UpPercent,
                            DnPercent,
                            nUpPerSec,
                            nDnPerSec,
                            nOverSub);

            //      copy the params fonr the next call

            self.nBytesUp = nBytesUp;

            self.nBytesDn = nBytesDn;

            self.lastTime = new Date(dtTime);

            }
        }
        else
        {

            //////////////////////////////////////////////////
            //      copy the params fonr the next call

            self.nBytesUp = nBytesUp;

            self.nBytesDn = nBytesDn;

            self.lastTime = new Date(dtTime);

            /////////////////////////////////////////////////
        }
    }

    ////////////////////////////////////////////////////////////////////
    //locals

    //copy the variables local
    this.uid            = uid;
    this.strName        = strName;
    this.icon           = icon;
    this.nUp            = nUpPercent;
    this.nDown          = nDnPercent;

    //icon size
    this.nIconXY = 64;
    this.nRowHeight = 48;

    //icon image object
    this.imgIcon = null;

    //this.strParentDiv = strParentDiv;

    //item div id
    //this.strDivID = null;

    //the content of our item div
    this.strDIV   =  null;

    //raphael
    this.r = null;

    this.nX = null;
    this.nY = null;
    this.nWidth = width;
    this.nHeight = self.nIconXY*1.2;

    //meter bars
    this.meterUp = null;
    this.meterDn = null;

    //text label object
    this.txtLabel = null;

    //offsets and dimensions of meter bar
    this.nProgOffX = 258;//this.nIconXY + this.nColOffX;
    this.nProgWidth = null;
    this.nProgHeight = null;
    this.nProgOffY = null;


    this.imgHover = null;

    //create the router image
    this.strImagePath   = null;

    //create the image object
    this.imgIcon = null;

    //create the oversub status object
    this.imgOverSub = null;

    var fadetime = 200;

    //text xy position
    this.txtX = null;
    this.txtY = null;

    //the font shouldn't be fixed it should be in a changable place
    this.font     = null;
    this.txtLabel = null;

    //create our meter bars
    this.meterUp = null;

    this.meterDn = null;

    //the click handler callback
    this.pClickHandler = pClickHandler;

    //are we selected?
    self.bSelected = false;

    ////////////////////////////////////////////////////////////////////

    //call the constructor for this class and set it up
    self.init();
}
