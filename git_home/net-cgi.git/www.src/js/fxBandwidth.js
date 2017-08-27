/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */

/*
    Variable: g_nMeterValue
    The current value of the progress meter in the dialog
*/
var g_nMeterValue = 0;
var g_bHasRun = false;
//if we have valid values put them in the text field
var g_strULEdit = "#streamboost_dl_speed";
var g_strDLEdit = "#streamboost_up_speed";

////////////////////////////////////////////////////////////////////////
/*
    Function: bwTest

    Call this to start a bandwidth test and display a progress

    Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
*/
////////////////////////////////////////////////////////////////////////
function bwTest(strULEdit,strDLEdit)
{
    //if the login is supposed to time out
    //stop it for the duration of the test
    if(isCheckingLogin() == true)
    {
        stopCheckLogin();
    }

    if( strULEdit != undefined && strULEdit != null &&
        strDLEdit != undefined && strDLEdit != null)
    {
        g_strULEdit = strULEdit;
        g_strDLEdit = strDLEdit;
    }

    $.ajax(
    {
        type: "PUT",
        async: false,
        url: g_path.ozker + "/api/aperture",
        cache: false,
        success: function(_kResult, _kStatus, _kRequest)
        {
            showBWTest();
        },
        error: function(_kResult, textStatus, errorThrown)
        {
            alert("failed to start aperture");
        }
    });
}

////////////////////////////////////////////////////////////////////////
/*
    Function: hasRunBWTest

    Has a successful test been run?

    Parameters:
            none.

        Returns:
            true/false

        See also:
            nothing.
*/
////////////////////////////////////////////////////////////////////////
function hasRunBWTest()
{
    return g_bHasRun;
}

////////////////////////////////////////////////////////////////////////
/*
    Function: showBWTest

    internal async callback for bwtest() to start the bw testing dialog

    Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
*/
////////////////////////////////////////////////////////////////////////
function showBWTest()
{
        $( "#dialog-bwtest" ).unbind( "dialogopen");

        $( "#dialog-bwtest" ).bind( "dialogopen", function(event, ui) {

            //clear the meter def value
            g_nMeterValue = 0;

            //create the progress bar
            $( "#bwprogress" ).progressbar({value: g_nMeterValue});

            //remove the close button
            $(".ui-dialog-titlebar-close").hide();

            //reset the meter back to 0
            g_nMeterValue = 0;

            //start the timeout
            setTimeout(doMeter,2000);
        });

        //hook the bandwidth estimation button
        $( "#dialog-bwtest" ).dialog({
            resizable: false,
            height:150,
            width:500,
            modal: true,
            zIndex: 95005,
            buttons: {
            }
        });
}

////////////////////////////////////////////////////////////////////////
/*
    Function: doMeter

    internal callback for bwtest() increment the meter and display
    error/success messasages

    Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
*/
////////////////////////////////////////////////////////////////////////
function doMeter()
{
    $( "#bwprogress" ).progressbar({value: g_nMeterValue++});

    var bStillRunning = true;
    //set the default values to an error state
    var nUpBW = -1;
    var nDnBW = -1;

    $.ajax(
    {
        type: "GET",
        async: false,
        cache: false,
        url: g_path.ozker + "/api/aperture",
        success: function(data)
        {
            bStillRunning = data.measuring;

            //check the error code for failure
            if( bStillRunning == false)
            {
                //do we have an error state
                if( data.last.retcode != undefined &&
                    (data.last.retcode != 0 && data.last.retcode != -99))
                {
                    //make the wizard stop running
                    bStillRunning = false;

                    alert(_t("Bandwidth test failed. Please check your network connection and try again."));
                }
                else //if we succeeded at our test
                {
                    //get the test values
                    nUpBW = parseInt((data.last.up),10);
                    nDnBW = parseInt((data.last.down),10);

                    //set the has run state to true
                    g_bHasRun = true;
                }
            }
        },
        error: function(_kResult, textStatus, errorThrown)
        {
            //make the wizard stop running
            bStillRunning = false;

            alert(_t("querying aperture failed."));
        }
    });

    if(g_nMeterValue < 100 && bStillRunning == true)
    {
        setTimeout(doMeter,2000);
    }
    else
    {
        //if we timed out
        if(g_nMeterValue >= 100 && bStillRunning == true)
        {
            alert(_t("Bandwidth test failed. Please check your network connection and try again."));
        }
        else if(nDnBW != NaN && nUpBW != NaN) //legit numbers?
        {
            //are the numbers valid?
            if(nDnBW!=-1 && nUpBW !=-1 && nDnBW!=0 && nUpBW !=0)
            {
                //if we have valid values put them in the text field
                $(g_strULEdit).val(nDnBW);
                $(g_strDLEdit).val(nUpBW);

                 getSpeedField();
            }
        }

        $( "#dialog-bwtest" ).dialog( "close" );

        //if the login is supposed to time out
        //restart it
        if(isCheckingLogin() == true)
        {
            checkLogin();
        }
    }
}
