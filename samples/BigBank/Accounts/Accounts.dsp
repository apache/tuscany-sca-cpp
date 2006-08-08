# Microsoft Developer Studio Project File - Name="Accounts" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Accounts - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Accounts.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Accounts.mak" CFG="Accounts - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Accounts - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Accounts - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Accounts - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ACCOUNTS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ACCOUNTS_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "Accounts - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ACCOUNTS_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(TUSCANY_SCACPP)\include" /I "$(TUSCANY_SDOCPP)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ACCOUNTS_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sca.lib tuscany_sdo.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"$(TUSCANY_SCACPP)/lib" /libpath:"$(TUSCANY_SDOCPP)/lib"

!ENDIF 

# Begin Target

# Name "Accounts - Win32 Release"
# Name "Accounts - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AccountDataServiceImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\AccountDataServiceImpl_AccountDataService_Proxy.cpp
# End Source File
# Begin Source File

SOURCE=.\AccountDataServiceImpl_AccountDataService_Wrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\AccountServiceImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\AccountServiceImpl_AccountDataService_Proxy.cpp
# End Source File
# Begin Source File

SOURCE=.\AccountServiceImpl_AccountDataService_Proxy.h
# End Source File
# Begin Source File

SOURCE=.\AccountServiceImpl_AccountService_Proxy.cpp
# End Source File
# Begin Source File

SOURCE=.\AccountServiceImpl_AccountService_Wrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\AccountServiceImpl_stockQuote_Proxy.cpp
# End Source File
# Begin Source File

SOURCE=.\StockQuoteServiceImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\StockQuoteServiceImpl_StockQuoteExternal_Proxy.cpp
# End Source File
# Begin Source File

SOURCE=.\StockQuoteServiceImpl_StockQuoteService_Proxy.cpp
# End Source File
# Begin Source File

SOURCE=.\StockQuoteServiceImpl_StockQuoteService_Wrapper.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AccountDataService.h
# End Source File
# Begin Source File

SOURCE=.\AccountDataServiceImpl.h
# End Source File
# Begin Source File

SOURCE=.\AccountDataServiceImpl_AccountDataService_Proxy.h
# End Source File
# Begin Source File

SOURCE=.\AccountDataServiceImpl_AccountDataService_Wrapper.h
# End Source File
# Begin Source File

SOURCE=.\AccountService.h
# End Source File
# Begin Source File

SOURCE=.\AccountServiceImpl.h
# End Source File
# Begin Source File

SOURCE=.\AccountServiceImpl_AccountService_Proxy.h
# End Source File
# Begin Source File

SOURCE=.\AccountServiceImpl_AccountService_Wrapper.h
# End Source File
# Begin Source File

SOURCE=.\AccountServiceImpl_stockQuote_Proxy.h
# End Source File
# Begin Source File

SOURCE=.\StockQuoteExternalService.h
# End Source File
# Begin Source File

SOURCE=.\StockQuoteService.h
# End Source File
# Begin Source File

SOURCE=.\StockQuoteServiceImpl.h
# End Source File
# Begin Source File

SOURCE=.\StockQuoteServiceImpl_StockQuoteExternal_Proxy.h
# End Source File
# Begin Source File

SOURCE=.\StockQuoteServiceImpl_StockQuoteService_Proxy.h
# End Source File
# Begin Source File

SOURCE=.\StockQuoteServiceImpl_StockQuoteService_Wrapper.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\WSAccountClient\services.xml
# End Source File
# Begin Source File

SOURCE="..\WSAccountClient\Tuscany-model.config"
# End Source File
# End Group
# Begin Source File

SOURCE=.\AccountDataServiceImpl.componentType
# End Source File
# Begin Source File

SOURCE=.\AccountService.wsdl
# End Source File
# Begin Source File

SOURCE=.\AccountServiceImpl.componenttype
# End Source File
# Begin Source File

SOURCE=.\sca.composite
# End Source File
# Begin Source File

SOURCE=.\sca.subsystem
# End Source File
# Begin Source File

SOURCE=.\StockQuoteService.wsdl
# End Source File
# Begin Source File

SOURCE=.\StockQuoteServiceImpl.componentType
# End Source File
# End Target
# End Project
