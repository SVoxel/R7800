# Microsoft Developer Studio Project File - Name="PhEmbedCom" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=PhEmbedCom - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PhEmbedCom.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PhEmbedCom.mak" CFG="PhEmbedCom - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PhEmbedCom - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PhEmbedCom - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PhEmbedCom - Win32 Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PhEmbedCom - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PhEmbedCom - Win32 Unicode Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PhEmbedCom - Win32 Unicode Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PhEmbedCom - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PhEmbedCom - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\Debug\PhEmbedCom.dll
InputPath=.\Debug\PhEmbedCom.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\DebugU
TargetPath=.\DebugU\PhEmbedCom.dll
InputPath=.\DebugU\PhEmbedCom.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinSize"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinSize
TargetPath=.\ReleaseMinSize\PhEmbedCom.dll
InputPath=.\ReleaseMinSize\PhEmbedCom.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=.\ReleaseMinDependency\PhEmbedCom.dll
InputPath=.\ReleaseMinDependency\PhEmbedCom.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinSize"
# PROP Intermediate_Dir "ReleaseUMinSize"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinSize
TargetPath=.\ReleaseUMinSize\PhEmbedCom.dll
InputPath=.\ReleaseUMinSize\PhEmbedCom.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinDependency"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /entry:"" /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinDependency
TargetPath=.\ReleaseUMinDependency\PhEmbedCom.dll
InputPath=.\ReleaseUMinDependency\PhEmbedCom.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PhEmbedCom___Win32_Release"
# PROP BASE Intermediate_Dir "PhEmbedCom___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PhEmbedCom___Win32_Release"
# PROP Intermediate_Dir "PhEmbedCom___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\PhEmbedCom___Win32_Release
TargetPath=.\PhEmbedCom___Win32_Release\PhEmbedCom.dll
InputPath=.\PhEmbedCom___Win32_Release\PhEmbedCom.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "PhEmbedCom - Win32 Debug"
# Name "PhEmbedCom - Win32 Unicode Debug"
# Name "PhEmbedCom - Win32 Release MinSize"
# Name "PhEmbedCom - Win32 Release MinDependency"
# Name "PhEmbedCom - Win32 Unicode Release MinSize"
# Name "PhEmbedCom - Win32 Unicode Release MinDependency"
# Name "PhEmbedCom - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\PhDDNS.cpp
# End Source File
# Begin Source File

SOURCE=.\PhEmbedCom.cpp
# End Source File
# Begin Source File

SOURCE=.\PhEmbedCom.def
# End Source File
# Begin Source File

SOURCE=.\PhEmbedCom.idl
# ADD MTL /tlb ".\PhEmbedCom.tlb" /h "PhEmbedCom.h" /iid "PhEmbedCom_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\PhEmbedCom.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\PhDDNS.h
# End Source File
# Begin Source File

SOURCE=.\PhEmbedComCP.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\PhDDNS.rgs
# End Source File
# End Group
# Begin Group "common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\common\base64.cpp

!IF  "$(CFG)" == "PhEmbedCom - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\common\bitstream.cpp

!IF  "$(CFG)" == "PhEmbedCom - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\common\bitstream.h
# End Source File
# Begin Source File

SOURCE=..\src\common\blowfish.cpp

!IF  "$(CFG)" == "PhEmbedCom - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\common\blowfish.h
# End Source File
# Begin Source File

SOURCE=..\src\common\coder.cpp

!IF  "$(CFG)" == "PhEmbedCom - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\common\coder.h
# End Source File
# Begin Source File

SOURCE=..\src\common\generate.cpp

!IF  "$(CFG)" == "PhEmbedCom - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\common\generate.h
# End Source File
# Begin Source File

SOURCE=..\src\common\global.h
# End Source File
# Begin Source File

SOURCE=..\src\common\log.cpp

!IF  "$(CFG)" == "PhEmbedCom - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\common\log.h
# End Source File
# Begin Source File

SOURCE=..\src\common\lutil.h
# End Source File
# Begin Source File

SOURCE=..\src\common\md5.cpp

!IF  "$(CFG)" == "PhEmbedCom - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\common\md5.h
# End Source File
# Begin Source File

SOURCE=..\src\common\PHSocket.cpp

!IF  "$(CFG)" == "PhEmbedCom - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\common\PHSocket.h
# End Source File
# End Group
# Begin Group "ph"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\PHGlobal.cpp

!IF  "$(CFG)" == "PhEmbedCom - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\PHGlobal.h
# End Source File
# Begin Source File

SOURCE=..\src\phupdate.cpp

!IF  "$(CFG)" == "PhEmbedCom - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinSize"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Unicode Release MinDependency"

!ELSEIF  "$(CFG)" == "PhEmbedCom - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\phupdate.h
# End Source File
# End Group
# End Target
# End Project
