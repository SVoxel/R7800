/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */

////////////////////////////////////////////////////////////////////////
/*
    Class: CArrows IE8 Version

    Version:   1.0

    Class for the bandwidth meters on the my network page. Currently
    shaped like an arrow and fills in as it approaches 100%.

    Has Left, Right, and Vertical orientations.

    Parameters:
          nX            - x position
          nY            - y position
          nOrientation  - 1 = left, 2 = right, 3 = up, 4 = down
          raphael       - handle to the raphael class system

*/
////////////////////////////////////////////////////////////////////////

function IEArrows(   nX,
                    nY,
                    nOrientation, // 1 = left, 2 = right, 3 = up, 4 = down
                    raphael,
                    nScale,
                    nUpBytesPerSecond,
                    nDnBytesPerSecond,
                    nPercentUp,
                    nPercentDn)
{
    /////////////////////////////////////
    //   raphael objects for the text
    /////////////////////////////////////

    /*
        Variable: m_txtUp
        raphael object for the up speed text label
    */
    var m_txtUp         = null;

    /*
        Variable: m_txtDn
        raphael object for the down speed text label
    */
    var m_txtDn         = null;

    /*
        Variable: m_shadowUp
        raphael object for the up speed text label's shadow
        this make in much more readable on various backgrounds
    */
    var m_shadowUp      = null;

    /*
        Variable: m_shadowDn
        raphael object for the down speed text label's shadow
        this make in much more readable on various backgrounds
    */
    var m_shadowDn      = null;

    /*
        Variable: m_shadowX
        x offset in pixels of the text shadow
    */
    var m_shadowX       = 0;

    /*
        Variable: m_shado
        y offset in pixels of the text shadow
    */
    var m_shadowY       = 0;

    ////////////////////////////////////
    //  raphael object for the arrows
    ////////////////////////////////////

    /*
        Variable: m_imgUpArrow
        raphael object for the up arrow image
    */
    var m_imgUpArrow        = null;

    /*
        Variable: m_imgDnArrow
        raphael object for the down arrow image
    */
    var m_imgDnArrow        = null;

    /*
        Variable: m_imgUpTrack
        raphael object for the up arrow track image
    */
    var m_imgUpTrack        = null;

    /*
        Variable: m_imgDnTrack
        raphael object for the down arrow track image
    */
    var m_imgDnTrack        = null;

    /*
        Variable: m_nUpPercent
        Up percent of the meter. This is used to draw the
        arrow fill
    */
    var m_nUpPercent = 0;

    /*
        Variable: m_nDnPercent
        Down percent of the meter. This is used to draw the
        arrow fill
    */
    var m_nDnPercent = 0;

    ///////////////////////////
    // position of the arrows
    ///////////////////////////

    /*
        Variable: m_nX
        X position of the meter
    */
    var m_nX = nX;

    /*
        Variable: m_nY
        y position of the meter
    */
    var m_nY = nY;

    /*
        Variable: m_nXScale
    */
    var m_nXScale = nScale || 1.0;

    /*
        Variable: m_nYScale
    */
    var m_nYScale = nScale || 1.0;

    /*
        Variable: m_nY
        width of the meter image
    */
    var m_nW = 100;//165;

    /*
        Variable: m_nY
        height of the meter image
    */
    var m_nH = 32;

    //if we are vertical swap width and height
    if(nOrientation == 3)
    {
        var nTemp = m_nW;
        m_nW=m_nH;
        m_nH=nTemp;
    }

    /*
        Variable: m_nOrientation
        Orientation of the meter.
        1 = left, 2 = right, 3 = up, 4 = down
    */
    var m_nOrientation = nOrientation;

    /*
        Variable: m_r
        Pointer to the raphael class which is used to do all
        the graphics drawing in this class
    */
    var m_r = raphael;

    /*
        Variable: nUpX
        x position of the up arrow
    */
    var nUpX = 0;

    /*
        Variable: nUpY
        y position of the up arrow
    */
    var nUpY = 0;

    /*
        Variable: nDnX
        x position of the down arrow
    */
    var nDnX = 0;

    /*
        Variable: nDnY
        y position of the down arrow
    */
    var nDnY = 0;

    var nTestVal = 50;

    this.getWindowWidth = function()
    {
        return $("#Display").width();
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: setUpDown

        Set the upload and download speed meters. Percent of the meter and
        text labels are separate.

        Parameters:
          nUpBytesPerSecond - bytes per second for the up label
          nDnBytesPerSecond - bytes per second for the down label
          nPercentUp        - percentage of the fill for the up arrow
          nPercentDn        - percentage of the fill for the down arrow
          nTime             - time for the animate to change from previous to current state

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.setUpDown = function(  nUpBytesPerSecond,
                                nDnBytesPerSecond,
                                nPercentUp,
                                nPercentDn,
                                nTime)
    {
        var nUp = parseInt(bytesToSize(nUpBytesPerSecond,2));
        var nDn = parseInt(bytesToSize(nDnBytesPerSecond,2));

        if(nUp<0)
        {
            nUpBytesPerSecond = 0;
            nPercentUp = 0;
        }

        if(nDn<0)
        {
            nDnBytesPerSecond = 0;
            nPercentDn = 0;
        }

        if(nTime == undefined)
            nTime = 0;

        m_nUpPercent = nPercentUp;
        m_nDnPercent = nPercentDn;

        if(m_txtUp != null && this.bKilled == false)
        {
            if(this.nUpBytesPerSecond != nUpBytesPerSecond)
            {
                this.nUpBytesPerSecond = nUpBytesPerSecond;
                var strUp = bytesToSize(nUpBytesPerSecond,2) + "ps";
                if(m_imgUpArrow != null)
                {
                    $(m_imgUpArrow.node).removeAttr('clip-path');
                }

                if(m_nOrientation == 3)
                {
                    m_shadowUp.attr('text', strUp);
                    m_shadowUp.transform("S"+m_nXScale+","+m_nYScale+"r90");
                    m_txtUp.attr('text', strUp);
                    m_txtUp.transform("S"+m_nXScale+","+m_nYScale+"r90");
                }
                else
                {
                    m_shadowUp.attr('text', strUp);
                    m_txtUp.attr('text', strUp);
                }
            }

            //if(this.nPercentUp != nPercentUp)
            {
                this.nPercentUp = nPercentUp;

                if(nPercentUp > 100)
                    nPercentUp = 100;

                if(m_nOrientation == 1 && this.bKilled == false)
                {
                    var nP = (nPercentUp/100);

                    var width = m_nW*nP;

                    nDnX = this.getDnX(nX);
                    nDnY = this.getDnY(nY);

                    nUpX = this.getUpX(nX);
                    nUpY = this.getUpY(nY);

                    txtUpX = (m_nW/1.8)*m_nXScale;
                    txtDnX = (m_nW/3)*m_nXScale
                    txtY = (m_nH/2)*m_nYScale;

                    m_imgUpArrow.attr({"x":nUpX,"y":nUpY,"width":width*m_nXScale,"height":m_nH*m_nYScale});

                    //m_imgUpArrow.attr({"clip-rect": [nUpX,nUpY,width,m_nH]});//,nTime,"<>");

                }
                if(m_nOrientation == 2 && this.bKilled == false)
                {
                    var nP = (nPercentDn/100);

                    var nSz = m_nW*nP;

                    var X = nUpX + (m_nW-nSz);
                    var W = nSz;
                    var myx = (nUpX+(m_nW*m_nXScale))-(W*m_nXScale);
                    m_imgUpArrow.attr({"x":myx,"y":nUpY,"width":W*m_nXScale,"height":m_nH*m_nYScale});
                }
                if(m_nOrientation == 3 && this.bKilled == false)
                {
                    var nP = (nPercentUp/100);
                    var height = m_nH*nP;
                    m_imgUpArrow.attr({"x":nUpX,"y":nUpY,"width":m_nW*m_nXScale,"height":height*m_nYScale});
                }
            }
        }

        if(m_txtDn != null && this.bKilled == false)
        {
            var msg = "";

            if(m_imgDnArrow != null && this.bKilled == false)
            {
                $(m_imgDnArrow.node).removeAttr('clip-path');
            }

            if(this.nDnBytesPerSecond != nDnBytesPerSecond)
            {
                this.nDnBytesPerSecond = nDnBytesPerSecond;
                var strDn = bytesToSize(nDnBytesPerSecond,2) + "ps";

                if(m_nOrientation == 3 && this.bKilled == false)
                {
                    m_shadowDn.attr('text', strDn);
                    m_shadowDn.transform("S"+m_nXScale+","+m_nYScale+"r90");
                    m_txtDn.attr('text', strDn);
                    m_txtDn.transform("S"+m_nXScale+","+m_nYScale+"r90");
                }
                else
                {
                    m_shadowDn.attr('text', strDn);
                    m_txtDn.attr('text', strDn);
                }
            }


            {
                this.nPercentDn = nPercentDn;

                if(nPercentDn > 100)
                    nPercentDn = 100;

                if(nPercentDn < 0)
                    nPercentDn = 0;



                if(m_nOrientation == 1 && this.bKilled == false)
                {
                    var nP = (nPercentDn/100);

                    var nSz = m_nW*nP;

                    var X = nDnX + (m_nW-nSz);
                    var W = nSz;
                    var myx = (nDnX+(m_nW*m_nXScale))-(W*m_nXScale);
                    m_imgDnArrow.attr({"x":myx,"y":nDnY,"width":W*m_nXScale,"height":m_nH*m_nYScale});

                    msg = "x: "+X;
                }
                if(m_nOrientation == 2 && this.bKilled == false)
                {

                    var nP = (nPercentDn/100);
                    var width = m_nW*nP;

                    nDnX = this.rightDnX(nX);
                    nDnY = this.rightDnY(nY);

                    nUpX = this.rightUpX(nX);
                    nUpY = this.rightUpY(nY);

                    txtUpX = (m_nW/1.8)*m_nXScale;
                    txtDnX = (m_nW/2.2)*m_nXScale;
                    txtY = (m_nH/2)*m_nYScale;

                    m_imgDnArrow.attr({"x":nDnX,"y":nDnY,"width":width*m_nXScale,"height":m_nH*m_nYScale});


                    console.log("width:"+width*m_nXScale);

                    console.log("height:"+m_nH*m_nYScale);
                }
                if(m_nOrientation == 3 && this.bKilled == false)
                {
                    var nP = (nPercentDn/100);

                    var nSz = m_nH*nP;

                    var Y = nDnY + m_nH;
                    var H = nSz;
                    var myy = nDnY+(m_nH*m_nYScale)-(H*m_nYScale);

                    m_imgDnArrow.attr({"x":nDnX,"y":myy,"width":m_nW*m_nXScale,"height":H*m_nYScale});

                    msg = "y: "+Y;
                }

                if(this.bKilled == false)
                {
                    m_imgDnArrow.toFront();
                    m_shadowDn.toFront();
                    m_txtDn.toFront();
                }
            }
        }
    };

    this.bKilled = false;
    ////////////////////////////////////////////////////////////////////////
    /*
        Function: Kill

        Kill this object

        Parameters:
          none.

        Returns:
          nothing.

    */
    ////////////////////////////////////////////////////////////////////////
    this.Kill = function()
    {
        this.bKilled = true;

        if(m_txtUp != null)
            m_txtUp.remove();
        if(m_txtDn != null)
            m_txtDn.remove();
        if(m_shadowUp != null)
            m_shadowUp.remove();
        if(m_shadowDn != null)
            m_shadowDn.remove();
        if(m_imgUpArrow != null)
            m_imgUpArrow.remove();
        if(m_imgDnArrow != null)
            m_imgDnArrow.remove();
        if(m_imgUpTrack != null)
            m_imgUpTrack.remove();
        if(m_imgDnTrack != null)
            m_imgDnTrack.remove();
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: resetOrientation

        Change the orientation of the arrows to the new orientation given.

        Parameters:
          orientation - new orientation to change to
          1 = left, 2 = right, 3 = up, 4 = down

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.resetOrientation = function(orientation)
    {
        if(orientation != m_nOrientation  && this.bKilled == false)
        {
            m_nOrientation = orientation;

            m_txtUp.remove();
            m_txtDn.remove();
            m_shadowUp.remove();
            m_shadowDn.remove();

            //raphael object for the arrows
            m_imgUpArrow.remove();
            m_imgDnArrow.remove();
            m_imgUpTrack.remove();
            m_imgDnTrack.remove();

            m_txtUp         = null;
            m_txtDn         = null;
            m_shadowUp      = null;
            m_shadowDn      = null;
            m_shadowX       = 1;
            m_shadowY       = 1;

            //raphael object for the arrows
            m_imgUpArrow        = null;
            m_imgDnArrow        = null;
            m_imgUpTrack        = null;
            m_imgDnTrack        = null;

            m_nUpPercent = 0;
            m_nDnPercent = 0;

            //position of the arrows
            m_nW = 100;//152;//141;
            m_nH = 32;//58;//53;

            nUpX = 0 ;
            nUpY = 0;
            nDnX = 0;
            nDnY = 0;

            this.setPos(m_nX,m_nY);

            this.setUpDown( this.nUpBytesPerSecond,
                            this.nDnBytesPerSecond,
                            this.nPercentUp,
                            this.nPercentDn,
                            0);
        }
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: setPos

        Set the  position of the arrow meter.

        Parameters:
          m_nX - x position of the meter
          m_nY - y position of the meter

        Returns:
          nothing.

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.setPos = function( nX,
                            nY)
    {
        m_nX = nX;
        m_nY = nY;

        if(m_nOrientation != 0 && this.bKilled == false)
        {
            var strDown = "";
            var strUp = "";
            var strDnTrack = "";
            var strUpTrack = "";
            var txtUpX =0;
            var txtDnX = 0;
            var txtY = 0;

            if(m_nOrientation == 1 && this.bKilled == false)
            {
                nDnX = this.getDnX(nX);
                nDnY = this.getDnY(nY);

                nUpX = this.getUpX(nX);
                nUpY = this.getUpY(nY);

                txtUpX = (m_nW/1.8)*m_nXScale;
                txtDnX = (m_nW/2.2)*m_nXScale;
                txtY = (m_nH/2)*m_nYScale;
            }

            if(m_nOrientation == 2 && this.bKilled == false)
            {
                nDnX = this.rightDnX(nX);
                nDnY = this.rightDnY(nY);

                nUpX = this.rightUpX(nX);
                nUpY = this.rightUpY(nY);

                txtUpX = (m_nW/1.8)*m_nXScale;
                txtDnX = (m_nW/2.2)*m_nXScale;
                txtY = (m_nH/2)*m_nYScale;
            }

            if(m_nOrientation == 3 && this.bKilled == false)
            {
                nDnX = this.vertDnX(nX);
                nDnY = this.vertDnY(nY);

                nUpX = this.vertUpX(nX);
                nUpY = this.vertUpY(nY);

                txtUpX = m_nW/4;
                txtDnX = m_nW/4;
                txtY = (m_nH/2)*m_nYScale;
            }

            var blue = "#0187C5";
            var gray = "#444444";

            var upRGB = gray;
            var dnRGB = blue;

            if(m_nOrientation == 3)
            {
                upRGB = blue;
                dnRGB = gray;
            }

            //do we need to create the arrows
            if(m_imgDnArrow == null && this.bKilled == false)
            {
                m_imgDnTrack = m_r.rect(    nDnX,
                                            nDnY,
                                            m_nW,
                                            m_nH).attr({fill: "#AAAAAA",stroke: "#888888"});;
                m_imgDnArrow = m_r.rect(    nDnX,
                                            nDnY,
                                            m_nW,
                                            m_nH).attr({fill: dnRGB,stroke: "#888888"});;
            }
            else
            {
                //m_imgDnArrow.attr({"x":nDnX,"y":nDnY});
                m_imgDnTrack.attr({"x":nDnX,"y":nDnY});
            }

            //do we need to create the arrows
            if(m_imgUpArrow == null && this.bKilled == false)
            {
                m_imgUpTrack = m_r.rect(    nUpX,
                                            nUpY,
                                            m_nW,
                                            m_nH).attr({fill: "#AAAAAA",stroke: "#888888"});;

                m_imgUpArrow = m_r.rect(    nUpX,
                                            nUpY,
                                            m_nW,
                                            m_nH).attr({fill: upRGB,stroke: "#888888"});;
            }
            else
            {
                m_imgUpTrack.attr({"x":nUpX,"y":nUpY});
            }

            var font        = {font: m_nFontSize+'px Century Gothic, Arial', opacity: 1.0};

            if(m_txtUp == null && this.bKilled == false)
            {
                m_shadowUp = m_r.text(nUpX+txtUpX+m_shadowX,nUpY+txtY+m_shadowY,"").attr(font).attr({fill: "rgb(0,0,0)",width: m_nW,stroke:"rgb(0,0,0)","stroke-width":3,"stroke-opacity":.6});
                m_txtUp = m_r.text(nUpX+txtUpX,nUpY+txtY,"").attr(font).attr({fill: "rgb(255,255,255)",width: m_nW});
            }
            else
            {
                m_shadowUp.attr({"x":nUpX+txtUpX+m_shadowX,"y":nUpY+txtY+m_shadowY});;
                m_txtUp.attr({"x":nUpX+txtUpX,"y":nUpY+txtY});;
            }

            if(m_txtDn == null && this.bKilled == false)
            {
                m_shadowDn = m_r.text(nDnX+txtDnX+m_shadowX,nDnY+txtY + m_shadowY,"").attr(font).attr({fill: "rgb(0,0,0)",width: m_nW,stroke:"rgb(0,0,0)","stroke-width":3,"stroke-opacity":.6});
                m_txtDn = m_r.text(nDnX+txtDnX,nDnY+txtY,"").attr(font).attr({fill: "rgb(255,255,255)",width: m_nW});
            }
            else
            {
                m_shadowDn.attr({"x":nDnX+txtDnX+m_shadowX,"y":nDnY+txtY+m_shadowY});;
                m_txtDn.attr({"x":nDnX+txtDnX,"y":nDnY+txtY});;
            }

            if(m_nOrientation == 3 && this.bKilled == false)
            {
                m_shadowUp.transform("S"+m_nXScale+","+m_nYScale+"r90");
                m_txtUp.transform("S"+m_nXScale+","+m_nYScale+"r90");
                m_shadowDn.transform("S"+m_nXScale+","+m_nYScale+"r90");
                m_txtDn.transform("S"+m_nXScale+","+m_nYScale+"r90");
            }
        }
    };

    //return the x position half way through the line
    this.getHalfLine = function()
    {
        //get the width of the drawing surface
        var nWidth = this.getWindowWidth();

        //if we are below the minimum width
        if(nWidth<g_nMinWidth)
        {
            nWidth = g_nMinWidth;
        }

        //calculate the middle of the screen
        var nHalfWidth = nWidth / 2;

        //get half the halfwidth
        var nHalfLine = (nHalfWidth / 2) -32;

        //if we are on the left side
        if(m_nX < nHalfWidth)
        {
            nHalfLine = m_nX + nHalfLine;
        }
        else // if we are on the right side
        {
            nHalfLine = m_nX - nHalfLine;
        }

        return nHalfLine;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getUpX

        Calculate the global x positon of the value given. Convert child
        to parent global x position.

        Parameters:
          val - x position to convert

        Returns:
          x value to use for positioning arrow

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.getUpX = function(val)
    {
        //up get positioned at half the line
        var nReturn = this.getHalfLine();

        return nReturn;
    };

    //ditto for the down arrow
    this.getDnX = function(val)
    {
        //down gets position at half the line minus most of the width of the icon
        var nReturn = this.getHalfLine() - ((m_nIconSize*m_nXScale)*.75) -4;

        return nReturn;
    };

    var nScalar = 64;
    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getUpY

        Calculate the global y positon of the value given. Convert child
        to parent global y position.

        Parameters:
          val - y position to convert

        Returns:
          y value to use for positioning arrow

        See the base class:

         <Module>
    */
    ////////////////////////////////////////////////////////////////////////
    this.getUpY = function(val)
    {
        var nOffset = (m_nH*m_nYScale);//(m_nH/2)-((m_nH*m_nYScale)/2);

        var nScaleOff = 0;
        if(m_nXScale<1)
        {
            nScaleOff = nScalar-(nScalar*m_nXScale);
        }

        var nReturn = val +  ((m_nIconSize*m_nXScale)/2) - nOffset +nScaleOff;// - ((m_nH*m_nYScale)*.27);

        return nReturn;
    };

    //ditto for the up arrow
    this.getDnY = function(val)
    {
        var nOffset = 0;

        if(m_nXScale<1)
        {
            nOffset = nScalar-(nScalar*m_nXScale);
        }

        var nReturn = val +  ((m_nIconSize*m_nXScale)/2) + nOffset;

        return nReturn;
    };

    ///////////////////////////////////////////////////////
    var rightOffset = 352;

    this.rightUpX = function(val)
    {
        var nReturn = this.getHalfLine()+((m_nIconSize*m_nXScale)*.25)+nScalar;
        return nReturn;

    };

    this.rightUpY = function(val)
    {
        var nOffset = (m_nH*m_nYScale);

        var nScaleOff = 0;
        if(m_nXScale<1)
        {
            nScaleOff = nScalar-(nScalar*m_nXScale);
        }

        var nReturn = val +  ((m_nIconSize*m_nXScale)/2) - nOffset + nScaleOff;

        return nReturn;
    };

    this.rightDnX = function(val)
    {

        var nReturn = this.getHalfLine()+((m_nIconSize*m_nXScale))+nScalar;
        return nReturn;

    };

    this.rightDnY = function(val)
    {
        var nOffset = 0;

        var nScaleOff = 0;
        if(m_nXScale<1)
        {
            nScaleOff = nScalar-(nScalar*m_nXScale);
        }

        var nReturn = val +  ((m_nIconSize*m_nXScale)/2) - nOffset + nScaleOff;

        return nReturn;
    };


    this.vertUpX = function(val)
    {
        var windowW = $("#Display").width();

        if(windowW<g_nMinWidth)
        {
            windowW = g_nMinWidth;
        }

        var winHalf = (windowW/2) - nScalar;

        var nReturn = winHalf;

        if(m_nXScale < 1.0)
        {
            var nScaleOff = (nScalar/2)-((nScalar/2)*m_nXScale);
            nReturn += nScaleOff;
        }

        return nReturn;
    };

    this.vertUpY = function(val)
    {
        var nHeight = $(window).height();

        var nReturn = val + (m_nH*m_nXScale)+16;

        return nReturn;
    };

    this.vertDnX = function(val)
    {
        var windowW = $("#Display").width();

        if(windowW<g_nMinWidth)
        {
            windowW = g_nMinWidth;
        }

        var nReturn = (windowW/2) - 32;

        return nReturn;
    };

    this.vertDnY = function(val)
    {
        var nHeight = $(window).height();
        var nReturn = val+16;

        return nReturn;
    };


    this.movePos = function( nX,
                             nY,
                             opacity,
                             time,
                             orientation)

    {
        this.setPos(nX,nY);

        return;

        if(m_nOrientation != 0 && this.bKilled == false)
        {

            if(orientation != m_nOrientation && this.bKilled == false)
            {
                this.resetOrientation(orientation);
            }

            if(m_nOrientation == 1  && this.bKilled == false)
            {
                nDnX = this.getDnX(nX);
                nDnY = this.getDnY(nY);

                nUpX = this.getUpX(nX);
                nUpY = this.getUpY(nY);

                txtUpX = (m_nW/1.8)*m_nXScale;
                txtDnX = (m_nW/2.2)*m_nXScale;
                txtY = (m_nH/2)*m_nYScale;
            }

            if( m_nOrientation == 2 && this.bKilled == false)
            {
                nDnX = this.rightDnX(nX);
                nDnY = this.rightDnY(nY);

                nUpX = this.rightUpX(nX);
                nUpY = this.rightUpY(nY);

                txtUpX = (m_nW/1.8)*m_nXScale;
                txtDnX = (m_nW/2.2)*m_nXScale;
                txtY = (m_nH/2)*m_nYScale;
            }

            //do we need to create the arrows
            if(m_imgDnArrow != null && this.bKilled == false)
            {
                m_imgDnArrow.attr({"x":nDnX,"y":nDnY,"opacity": opacity});
            }
            //do we need to create the arrows
            if(m_imgUpArrow != null && this.bKilled == false)
            {
                m_imgUpArrow.attr({"x":nUpX,"y":nUpY,"opacity": opacity});
            }

            if(m_imgUpTrack != null && this.bKilled == false)
            {
                m_imgUpTrack.attr({"x":nUpX,"y":nUpY,"opacity": opacity});
            }
            //do we need to create the arrows
            if(m_imgDnTrack != null && this.bKilled == false)
            {
                m_imgDnTrack.attr({"x":nDnX,"y":nDnY,"opacity": opacity});
            }

            if(m_txtUp != null && this.bKilled == false)
            {
                m_txtUp.attr({"x":nUpX+txtUpX,"y":nUpY+txtY,"opacity": opacity});
            }

            if(m_txtDn != null && this.bKilled == false)
            {
                m_txtDn.attr({"x":nDnX+txtDnX,"y":nDnY+txtY+m_shadowY,"opacity": opacity});
            }
            if(m_shadowUp != null && this.bKilled == false)
            {
                m_shadowUp.attr({"x":nUpX+txtUpX+m_shadowX,"y":nUpY+txtY+m_shadowY,"opacity": opacity});
            }

            if(m_shadowDn != null && this.bKilled == false)
            {
                m_shadowDn.attr({"x":nDnX+txtDnX+m_shadowX,"y":nDnY+txtY,"opacity": opacity});
            }

            m_nX = nX;
            m_nY = nY;
        }
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
        var nDeltaX = m_nX;
        var nDeltaY = m_nY;

        {
            m_nXScale = nXScale;
            m_nYScale = nYScale;

            if(m_imgUpTrack != null && this.bKilled == false)
            {
                m_imgUpTrack.attr({x:nUpX,y:nUpY,width:m_nW*nXScale,height:m_nH*nYScale});
            }

            if(m_imgDnTrack != null && this.bKilled == false)
            {
                m_imgDnTrack.attr({x:nUpX,y:nUpY,width:m_nW*nXScale,height:m_nH*nYScale});
            }

            if(m_nOrientation != 3)
            {
                if(m_imgDnArrow != null && this.bKilled == false)
                {
                    m_imgDnArrow.attr({x:nUpX,y:nUpY,width:m_nW*nXScale,height:m_nH*nYScale});
                }

                if(m_imgUpArrow != null && this.bKilled == false)
                {
                    m_imgUpArrow.attr({x:nUpX,y:nUpY,width:m_nW*nXScale,height:m_nH*nYScale});
                }
            }

            if(m_txtUp != null && this.bKilled == false)
            {
                if(m_nOrientation == 3 && this.bKilled == false)
                {
                    m_txtUp.transform("S"+nXScale+","+nYScale+"r90");
                }
                else
                {
                    m_txtUp.transform("S"+nXScale+","+nYScale);
                }
            }

            if(m_txtDn != null && this.bKilled == false)
            {
                if(m_nOrientation == 3 && this.bKilled == false)
                {
                    m_txtDn.transform("S"+nXScale+","+nYScale+"r90");
                }
                else
                {
                    m_txtDn.transform("S"+nXScale+","+nYScale);
                }
            }

            if(m_shadowUp != null && this.bKilled == false)
            {
                if(m_nOrientation == 3 && this.bKilled == false)
                {
                    m_shadowUp.transform("S"+nXScale+","+nYScale+"r90");
                }
                else
                {
                    m_shadowUp.transform("S"+nXScale+","+nYScale);
                }
            }

            if(m_shadowDn != null && this.bKilled == false)
            {
                if(m_nOrientation == 3 && this.bKilled == false)
                {
                    m_shadowDn.transform("S"+nXScale+","+nYScale+"r90");
                }
                else
                {
                    m_shadowDn.transform("S"+nXScale+","+nYScale);
                }
            }
        }

        this.setPos(m_nX,m_nY);
    }


    //call the function to do the initial display
    this.setPos(nX,nY);

    if( nUpBytesPerSecond != undefined &&
        nDnBytesPerSecond != undefined &&
        nPercentUp != undefined &&
        nPercentDn &&
        this.bKilled == false)
    {

        this.setUpDown( nUpBytesPerSecond,
                        nDnBytesPerSecond,
                        nPercentUp,
                        nPercentDn,
                        0);
    }
    else
    {
        this.setUpDown( 0,
                        0,
                        0,
                        0,
                        0);
    }

};
