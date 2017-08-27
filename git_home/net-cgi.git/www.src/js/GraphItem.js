/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */

function CGraphRoutines(pSetUpDn,
                        pAddLine,
                        pMoveLine,
                        pKillLine,
                        pRaphael,
                        pGetLine)
{
    this.setUpDon   = pSetUpDn;
    this.addLine    = pAddLine;
    this.moveLine   = pMoveLine;
    this.killLine   = pKillLine;
    this.getLine    = pGetLine;
    this.r          = pRaphael;
};

//=======================================================================
//
function CGraphItem(strName,
                    strDescription,
                    strIconNormal,
                    bWireless,
                    nWidth,
                    nHeight,
                    nXPos,
                    nYPos,
                    nOpacity,
                    pGraphRoutines,
                    pSelectFunction,
                    pOnHover,
                    pNode,
                    pDeleteFunction,
                    bDrawLines)
{
    //this replacement for events
    var self = this;

    var m_nTestScale        = 1.0;

    //do or dont draw lines
    var m_bDrawLines = true;

    if(bDrawLines != undefined && bDrawLines != null)
    {
        m_bDrawLines = bDrawLines;
    }

    var m_nFixedHeight = nHeight;

    //local
    var m_strName           = strName;
    var m_strDescription    = strDescription;
    var m_strIconNormal     = strIconNormal;
    var m_bWireless         = bWireless;
    var m_nWidth            = nWidth;
    var m_nHeight           = nHeight;
    var m_nXPos             = nXPos;
    var m_nYPos             = nYPos;
    var m_nXScale           = m_nTestScale;
    var m_nYScale           = m_nTestScale;

    this.nXPos              = nXPos;
    this.nYPos              = nYPos;

    var m_g                 = pGraphRoutines;

    var m_bSelectable       = false;
    var m_bDeleteable       = false;
    var m_pSelectFunction   = null;
    var m_pDeleteFunction   = null;

    this.bOnline            = true;

    this.pOnHover           = pOnHover;

    this.m_pNode            = pNode;

    this.m_nUpPerSec        = 0;
    this.m_nDnPerSec        = 0;

    if(pSelectFunction != undefined)
    {
        m_bSelectable = true;
        m_pSelectFunction = pSelectFunction
    }

    if(pDeleteFunction != undefined && pSelectFunction != null)
    {
        m_bDeleteable = true;
        m_pDeleteFunction = pDeleteFunction
    }
    else
    {
        m_bDeleteable = false;
    }

    //pointers to the actual image surface on the screen
    var m_imgNormal         = null;
    var m_imgOffline        = null;
    var m_imgOnline         = null;
    var m_imgHover          = null;
    var m_txtLabel          = null;
    var m_imgType           = null;

    var m_nDivWidth = 0;
    var m_nDivHeight = 0;

    var m_nLineID = -1;

    var m_lArrow = null;

    //create the router image
    var strImagePath            = m_strIconNormal;
    var strImageOfflinePath     = strImagePath.replace(".png","_gray.png");

    this.m_sIcon = m_strIconNormal;

    var Sync  = 0;

    //make the label of g_nNodeStringSize chars long
    var m_strTempLabel = makeLabel(m_strName,g_nNodeStringSize);

    if(m_strName == "Router" || m_strName == "Internet")
    {
        m_strTempLabel = "";
    }

    this.getOpacity = function()
    {
        var value = 0;
        if(m_imgNormal != null)
        {
            value =  m_imgNormal.attr("opacity");;
        }
        return value;
    }
    this.Init = function()
    {
    };

    this.bKilled = false;

    this.Kill= function()
    {
        self.bKilled = true;

        m_g.killLine(m_nLineID);

        if(m_imgType != null)
        {
            m_imgType.remove();
        }

        m_imgOffline.remove();
        m_imgOnline.remove();
        m_imgNormal.remove();
        m_imgHover.remove();
        m_txtLabel.remove();

        m_lArrow.Kill();
    };

    //fade this guy out
    this.fadeOut= function(time,callback)
    {
        if(self.bKilled == false)
        {
            self.draw(  m_nXPos,
                        m_nYPos,
                        eShow,
                        eHide,
                        time,
                        m_nXScale,
                        callback);
        }
    };

    //immediately move the image to xy
    this.move= function(x,y)

    {
        if(self.bKilled == false)
        {
            if(m_imgNormal!=null)
                m_imgNormal.attr({"x":x,"y":y});


            m_nXPos = x;
            m_nYPos = y;
        }
    };
    this.setUpDown = function(  Up,
                                Dn,
                                nPercentUp,
                                nPercentDn)
    {
        if(m_lArrow!=null && self.bKilled == false)
        {
            m_lArrow.setUpDown( Up,
                                Dn,
                                nPercentUp,
                                nPercentDn);
        }
    }

    this.makeUID = function(strName)
    {
        var strSpace  = "";
        var strPeriod = "";
        var strColon = "";
        var strComma = "";
        var strApostrophe = "";
        var strResult = "";

        if(strName != undefined)
        {
            strSpace = strName.replace(/\ /g,'_');
            strPeriod = strSpace.replace(/\./g,'_');
            strColon = strPeriod.replace(/\:/g,'_');
            strComma = strColon.replace(/\,/g,'_');
            strApostrophe = strComma.replace(/\'/g,'_');

            strResult = "node_id_"+strApostrophe;
        }

        return strResult;
    }

    this.changeIcon = function(strIcon)
    {


        if(self.bKilled == false)
        {
            m_strIconNormal         = strIcon;
            strImagePath            = iconPath(m_strIconNormal);
            strImageOfflinePath     = strImagePath.replace(".png","_gray.png");

            if(m_imgNormal != undefined && m_imgNormal!=null)
            {
                m_imgOffline.remove();
                m_imgOnline.remove();

                m_imgOffline=null;
                m_imgOnline=null;
                m_imgNormal=null;
            }

            this.draw(  m_nXPos,
                        m_nYPos,
                        eShow,
                        eShow,
                        0,
                        m_nXScale);
        }
    }

    //handy icon update function for the fxNetwork update item
    this.updateIcon = function(strIcon)
    {
        //if the path is different
        if(m_strIconNormal != strIcon)
        {
            self.changeIcon(strIcon);
        }
    }

    this.nHoverOffset = 5;

    //animate this guy to the new xy, new opacity
    this.draw = function(   nX,
                            nY,
                            startOpacity,
                            endOpacity,
                            animTime,
                            nScale,
                            callback)
    {
        //have to start off with the set scale
        this.setScale(nScale,nScale);

        //default opacity start/end and animation time
        var start = eShow;
        var end = eShow;
        var time = 0;
        var bFirstTime = false;
        //1st record the previous pos for animation purposes
        var nPrevX = m_nXPos;
        var nPrevY = m_nYPos;

        if(nScale == undefined)
        {
            nScale = m_nXScale;
        }

        //if we dont have an x and y position make one
        if(nX!=undefined && nY!=undefined)
        {
            this.nXPos = m_nXPos = nX;
            this.nYPos = m_nYPos = nY;
            end = eShow;
            start = eShow;
        }

        //ditto for the hover...if its not here make it.
        if(m_imgHover == null && self.bKilled == false)
        {
            m_imgHover = m_g.r.rect(m_nXPos-self.nHoverOffset, m_nYPos-self.nHoverOffset, m_nWidth+10, m_nHeight+10, 10).attr({fill: "#b8b8b8",stroke: "#888888",opacity: eHide});
            
            fixAlpha(m_imgHover,eHide);
        }

        //if we don't have a normal image make it
        if(m_imgNormal == null && self.bKilled == false)
        {
            //create the image object
            m_imgOffline = m_g.r.image( strImageOfflinePath,
                                        m_nXPos,
                                        m_nYPos,
                                        m_nWidth,
                                        m_nHeight).attr({opacity: eHide});;
            fixAlpha(m_imgOffline,eHide);

            //create the image object
            m_imgOnline = m_g.r.image(strImagePath,
                                        m_nXPos,
                                        m_nYPos,
                                        m_nWidth,
                                        m_nHeight);

            m_imgNormal = m_imgOnline;

            //add an id to this element
            if(pNode!=undefined && pNode != null)
            {
                $(m_imgNormal.node).attr('id',this.makeUID(pNode.mac));
            }

            bFirstTime = true;
        }


        if(this.bOnline == true)
        {
            m_imgNormal = m_imgOnline;

            m_imgOnline.attr({opacity: eShow});
            m_imgOffline.attr({opacity: eHide});
            fixAlpha(m_imgOnline,eShow);
            fixAlpha(m_imgOffline,eHide);
        }
        else
        {
            m_imgNormal = m_imgOffline;

            m_imgOnline.attr({opacity: eHide});
            m_imgOffline.attr({opacity: eShow});
            fixAlpha(m_imgOnline,eHide);
            fixAlpha(m_imgOffline,eShow);
        }

        //bring the image to the top layer os it draws over the wires
        //m_imgNormal.toFront();

        var typeX = (m_nIconSize/2)-((m_nIconSize*m_nXScale)/2);
        var typeY = ((m_nFixedHeight/2)+((m_nFixedHeight*m_nXScale)/2)) *1.04;//* 1.5;//138;
        var typeW = (m_nIconSize*m_nXScale) *.1875;//24;
        var typeH = (m_nIconSize*m_nXScale) *.1875;//24;

        //if we don't have a network type icon make it
        if(m_imgType == null && m_bWireless != -1  && self.bKilled == false)
        {
            var strType;

            //we currently only support wired and wireless
            //todo: these icons shouldn't be fixed in name
            //move them to some place central
            if(m_bWireless == true)
            {
                strType =  g_path.images + "wifi.png";
            }
            else
            {
                strType = g_path.images + "wired.png"
            }

            m_imgType = m_g.r.image(    strType,
                                    m_nXPos + typeX,
                                    m_nYPos + typeY,
                                    typeW,
                                    typeH);
        }

        var txtX = 32;
        var txtY = ((m_nFixedHeight/2)+((m_nFixedHeight*m_nXScale)/2))*1.15625;//148;

        //if we don't have a label make it
        if(m_txtLabel == null && self.bKilled == false)
        {
            //the font shouldn't be fixed it should be in a changable place
            var font        = {font: m_nFontSize+'px Century Gothic, Arial', opacity: eShow};
            m_txtLabel = m_g.r.text(m_nXPos+txtX,m_nYPos+txtY,m_strTempLabel).attr(font).attr({fill: g_rgbWorkText}).attr({"text-anchor": "start",width: 256});
            fixAlpha(m_txtLabel,eShow);
        }

        if(m_nLineID == -1 && nX!=undefined && nY != undefined && m_bDrawLines == true && self.bKilled == false)
        {
            m_nLineID = m_g.addLine(nX,nY);
        }

        //if we don't have bandwidt labels
        if(m_lArrow == null && self.bKilled == false)
        {
            var orientation = 0;

            //are we on the left side?
            if( m_nXPos < ((g_nWidth/2) - m_nIconSize))
            {
                orientation = 1;
            }

            //are we on the right side?
            if( m_nXPos > g_nWidth/2)
            {
                orientation = 2;
            }

            if(isIE8())
            {
                m_lArrow = new IEArrows( m_nXPos,
                                        m_nYPos,
                                        orientation,
                                        m_g.r,
                                        m_nXScale);
            }
            else
            {
                m_lArrow = new CArrows( m_nXPos,
                                        m_nYPos,
                                        orientation,
                                        m_g.r,
                                        m_nXScale);
            }
        }

        //are we supposed to animate?
        if(startOpacity != undefined && endOpacity != undefined && animTime != undefined && self.bKilled == false)
        {
            //set up the animation time if we can
            if(startOpacity != undefined)
            {
                start = startOpacity;
                end = endOpacity;
                time = animTime;
            }

            //are we supposed to animate a movement??
            if(nPrevX == -1 && nPrevY==-1)
            {
                nPrevX = nX;
                nPrevY = nY;
                //start = 0.0;
            }

            //m_imgHover.animate({"x":m_nXPos,"y":m_nYPos},time,"linear");
            m_imgHover.attr({"x":m_nXPos-self.nHoverOffset,"y":m_nYPos-self.nHoverOffset});

            //animate the image
            if(m_imgNormal != undefined && m_imgNormal != null && self.bKilled == false)
            {
                //try
                {
                    /*m_imgNormal.attr({opacity: start}).animate({"x":m_nXPos,"y":m_nYPos,"opacity": end},time,"linear",function(callback)
                    {
                        if(callback != undefined)
                        {
                            callback();
                        }
                    }(callback));*/
                    m_imgNormal.attr({opacity: start,"x":m_nXPos,"y":m_nYPos,"opacity": end});
                    fixAlpha(m_imgNormal,end);
                    if(callback != undefined)
                    {
                        callback();
                    }
                }
                /*catch(err)
                {
                    console.log("error in graphitem m_imgNormal");
                }*/
            }

            //are we supposed to move the lines?
            if(m_nLineID != -1 && m_bDrawLines == true && self.bKilled == false)
            {
                //m_g.moveLine(m_nLineID,nPrevX,nPrevY);

                m_g.moveLine(m_nLineID,nX,nY,end,time);
            }

            //are we supposed to move the bandwidth arrows?
            /*if(m_lArrow != null)
            {
                var orientation = 0;

                //are we on the left side?
                if( m_nXPos < ((g_nWidth/2) - m_nIconSize))
                {
                    orientation = 1;
                }

                //are we on the right side?
                if( m_nXPos > g_nWidth/2)
                {
                    orientation = 2;
                }

                m_lArrow.movePos(nX,nY,end,time,orientiation);
            }*/

            if(m_txtLabel != undefined && m_imgNormal != undefined &&m_txtLabel != null && m_imgNormal != null && self.bKilled == false)
            {
                //move the label with this
                m_txtLabel.attr({"x":m_nXPos+txtX,"y":m_nYPos+txtY,"opacity": eShow});
                fixAlpha(m_txtLabel,eShow);
                //m_txtLabel.attr({opacity: start,"x":m_nXPos+txtX,"y":m_nYPos+txtY,"opacity": end},time,"linear");
            }
            //are we supposed to move the network type icon
            if(m_imgType != undefined && m_imgNormal != undefined &&m_imgType != null && m_imgNormal != null && self.bKilled == false)
            {
                m_imgType.attr({"x":m_nXPos+typeX,"y":m_nYPos+typeY,"opacity": eShow});
                fixAlpha(m_imgType,eShow);
                //m_imgType.attr({opacity: start,"x":m_nXPos+typeX,"y":m_nYPos+typeY,"opacity": end},time,"linear");
            }

            if(m_lArrow!=null && self.bKilled == false)
            {
                var orientation = 0;

                //are we on the left side?
                if( m_nXPos < ((g_nWidth/2) - m_nIconSize))
                {
                    orientation = 1;
                }

                //are we on the right side?
                if( m_nXPos > g_nWidth/2)
                {
                    orientation = 2;
                }

                m_lArrow.movePos(m_nXPos,
                                 m_nYPos,
                                 end,
                                 time,
                                 orientation);
            }

        }
        else // if we aren't supposed to animate then just do a quick move
        {
            if( self.bKilled == false)
            {
                //move the icon and label
                m_imgNormal.attr({"x":m_nXPos,"y":m_nYPos,"opacity": end});
                fixAlpha(m_imgNormal,end);
                m_txtLabel.attr({"x":m_nXPos+txtX,"y":m_nYPos+txtY,"opacity": end});
                fixAlpha(m_txtLabel,end);

                //if we have a type move it too
                if(m_imgType != null)
                {
                    m_imgType.attr({"x":m_nXPos+typeX,"y":m_nYPos+typeY,"opacity": end});
                    fixAlpha(m_imgType,end);
                }

                //if we have a line then move it.
                if(m_nLineID != -1 && m_bDrawLines == true && self.bKilled == false)
                {
                    m_g.moveLine(m_nLineID,nX,nY);
                }

                if(m_lArrow!=null)
                {
                    m_lArrow.setPos(    m_nXPos,
                                    m_nYPos);
            }
            }
        }


            var fadetime = 200;

            if(m_bSelectable == true && bFirstTime == true)
            {

                //hover on the raphael node
                m_imgNormal.hover(
                function(){

                    //show the mouse hand
                    $(m_imgNormal.node).css('cursor', 'pointer');

                    m_imgHover.attr({opacity: eHide}).animate({"opacity": eShow},fadetime,"linear");
                    fixAlpha(m_imgHover,eShow);

                    //call the on hover
                    if(self.pOnHover != undefined && self.pOnHover != null)
                    {
                        self.pOnHover(true,self);
                    }

                },
                function ()
                {
                    m_imgHover.attr({opacity: eShow}).animate({"opacity": eHide},fadetime,"linear");
                    fixAlpha(m_imgHover,eHide);

                    //call the on hover
                    if(self.pOnHover != undefined && self.pOnHover != null)
                    {
                        self.pOnHover(false,self);
                    }
                });

                $(m_imgNormal.node).click(
                function(){
                    if(m_pSelectFunction!=undefined)
                    {
                        m_pSelectFunction(m_strIconNormal,m_strName,m_nXPos,m_nYPos,self);
                    }
                });

              /*  if(m_bDeleteable == true && m_pDeleteFunction!=undefined && m_pDeleteFunction!= null)
                {
                    // Show menu when #myDiv is clicked
                    $(m_imgNormal.node).contextMenu({
                        menu: 'myMenu'
                    },
                        function(action, el, pos) {
                        //call the delete handler if it exists
                        if(m_bDeleteable == true && m_pDeleteFunction!=undefined && m_pDeleteFunction!= null)
                        {
                            m_pDeleteFunction(self);
                        }
                    },
                    function()
                    {
                        self.pOnHover(false,self);
                    });
                }*/

            }


        this.setScale(nScale,nScale);
    };


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: setScale

        Set the scale of the image to the percent of the original size

        Parameters:
          nXScale   - float of x scale 1.0 = 100%
          nYScale   - float of y scale 1.0 = 100%

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.setScale = function(   nXScale,
                                nYScale)
    {
        if(self.bKilled == false)
        {
            m_nXScale = nXScale;
            m_nYScale = nYScale;

            if(m_nYScale < .5)
            {
                m_nYScale = .5;
            }

            if(m_nXScale < .5)
            {
                m_nXScale = .5;
            }

            if(m_imgNormal != null && m_imgNormal != undefined)
            {
                m_imgNormal.transform("S"+m_nXScale+","+m_nYScale);
            }

            if(m_imgHover != null && m_imgHover != undefined)
            {
                m_imgHover.transform("S"+m_nXScale+","+m_nYScale);
            }

            if(m_txtLabel != null && m_txtLabel != undefined)
            {
                m_txtLabel.transform("S"+m_nXScale+","+m_nYScale);
            }

            if(m_imgType != null && m_imgType != undefined)
            {
                m_imgType.transform("S"+m_nXScale+","+m_nYScale);
            }

            if(m_lArrow != null && m_lArrow != undefined)
            {
                //if(m_nXScale < 1)
                {
                    var strUP = m_lArrow.nUpBytesPerSecond;
                    var strDN = m_lArrow.nDnBytesPerSecond;
                    var nUpPercent = m_lArrow.nPercentUp;
                    var nDnPercent = m_lArrow.nPercentDn;

                    m_lArrow.Kill();
                    m_lArrow = null;
                    var orientation = 0;

                    //are we on the left side?
                    if( m_nXPos < ((g_nWidth/2) - m_nIconSize))
                    {
                        orientation = 1;
                    }

                    //are we on the right side?
                    if( m_nXPos > g_nWidth/2)
                    {
                        orientation = 2;
                    }

                    if(m_lArrow == null && self.bKilled == false)
                    {
                        if(isIE8())
                        {
                                    m_lArrow = new IEArrows( m_nXPos,
                                                            m_nYPos,
                                                            orientation,
                                                            m_g.r,
                                                            m_nXScale,
                                                            strUP,
                                                            strDN,
                                                            nUpPercent,
                                                            nDnPercent);
                        }
                        else
                        {
                                    m_lArrow = new CArrows( m_nXPos,
                                                            m_nYPos,
                                                            orientation,
                                                            m_g.r,
                                                            m_nXScale,
                                                            strUP,
                                                            strDN,
                                                            nUpPercent,
                                                            nDnPercent);
                        }
                    }
                }

                m_lArrow.setScale(m_nXScale,m_nYScale);
            }
        }
    }

    var nPopSpacer  = 15;
    var nPopWidth   = 180;
    var nPopHeight  = 64;

    this.popupX = function()
    {
        var x = m_imgNormal.getBBox().x;
        var rHalf = $("#Display").width()/2;

        if(self.m_pNode.os.type == "Router")
        {
            x  = rHalf - nPopWidth;
        }
        else if(x > rHalf)
        {
            x -= ((m_nIconSize)+nPopWidth+nPopSpacer);
        }
        else
        {
            x += (m_nIconSize*m_nXScale)+nPopSpacer;
        }

        return x;
    }

    this.popupY = function()
    {

        var scrollTop = $(window).scrollTop();

        var y =  m_imgNormal.getBBox().y - scrollTop;

        if(self.m_pNode.os.type == "Router")
        {
            y += (m_nIconSize*m_nXScale)+nPopSpacer;
        }

        return y;
    }

    this.setOnline = function(bOnline)
    {
        this.bOnline = bOnline;
    }
}
