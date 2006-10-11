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
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ACCOUNTS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(TUSCANY_SCACPP)\include" /I "$(TUSCANY_SCACPP)\extensions\cpp\include" /I "$(TUSCANY_SDOCPP)\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ACCOUNTS_EXPORTS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib tuscany_sca.lib tuscany_sca_cpp.lib /nologo /dll /machine:I386 /out:"Release/Account.dll" /libpath:"$(TUSCANY_SCACPP)/extensions/cpp/lib" /libpath:"$(TUSCANY_SCACPP)/lib" /libpath:"$(TUSCANY_SDOCPP)/lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=call ..\..\..\..\..\BigBank\deploy.bat ..\..\..\..\..\BigBank Release
# End Special Build Tool

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(TUSCANY_SCACPP)\include" /I "$(TUSCANY_SCACPP)\extensions\cpp\include" /I "$(TUSCANY_SDOCPP)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ACCOUNTS_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib tuscany_sca.lib tuscany_sca_cpp.lib /nologo /dll /debug /machine:I386 /out:"Debug/Account.dll" /pdbtype:sept /libpath:"$(TUSCANY_SCACPP)/extensions/cpp/lib" /libpath:"$(TUSCANY_SCACPP)/lib" /libpath:"$(TUSCANY_SDOCPP)/lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=call ..\..\..\..\..\BigBank\deploy.bat ..\..\..\..\..\BigBank Debug
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Accounts - Win32 Release"
# Name "Accounts - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountDataServiceImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountDataServiceImpl_AccountDataService_Proxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountDataServiceImpl_AccountDataService_Wrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountServiceImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountServiceImpl_accountDataService_Proxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountServiceImpl_AccountService_Proxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountServiceImpl_AccountService_Wrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountServiceImpl_stockQuoteService_Proxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\StockQuoteServiceImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\StockQuoteServiceImpl_StockQuoteService_Proxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\StockQuoteServiceImpl_StockQuoteService_Wrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\StockQuoteServiceImpl_webService_Proxy.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountDataService.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountDataServiceImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountDataServiceImpl_AccountDataService_Proxy.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountDataServiceImpl_AccountDataService_Wrapper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountService.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountServiceImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountServiceImpl_accountDataService_Proxy.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountServiceImpl_AccountService_Proxy.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountServiceImpl_AccountService_Wrapper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountServiceImpl_stockQuoteService_Proxy.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\StockQuoteService.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\StockQuoteServiceImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\StockQuoteServiceImpl_StockQuoteService_Proxy.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\StockQuoteServiceImpl_StockQuoteService_Wrapper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\StockQuoteServiceImpl_webService_Proxy.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\StockQuoteWebService.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountDataServiceImpl.componentType
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountService.wsdl
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountServiceImpl.componentType
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\bigbank.account.composite
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\StockQuoteServiceImpl.componentType
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\StockQuoteTypes.xsd
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\BigBank\bigbank.account\StockQuoteWebService.wsdl
# End Source File
# End Group
# End Target
# End Project
