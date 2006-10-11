# Microsoft Developer Studio Generated NMAKE File, Based on Accounts.dsp
!IF "$(CFG)" == ""
CFG=Accounts - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Accounts - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Accounts - Win32 Release" && "$(CFG)" != "Accounts - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Accounts - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Account.dll"

!ELSE 

ALL : "AccountClient - Win32 Release" "$(OUTDIR)\Account.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"AccountClient - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AccountDataServiceImpl.obj"
	-@erase "$(INTDIR)\AccountDataServiceImpl_AccountDataService_Proxy.obj"
	-@erase "$(INTDIR)\AccountDataServiceImpl_AccountDataService_Wrapper.obj"
	-@erase "$(INTDIR)\AccountServiceImpl.obj"
	-@erase "$(INTDIR)\AccountServiceImpl_accountDataService_Proxy.obj"
	-@erase "$(INTDIR)\AccountServiceImpl_AccountService_Proxy.obj"
	-@erase "$(INTDIR)\AccountServiceImpl_AccountService_Wrapper.obj"
	-@erase "$(INTDIR)\AccountServiceImpl_stockQuoteService_Proxy.obj"
	-@erase "$(INTDIR)\StockQuoteServiceImpl.obj"
	-@erase "$(INTDIR)\StockQuoteServiceImpl_StockQuoteService_Proxy.obj"
	-@erase "$(INTDIR)\StockQuoteServiceImpl_StockQuoteService_Wrapper.obj"
	-@erase "$(INTDIR)\StockQuoteServiceImpl_webService_Proxy.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Account.dll"
	-@erase "$(OUTDIR)\Account.exp"
	-@erase "$(OUTDIR)\Account.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "$(TUSCANY_SCACPP)\include" /I "$(TUSCANY_SCACPP)\extensions\cpp\include" /I "$(TUSCANY_SDOCPP)\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ACCOUNTS_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Accounts.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib tuscany_sca.lib tuscany_sca_cpp.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\Account.pdb" /machine:I386 /out:"$(OUTDIR)\Account.dll" /implib:"$(OUTDIR)\Account.lib" /libpath:"$(TUSCANY_SCACPP)/extensions/cpp/lib" /libpath:"$(TUSCANY_SCACPP)/lib" /libpath:"$(TUSCANY_SDOCPP)/lib" 
LINK32_OBJS= \
	"$(INTDIR)\AccountDataServiceImpl.obj" \
	"$(INTDIR)\AccountDataServiceImpl_AccountDataService_Proxy.obj" \
	"$(INTDIR)\AccountDataServiceImpl_AccountDataService_Wrapper.obj" \
	"$(INTDIR)\AccountServiceImpl.obj" \
	"$(INTDIR)\AccountServiceImpl_accountDataService_Proxy.obj" \
	"$(INTDIR)\AccountServiceImpl_AccountService_Proxy.obj" \
	"$(INTDIR)\AccountServiceImpl_AccountService_Wrapper.obj" \
	"$(INTDIR)\AccountServiceImpl_stockQuoteService_Proxy.obj" \
	"$(INTDIR)\StockQuoteServiceImpl.obj" \
	"$(INTDIR)\StockQuoteServiceImpl_StockQuoteService_Proxy.obj" \
	"$(INTDIR)\StockQuoteServiceImpl_StockQuoteService_Wrapper.obj" \
	"$(INTDIR)\StockQuoteServiceImpl_webService_Proxy.obj"

"$(OUTDIR)\Account.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "AccountClient - Win32 Release" "$(OUTDIR)\Account.dll"
   call ..\..\..\..\..\BigBank\deploy.bat ..\..\..\..\..\BigBank Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "Accounts - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Account.dll"

!ELSE 

ALL : "AccountClient - Win32 Debug" "$(OUTDIR)\Account.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"AccountClient - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AccountDataServiceImpl.obj"
	-@erase "$(INTDIR)\AccountDataServiceImpl_AccountDataService_Proxy.obj"
	-@erase "$(INTDIR)\AccountDataServiceImpl_AccountDataService_Wrapper.obj"
	-@erase "$(INTDIR)\AccountServiceImpl.obj"
	-@erase "$(INTDIR)\AccountServiceImpl_accountDataService_Proxy.obj"
	-@erase "$(INTDIR)\AccountServiceImpl_AccountService_Proxy.obj"
	-@erase "$(INTDIR)\AccountServiceImpl_AccountService_Wrapper.obj"
	-@erase "$(INTDIR)\AccountServiceImpl_stockQuoteService_Proxy.obj"
	-@erase "$(INTDIR)\StockQuoteServiceImpl.obj"
	-@erase "$(INTDIR)\StockQuoteServiceImpl_StockQuoteService_Proxy.obj"
	-@erase "$(INTDIR)\StockQuoteServiceImpl_StockQuoteService_Wrapper.obj"
	-@erase "$(INTDIR)\StockQuoteServiceImpl_webService_Proxy.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Account.dll"
	-@erase "$(OUTDIR)\Account.exp"
	-@erase "$(OUTDIR)\Account.ilk"
	-@erase "$(OUTDIR)\Account.lib"
	-@erase "$(OUTDIR)\Account.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(TUSCANY_SCACPP)\include" /I "$(TUSCANY_SCACPP)\extensions\cpp\include" /I "$(TUSCANY_SDOCPP)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ACCOUNTS_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Accounts.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib tuscany_sca.lib tuscany_sca_cpp.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\Account.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Account.dll" /implib:"$(OUTDIR)\Account.lib" /pdbtype:sept /libpath:"$(TUSCANY_SCACPP)/extensions/cpp/lib" /libpath:"$(TUSCANY_SCACPP)/lib" /libpath:"$(TUSCANY_SDOCPP)/lib" 
LINK32_OBJS= \
	"$(INTDIR)\AccountDataServiceImpl.obj" \
	"$(INTDIR)\AccountDataServiceImpl_AccountDataService_Proxy.obj" \
	"$(INTDIR)\AccountDataServiceImpl_AccountDataService_Wrapper.obj" \
	"$(INTDIR)\AccountServiceImpl.obj" \
	"$(INTDIR)\AccountServiceImpl_accountDataService_Proxy.obj" \
	"$(INTDIR)\AccountServiceImpl_AccountService_Proxy.obj" \
	"$(INTDIR)\AccountServiceImpl_AccountService_Wrapper.obj" \
	"$(INTDIR)\AccountServiceImpl_stockQuoteService_Proxy.obj" \
	"$(INTDIR)\StockQuoteServiceImpl.obj" \
	"$(INTDIR)\StockQuoteServiceImpl_StockQuoteService_Proxy.obj" \
	"$(INTDIR)\StockQuoteServiceImpl_StockQuoteService_Wrapper.obj" \
	"$(INTDIR)\StockQuoteServiceImpl_webService_Proxy.obj"

"$(OUTDIR)\Account.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "AccountClient - Win32 Debug" "$(OUTDIR)\Account.dll"
   call ..\..\..\..\..\BigBank\deploy.bat ..\..\..\..\..\BigBank Debug
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Accounts.dep")
!INCLUDE "Accounts.dep"
!ELSE 
!MESSAGE Warning: cannot find "Accounts.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Accounts - Win32 Release" || "$(CFG)" == "Accounts - Win32 Debug"
SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountDataServiceImpl.cpp

"$(INTDIR)\AccountDataServiceImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountDataServiceImpl_AccountDataService_Proxy.cpp

"$(INTDIR)\AccountDataServiceImpl_AccountDataService_Proxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountDataServiceImpl_AccountDataService_Wrapper.cpp

"$(INTDIR)\AccountDataServiceImpl_AccountDataService_Wrapper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountServiceImpl.cpp

"$(INTDIR)\AccountServiceImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountServiceImpl_accountDataService_Proxy.cpp

"$(INTDIR)\AccountServiceImpl_accountDataService_Proxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountServiceImpl_AccountService_Proxy.cpp

"$(INTDIR)\AccountServiceImpl_AccountService_Proxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountServiceImpl_AccountService_Wrapper.cpp

"$(INTDIR)\AccountServiceImpl_AccountService_Wrapper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\BigBank\bigbank.account\AccountServiceImpl_stockQuoteService_Proxy.cpp

"$(INTDIR)\AccountServiceImpl_stockQuoteService_Proxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\BigBank\bigbank.account\StockQuoteServiceImpl.cpp

"$(INTDIR)\StockQuoteServiceImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\BigBank\bigbank.account\StockQuoteServiceImpl_StockQuoteService_Proxy.cpp

"$(INTDIR)\StockQuoteServiceImpl_StockQuoteService_Proxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\BigBank\bigbank.account\StockQuoteServiceImpl_StockQuoteService_Wrapper.cpp

"$(INTDIR)\StockQuoteServiceImpl_StockQuoteService_Wrapper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\BigBank\bigbank.account\StockQuoteServiceImpl_webService_Proxy.cpp

"$(INTDIR)\StockQuoteServiceImpl_webService_Proxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "Accounts - Win32 Release"

"AccountClient - Win32 Release" : 
   cd "..AccountClient"
   $(MAKE) /$(MAKEFLAGS) /F .\AccountClient.mak CFG="AccountClient - Win32 Release" 
   cd "..\Accounts"

"AccountClient - Win32 ReleaseCLEAN" : 
   cd "..AccountClient"
   $(MAKE) /$(MAKEFLAGS) /F .\AccountClient.mak CFG="AccountClient - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Accounts"

!ELSEIF  "$(CFG)" == "Accounts - Win32 Debug"

"AccountClient - Win32 Debug" : 
   cd "..AccountClient"
   $(MAKE) /$(MAKEFLAGS) /F .\AccountClient.mak CFG="AccountClient - Win32 Debug" 
   cd "..\Accounts"

"AccountClient - Win32 DebugCLEAN" : 
   cd "..AccountClient"
   $(MAKE) /$(MAKEFLAGS) /F .\AccountClient.mak CFG="AccountClient - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Accounts"

!ENDIF 


!ENDIF 

