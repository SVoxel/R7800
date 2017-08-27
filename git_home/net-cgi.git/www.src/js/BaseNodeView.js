/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */

////////////////////////////////////////////////////////////////////////
/*
    Class: BaseNodeView

    Version:   1.0

    BaseNodeView
*/
////////////////////////////////////////////////////////////////////////

function BaseNodeView()
{
    /*
        Variable: self

        Easy access to the this of the class, to keep it separate
        from other this scopes
    */
    var self = this;

    /*
        Variable: Base
        Derive from Subscriber
    */
    this.Base               = CSubscriber;
    this.Base();

    /*
        Variable: Base2
        Derive from Control Monitor
    */
    this.Base2              = CControlMonitor;
    this.Base2();


    /*
        Variable: m_sName
        English readable name of the view
    */
    this.m_sName            = "Unknown Module";

    /*
        Variable: m_bModified
        set true for state based saving.
    */
    this.m_bModified        = false;

    /*
        Variable: m_pApp
        Reference back to the main application
    */
    this.m_pApp             = null;


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: init

        Initialize the class. Called when the mynetwork module loads you.

        Parameters:
            mainApp  - Reference back to the main application

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.init = function(mainApp)
    {
        self.m_pApp = mainApp;
    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: kill

        Kill the class. When this is called do anything you need to and
        free everything you have.

        Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.kill = function()
    {

    }

    ////////////////////////////////////////////////////////////////////////
    /*
        Function: show

        When this is called you are supposed to show the node that is given
        to this class. It should open its display and start any necessary
        timers to run.

        Parameters:
            node  - the which you are supposed to tdisplay

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.show = function(node)
    {

    }


    ////////////////////////////////////////////////////////////////////////
    /*
        Function: hide

        Turn off the display and shut down any unnecessary timers to
        relinquish control back to the main program.

        Parameters:
            none.

        Returns:
            nothing.

        See also:
            nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    this.hide = function()
    {

    }
}
