/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
var g_nShortTimer = 2;
var g_nShortTimerM = g_nShortTimer*1000;

//a globally unique name timeout variable for other javascripts

var m_nIconSize = 128;
var m_nHalfSize = m_nIconSize/2;
var m_nFontSize = m_nIconSize*.11;
var g_nWidth = 0;

// getTimezoneOffset returns minutes, convert to MS
var TZOFFSETMS = 0;//(new Date().getTimezoneOffset()) * 60 * 1000;

var g_routerDelta; // add this to any router absolute time to get the actual time
var g_routerDeltaGMT; // this is for jquery flot which can't figure out how to do timezones :(

var g_nMinWidth     = 671;

function getIconSize(nScale)
{
    return (m_nIconSize)*nScale;
}

//global upload/download limits
var g_nMaxDownload;
var g_nMaxUpload;

if(typeof g_nMaxUpload == 'undefined' || g_nMaxUpload <= 1024)
{
    g_nMaxUpload   = 5000000;
}

if(typeof g_nMaxDownload == 'undefined' || g_nMaxDownload <= 1024)
{
    g_nMaxDownload = 20000000;
}

var g_nMaxRollOver = parseInt("FFFFFFFF",16);

//global for the flow string to icon.png name
var g_strIcons = [];

//add the defaut icons for diffent pc types
g_strIcons["Windows"] = "Computer-256x256.png",
g_strIcons["Mac OS X"] = "iMac-icon.png";
g_strIcons["iOS"] = "iphone.png";
g_strIcons["Apple"] = "Apple.png";
g_strIcons["Apple TV"] = "Apple-TV-icon.png";
g_strIcons["Nintendo Wii"] = "wii.png";
g_strIcons["Android Phone"] = "android.png";
g_strIcons["Android 4.x"] = "android.png";
g_strIcons["Wireless"] = "Laptop.svg.med.png";
g_strIcons["X-Box 360"] = "xbox360.png";
g_strIcons["Xbox"] = "xbox360.png";
g_strIcons["Tivo"] = "hd-tivo-icone-4593-128.png";
g_strIcons["Linux"] = "Laptop.svg.med.png";
g_strIcons["Unknown"] = "UnknownDevice.png";
g_strIcons["Playstation"] = "ps3-slim.png";
g_strIcons["iPhone"] = "iphone.png";
g_strIcons["Nintendo"] = "wii.png";
g_strIcons["iPad"] = "ipad.png";
g_strIcons["Playstation 3"] = "ps3-slim.png";
g_strIcons["Xbox 360"] = "xbox360.png";
g_strIcons["Android"] = "tablet.png";
g_strIcons["iPod Touch"] = "iphone.png";
g_strIcons["Blackberry"] = "android.png";
g_strIcons["Vizio Smart Device"] = "smarttv.png";
g_strIcons["Google TV"] = "googletv.png";
g_strIcons["Roku"] = "roku.png";
g_strIcons["Boxee"] = "boxee.png";
g_strIcons["Playstation Vita"] = "psvita.png";
g_strIcons["Nintendo 3DS"] = "nintendo3ds.png";
g_strIcons["Alienware PC"] = "alien.png";
g_strIcons["WD TV"] = "wdtv.png";
g_strIcons["D-Link DSM-312 MovieNite Plus"] = "dlinkmovienite.png";
g_strIcons["Router"] = "Killer.png";
g_strIcons["Internet"] = "internet-globe-earth-terra.png";
g_strIcons["NVIDIA SHIELD"] = "nvidia_shield.png";
g_strIcons["Sony BD Player"] = "sonybd.png";
g_strIcons["HTC Amaze 4G"] = "android.png";
g_strIcons["HTC Android Device"] = "android.png";
g_strIcons["HTC Desire"] = "android.png";
g_strIcons["HTC Droid DNA"] = "android.png";
g_strIcons["HTC J"] = "android.png";
g_strIcons["HTC One"] = "android.png";
g_strIcons["HTC One S"] = "android.png";
g_strIcons["HTC One X"] = "android.png";
g_strIcons["HTC Rezound"] = "android.png";
g_strIcons["HTC Shift 4G"] = "android.png";
g_strIcons["HTC ThunderBolt"] = "android.png";
g_strIcons["iPad 2"] = "ipad.png";
g_strIcons["iPad Mini"] = "ipad.png";
g_strIcons["iPad Air"] = "ipad.png";
g_strIcons["iPad Retina"] = "ipad.png";
g_strIcons["iPad Mini Retina"] = "ipad.png";
g_strIcons["iPhone 4"] = "iphone.png";
g_strIcons["iPhone 4s"] = "iphone.png";
g_strIcons["iPhone 5"] = "iphone.png";
g_strIcons["iPhone 5c"] = "iphone.png";
g_strIcons["iPhone 5s"] = "iphone.png";
g_strIcons["Kindle Fire"] = "android.png";
g_strIcons["Nexus 4"] = "android.png";
g_strIcons["Nexus 5"] = "android.png";
g_strIcons["Nexus 7"] = "android.png";
g_strIcons["Nexus 10"] = "android.png";
g_strIcons["Samsung Device"] = "android.png";
g_strIcons["Samsung Galaxy Nexus"] = "android.png";
g_strIcons["Samsung Galaxy Note"] = "android.png";
g_strIcons["Samsung Galaxy Note 3"] = "android.png";
g_strIcons["Samsung Galaxy Note II"] = "android.png";
g_strIcons["Samsung Galaxy S III"] = "android.png";
g_strIcons["Samsung Galaxy S II"] = "android.png";
g_strIcons["Samsung Galaxy S"] = "android.png";
g_strIcons["Samsung Galaxy S4"] = "android.png";
g_strIcons["Samsung Galaxy S4 Mini"] = "android.png";
g_strIcons["Samsung Smart TV"] = "smarttv.png";
g_strIcons["Sony KDL-55W900A"] = "smarttv.png";
g_strIcons["Sony KDL-55HX850"] = "smarttv.png";
g_strIcons["Sony TV"] = "smarttv.png"

/*
    strMessage - the message to display
    strType    - 'notice','success','warning','error'
    bSticky    - true = stay till user closes  - false = auto fade after a few seconds
    pCallback  - when the user closes this or the windows fades call the calback
*/

function addAlert( strMessage, strType, bSticky, pCallback)
{
    // making a test message
    $().toastmessage('showToast', {
        text     : strMessage,
        sticky   : bSticky,
        position : 'top-right',
        type     : strType,
        closeText: '',
        close    : function () {
           if(pCallback != undefined)
           {
                pCallback();
           }
        }
    });
}

//this function allows only valid letters an input
//hand in the control id as paramater
function SimpleText(control)
{
 /*   $(control).bind('cut paste copy', function(e) {
        e.preventDefault();
    });

    $(control).disableSelection();
*/
    $(control).bind('keypress', function(e)
    {
        var textArea = $(control);
        var len = textArea.val().length;
        var start = textArea[0].selectionStart;
        var end = textArea[0].selectionEnd;
        var selectedText = textArea.val().substring(start, end);

        if(selectedText == "")
        {
            //are we an invalid key??
            if( e.which > 0 &&                  //not negative
                e.which != 8 &&                 //allow backspace
                e.which != 45 &&                 //allow -
                e.which != 95 &&                 //allow _
                e.which != 32 &&                 //allow space
                !(e.which >=48 && e.which <=57) && //allow number
                !(e.which >=65 && e.which <=90) && //allow caps
                !(e.which >=96 && e.which <=122) //allow lowers
              )
            {
                e.preventDefault();
            }
        }
    });
}

//this function allows only valid letters an input
//hand in the control id as paramater
function LengthText(control,len)
{
   /* $(control).bind('cut paste copy', function(e) {
        e.preventDefault();
    });
    $(control).disableSelection();
*/
    $(control).bind('keypress', function(e)
    {
        var textArea = $(control);
        var slen = textArea.val().length;
        var start = textArea[0].selectionStart;
        var end = textArea[0].selectionEnd;
        var selectedText = textArea.val().substring(start, end);

        if(selectedText == "")
        {
            var strValue = $(control).val();

            //are we an invalid key??
            if( e.which > 0 &&                  //not negative
                e.which != 8 &&                 //allow backspace
                e.which != 45 &&                 //allow -
                e.which != 95 &&                 //allow _
                e.which != 32 &&                 //allow space
                !(e.which >=48 && e.which <=57) && //allow number
                !(e.which >=65 && e.which <=90) && //allow caps
                !(e.which >=96 && e.which <=122)  //allow lowers
              )
            {
                e.preventDefault();
            }
            else if(strValue.length >= len)
            {
                e.preventDefault();
            }

        }
    });
}

//this function formats ip controls
//hand in the control id as paramater
function PortRange(control)
{
 /*   $(control).bind('cut paste copy', function(e) {
        e.preventDefault();
    });

    $(control).disableSelection();
*/
    $(control).bind('keypress', function(e)
    {
        var textArea = $(control);
        var len = textArea.val().length;
        var start = textArea[0].selectionStart;
        var end = textArea[0].selectionEnd;
        var selectedText = textArea.val().substring(start, end);

        //cache the caret position
        var nCaret = $(this).caret().start;

        if(selectedText == "")
        {
            //are we an invalid key??
            if( e.which > 0 &&                  //not negative
                e.which != 8 &&                 //allow backspace
                e.which != 45 &&                //allow period
                !(e.which >=48 && e.which <=57)  //allow number
              )
            {
                e.preventDefault();
            }
            else //if we are a valid key
            {
                //if we aren't the backspace key
                if(e.which != 8)
                {
                    var strValue = $(control).val();
                    var strLeftChar = strValue.substr(nCaret-1,nCaret);

                    if(strLeftChar == '')
                    {
                        strLeftChar = '.';
                    }

                    //if the string is to long
                    if(strValue.length >= 12 )
                    {
                        e.preventDefault();
                    }
                    else if(e.which == 45 && strLeftChar == '-')
                    {
                        e.preventDefault();
                    }
                    else //if we have to many segments
                    {
                        var array = strValue.split("-");

                        //make the value
                        var stredit = strValue.substr(0,nCaret) + String.fromCharCode(e.which) + strValue.substr(nCaret);

                        var newArray = stredit.split("-");

                        if( array.length >= 5) //do we have to many segments
                        {
                            e.preventDefault();
                        }
                        else if((array.length - 1) >= 1 && e.which == 45) // if we already have 3 period
                        {
                            e.preventDefault();
                        }
                        else if(newArray.length >= 1 && newArray[0] > 65535) // if the 1st part is > 65535
                        {
                            e.preventDefault();
                        }
                        else if(newArray.length >= 2 && newArray[1] > 65535)  // if the 2nd part is > 65535
                        {
                            e.preventDefault();
                        }

                        //if we don't have a complete ip
                        /*if(e.isDefaultPrevented() == false && newArray.length<2)
                        {
                            //if the part they just typed is 3 digits long
                            if(newArray[newArray.length-1].length >= 2)
                            {
                                e.preventDefault();
                                $(control).val(strValue+String.fromCharCode(e.which)+"-");
                            }
                        }*/
                    }
                }
            }
        }
    });
}
//this function formats ip controls
//hand in the control id as paramater
function MacField(control)
{
 /*   $(control).bind('cut paste copy', function(e) {
        e.preventDefault();
    });

    $(control).disableSelection();
   */
    $(control).bind('keypress', function(e)
    {
        var textArea = $(control);
        var len = textArea.val().length;
        var start = textArea[0].selectionStart;
        var end = textArea[0].selectionEnd;
        var selectedText = textArea.val().substring(start, end);

        //cache the caret position
        var nCaret = $(this).caret().start;

        if(selectedText == "")
        {
            //are we an invalid key??
            if( e.which > 0 &&                  //not negative
                e.which != 8 &&                 //allow backspace
                e.which != 58 &&                //allow period
                !(e.which >=48 && e.which <=57)&& //allow number
                !(e.which >=10 && e.which <=15)&&
                !(e.which >=97 && e.which <=102)
              )
            {
                e.preventDefault();
            }
            else //if we are a valid key
            {
                //if we aren't the backspace key
                if(e.which != 8)
                {
                    var strValue = $(control).val();
                    var strLeftChar = strValue.substr(nCaret-1,nCaret);

                    if(strLeftChar == '')
                    {
                        strLeftChar = ':';
                    }

                    //to upper
                    /*if(e.which >=97 && e.which <=102)
                    {
                        e.which -= 87;
                    }*/

                    //if the string is to long
                    if(strValue.length >= 18 )
                    {
                        e.preventDefault();
                    }
                    else if(e.which == 46 && strLeftChar == ':')
                    {
                        e.preventDefault();
                    }
                    else //if we have to many segments
                    {
                        var array = strValue.split(":");

                        //make the value
                        var stredit = strValue.substr(0,nCaret) + String.fromCharCode(e.which) + strValue.substr(nCaret);

                        var newArray = stredit.split(":");

                        if( array.length >= 7) //do we have to many segments
                        {
                            e.preventDefault();
                        }
                        else if((array.length - 1) >= 5 && e.which == 58) // if we already have 3 period
                        {
                            e.preventDefault();
                        }/*
                        else if(newArray.length >= 1 && newArray[0].length > 2) // if the 1st part is > 2 chars
                        {
                            e.preventDefault();
                        }
                        else if(newArray.length >= 2 && newArray[1].length > 2) // if the 2nd part is > 2 chars
                        {
                            e.preventDefault();
                        }
                        else if(newArray.length >= 3 && newArray[2].length > 2) // if the 3rd part is > 2 chars
                        {
                            e.preventDefault();
                        }
                        else if(newArray.length >= 4 && newArray[3].length > 2) // if the 4th part is > 2 chars
                        {
                            e.preventDefault();
                        }
                        else if(newArray.length >= 5 && newArray[4].length > 2) // if the 5th part is > 2 chars
                        {
                            e.preventDefault();
                        }
                        else if(newArray.length >= 6 && newArray[5].length > 2) // if the 6th part is > 2 chars
                        {
                            e.preventDefault();
                        } */

                        //if we don't have a complete ip
                        if(e.isDefaultPrevented() == false && newArray.length<6)
                        {
                            //if the part they just typed is 3 digits long
                            if(newArray[newArray.length-1].length >= 2)
                            {
                                e.preventDefault();
                                $(control).val(strValue+String.fromCharCode(e.which)+":");
                            }
                        }

                        if(e.isDefaultPrevented() == false)
                        {
                            e.preventDefault();

                            //make the value
                            var stredit = strValue.substr(0,nCaret) + String.fromCharCode(e.which) + strValue.substr(nCaret);
                            $(control).val(stredit.toUpperCase()).caret(nCaret+1,nCaret+1);

                        }
                    }
                }
            }
        }
    });
}

//this function formats ip controls
//hand in the control id as paramater
function IPField(control)
{
 /*   $(control).bind('cut paste copy', function(e) {
        e.preventDefault();
    });

    $(control).disableSelection();
   */
    $(control).bind('keypress', function(e)
    {
        var textArea = $(control);
        var len = textArea.val().length;
        var start = textArea[0].selectionStart;
        var end = textArea[0].selectionEnd;
        var selectedText = textArea.val().substring(start, end);

        //cache the caret position
        var nCaret = $(this).caret().start;

        if(selectedText == "")
        {
            //are we an invalid key??
            if( e.which > 0 &&                  //not negative
                e.which != 8 &&                 //allow backspace
                e.which != 46 &&                //allow period
                !(e.which >=48 && e.which <=57)  //allow number
              )
            {
                e.preventDefault();
            }
            else //if we are a valid key
            {
                //if we aren't the backspace key
                if(e.which != 8)
                {
                    var strValue = $(control).val();
                    var strLeftChar = strValue.substr(nCaret-1,nCaret);

                    if(strLeftChar == '')
                    {
                        strLeftChar = '.';
                    }

                    //if the string is to long
                    if(strValue.length >= 15 )
                    {
                        e.preventDefault();
                    }
                    else if(e.which == 46 && strLeftChar == '.')
                    {
                        e.preventDefault();
                    }
                    else //if we have to many segments
                    {
                        var array = strValue.split(".");

                        //make the value
                        var stredit = strValue.substr(0,nCaret) + String.fromCharCode(e.which) + strValue.substr(nCaret);

                        var newArray = stredit.split(".");

                        if( array.length >= 5) //do we have to many segments
                        {
                            e.preventDefault();
                        }
                        else if((array.length - 1) >= 3 && e.which == 46) // if we already have 3 period
                        {
                            e.preventDefault();
                        }
                        /*else if((newArray.length >= 1 && parseInt(newArray[0]) > 255 ) || (newArray.length >= 1 && newArray[0].length >3) || (parseInt(newArray[0]) == 0)) // if the 1st part is > 255
                        {
                            e.preventDefault();
                        }
                        else if((newArray.length >= 2 &&  parseInt(newArray[1]) > 255) || (newArray.length >= 2 && newArray[1].length >3))  // if the 2nd part is > 255
                        {
                            e.preventDefault();
                        }
                        else if((newArray.length >= 3 &&  parseInt(newArray[2]) > 255) || (newArray.length >= 3 && newArray[2].length >3))  // if the 3rd part is > 255
                        {
                            e.preventDefault();
                        }
                        else if((newArray.length >= 4 &&  parseInt(newArray[3]) > 255) || (newArray.length >= 4 && newArray[3].length >3))  // if the 4th part is > 255
                        {
                            e.preventDefault();
                        }*/

                        //if we don't have a complete ip
                        if(e.isDefaultPrevented() == false && newArray.length<4)
                        {
                            //if the part they just typed is 3 digits long
                            if(newArray[newArray.length-1].length >= 3)
                            {
                                e.preventDefault();
                                $(control).val(strValue+String.fromCharCode(e.which)+".");
                            }
                        }
                    }
                }
            }
        }
    });
}

//this function allows only numbers an input
//hand in the control id as paramater
function NumField(control)
{
    /*$(control).bind('cut paste copy', function(e) {
        e.preventDefault();
    });

    $(control).disableSelection();
    */

    $(control).bind('keypress', function(e)
    {
        var textArea = $(control);
        var len = textArea.val().length;
        var start = textArea[0].selectionStart;
        var end = textArea[0].selectionEnd;
        var selectedText = textArea.val().substring(start, end);


        if(selectedText == "")
        {
        //are we an invalid key??
            if( e.which > 0 &&                  //not negative
                e.which != 8 &&                 //allow backspace
                !(e.which >=48 && e.which <=57)  //allow number
              )
            {
                e.preventDefault();
            }
        }
    });
}

//check if the current browser is firefox or not
function isFirefox()
{
    var bReturn = false;

    if(navigator.userAgent.toLowerCase().indexOf('firefox') != -1)
    {
        bReturn = true;
    }

    return bReturn;
}

//this function allows  numbers an input
//with one point upto length
function FloatLenField(control,length)
{
    $(control).bind('cut paste copy', function(e) {
        e.preventDefault();
    });

    //check if this is firefox because
    //disabling selection breaks it
    if(isFirefox() == false)
    {
        //disable text selection
        $(control).disableSelection();
    }

    $(control).bind('keypress', function(e)
    {
        var textArea = $(control);
        var len = textArea.val().length;
        var start = textArea[0].selectionStart;
        var end = textArea[0].selectionEnd;
        var selectedText = textArea.val().substring(start, end);

        var bAlreadyHasDot = false;

        if(textArea.val().indexOf(".") != -1)
        {
            bAlreadyHasDot = true;
        }

        //are we an invalid key??
        if( e.which > 0 &&                  //not negative
            e.which != 8 &&                 //allow backspace
            e.which != 46 &&
            !(e.which >=48 && e.which <=57)  //allow number
          )
        {
            e.preventDefault();
        }
        else
        {
            if(e.which != 8)
            {
                if(bAlreadyHasDot == true && e.which == 46)
                {
                    e.preventDefault();
                }

                var strValue = $(control).val();
                var strNewValue = strValue + String.fromCharCode(e.which);

                if(strNewValue.length > length)
                {
                    e.preventDefault();
                }
            }
        }
    });
}

//this function allows only numbers an input
//hand in the control id as paramater
function MinMaxField(control,min,max,len)
{
  /*  $(control).bind('cut paste copy', function(e) {
        e.preventDefault();
    });

    $(control).disableSelection();
    */

    $(control).bind('keypress', function(e)
    {
        var textArea = $(control);
        var slen = textArea.val().length;
        var start = textArea[0].selectionStart;
        var end = textArea[0].selectionEnd;
        var selectedText = textArea.val().substring(start, end);


        if(selectedText == "")
        {
            //are we an invalid key??
            if( e.which > 0 &&                  //not negative
                e.which != 8 &&                 //allow backspace
                e.which !=32 &&                 //allow a space
                !(e.which >=48 && e.which <=57)  //allow number
              )
            {
                e.preventDefault();
            }
            else
            {
                if(e.which != 8)
                {
                    var strValue = $(control).val();
                    var strNewValue = strValue + String.fromCharCode(e.which);

                   /* if(strNewValue > max )
                    {
                        e.preventDefault();
                    }
                    else*/ if(strNewValue.length > len)
                    {
                        e.preventDefault();
                    }
                }
            }
        }
    });
     $(control).bind('blur', function(e)
    {

            var strValue = $(control).val();
            var strNewValue = strValue + String.fromCharCode(e.which);

            if(strValue < min)
            {
                strValue = min;
                $(control).val(strValue);
            }
            else if(strValue > max)
            {
                strValue = max;
                $(control).val(strValue);
            }

    });
}

//=======================================================================
//  a pretty bytes to size with controlable precision for text display
//=======================================================================
function bytesToSize(bytes, precision)
{
    var strValue = "";
    // Ben/Ryan say all numbers in Megabytes (10^6) not Mebibytes (2^20)
    var kilobyte = 1000;
    var megabyte = kilobyte * 1000;
    var gigabyte = megabyte * 1000;
    var terabyte = gigabyte * 1000;

    if ((bytes >= 0) && (bytes < kilobyte))
    {
       strValue = bytes + ' b';

    }
    else if ((bytes >= kilobyte) && (bytes < megabyte))
    {
        strValue = (bytes / kilobyte).toFixed(precision) + ' Kb';

    }
    else if ((bytes >= megabyte) && (bytes < gigabyte))
    {
        strValue = (bytes / megabyte).toFixed(precision) + ' Mb';

    }
    else if ((bytes >= gigabyte) && (bytes < terabyte))
    {
        strValue = (bytes / gigabyte).toFixed(precision) + ' Gb';

    }
    else if (bytes >= terabyte)
    {
        strValue = (bytes / terabyte).toFixed(precision) + ' Tb';

    }
    else
    {
        strValue = bytes + ' b';
    }

    return strValue;
}

//=======================================================================
//          clone a javascript object - ie a full copy
//=======================================================================
function clone(obj)
{
    //if we don't have an object or we have the wrong type
    if (null == obj || "object" != typeof obj)
    {
        return obj;
    }

    //create a new object of the thing we are copying
    var copy = obj.constructor();

    //loop through and copy each of the attributes in the object
    for (var attr in obj)
    {
        //if the atrribute exists copy it.
        if (obj.hasOwnProperty(attr))
        {
            copy[attr] = obj[attr];
        }
    }

    //return our pretty new object
    return copy;
}


//=======================================================================
//          is the given value a valid ip
//=======================================================================
function isValidIP(strValue)
{
    var bReturn = false;

    var array = strValue.split(".");

    if( array.length == 4 ) //do we have to many segments
    {
        if(parseInt(array[0]) <= 255 && parseInt(array[0]) >= 0) // 0 - 255
        {
            if(parseInt(array[1]) <= 255 && parseInt(array[1]) >= 0) // 0 - 255
            {
                if(parseInt(array[2]) <= 255 && parseInt(array[2]) >= 0) // 0 - 255
                {
                    if(parseInt(array[3]) <= 255 && parseInt(array[3]) >= 0) // 0 - 255
                    {
                        bReturn = true;
                    }
                }
            }
        }
    }

    return bReturn;
}

function isIPLastDigit0(strValue)
{
    var bReturn = false;

    var array = strValue.split(".");

    if( array.length == 4 ) //do we have to many segments
    {
            if(parseInt(array[3]) == 0) // 0 - 255
            {
                bReturn = true;
            }
    }

    return bReturn;
}

//=======================================================================
//          is the given value a valid ip
//=======================================================================
function isValidMAC(strValue)
{
    var bReturn = false;

    var array = strValue.split(":");

    regex=/^([0-9a-f]{2}([:-]|$)){6}$|([0-9a-f]{4}([.]|$)){3}$/i;

    if( array.length == 6 ) //do we have to many segments
    {
        //check characters
        if(regex.test(strValue))
        {
            //check unit lengths
            if(array[0].length == 2)
            {
                if(array[1].length == 2)
                {
                    if(array[2].length == 2)
                    {
                        if(array[3].length == 2)
                        {
                            if(array[4].length == 2)
                            {
                                if(array[5].length == 2)
                                {
                                    bReturn = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return bReturn;
}

function setGlobalUpDown()
{
    var results = null;
    //get the data syncronously
    $.ajax(
            {
                type: "GET",
                async: false,
                url: g_path.ozker+"/api/bw_config",
                dataType: "json",
                cache : false,
                //-------------------------------------------------------------------
                // Issue callback with result data
                success: function(data, status, request)
                {
                    results = data;
                }
            }
    );


    if(results!=null && results.bw_config!=undefined )
    {
        g_nMaxDownload =  results.bw_config.down_limit * 8;
        g_nMaxUpload   = results.bw_config.up_limit * 8;
    }
}

////////////////////////////////////////////////////////////////////////
/*
    Function: $.urlParam

    Get a url encoded name paramater for use in javascrapt

    Parameters:
            name - name of the parameter to return

        Returns:
            the value of the parameter asked for

            example.com?param1=name&param2=&id=6
            $.urlParam('param1'); // name

        See also:
*/
////////////////////////////////////////////////////////////////////////

$.urlParam = function(name){
    var results = new RegExp('[\\?&]' + name + '=([^&#]*)').exec(window.location.href);

    if(results !=undefined && results.length>=1)
    {
        return results[1];
    }
    else
    {
        return 0;
    }
}

////////////////////////////////////////////////////////////////////////
/*
    Function: iconPath

    Get a url encoded name paramater for use in javascrapt

    Parameters:
            name - name of the icon to get a path too

        Returns:
            a path url to the valid version of the icon
            if it fails it will return a default icon

        See also:
*/
////////////////////////////////////////////////////////////////////////
iconPath = function(strIconName)
{
    var strReturn = strIconName;

    //if this isnt a local file path already
    if(strIconName.search("images/") == -1)
    {
        //default return to the cloud path
        strReturn = g_path.cloud+strIconName;

        //local path to get the image (if it exists)
        var strLocal = g_path.images+strIconName;
        var bLocal = false;
        //first check for local
        $.ajax(
        {
            url: strLocal,
            async: false,
            success: function(data, status, request)
            {
               //if we succeed this is a local resource so send it
               strReturn = strLocal;
               bLocal = true;
            }
        });

        //try to precache the image
        if(bLocal = false)
        {
            var imageObj = new Image();

            imageObj.src=strReturn;
        }
    }
    return strReturn;
}

//ten minute default timeout
var g_nTimeoutMS=g_nTime*1000;

//login timeout id
var g_nLoginTimeoutTimer = -1;

//is the check login timeout running
//for auto logout
isCheckingLogin = function()
{
    var bReturn = false;

    if(g_nLoginTimeoutTimer != -1)
    {
        bReturn = true;
    }

    return bReturn;
}

//check the login time and autologout if it hits
checkLogin = function(nTimeoutMS)
{
    //set to the default timout
    var nTime = g_nTimeoutMS;

    //if they did supply a timeout us it
    if(nTimeoutMS != undefined)
    {
        nTime = nTimeoutMS;
    }

    //call the timeout and if the user leaves
    //the page before that it resets
    g_nLoginTimeoutTimer = setTimeout(function()
    {
        window.location.href="login_pic.asp";
    },
    nTime);

    //start the keepAlive to
    //make sure we stay logged in during
    //befor timeout period
    keepAlive();
}

//stop the check Login Timeout
stopCheckLogin = function()
{
    if(g_nLoginTimeoutTimer != -1)
    {
        clearTimeout(g_nLoginTimeoutTimer);
    }
}

//simple function to keep us alive until our timeout
//by continually getting an image to fakeout the router
keepAlive = function()
{
    var bLoggedIn = false;

    //load an image
    $.ajax(
    {
        url: "/images/1-circle.png",
        async: false,
        success: function(data, status, request)
        {
           bLoggedIn = true;
        }
    });

    //if we are logged in keep up the keep alive
    if(bLoggedIn == true)
    {
        setTimeout("keepAlive()", 5000);
    }
    else //if we aren't logged in direct to the front page
    {
        //console.log("keepAlive: logged out, redirecting.");
        window.location.href="login_pic.asp";
    }
}

var g_nMegSize = 1000;

//convert Megabits to bytes no decimal
function Mb2B(val)
{
    var nReturn = 0;

    nReturn = ((val * g_nMegSize) * g_nMegSize) / 8;

    if(nReturn < 0)
    {
        nReturn = 0;
    }

    return (nReturn).toFixed(0);
}

//convert bytes to Megabits 2 decimal places
function B2Mb(val)
{
    var nReturn = 0;

    nReturn = ((val / g_nMegSize) / g_nMegSize) * 8;

    if(nReturn < 0)
    {
        nReturn = 0;
    }

    return (nReturn).toFixed(2);
}

//convert bytes to Megabits 2 decimal places
function B2Megabytes(val)
{
    var nReturn = 0;

    nReturn = ((val / g_nMegSize) / g_nMegSize);

    if(nReturn < 0)
    {
        nReturn = 0;
    }

    return nReturn;
}

function errOnRange(    strValue,
                        strUnit,
                        nValue,
                        nMin,
                        nMax)
{
    var bReturn = false;
    var strMessage = "none";

    if(nValue<nMin || nValue>nMax)
    {
        strMessage = strValue + " must be between " + nMin + " " + strUnit+ " and " + nMax + " " + strUnit+ ".";
    }
    else
    {
        bReturn = true;
    }

    if(bReturn==false)
    {
        alert(strMessage);
    }

    return bReturn;
}

//in Mbps
var g_nMaxBandwidth = 2000;
var g_nMinBandwidth = .003;

var g_nNodeStringSize = 14

//truncate the string to the given length
function makeLabel(strInput,nLength)
{
    var strReturn = strInput;

    if(strInput.length > nLength)
    {
        strReturn = strInput.substring(0,nLength)+"...";
    }

    return strReturn;
}

function makeNodeName(node)
{
    //default the name
    var strName = "Unknown";

    //try to get the real naem
    if(typeof node.name != 'undefined' && node.name != ""  && node.name != "*" && node.name != "UNKNOWN")
    {
        strName = node.name;
    }
    else if(typeof node.type != 'undefined' && node.type != "") //if not try to get the device type
    {
        strName = node.type;
    }
    else if(typeof node.ip != 'undefined' && node.ip != "") //if not get the ip
    {
        strName = node.ip;
    }
    else //if not get the mac addr
    {
        strName = node.mac;
    }

    return strName;
}

    //timeout for below function isActiveNode (currently 15 minutes)
    var g_nTimeOutSeconds = 15*60; //minutes * seconds

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: isActiveNode

        This function does looks at a node and tells you if its currently
        active on the network. IE has it passed data in the timeout range
        of g_nTimeOutSeconds

        Parameters:
          none.

        Returns:
          true  = active
          false = not active

        See also:
          none.
    */
    ////////////////////////////////////////////////////////////////////////
    isActiveNode = function(node)
    {
        var bReturn = false;

        //if the node seems valid
        if(typeof node != 'undefined' &&
           node.epoch != null &&
           node.epoch != 0 &&
           node.epoch_last_change != null &&
           node.epoch_last_change != 0 &&
           (typeof node.connected == 'undefined' || node.connected != false || node.connected != "0"))
        {
            //get seconds for the last data transfer time
            var nLastUpdate = node.epoch_last_change;

            //seconds for nodes current time
            var nNodeTicks = node.epoch;

            //seconds since last node upadet
            var nMS = nNodeTicks - nLastUpdate;

            //if the node has passed data in less g_nTimeOutSeconds seconds then its active
            if(nMS < g_nTimeOutSeconds)
            {
                bReturn = true;
            }
        }

        return bReturn;
    }

////////////////////////////////////////////////////////////////////////
/*
    Function: addUpdateLicense

    Add the Streamboost Update License to a div.

    Parameters:
      div - the div to add the eula to.

    Returns:
      nothing

    See also:
      none.
*/
////////////////////////////////////////////////////////////////////////
function addUpdateLicense(div)
{
    //load the translated license
    var strLicense =   _t("Update_License");

    //if we get back a valid string array
    if(strLicense!= undefined && strLicense != null && strLicense.length != undefined && strLicense.length > 0)
    {
        //join the array of the license and put it in the div
        $(div).append(strLicense.join(""));
    }
}

//the previous router time to be returned
var g_prevRouterTime = null;

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: getCurrentTime

        get the current router time

        Parameters:
          none.

        Returns:
          Javascript Date Object with routers time

          On fail scenario returns previous router time.
          If this fails with no previous good sample
          returns null.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    function getCurrentTime()
    {
         var strUrl = g_path.ozker+"/api/global";
         var routerTime = g_prevRouterTime;

         $.ajax({
                type: "GET",
                async: false,
                cache: true,
                url: strUrl,
                dataType: "json",
                //-------------------------------------------------------------------
                // Issue callback with result data
                success: function(data, status, xhr)
                {
                    //create the date object to replace the previous
                    routerTime = new Date();

                    //create javascript date object
                    routerTime = new Date(parseInt(data.epoch)*1000);

                    g_prevRouterTime = routerTime;
                }
         });

         return routerTime;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: calculateTime

        make the timestamp and calcualet the offsets used for router to
        computer time calculations

        Parameters:
          none.

        Returns:
          none.

        See also:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    function calculateTime()
    {
        //get the current time
        var routerTime = getCurrentTime();

        if(routerTime != null)
        {
            //convert current time to epoch
            var routerTimeEpoch = routerTime.getTime();

            //calc the global delta with computer time
            g_routerDelta = new Date().getTime() - routerTimeEpoch;

            //calc gmt delta offset
            g_routerDeltaGMT = g_routerDelta - TZOFFSETMS;
        }
        else //we got an invalid time back
        {
            //if we are here have haven't had a vaild time
            //sample at all so we need to set the deltas to
            //0 for time calculation
            g_routerDelta = 0;
            g_routerDeltaGMT = 0;
        }

        return routerTime;
    }

////////////////////////////////////////////////////////////////////////
/*
    Function: getArrayLength

    get the length of an array. only really useful for associative
    arrays as their .length member is inaccurate

    Parameters:
      array - the array to  count members in

    Returns:
      the number of elements in the array

    See also:
      nothing.
*/
////////////////////////////////////////////////////////////////////////
function getArrayLength(array)
{
    var nCount = 0;

    if(array != undefined)
    {
        for (var item in array)
        {
            nCount++;
        }
    }

    return nCount;
};

////////////////////////////////////////////////////////////////////////
/*
    Function: createImage

    Create a raphael image object . This is for remote resources and
    allows a default image that displays until the real image loads.
    On failure to load external image default remains.

    Parameters:
        pRaphael            - raphael instance to use
        strDefaultImage     - image+path to load for default
        strRealImage        - image+path to attempt to load
        nX                  - x position
        nY                  - y position
        nWidth              - Width of image
        nHeight             - Height of image

    Returns:
      image created

    See also:
      nothing.
*/
////////////////////////////////////////////////////////////////////////
function createImage(   pRaphael,
                        strDefaultImage,
                        strRealImage,
                        nX,
                        nY,
                        nWidth,
                        nHeight)
{
    //image object to return
    var imgIcon = null;

    //create the image object
    imgIcon = pRaphael.image(   strDefaultImage,
                                nX,
                                nY,
                                nWidth,
                                nHeight);

    //try to load the image
    var imgTest = new Image();

    //when the image loads call this function
    imgTest.onload = function()
    {
        //if it succeeds set our svg
        imgIcon.attr({src:strRealImage});
    };

    //set the url to load our image
    imgTest.src = strRealImage;

    return imgIcon;
}


////////////////////////////////////////////////////////////////////////
/*
    Function: post alert

    Post a formatted alert message to the given div.

    Parameters:
        strDiv - div to post the message too
        strMsg - message to post

    Returns:
      nothing.

    See also:
      nothing.
*/
////////////////////////////////////////////////////////////////////////
function postAlert( strDiv,
                    strMsg)
{
    $("<h2 style=\"text-align:center;\">"+strMsg+"</h2>").appendTo(strDiv);
}

/*
    Variable: g_FlowTable

    global instance of the translation table

    file format for load translation is json:
    {
        "flows"
        [
            {"id 1" : "flow name 1"},
            {"id 2" : "flow name 2"},
        ]
    }
*/
var g_FlowTable = null;

////////////////////////////////////////////////////////////////////////
/*
    Function: loadFlowTable

    loadTranslation from the web server

    Parameters:
      none.

    Returns:
      false = failure
      true  = success

    See also:
      nothing.
*/
////////////////////////////////////////////////////////////////////////
function loadFlowTable(callback)
{
    //1st get the local language
    var strFile = "flows_"+getLanguage()+".js";

    var strURL = g_path.strings + strFile;

    //load the table
    $.ajax({
                url: strURL,
                dataType: "jsonp",
                jsonpCallback: "jsonpCallback",
                timeout: 3000,
                success: function(data, status, request)
                {
                    //set the global language to the returned data
                    g_FlowTable = data;

                    //if the user gave us a callback tell
                    //them we are done
                    if(callback != undefined)
                    {
                        callback();
                    }
                },
                error: function(data,status)
                {
                    //we timed out, but still need to call the user
                    if(callback != undefined)
                    {
                        callback();
                    }
                }
    });
}

////////////////////////////////////////////////////////////////////////
/*
    Function: flowName

    take a flow id and return an name or unknown if it
    doesn't have that name

    Parameters:
      none.

    Returns:
      the flow name for the given id

    See also:
      nothing.
*/
////////////////////////////////////////////////////////////////////////
function flowName(strFlowID)
{
    //set the string given to return by default
    var strReturn = strFlowID;

    //if we have a table at all
    if(g_FlowTable!=undefined && g_FlowTable != null)
    {
        //to get the language string
        var strTemp = g_FlowTable.flows[strFlowID];

        //if we got back a valid string
        if(strTemp != undefined)
        {
            //set the return
            strReturn = strTemp;
        }
    }

    return strReturn;
}

function isIE8()
{
    var bReturn = false;

    if ($.browser.msie  && parseInt($.browser.version, 10) == 8)
    {
        bReturn = true;
    }

    return bReturn;
}


var browser_agents = {};

// http://caniuse.com/#feat=svg-html5
// these are minimal versions for inline svg in html5
browser_agents['MSIE'] = [/MSIE ([0-9]+\.[0-9]*)/,9];
browser_agents['Mozilla'] = [/Mozilla\/([0-9]+\.[0-9]*)/,5]; // this is actually IE 11 as it lies about who it is
browser_agents['Firefox'] = [/Firefox\/([0-9]+\.[0-9]*)/,17]; // amazingly, ff 16 should work too
browser_agents['Chrome'] = [/Chrome\/([0-9]+\.[0-9]*)/,23];
browser_agents['Safari'] = [/Safari\/([0-9]+\.[0-9]*)/,533]; // 533 is Safari 5.0.5, 5.0.4, 534 is 5.1
browser_agents['Opera'] = [/Opera ([0-9]+\.[0-9]*)/,12]; // technically, 11.6 might work
browser_agents['Opera2'] = [/Opera\/9\.80.*Version\/([0-9]+\.[0-9]*)/,12];

// for mobile browsers
// android browser 3.0 or higher
// android chrome ???? 18?


// check for streamboost supported/tested browsers
function AlertUnsupportedBrowser() {
    if (document.cookie.indexOf("unsupportedbrowser=") != -1) return;

    var ua = navigator.userAgent;
    for (var b in browser_agents) {
        var br = browser_agents[b];
        var matchret;
        if ((matchret = ua.match(br[0])) == null) continue;
        if (matchret.length > 1) {
            if (parseFloat(matchret[1]) >= br[1]) return; // too old
            else break;
        }
    }
    var msg = _t("You are using a browser that isn't capable of rendering HTML5 webpages. "+
              "To fix this download the latest version of one of the following browsers: "+
              "Chrome, Firefox, Internet Explorer, or Safari. If you choose to continue "+
              "with your current browser there is no guarantee anything on the webpage "+
              "will work.");
    alert(msg);
    // a session only cookie
    document.cookie = "unsupportedbrowser=1; path=/";
    return;
}

////////////////////////////////////////////////////////////////////////
/*
    Class: AlertInfo

    The popup window that display info about a alert when
    the mouse hovers over it.
*/
////////////////////////////////////////////////////////////////////////


function AlertInfo()
{
    var self = this;

    //background color of this windows
    this.bkgColor = "#EEEEEE";

    //the currently selected node
    //this is a graphitem class see it for details
    this.nodeCurrent = null;

    this.strMessage = "";
    //show the dialog for this node
    this.show = function(strMessage)
    {
        //set the currently selected node
        self.strMessage = strMessage;

        var x = 32;

        var nCalcHeight = $("#TL").height() + $("#ML").height()+($(window).height()-300);
        var y = Math.max(nCalcHeight, PoweredMinPos);

        $("#dialog-alert").dialog('option', 'position', [x,y]);

        //open the dialog
        $( "#dialog-alert" ).dialog( "open" );
    }

    //hide the dialog
    this.hide = function()
    {
        //show the dialog
        $( "#dialog-alert" ).dialog( "close" );
    }

    this.init = function()
    {
        //create the dialog
        $('#dialog-alert').dialog({
                    autoOpen: false,
                    height: 50,
                    width: 300,
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
        $( "#dialog-alert" ).bind( "dialogopen", function(event, ui) {
                //call the handler when this is hit
                self.onOpen(event,ui);
            });
    }

    //when thi dialog is opened set up the info
    this.onOpen = function(event,ui)
    {
        //set the node text
        $("#alert-message").text(self.strMessage);
    }

    //call our init function to set the class up
    this.init();
}


//create a global uninitialize variable for our device table
var g_DeviceTable;

//only set it to black if its undefined
//if not don't reload it!
if(typeof g_DeviceTable == "undefined")
{
     g_DeviceTable = [];
}

//the count array of id's for the different
//threads trying to load the device table json from the cloud
var g_nCount;

//list assigned id for the g_nAccount array
//incerment this for threads
var g_nLastID;

//if this is the 1st time this file is loaded
//and we are undefined
if(typeof g_nCount == "undefined")
{
    //init the variables.
     g_nCount = [];
     g_nLastID = 0;
}

////////////////////////////////////////////////////////////////////////
/*
    Function: loadDeviceTable

    loadTranslation from the web server

    Parameters:
      none.

    Returns:
      false = failure
      true  = success

    See also:
      nothing.
*/
////////////////////////////////////////////////////////////////////////
function loadDeviceTable(callback)
{
    //1st get the local language
    var strFile = "devices_icons.js";

    var strURL = g_path.strings + strFile;

    //if we don't have our lock global
    if(typeof window.bInFlight == 'undefined')
    {
        window.bInFlight = false;
    }

    //if we don't have our calback global
    if(typeof window.lsCallbacks == 'undefined')
    {
        window.lsCallbacks = [];
    }

    //put the callback in the list even if its null/undefined
    window.lsCallbacks.push(callback);

    //if the callback isn't in flight
    if(window.bInFlight == false)
    {
        //arm ourselves
        window.bInFlight = true;

        //load the table
        $.ajax({
                    url: strURL,
                    dataType: "jsonp",
                    jsonpCallback: "jsonpDevicesCallback",
                    timeout: 3000,
                    success: function(data, status, request)
                    {
                        //get a local id variable to for
                        //our thread. Incerment so we wont
                        //conflict with other threads
                        var id = g_nLastID++;

                        //set the count to 0
                        g_nCount[id] = 0;

                        //count the # of devices
                        for(var key in data.devices)
                        {
                            g_nCount[id]++;
                        }

                        //copy all the keys with th cloud path to the g_strIcons array
                        //this keep changes to a minimum
                        for(var key in data.devices)
                        {
                            //create our image objecw
                            var imageObj = new Image();

                            //get the icon name
                            var strIconName = data.devices[key];

                            //icon cloud path
                            var strCloud = g_path.cloud+strIconName;

                            //set the key into our global strings list
                            g_strIcons[key] = strCloud;

                            //callback for image load error
                            imageObj.onerror = function(myKey,me){return function()
                                {
                                //get the icon name
                                var strIconName = data.devices[myKey];

                                //icon local path
                                var strLocal = g_path.images+strIconName;

                                //we failed check for local file
                                $.ajax(
                                {
                                    url: strLocal,
                                    async: false,
                                    success: function(data, status, request)
                                    {
                                       //if we succeed this is a local resource so send it
                                        g_strIcons[myKey] = strLocal;
                                    },
                                    error: function(err)
                                    {
                                        //if everything fails this is our image
                                        g_strIcons[myKey] = g_path.images+"UnknownDevice.png";
                                    }
                                });

                                //decrement the device count
                                g_nCount[id]--;

                                //if this is the last one
                                if(g_nCount[id] <= 0)
                                {
                                    //if the user gave us a callback tell them we are done
                                    for(var i in window.lsCallbacks)
                                    {
                                        var cb = window.lsCallbacks[i];

                                        //if we have a valid function
                                        if(typeof cb == 'function')
                                        {
                                            cb();
                                        }
                                    }

                                    //reset our globals
                                    window.bInFlight = false;
                                    window.lsCallbacks = [];
                                }
                            };}(key,imageObj);

                            //callback for image load success
                            imageObj.onload = function(myKey,me){return function()
                                {
                                //get icon name
                                var strIconName = data.devices[myKey];

                                //icon cloud path
                                var strCloud = g_path.cloud+strIconName;

                                //set the icon key to this name & path
                                g_strIcons[myKey] = strCloud;

                                //decrement our count
                                g_nCount[id]--;

                                //if this is the last one
                                if(g_nCount[id] <= 0)
                                {
                                    //if the user gave us a callback tell
                                    //them we are done
                                    //if the user gave us a callback tell them we are done
                                    for(var i in window.lsCallbacks)
                                    {
                                        var cb = window.lsCallbacks[i];

                                        //if we have a valid function
                                        if(typeof cb == 'function')
                                        {
                                            cb();
                                        }
                                    }

                                    //reset our globals
                                    window.bInFlight = false;
                                    window.lsCallbacks = [];
                                }
                            };}(key,imageObj);

                            //try to load the cloud image
                            imageObj.src=g_path.cloud+data.devices[key];
                        }

                    },
                    error: function(data,status)
                    {
                        //set some default icon name/paths for local if this fails to retrieve
                        g_strIcons["Internet"] = g_path.images+"internet-globe-earth-terra.png";
                        g_strIcons["Router"] = g_path.images+"Killer.png";
                        g_strIcons["Unknown"] = g_path.images+"UnknownDevice.png";

                        //we timed out, but still need to call the user
                        //if the user gave us a callback tell them we are done
                        for(var i in window.lsCallbacks)
                        {
                            var cb = window.lsCallbacks[i];

                            //if we have a valid function
                            if(typeof cb == 'function')
                            {
                                cb();
                            }
                        }

                        //reset our globals
                        window.bInFlight = false;
                        window.lsCallbacks = [];
                    }
        });
    }
}

//returned bytes if true / bits if false
function maxUp(bytes)
{
    var rt = g_nMaxUpload;

    if(bytes == true)
    {
        rt/=8;
    }

    return rt;
}

//returned bytes if true / bits if false
function maxDn(bytes)
{
    var rt = g_nMaxDownload;

    if(bytes == true)
    {
        rt/=8;
    }

    return rt;
}


/////////////////////////////////////////////////////////////
/*
    in chrome version 34.0.1847.116 SVG elements no longer
    render correctly.

    the 1st SVG element with an opacity of 0.0 breaks all
    future renders and makes the rest behave flakely

    the following variables and functions are to detect
    and remedy the situation
*/
/////////////////////////////////////////////////////////////

// these are the show and hide values for svg elements
var eShow = 1.0;
var eHide = 0.0;

//if this is true we need to do the chrome hack for bad SVG opacity
var g_bDoChromeHack = false;

//set the show hide opacity state if we have the bad version
//of chrome
function setChromeState()
{
    //get version of browser
    var strVersion = window.navigator.appVersion;

    //are we the bad guy?
    if(strVersion.indexOf("Chrome/34.0.") != -1)
    {
        eHide = 1.1;
        g_bDoChromeHack = true;
    }
}

//call the function with an svg element
//to fix alpha state for chrome bad versions
function fixAlpha(svg,state)
{
    if(g_bDoChromeHack == true)
    {
        if(typeof svg.node != 'undefined')
        {
            if(state == eHide)
            {
                $(svg.node).hide();
            }
            else
            {
                $(svg.node).show();
            }
        }
    }
}

//do the chrome fix on page load
$(document).ready(function()
{
    setChromeState();
})