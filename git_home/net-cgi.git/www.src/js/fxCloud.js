/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */

/*
    Variable: g_strImagePath

    path to load the images from

*/
var g_strImagePath = "";

/*
    Variable: g_strFlowPath

    path to load the flow information from

*/
var g_strFlowPath = "";

/*
    Variable: g_strNodePath

    path to load the node information from

*/
var g_strNodePath = "";

////////////////////////////////////////////////////////////////////////
/*
    Class: NodeInformation

    Information returned on a node
*/
////////////////////////////////////////////////////////////////////////
function NodeInformation
{
    var strName = "";
    var strDescription = "";
    var strIcon = "";
}

////////////////////////////////////////////////////////////////////////
/*
    Class: FlowInformation

    Information returned on a flow
*/
////////////////////////////////////////////////////////////////////////
function FlowInformation
{
    var strName = "";
    var strDescription = "";
    var strIcon = "";
}

////////////////////////////////////////////////////////////////////////
/*
    Function: getImage

    get an image from the cloud / web server

    Parameters:
      strImage - image to load (just the image name "computer.png")
      strPath  - path to load from. (if not defined use default path)

    Returns:
      image = succuss
      null = undefined

    See also:
      nothing.
*/
////////////////////////////////////////////////////////////////////////
function getImage(  strImage,
                    strPath)
{
    var image = null;

    return image;
}

////////////////////////////////////////////////////////////////////////
/*
    Function: getPathname

    get an image from the cloud / web server

    Parameters:
      strImage - image to get path for

    Returns:
      image path + filename ("/images/theme1/computer.png")

    See also:
      nothing.
*/
////////////////////////////////////////////////////////////////////////
function getPathname(strImage)
{
    var strImagePath = g_strImagePath + strImage;

    return strImagePath;
}

////////////////////////////////////////////////////////////////////////
/*
    Function: getFlowInfo

    get flow information

    Parameters:
      strFlowID - flow id to get info about (given in api/flows)
      strPath   - path to load from. (if not defined use default path)

    Returns:
      flowinfo - succuss
      null     - fail

    See also:
      nothing.
*/
////////////////////////////////////////////////////////////////////////
function getFlowInfo(   strFlowID,
                        strPath)
{

}

////////////////////////////////////////////////////////////////////////
/*
    Function: getNodeInfo

    get node information

    Parameters:
      strNodeID - nod id to get info about (given in api/nodes)
      strPath   - path to load from. (if not defined use default path)

    Returns:
      nodeinfo - succuss
      null     - fail

    See also:
      nothing.
*/
////////////////////////////////////////////////////////////////////////
function getNodeInfo(   strNodeID,
                        strPath)
{

}
