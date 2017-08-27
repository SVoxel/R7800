/*
  Copyright (c) 2013 Qualcomm Atheros, Inc..

  All Rights Reserved.
  Qualcomm Atheros Confidential and Proprietary
*/


//=======================================================================
//
//
//
//=======================================================================
function CModuleBase()
{
    //-------------------------------------------------------------------
    // Derive from Subscriber and Control Monitor
    this.Base               = CSubscriber;
    this.Base();
    this.Base2              = CControlMonitor;
    this.Base2();

    var self = this;

    //=======================================================================
    //
    // Data
    //
    //=======================================================================
    this.m_kPages           = [];
    this.m_sName            = "Unknown Module";
    this.m_sIcon            = "";
    this.m_sIconHover       = "";
    this.m_sIconSelected    = "";
    this.m_sHTML            = "";
    this.m_bModified        = false;

    this.m_pApp             = null;

    //=======================================================================
    //
    // Child Pages
    //
    //======================================================================
    this.AddPage            = function(_kPage)
    {
        _kPage.m_kParent = this;
        this.m_kPages.push(_kPage);
    }

    //=======================================================================
    //
    // Init/UnInit
    //
    //=======================================================================
    this.Init               = function(mainApp) { this.m_pApp = mainApp; }
    this.UnInit             = function() { }

    //=======================================================================
    //
    //
    //
    //=======================================================================
    this.isValid = function()
    {
        var bReturn = false;

        for(var i=0;i<this.m_kPages.length;i++)
        {
            bReturn = this.m_kPages[i].isValid();

            //if we get 1 false break and return it
            if(bReturn == false)
            {
                break;
            }
        }

        return bReturn;
    }


    this.strValName = "";
    this.strValue = "";
    this.strError = "";

    //=======================================================================
    //
    //
    //
    //=======================================================================
    this.setLastError = function(strValName,strValue,strError)
    {

        //save off last error
        self.strValName = strValName;
        self.strValue = strValue;
        self.strError = strError;
    }

    this.displayError = function()
    {

        var bReturn = false;

        if(this.m_kPages.length > 0)
        {
            for(var i=0;i<this.m_kPages.length;i++)
            {
                //if this is the invalid page
                if(this.m_kPages[i].isValid() == false)
                {
                    //show the error
                    this.m_kPages[i].displayError();

                    break;
                }
            }
        }
        else
        {
            //if we get a non-specific error don't freak out the user
            if(self.strValName!="" || self.strValue!="")
            {
                $("#error-dialog-value").text(self.strValName+" : "+self.strValue);
                $("#error-dialog-errmsg").text("Error : "+self.strError);

                $( "#save-error-dialog" ).dialog({
                    modal: true,
                        zIndex : 65536,
                    buttons: {
                        Ok: function() {
                            $( this ).dialog( "close" );
                        }
                    }
                });
            }
        }
    }

    //=======================================================================
    //
    //
    //
    //=======================================================================
    this.OnSave             = function()
    {
        for(i=0;i<this.m_kPages.length;i++)
        {
            this.m_kPages[i].OnSave();
            this.m_kPages[i].SetModified(false);
        }

        this.SetModified(false);
    }

    //=======================================================================
    //
    //
    //
    //=======================================================================
    this.OnCancel           = function()
    {
        g_kRouter.Revert();
        this.SetModified(false);

        for(i=0;i<this.m_kPages.length;i++)
            this.m_kPages[i].SetModified(false);
    }

    //=======================================================================
    //
    //
    //
    //=======================================================================
    this.IsModified         = function() {return this.m_bModified;}

    //=======================================================================
    //
    //
    //
    //=======================================================================
    this.SetModified        = function(_bModified)
    {
        if (this.m_bModified != _bModified)
        {
            this.m_bModified = _bModified;
            g_kSavePanel.Show(_bModified);
            this.OnModified(_bModified);
        }
    }

    //=======================================================================
    //
    // Callbacks
    //
    //=======================================================================
    this.OnActivated        = function(_bActive) { }
    this.OnModified         = function(_bModified) { }

    //=======================================================================
    //
    //
    //
    //=======================================================================
    this.SetActive = function(_bActive)
    {
        if (_bActive)
            g_kSavePanel.SetActiveModule(this);

        for(i=0;i<this.m_kPages.length;i++)
        {
            this.m_kPages[i].OnActivated(_bActive);
        }

        this.OnActivated(_bActive);
    }

this.strDivs = [];

this.addDiv = function(strDivName)
{
    self.strDivs.push(strDivName);
}

this.getDivs = function()
{
    return self.strDivs;
}

this.getWidth = function()
{
    var nReturn = 1000000;

    //lop over and return the lowest value
    for(var i=0;i<self.strDivs.length;i++)
    {
        var nNew = $(self.strDivs[i]).width();

        if(nNew < nReturn)
        {
            nReturn = nNew;
        }
    }

    return nReturn;
}

//set the width to all the subdivs
this.setWidth = function(nWidth)
{
    for(var i=0;i<self.strDivs.length;i++)
    {
        $(self.strDivs[i]).width(nWidth);
    }
}

    //=======================================================================
    //
    // Accessors
    //
    //=======================================================================
    this.GetIcon            = function() { return this.m_sIcon; }
    this.GetName            = function() { return this.m_sName; }
    this.GetPages           = function() { return this.m_kPages; }
    this.GetIconHover       = function() { return this.m_sIconHover; }
    this.GetIconSelected    = function() { return this.m_sIconSelected; }
    this.GetHTML            = function() { return this.m_sHTML; }
}
