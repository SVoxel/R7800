/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
////////////////////////////////////////////////////////////////////////
/*
    Class: fxHelp

    Version:   1.0

    URL sensative help pane.
*/
////////////////////////////////////////////////////////////////////////
function fxHelp(strDivID,pHelp)
{
    //load the translation library
    loadScript("fxTranslate-2.0.js");

    /*
        Variable: self
        The reference to the class to allow
        allow callbacks
    */
    var self = this;

    /*
        Variable: strDivID
        The div our help pane is in
    */
    this.strDivID = "";

    /*
        Variable: bShow
        Current state of the help pane.
        true  = shown
        false = hidden
    */
    this.bShow = false;

    /*
        Variable: pHelpCB
        callback to get the help id in the trans file
    */
    this.pHelpCB = null;

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: initialize

        initialize our help pane

        Parameters:
          strDivID  - div to append our pane to
          pHelp     - callback to return help file id in trans file

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.initialize = function(strDivID,pHelp)
    {
        //set the div
        self.strDivID = strDivID;
        self.pHelpCB  = pHelp;


        var strHelp = "<div id=\"H\" class=\"Help\"><div id=\"helpPane\" class=\"HelpPane\"></div></div>";

        var strButton = "<div id=\"helpBT\" class=\"HelpButton\" style=\"z-index:100000;\"></div><div id=\"helpHv\" class=\"HelpHover\"></div>";

        $("body").append(strButton);
        $("body").append(strHelp);
        $("#helpHv").text(_t("Click to open the help system."));

        //hook up the help clicks
        $("#helpBT").click(function()
        {
            //if we are not showing the pane
            if(self.bShow == false)
            {
                //set the state
                self.bShow = true;

                //display the help pane
                $("#H").css("display","block");

                //set the button to selected
                $("#helpBT").addClass("HelpButtonSelect");
                $("#helpBT").removeClass("HelpButton");

                //change hover message to clase
                $("#helpHv").text(_t("Click to close the help system."));
            }
            else
            {
                //set state
                self.bShow = false;

                //hide display
                $("#H").css("display","none");

                //set button state
                $("#helpBT").addClass("HelpButton");
                $("#helpBT").removeClass("HelpButtonSelect");

                //change hover message to open
                $("#helpHv").text(_t("Click to open the help system."));
            }
        });

        //hook up the mouseover message
        $("#helpBT").hover(function()
        {
            //show hover message
            $("#helpHv").css("display","block");
        },
        function()
        {
            //hide message
            $("#helpHv").css("display","none");
        });

        //get the current help path file name to load
        //from the tranlation
        var strHelpIndex = self.pHelpCB();

        //call add help to put it in the pane
        self.addHelp("#helpPane",strHelpIndex);
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: addHelp

        Add the help text from tranlation files to a div.

        Parameters:
          div - the div to add the help to.

        Returns:
          nothing

        See also:
          none.
    */
    ////////////////////////////////////////////////////////////////////////
    this.addHelp = function(div,help)
    {
        //make sure the trans files are loaded for help
        loadTranslation();

        var strHTML = "<h1>"+help+" is not found</h1>";

        //load the translated help
        var strHelp =   _t(help);

        //if we get back a valid string array
        if(strHelp!= undefined && strHelp != null && strHelp.length != undefined && strHelp.length > 0 && strHelp instanceof Array)
        {
            //join the array of the help and put it in the div
            strHTML = strHelp.join("");
        }

        //join the array of the help and put it in the div
        $(div).html(strHTML);

    }

    //if we havent made the div yet
    if(self.strDivID=="")
    {
        //call the main init routine at construction time
        self.initialize(strDivID,pHelp);
    }
}