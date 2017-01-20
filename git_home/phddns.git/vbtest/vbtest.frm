VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   4860
   ClientLeft      =   120
   ClientTop       =   450
   ClientWidth     =   8025
   LinkTopic       =   "Form1"
   ScaleHeight     =   4860
   ScaleWidth      =   8025
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox UserInfoBox 
      Height          =   1935
      Left            =   4080
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   4
      Top             =   2760
      Width           =   3735
   End
   Begin VB.TextBox DomainInfoBox 
      Height          =   1935
      Left            =   120
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   3
      Top             =   2760
      Width           =   3735
   End
   Begin VB.CommandButton Stop 
      Caption         =   "Stop"
      Height          =   495
      Left            =   2520
      TabIndex        =   2
      Top             =   240
      Width           =   1695
   End
   Begin VB.Timer NextTimer 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   6480
      Top             =   360
   End
   Begin VB.CommandButton Run 
      Caption         =   "Run"
      Height          =   495
      Left            =   240
      TabIndex        =   1
      Top             =   240
      Width           =   1695
   End
   Begin VB.ListBox List1 
      Height          =   1230
      Left            =   120
      TabIndex        =   0
      Top             =   1200
      Width           =   7695
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim WithEvents PhDDNS As PHEMBEDCOMLib.PhDDNS
Attribute PhDDNS.VB_VarHelpID = -1

Private Sub Form_Load()

End Sub

Private Sub NextTimer_Timer()
    NextInSeconds = PhDDNS.Step()
    NextTimer.Interval = NextInSeconds * 1000
    NextTimer.Enabled = True
End Sub

Private Sub PhDDNS_DomainInfoCome(ByVal DomainInfo As String)
    DomainInfoBox.Text = DomainInfo
End Sub

Private Sub PhDDNS_DomainRegistered(ByVal DomainName As String)
    'MsgBox DomainName
End Sub

Private Sub PhDDNS_StatusChanged(ByVal Status As String, ByVal Data As String)
    List1.AddItem (Status + Data)

End Sub

Private Sub PhDDNS_UserInfoCome(ByVal UserInfo As String)
    UserInfoBox.Text = UserInfo
End Sub

Private Sub Run_Click()
    Set PhDDNS = New PHEMBEDCOMLib.PhDDNS
    ret = PhDDNS.Initialize("phddns60.oray.net", "skyvense12", "checkin", 645699397, 503842398)
    NextInSeconds = PhDDNS.Step()
    NextTimer.Interval = NextInSeconds * 1000
    NextTimer.Enabled = True
End Sub

Private Sub Stop_Click()
    NextTimer.Enabled = False
    PhDDNS.Stop
    Set PhDDNS = Nothing
End Sub
