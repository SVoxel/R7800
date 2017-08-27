/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
////////////////////////////////////////////////////////////////////////
/*
    Class: BWView
    Version:   1.0

    Bandwidth View for the Dual graph node viwe
*/
////////////////////////////////////////////////////////////////////////

function BWView(strParent,bSubItems,pSelectCallback)
{
    //set or local ref
    var self = this;

    //callback for selection
    this.pSelectCallback = pSelectCallback;

    //our list of items
    this.lsItem = [];

    //index to actual parent
    this.divParent = strParent;

    //do we allow subitems?
    this.bSubItems = bSubItems;

    //parent x,y,width, and height
    this.nX         = 0;//$("#"+this.divParent).position().left;
    this.nY         = 0;//$("#"+this.divParent).position().top;
    this.nWidth     = $("#"+this.divParent).width();
    this.nHeight    = $("#"+this.divParent).height();
    this.nRHeight   = 0;

    //offset to add new item
    this.yOffSet    = 64;
    this.xOffSet    = 0;

    //the raphael object to do all the drawing
    /*this.r = Raphael( this.divParent,
                        this.nWidth,
                        0);//this.nHeight);*/

    $("#"+this.divParent).resize(function()
        {
            self.nWidth     = $("#"+self.divParent).width();
            //self.nHeight  = $("#"+self.divParent).height();

            /*self.r.setSize(   self.nWidth,
                            self.nRHeight);*/

            self.draw();

        });


    //the number of items in the view
    this.count = function()
    {
        var nIndex = 0;

        for(var item in this.lsItem)
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
                            nOverSub)           //4-1   -  4 = none/green, 3 = yellow,2 = orange,1 red and blocked
    {
        var exists = false;//this.lsItem[uid];

        //try to find the uid
        for(var item in this.lsItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsItem[item].uid == uid)
            {
                exists = true;
                break;
            }
        }

        if(exists == false)
        {
            //self.nWidth   = $("#"+this.divParent).width();

            var newitem = new BWItem(   uid,
                                        strName,
                                        icon,
                                        nUpPercent,
                                        nDnPercent,
                                        nUpBytesPerSec,
                                        nDnBytesPerSec,
                                        this.divParent,
                                        self.nWidth,
                                        self.nHeight,
                                        self.onSelect,
                                        nOverSub);

            self.yOffSet = newitem.nIconXY*1.10;
            self.nRHeight = self.count() * self.yOffSet;

            self.lsItem.push(newitem);

            self.draw();

        }
    }

    //delete item by uid
    this.delItem = function(uid)
    {
        var exists = false;//this.lsItem[uid];

        //try to find the uid
        for(var item in this.lsItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsItem[item].uid == uid)
            {
                this.lsItem[item].destroy();

                delete this.lsItem[item];
                this.lsItem.splice(item,1);

                exists = true;
                break;
            }
        }

        if(exists == true)
        {
            //this.draw();
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
        for(var item in this.lsItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsItem[item].uid == uid)
            {
                //updates the icon if the path changes
                this.lsItem[item].updateIcon(icon);

                if(strRealName != undefined)
                {
                    this.lsItem[item].updateName(strRealName);
                }

                //set the bandwidth info if it changes
                this.lsItem[item].setUpDn(  up,
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
    this.setBytesByTime = function( uid,
                                    nBytesUp,       // bytes that went up
                                    nBytesDn,       // bytes that went down
                                    dtTime,         // this is a Date Object
                                    nOverSub)
    {

        //try to find the uid
        for(var item in this.lsItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsItem[item].uid == uid)
            {
                this.lsItem[item].setBytesByTime(   nBytesUp,   // bytes that went up
                                                    nBytesDn,   // bytes that went down
                                                    dtTime,     // this is a Date Object
                                                    nOverSub);
                break;
            }
        }
    }

    //get the last 60 bw sample for an item
    this.getItemDL = function(uid)
    {
        var itemBW = [];

        //try to find the uid
        for(var item in this.lsItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsItem[item].uid == uid)
            {
                itemBW = this.lsItem[item].getBandwidthDL();
                break;
            }
        }

        return itemBW;
    }

    //get the last 60 bw sample for an item
    this.getItemUL = function(uid)
    {
        var itemBW = [];

        //try to find the uid
        for(var item in this.lsItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsItem[item].uid == uid)
            {
                itemBW = this.lsItem[item].getBandwidthUL();
                break;
            }
        }

        return itemBW;
    }


    //get the last 60 bw sample for an item
    this.getTotalDL = function(selectedUID)
    {
        var itemBW = [];
         for (var i = 0; i <= 59; i += 1)
            itemBW.push([i, 0]);

        //try to find the uid
        for(var item in this.lsItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsItem[item].uid != selectedUID)
            {
                var bw = this.lsItem[item].getBandwidthDL();

                 for (var i = 0; i <= 59; i += 1)
                    itemBW[i][1] += bw[i][1];
            }
        }

        return itemBW;
    }

    //get the last 60 bw sample for an item
    this.getTotalUL = function(selectedUID)
    {
        var itemBW = [];
         for (var i = 0; i <= 59; i += 1)
            itemBW.push([i, 0]);

        //try to find the uid
        for(var item in this.lsItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsItem[item].uid != selectedUID)
            {
                var bw = this.lsItem[item].getBandwidthUL();

                 for (var i = 0; i <= 59; i += 1)
                    itemBW[i][1] += bw[i][1];
            }
        }

        return itemBW;
    }

    //get all the items in an array
    this.enumItems = function()
    {
        return this.lsItem;
    }

    //redraw the display
    this.draw = function()
    {
        var nIndex = 0;

        self.nWidth     = $("#"+this.divParent).width();

        for(var item in this.lsItem)
        {
            if (item == 'indexOf')
                continue;

            var yPos = this.nY + (this.yOffSet*nIndex++);
            var xPos = this.nX + this.xOffSet;

            self.lsItem[item].draw( xPos,
                                    yPos,
                                    self.nWidth,
                                    self.nHeight);
        }
    }

    //kill every item in the list
    this.destroy = function()
    {
        for(var item in this.lsItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsItem[item] != null)
            {
                this.lsItem[item].destroy();

                delete this.lsItem[item];
            }
        }

        this.lsItem = [];
    }

    //does this uid exist in the list
    this.exists = function(uid)
    {
        var bReturn = false;

        //try to find the uid
        for(var item in this.lsItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsItem[item].uid == uid)
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

        var nSubHeight = 0;

        if( self.pCurrentSelect!=null &&
            self.pCurrentSelect != undefined &&
            self.pCurrentSelect != pSelect)
        {
            nSubHeight = self.pCurrentSelect.getSubHeight();
        }

        if(self.pSelectCallback != undefined && self.pSelectCallback != null)
        {
            self.pSelectCallback(self.pCurrentSelect);
        }
        else
        {
            self.pSelectCallback(null);
        }
    }

    //add an item to the graph
    this.addSubItem = function( uidParent,          //uid of the parent node to add this too
                                uid,                //unique identifier of flow/node
                                strName,            //label of item
                                icon,               //png icon in /images
                                nUpPercent,         //perent of meter
                                nDnPercent,         //perent of meter
                                nUpBytesPerSec,     //bw up label
                                nDnBytesPerSec,     //bw dn label
                                nOverSub)
    {
        //try to find the uid
        for(var item in this.lsItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsItem[item].uid == uidParent)
            {
                this.lsItem[item].addSubItem(   uid,                //unique identifier of flow/node
                                                strName,            //label of item
                                                icon,               //png icon in /images
                                                nUpPercent,         //perent of meter
                                                nDnPercent,         //perent of meter
                                                nUpBytesPerSec,     //bw up label
                                                nDnBytesPerSec,     //bw dn label)
                                                nOverSub);
                break;
            }
        }
    }

    //delete item by uid
    this.delSubItem = function( uidParent,          //uid of the parent node to add this too
                                uid)                //uid to delete
    {
        //try to find the uid
        for(var item in this.lsItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsItem[item].uid == uidParent)
            {
                this.lsItem[item].delSubItem(uid);              //unique identifier of flow/node
                break;
            }
        }
    }

    //update the item BW by uid
    this.updateSubItem = function(  uidParent,          //uid of the parent node to add this too
                                    uid,
                                    icon,
                                    up,
                                    dn,
                                    nUpBytesPerSec,
                                    nDnBytesPerSec,
                                    nOverSub,
                                    strRealName)
    {
         //try to find the uid
        for(var item in this.lsItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsItem[item].uid == uidParent)
            {
                this.lsItem[item].updateSubItem(uid,
                                                icon,
                                                up,
                                                dn,
                                                nUpBytesPerSec,
                                                nDnBytesPerSec,
                                                nOverSub,
                                                strRealName);
                break;
            }
        }
    }


    //set the bytes that went by and the time of the sample
    //after the 2nd call the item will show accurate numbers
    this.setSubBytesByTime = function(  uidParent,      //uid of the parent node to add this too
                                        uid,
                                        nBytesUp,       // bytes that went up
                                        nBytesDn,       // bytes that went down
                                        dtTime,         // this is a Date Object
                                        nOverSub)
    {

        //try to find the uid
        for(var item in this.lsItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsItem[item].uid == uidParent)
            {
                this.lsItem[item].setSubBytesByTime(uid,
                                                    nBytesUp,   // bytes that went up
                                                    nBytesDn,   // bytes that went down
                                                    dtTime,     // this is a Date Object
                                                    nOverSub);
                break;
            }
        }
    }

    //get all the items in an array
    this.enumSubItems = function(uidParent)//uid of the parent node to enum
    {
        var items = [];
        //try to find the uid
        for(var item in this.lsItem)
        {
            if (item == 'indexOf')
                continue;

            if(this.lsItem[item].uid == uidParent)
            {
                items = this.lsItem[item].enumSubItems();               //unique identifier of flow/node
                break;
            }
        }

        return items;
    }

    //the currently selected item
    this.pCurrentSelect = null;
}
