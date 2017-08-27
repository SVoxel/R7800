/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
////////////////////////////////////////////////////////////////////////
/*
    Class: PRItem
    Version:   1.0

    Priority Item for the Priority View
*/
////////////////////////////////////////////////////////////////////////


function PRItem(    uid,                //unique identifier of flow/node
                    strName,            //label of item
                    strMac,
                    strType,
                    nPriority,
                    icon,               //png icon in /images
                    strParentDiv,       //the div of the parent to insert ourself into
                    width,              //width of item
                    height,             //height of item
                    pClickHandler)      //callback to call upon selection
{
    //set or local ref
    var self = this;

    this.strMAC = strMac;
    this.strType = strType;

    this.nPriority = nPriority.toString();


    //column x values
    this.arColumn = [];


    this.init = function()
    {
        self.initGraphics();
        self.initHandlers();
    }

    this.initGraphics = function()
    {
        self.strParentDiv = strParentDiv;

        self.strDivID = "pritem_"+uid;
        self.strSubDivID = "prsub_"+uid;

        self.strDIV   =  "<div id=\""+self.strDivID+"\" style=\"width: 100%;height:"+self.nHeight+"px;\"></div>";

        //Add the div to the parent
        $(self.strDIV).appendTo("#"+self.strParentDiv);

        self.nWidth = 900;

        //old way of using raphael
        //self.r = pRaphael;
        self.r = Raphael(   self.strDivID,
                            self.nWidth,
                            this.nHeight);

        self.nX = self.nIconXY*.1;//x;
        self.nY = self.nIconXY*.1;//y;
        self.nWidth = width;
        self.nHeight =self.nIconXY*1.2;


        //column offsets
        self.arColumn[0] = 0;
        self.arColumn[1] = this.arColumn[0]  + 72;
        self.arColumn[2] = this.arColumn[1]  + 250;
        self.arColumn[3] = this.arColumn[2]  + 200;

        //meter bars
        self.meterUp = null;
        self.meterDn = null;

        //text label object
        self.txtName = null;
        self.txtType = null;
        self.txtMAC = null;

        //offsets and dimensions of meter bar
        self.nProgOffX = 350;//self.nIconXY + self.nColOffX;
        self.nProgWidth = self.nWidth - self.nProgOffX - 50;
        self.nProgHeight = self.nRowHeight/2;
        self.nProgOffY = self.nProgHeight+2;


        //the font shouldn't be fixed it should be in a changable place
        self.font       = {font: '16px Century Gothic, Arial', opacity: 1.0};



        self.imgPriorityBox = self.r.rect(0, 6, 64,64, 10).attr({fill: "#555555",stroke: "#555555",opacity: 1});
        self.txtPriority = self.r.text(self.nX+self.txtX+14,self.nY+32, self.nPriority).attr(self.font).attr({fill: "#ffffff"}).attr({"text-anchor": "middle",width: 32});


        self.rectUp = self.r.rect(35, 20, 16,16, 4).attr({fill: "#222222",stroke: "#222222",opacity: 1});
        self.rectDn = self.r.rect(35, 40, 16,16, 4).attr({fill: "#222222",stroke: "#222222",opacity: 1});

        //create the image object
        self.imgUp = self.r.image(  g_path.images + "lil_arrow_up_wht.png",
                                    39,
                                    24,
                                    8,
                                    8);
        //create the image object
        self.imgDn = self.r.image(  g_path.images + "lil_arrow_dn_wht.png",
                                    39,
                                    44,
                                    8,
                                    8);



        //self.imgSubItems = self.sub_r.rect(self.nIconXY, -10, (self.nWidth*.98)-(self.nIconXY*2),self.nSubDivY, 10).attr({fill: "#4e5964",stroke: "#4e5964",opacity: 0.0});

        //create the router image
        self.strImagePath   = self.icon;//iconPath(self.icon);

        //create the image object
        self.imgIcon = self.r.image(self.strImagePath,
                                    self.nX + self.arColumn[1],
                                    self.nY,
                                    self.nIconXY,
                                    self.nIconXY);


        var fadetime = 200;

        //text xy position
        self.txtX = self.nIconXY + 12;
        self.txtY = self.nIconXY/2;


        self.txtName = self.r.text(self.nX+self.arColumn[1]+self.txtX,self.nY+self.txtY,self.strName).attr(self.font).attr({fill: "#000000"}).attr({"text-anchor": "start",width: 200});

        //the font shouldn't be fixed it should be in a changable place
        self.txtType = self.r.text(self.nX+self.arColumn[2]+self.txtX,self.nY+32,self.strType).attr(self.font).attr({fill: "#000000"}).attr({"text-anchor": "start",width: 200});

        //the font shouldn't be fixed it should be in a changable place
        self.txtMAC = self.r.text(self.nX+self.arColumn[3]+self.txtX,self.nY+32,self.strMAC).attr(self.font).attr({fill: "#000000"}).attr({"text-anchor": "start",width: 400});
    }

    this.initHandlers = function()
    {
        //hook up the hovers
        self.rectUp.hover(
        function(){
            this.attr({fill: "#398aaf",stroke: "#398aaf"});
        },
        function ()
        {
            this.attr({fill: "#222222",stroke: "#222222"});
        });

        self.imgUp.hover(
        function(){
            self.rectUp.attr({fill: "#398aaf",stroke: "#398aaf"});
        },
        function ()
        {
            self.rectUp.attr({fill: "#222222",stroke: "#222222"});
        });


        self.rectDn.hover(
        function(){
            this.attr({fill: "#398aaf",stroke: "#398aaf"});
        },
        function ()
        {
            this.attr({fill: "#222222",stroke: "#222222"});
        });

        self.imgDn.hover(
        function(){
            self.rectDn.attr({fill: "#398aaf",stroke: "#398aaf"});
        },
        function ()
        {
            self.rectDn.attr({fill: "#222222",stroke: "#222222"});
        });
        //hook up the clicks
        self.rectUp.click(
            function(){
                self.onClickUp();
        });

        self.imgUp.click(
            function(){
                self.onClickUp();
        });

        self.rectDn.click(
            function(){
                self.onClickDn();
        });

        self.imgDn.click(
            function(){
                self.onClickDn();
        });
    }

    this.onClickUp = function()
    {
        self.pClickHandler(self,true);
    }

    this.onClickDn = function()
    {
        self.pClickHandler(self,false);
    }



    this.killHandlers = function()
    {
        //add a hover handler to every element
        //self.delHover(self.txtName);
        //self.delHover(self.imgIcon);
        //self.delHover("#"+self.strDivID);

        //add a click handler to every element
        //self.delClick(self.txtName);
        //self.delClick(self.imgIcon);
        //self.delClick("#"+self.strDivID);

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


    //add a hover handler to the given control
    this.delHover = function(ctrl)
    {
        $(ctrl).unbind('hover');
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


    //add a click handler to the given control
    this.delClick = function(ctrl)
    {
        $(ctrl).unbind('click');
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
        var nFull = .5;//1.0;
        var nPrev = .5;//1.0;

        //if we are selected / unselect the start and stop animations are different
        if(self.selected() == true)
        {
            nPrev = nFull;
        }

        if(bHighlight == true)
        {
            self.imgPriorityBox.attr({opacity: nPrev}).animate({"opacity": nFull},fadetime,"linear");
        }
        else
        {
            self.imgPriorityBox.attr({opacity: nPrev}).animate({"opacity": 0.0},fadetime,"linear");
        }
    }

    //get the current selected state
    this.selected = function()
    {
        return self.bSelected;
    }


    this.PrevW = 0;
    this.PrevH = 0;

    //draw th prItem
    this.draw = function(x,y,newWidth,newHeight)
    {
        //set new positions
        self.nWidth     = newWidth;
        self.imgIcon.attr({"x":self.nX+self.arColumn[1],"y":self.nY});
        self.txtName.attr({"x":self.nX+self.txtX+self.arColumn[1],"y":self.nY+self.txtY});

        //calculate the width and height
        self.nProgWidth     = self.nWidth - self.nProgOffX - 50;
        self.nProgHeight    = self.nRowHeight/2;
        self.nProgOffY      = self.nProgHeight+2;

        if(self.PrevW != self.nWidth || self.PrevH != self.nHeight)
        {
            self.PrevW = self.nWidth;
            self.PrevH = self.nHeight;

            self.r.setSize(self.nWidth,self.nHeight);

            //self.imgPriorityBox.attr({width:self.nWidth-16});
        }

    };

    this.updatePriority = function()
    {
        self.txtPriority.attr({"text":self.nPriority});
        self.txtPriority.attr({"y":36});
    }


    //destroy this item
    this.destroy = function()
    {
        self.txtName.remove();
        self.imgIcon.remove();
        self.imgPriorityBox.remove();
        self.txtPriority.remove();

        $("#"+self.strDivID).remove();
        $("#"+self.strSubDivID).remove();
    };

    this.changeIcon = function(icon)
    {
        if(icon != self.strImagePath)
        {
            self.imgIcon.remove();

            self.strImagePath = icon;

            //create the image object
            self.imgIcon = self.r.image(self.strImagePath,
                                        self.nX + self.arColumn[1],
                                        self.nY,
                                        self.nIconXY,
                                        self.nIconXY);
        }
    }

    ////////////////////////////////////////////////////////////////////
    //locals

    //copy the variables local
    this.uid            = uid;

    this.strName        = strName;

    if(strName.length > 36)
    {
        this.strName = strName.substring(0,35);
    }

    this.icon           = icon;

    //icon size
    this.nIconXY = 64;
    this.nRowHeight = 48;

    //icon image object
    this.imgIcon    = null;
    this.imgUp      = null;
    this.imgDn      = null;


    this.rectUp     = null;
    this.rectDn     = null;

    this.strParentDiv = strParentDiv;

    //item div id
    this.strDivID = null;

    //the content of our item div
    this.strDIV   =  null;

    //raphael
    this.r = null;

    this.nX = null;
    this.nY = null;
    this.nWidth = width;
    this.nHeight = self.nIconXY*1.2;

    //text label object
    this.txtName = null;

    //offsets and dimensions of meter bar
    this.nProgOffX = 350;
    this.nProgWidth = null;
    this.nProgHeight = null;
    this.nProgOffY = null;


    this.imgPriorityBox = null;

    //create the router image
    this.strImagePath   = null;

    //create the image object
    this.imgIcon = null;


    var fadetime = 200;

    //text xy position
    this.txtX = null;
    this.txtY = null;

    //the font shouldn't be fixed it should be in a changable place
    this.font    = null;
    this.txtName = null;

    this.txtType        = null;
    this.txtMAC         = null;
    this.txtPriority    = null;

    //the click handler callback
    this.pClickHandler = pClickHandler;

    //are we selected?
    this.bSelected = false;

    ////////////////////////////////////////////////////////////////////

    //call the constructor for this class and set it up
    self.init();
}
