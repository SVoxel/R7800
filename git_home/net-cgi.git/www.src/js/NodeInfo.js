/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
////////////////////////////////////////////////////////////////////////
/*
    Class: NodeInfo

    The popup window that display info about a my network node when
    the mouse hovers over it.
*/
////////////////////////////////////////////////////////////////////////


function NodeInfo(selector, dheight, dwidth)
{
    var self = this;

    //background color of this windows
    this.bkgColor = "#EEEEEE";

    var x1 = $(selector).parent().parent().offset().left;
    var y1 = $(selector).parent().parent().offset().top;

    //the currently selected node
    //this is a graphitem class see it for details
    this.nodeCurrent = null;

    //show the dialog for this node
    this.show = function(x,y,opener)
    {
        self.onOpen = opener;

        $(selector).dialog('option', 'position', [x+x1,y+y1]);

        //open the dialog
        $( selector ).dialog( "open" );
    }

    //hide the dialog
    this.hide = function()
    {
        //show the dialog
        $( selector ).dialog( "close" );
    }

    this.init = function()
    {
        //create the dialog
        $(selector).dialog({
                    autoOpen: false,
                    height: dheight,
                    width: dwidth,
                    modal: false,
                    resizable: false,
                    zIndex:76000,
                    dialogClass: "dlghover",
                    close: function() {
                    }
        });

        //turn a standard jquery dialog into a neat popup info window
        $(".dlghover .ui-dialog-titlebar").css("display", "none");
        $(".dlghover .ui-dialog-title").css("font-size", "5px");
        $(".dlghover .ui-widget-header").css("display", "none");
        $(".dlghover").css("background", self.bkgColor);
        $(".dlghover").css("color", "black");
        $(".dlghover .ui-widget-content").css("background-color", self.bkgColor);
        $(".dlghover .ui-widget-content").css("font-size", "12px");
        $(".dlghover .ui-widget-content").css("color", "black");
        $(".dlghover .ui-corner-all").css("background-color", "green");
        $(".dlghover .ui-resizable-n").css("background-image", "none");

        //bind the on open event to our handler
        $( selector ).bind( "dialogopen", function(event, ui) {

                //do we have valid on open
                if( typeof self.onOpen != 'undefined' &&
                    typeof self.onOpen == 'function' &&
                    self.onOpen != null)
                {
                    //call the handler when this is hit
                    self.onOpen();
                }
            });
    }

    //call our init function to set the class up
    this.init();
}
