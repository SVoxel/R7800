/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
////////////////////////////////////////////////////////////////////////
/*
    Function: getLanguage

    get the language the browser would like us to display

    Parameters:
      none.

    Returns:
      language code string (Example: EN-US)

    See also:
      nothing.
*/
////////////////////////////////////////////////////////////////////////
function getLanguage()
{
    //get the browsers language
    var language = window.navigator.userLanguage || window.navigator.language;

    //if we get something
    if(language != undefined)
    {
        //convert to lower
        language = language.toUpperCase();
    }

    //give it back
    return language;
}

/*
    Variable: g_Language

    global instance of the translation table

    file format for load translation is json:
    {
        "language": "name",
        trans
        [
            {"string 1" : "translation 2"},
            {"string 2" : "translation 2"},
        ]
    }
*/
var g_Language = null;



////////////////////////////////////////////////////////////////////////
/*
    Function: loadTranslation

    loadTranslation from the web server

    Parameters:
      strPath - Web path tho load the translation

    Returns:
      false = failure
      true  = success

    See also:
      nothing.
*/
////////////////////////////////////////////////////////////////////////
function loadTranslation(strFile)
{
    //fail by default
    var bReturn = false;

    //1st get the local language
    var strLocaleFile = "translate_"+getLanguage()+".json";

    if(strFile != undefined && strFile != "")
    {
        strLocaleFile = strFile;
    }

    var strURL = g_path.lstrings + strLocaleFile;

    //try to load the language
    $.ajax(
            {
                type: "GET",
                async: false,
                url: strURL,
                dataType: "json",
                //-------------------------------------------------------------------
                // Issue callback with result data
                success: function(data, status, request)
                {
                    //set the global language to the returned data
                    g_Language = data;

                    //if we loaded the language return success
                    bReturn = true;
                },
                error: function(data, status, request)
                {
                    //if we arent already english
                    if(strLocaleFile != "translate_EN-US.json")
                    {
                        //try to load english
                        strLocaleFile = "translate_EN-US.json";

                        //do the load / if this doesn't work the
                        //router is most likely really really broken
                        loadTranslation(strLocaleFile);
                    }
                    else
                    {
                        //if we are here the browser failed to load a translation file
                        //alert("Unable to load translation files. Please check your network connection.");
                    }
                },

    });

    return bReturn;
}

////////////////////////////////////////////////////////////////////////
/*
    Function: _t

    get the translated version of a string

    Parameters:
      strTranslate - string to translate

    Returns:
      the translated string

    See also:
      nothing.
*/
////////////////////////////////////////////////////////////////////////
function _t(strTranslate)
{
    //set the string given to return by default
    var strReturn =strTranslate;

    if(g_Language!=undefined && g_Language != null)
    {
        //to get the language string
        var strTemp = g_Language.trans[strTranslate];

        //if we got back a valid string
        if(strTemp != undefined)
        {
            //set the return
            strReturn = strTemp;
        }
    }

    return strReturn;
};

