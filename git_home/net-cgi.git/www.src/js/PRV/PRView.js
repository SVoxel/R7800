/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
////////////////////////////////////////////////////////////////////////
/*
    Class: PRView
    Version:   1.0

    List to display the priority of devices on your system
    and allow you to move them up and down the list
*/
////////////////////////////////////////////////////////////////////////

function PRView(strParent,callback)
{
    //set or local ref
    var self = this;

    this.pCallback = callback;

    //our list of items
    this.lsItem = [];

    //index to actual parent
    this.divParent = strParent;

    //parent x,y,width, and height
    this.nX         = 0;
    this.nY         = 0;
    this.nWidth     = $("#"+this.divParent).width();
    this.nHeight    = $("#"+this.divParent).height();
    this.nRHeight   = 0;

    //offset to add new item
    this.yOffSet    = 64;
    this.xOffSet    = 0;
    $("#"+this.divParent).resize(function()
        {
            self.nWidth     = $("#"+self.divParent).width();

            self.draw();

        });


    //the number of items in the view
    this.count = function()
    {
        var nIndex = 0;

        for(var item in this.lsItem)
        {
            nIndex++;
        }

        return nIndex;
    }

    function slidedn(first, last) {
        var temp = self.lsItem[first];
        for (var index = first; index < last; index++) {
            var div = $("#" + temp.strDivID);
            var divp = $("#" + temp.strParentDiv);
            //console.log("moving " + temp.strDivID);
            div.animate({top: "+=" + temp.nHeight});
            temp.nPriority = index+2;
            temp.updatePriority();
            var otemp = temp;
            temp = self.lsItem[index + 1];
            self.lsItem[index+1] = otemp;
        }
    }

    function slideup(first, last) {
        var temp = self.lsItem[first];

        for (var index = first; index < last; index++) {
            var div = $("#" + temp.strDivID);
            var divp = $("#" + temp.strParentDiv);
            //console.log("moving " + temp.strDivID);
            div.animate({top: "-=" + temp.nHeight});
            temp.nPriority = index;
            temp.updatePriority();
            var otemp = temp;
            temp = self.lsItem[index + 1];
            self.lsItem[index - 1] = otemp;
        }
    }

    //add an item to the graph
    this.addItem = function(priority,
                            uid,                //unique identifier of node
                            icon,               //icon of the node
                            strName,            //label of item
                            strMAC,             //strMAC
                            strType)            //bw dn label
    {
        var exists = false;//this.lsItem[uid];

        //try to find the uid
        for(var item in this.lsItem)
        {
            if(this.lsItem[item].uid == uid)
            {
                exists = true;
                break;
            }
        }

        if(exists == false)
        {
            self.nWidth     = $("#"+this.divParent).width();

            //create the priority by position
            var nPriority = self.count()+1;

            var newitem = new PRItem(   uid,
                                        strName,
                                        strMAC,
                                        strType,
                                        nPriority,
                                        icon,
                                        this.divParent,
                                        self.nWidth,
                                        self.nHeight,
                                        self.onClick);

            self.yOffSet = newitem.nIconXY*1.10;
            self.nRHeight = self.count() * self.yOffSet;

            self.lsItem.push(newitem);
            newitem.index = self.lsItem.length - 1;

            var div = $("#" + newitem.strDivID);

            //make the div dragable
            div.draggable({
                containment: 'parent',
                cursor: 'move',
                axis: 'y',
                //revert: true,
                drag: function(event,ui) {
                    //lock the scroll while dragging to stop badness from happpening
                    $("#" + self.divParent).css("overflow","visible");

                    var bottom = ui.offset.top + ui.helper.height();
                    var parenttop = $("#" + self.divParent).offset().top;
                    var nitem = (ui.offset.top - parenttop) / ui.helper.height();
                    ui.helper.css("z-index",100);
                    // zero, before first item, < 1 before 2nd, < 2 before 3rd, etc.
                    //console.log(ui.helper.attr("id") + " " + ui.offset.top + " over " + nitem);
                },
                stop: function(event,ui) {
                    var bottom = ui.offset.top + ui.helper.height();
                    var parenttop = $("#" + self.divParent).offset().top;
                    var ndragger = self.lsItem.indexOf(newitem);
                    var nitem = (ui.offset.top - parenttop) / ui.helper.height();
                    // zero, before first item, 0 - 1 before 2nd, 1 - 2 before 3rd, etc.
                    //console.log(ui.helper.attr("id") + " " + ui.offset.top + " over " + nitem);
                    div.css("z-index","");

//                 slide down items by 1 from ceil(nitem) to ndragger
//                 redraw ndragger item at ceil(nitem)
                    var drop = Math.ceil(nitem);
                    if (nitem > ndragger) {
                        var drop1 = Math.round(nitem);
                        drop = Math.min(drop1,self.lsItem.length-1);
                        slideup(ndragger+1,drop+1);
                        //newitem.top = ui.offset.top + (ui.helper.height() * (first)
                    }
                    else {
                        slidedn(drop,ndragger);
                    }
                    var incsym;
                    if (nitem > drop) {
                        incsym = "-=";
                    }
                    else incsym = "+=";

                    $("#" + newitem.strDivID).animate({top: incsym + ui.helper.height() * Math.abs(nitem-drop)});
                    self.lsItem[drop] = newitem;
                    newitem.nPriority = drop+1;
                    newitem.updatePriority();

                    self.pCallback();

                    //re-allow scrolling after drop
                    $("#" + self.divParent).css("overflow","auto");
                }
            });
//  Drops are handled by the draggable callback "stop"
//            div.droppable();

            self.draw();

        }
        else
        {
            this.changeIcon(uid,icon);
        }
    }

    //delete item by uid
    this.delItem = function(uid)
    {
        var exists = false;

        //try to find the uid
        for(var item in this.lsItem)
        {
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


    //get the last 60 bw sample for an item
    this.getItemDL = function(uid)
    {
        var itemBW = [];

        //try to find the uid
        for(var item in this.lsItem)
        {
            if(this.lsItem[item].uid == uid)
            {
                itemBW = this.lsItem[item].getBandwidthDL();
                break;
            }
        }

        return itemBW;
    }

    //get all the items in an array
    this.enumItems = function()
    {
        return this.lsItem;
    }

    this.clearAll = function()
    {

            var lsPri = clone(self.enumItems());

            //loop through the keys
            for(var i = 0;i<lsPri.length;i++)
            {
                //get the item
                var prItem = lsPri[i];

                self.delItem(prItem.uid);
            }

    }

    //redraw the display
    this.draw = function()
    {
        var nIndex = 0;

        for(var item in this.lsItem)
        {
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
            if(this.lsItem[item] != null)
            {
                this.lsItem[item].destroy();

                delete this.lsItem[item];
            }
        }

        this.lsItem = [];
    }

    //when the item is clicked on if bUp = true up if not moving down
    this.onClick = function(pSelect, bUp)
    {
        // the object's index in the lsItem array
        var src = pSelect.nPriority - 1;
        var dst = null;

        function swapItems(a, b)
        {
            // update our list
            var item = self.lsItem[a];
            self.lsItem[a] = self.lsItem[b];
            self.lsItem[b] = item;

            // update the object priority
            var prio = self.lsItem[a].nPriority;
            self.lsItem[a].nPriority = self.lsItem[b].nPriority;
            self.lsItem[b].nPriority = prio;

            // notify the objects about the priority change
            self.lsItem[a].updatePriority();
            self.lsItem[b].updatePriority();

            // update the UI
            $("#" + self.lsItem[a].strDivID).swap({
                target: self.lsItem[b].strDivID,
                opacity: "0.5",
                speed: 500,
                callback: function() { }    // Callback once swap is complete
            });
        }

        if (bUp == true)
        {
            if (src <= 0)
            {
                // can't go up past the first element
                return;
            }
            dst = src - 1;
        }
        else
        {
            if (src >= (self.lsItem.length - 1))
            {
                // can't go down past the last element
                return;
            }
            dst = src + 1;
        }

        swapItems(src, dst);
        self.pCallback();
    }

    //delete item by uid
    this.changeIcon = function(uid, icon)
    {
        var exists = false;

        //try to find the uid
        for(var item in this.lsItem)
        {
            if(this.lsItem[item].uid == uid)
            {
                //change the icon
                this.lsItem[item].changeIcon(icon);

                break;
            }
        }
    }

    //the currently selected item
    this.pCurrentSelect = null;
}
