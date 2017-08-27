/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
////////////////////////////////////////////////////////////////////////
/*
    Class: BWSubView
    Version:   1.0

    Sub View for the Dual graph node view
*/
////////////////////////////////////////////////////////////////////////

function BWSubView(uid,strParent)
{
    //set or local ref
    var self = this;

    //our list of items
    this.lsSubItem = [];

    //index to actual parent
    this.divParent = strParent;

    //parent x,y,width, and height
    this.nX         = 0;
    this.nY         = 0;
    this.nWidth     = $("#"+this.divParent).width();
    this.nHeight    = 0;//$("#"+this.divParent).height();
    this.nRHeight   = 0;

    //offset to add new item
    this.yOffSet    = 64;
    this.xOffSet    = 0;

    $("#"+this.divParent).resize(function()
        {
            self.nWidth     = $("#"+self.divParent).width();

            self.draw();
        });

    this.nIconXY = 64;


    this.uid = uid;

    self.strDivID = "bwsubview_"+self.uid;

    this.strDIV   =  "<div id=\""+self.strDivID+"\" style=\"width: 100%;height:"+self.nHeight+"px;display:none;\"></div>";

    //Add the div to the parent
    $(self.strDIV).appendTo("#"+self.divParent);

    //check for invalid values
    if(self.nWidth < 0)
    {
        self.nWidth = 1;
    }

    if(self.nHeight < 0)
    {
        self.nHeight = 1;
    }

    //creat the surface
    this.r = Raphael(   self.strDivID,
                        self.nWidth,
                        self.nHeight);

    //calc our positions
    var rX = self.nIconXY;
    var rY = 0; //-10
    var rW = (self.nWidth*.98)-(self.nIconXY*2);
    var rH = self.nHeight;

    //check for valid values
    if(rX<0)
    {
        rX = 1;
    }
    if(rY<0)
    {
        rY = 1;
    }
    if(rW<0)
    {
        rW = 1;
    }
    if(rH<0)
    {
        rH = 1;
    }

    //draw the background
    this.imgBackground = self.r.rect(rX, rY, rW, rH, 10).attr({fill: "#FFFFFF",stroke: "#FFFFFF",opacity: .3});

    //the number of items in the view
    this.count = function()
    {
        var nIndex = 0;

        for(var item in this.lsSubItem)
        {
            if (item == 'indexOf')
                continue;

            nIndex++;
        }

        return nIndex;
    }

    //add an item to the graph
    this.addItem = function(uid,                //unique identifier of flow/node
                            strName,            //label of item
                            icon,               //png icon in /images
                            nUpPercent,         //perent of meter
                            nDnPercent,         //perent of meter
                            nUpBytesPerSec,     //bw up label
                            nDnBytesPerSec,     //bw dn label
                            nOverSub)
    {
        var exists = false;//this.lsSubItem[uid];

        var nCount = 0;

        //try to find the uid
        for(var item in this.lsSubItem)
        {
            if (item == 'indexOf')
                continue;

            nCount++;
            if(this.lsSubItem[item].uid == uid)
            {
                exists = true;
                break;
            }
        }

        if(exists == false)
        {
            self.nWidth     = $("#"+self.divParent).width();

            var yPos = self.nY + (self.yOffSet*nCount);

            var newitem = new BWSubItem(    uid,
                                            strName,
                                            icon,
                                            nUpPercent,
                                            nDnPercent,
                                            nUpBytesPerSec,
                                            nDnBytesPerSec,
                                            self.r,
                                            0,
                                            yPos,
                                            self.nWidth,
                                            self.nHeight,
                                            self.onSelect,
                                            nOverSub);

            self.yOffSet = newitem.nIconXY*1.10;
            self.nRHeight += self.yOffSet;

            self.lsSubItem.push(newitem);

            self.draw();
        }
    }

    //delete item by uid
    this.delItem = function(uid)
    {
        var exists = false;

        //try to find the uid
        for(var item in this.lsSubItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsSubItem[item].uid == uid)
            {
                this.lsSubItem[item].destroy();

                delete this.lsSubItem[item];
                this.lsSubItem.splice(item,1);
                exists = true;
                break;
            }
        }

        if(exists == true)
        {
            this.draw();
        }
    }

    //update the item BW by uid
    this.updateItem = function( uid,
                                icon,
                                up,
                                dn,
                                nUpBytesPerSec,
                                nDnBytesPerSec,
                                nOverSub,
                                strRealName)
    {
        //try to find the uid
        for(var item in this.lsSubItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsSubItem[item].uid == uid)
            {
                //updates the icon if the path changes
                this.lsSubItem[item].updateIcon(icon);


                if(strRealName != undefined)
                {
                    this.lsSubItem[item].updateName(strRealName);
                }

                this.lsSubItem[item].setUpDn(   up,
                                                dn,
                                                nUpBytesPerSec,
                                                nDnBytesPerSec,
                                                nOverSub);
                break;
            }
        }
    }



    //set the bytes that went by and the time of the sample
    //after the 2nd call the item will show accurate numbers
    this.setSubBytesByTime = function(  uid,
                                        nBytesUp,       // bytes that went up
                                        nBytesDn,       // bytes that went down
                                        dtTime,         // this is a Date Object
                                        nOverSub)
    {

        //try to find the uid
        for(var item in this.lsSubItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsSubItem[item].uid == uid)
            {
                this.lsSubItem[item].setBytesByTime(nBytesUp,   // bytes that went up
                                                    nBytesDn,   // bytes that went down
                                                    dtTime,     // this is a Date Object
                                                    nOverSub);
                break;
            }
        }
    }

    //get all the items in an array
    this.enumItems = function()
    {
        return this.lsSubItem;
    }

    //redraw the display
    this.draw = function()
    {
        var nIndex = 0;


        //set the height of the display
        self.setHeight();

        var nValue = self.nWidth - 140;

        //fix debug error on initial drawing
        if(nValue<0)
        {
            nValue = 1;
        }

        self.imgBackground.attr({'width':nValue});

        for(var item in this.lsSubItem)
        {
            if (item == 'indexOf')
                continue;

            var yPos = this.nY + (this.yOffSet*nIndex++);
            var xPos = this.nX + this.xOffSet;

            self.lsSubItem[item].draw(  xPos,
                                        yPos,
                                        self.nWidth,
                                        self.nHeight);
        }
    }

    //kill every item in the list
    this.destroy = function()
    {
        for(var item in this.lsSubItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsSubItem[item] != null)
            {
                this.lsSubItem[item].destroy();

                delete this.lsSubItem[item];
            }
        }

        this.lsSubItem = [];

        $("#"+self.strDivID).remove();
    }

    //does this uid exist in the list
    this.exists = function(uid)
    {
        var bReturn = false;

        //try to find the uid
        for(var item in this.lsSubItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsSubItem[item].uid == uid)
            {
                bReturn = true;
                break;
            }
        }


        return bReturn;
    }

    //when the item is clicked on
    this.onSelect = function(pSelect)
    {
        if( self.pCurrentSelect!=null &&
            self.pCurrentSelect != undefined &&
            self.pCurrentSelect != pSelect)
        {
            //deselect the previous selction
            self.pCurrentSelect.select(false);
        }

        self.pCurrentSelect = pSelect;
    }

    //current visible state
    this.bShowing = false;

    //show/hide the subitem view
    this.show = function(bShow)
    {
        //set the main state
        self.bShowing = bShow;

        //default to no display/hide the subitems
        var display = "none";

        //if true set display to block model
        if(bShow == true)
        {
            display = "block";
        }

        //display/hid the subitem div
        $("#"+self.strDivID).css({"display":display});

        //set the height of the display
        self.setHeight();

        //ie9 requires the window to redraw to get the correct positioning.
        $("#"+self.divParent).resize();
    }

    this.setHeight = function()
    {

        self.yOffSet = self.nIconXY*1.10;

        var nSize = (this.yOffSet)*self.count();

        self.nHeight = nSize + 5;

        self.r.setSize( self.nWidth,
                        self.nHeight+1);

        $("#"+self.strDivID).height(self.nHeight);
        self.imgBackground.attr({'height':self.nHeight});
    }

    this.getHeight = function()
    {
        return self.nHeight;
    }

    //show/hide the subitem view
    this.isShowing = function()
    {
        return self.bShowing;
    }

    //the currently selected item
    this.pCurrentSelect = null;
}
