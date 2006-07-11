# Microsoft Developer Studio Generated NMAKE File, Based on tuscany_sca_test_CustomerInfo.dsp
!IF "$(CFG)" == ""
CFG=tuscany_sca_test_CustomerInfo - Win32 Debug
!MESSAGE No configuration specified. Defaulting to tuscany_sca_test_CustomerInfo - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tuscany_sca_test_CustomerInfo - Win32 Release" && "$(CFG)" != "tuscany_sca_test_CustomerInfo - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tuscany_sca_test_CustomerInfo.mak" CFG="tuscany_sca_test_CustomerInfo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tuscany_sca_test_CustomerInfo - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tuscany_sca_test_CustomerInfo - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "tuscany_sca_test_CustomerInfo - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\CustomerInfo.dll"

!ELSE 

ALL : "tuscany_sca - Win32 Release" "$(OUTDIR)\CustomerInfo.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"tuscany_sca - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\CustomerInfoImpl.obj"
	-@erase "$(INTDIR)\CustomerInfoImpl_CustomerInfoService_Proxy.obj"
	-@erase "$(INTDIR)\CustomerInfoImpl_CustomerInfoService_Wrapper.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\CustomerInfo.dll"
	-@erase "$(OUTDIR)\CustomerInfo.exp"
	-@erase "$(OUTDIR)\CustomerInfo.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "../../../runtime/core/src" /I "$(TUSCANY_SDOCPP)/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_TEST_CUSTOMERINFO_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca_test_CustomerInfo.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sca.lib tuscany_sdo.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\CustomerInfo.pdb" /machine:I386 /out:"$(OUTDIR)\CustomerInfo.dll" /implib:"$(OUTDIR)\CustomerInfo.lib" /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"..\..\..\deploy\lib" 
LINK32_OBJS= \
	"$(INTDIR)\CustomerInfoImpl.obj" \
	"$(INTDIR)\CustomerInfoImpl_CustomerInfoService_Proxy.obj" \
	"$(INTDIR)\CustomerInfoImpl_CustomerInfoService_Wrapper.obj" \
	"..\tuscany_sca\Release\tuscany_sca.lib"

"$(OUTDIR)\CustomerInfo.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tuscany_sca_test_CustomerInfo - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\CustomerInfo.dll"

!ELSE 

ALL : "tuscany_sca - Win32 Debug" "$(OUTDIR)\CustomerInfo.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"tuscany_sca - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\CustomerInfoImpl.obj"
	-@erase "$(INTDIR)\CustomerInfoImpl_CustomerInfoService_Proxy.obj"
	-@erase "$(INTDIR)\CustomerInfoImpl_CustomerInfoService_Wrapper.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\CustomerInfo.dll"
	-@erase "$(OUTDIR)\CustomerInfo.exp"
	-@erase "$(OUTDIR)\CustomerInfo.ilk"
	-@erase "$(OUTDIR)\CustomerInfo.lib"
	-@erase "$(OUTDIR)\CustomerInfo.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../runtime/core/src" /I "$(TUSCANY_SDOCPP)/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_TEST_CUSTOMERINFO_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca_test_CustomerInfo.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sca.lib tuscany_sdo.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\CustomerInfo.pdb" /debug /machine:I386 /out:"$(OUTDIR)\CustomerInfo.dll" /implib:"$(OUTDIR)\CustomerInfo.lib" /pdbtype:sept /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\CustomerInfoImpl.obj" \
	"$(INTDIR)\CustomerInfoImpl_CustomerInfoService_Proxy.obj" \
	"$(INTDIR)\CustomerInfoImpl_CustomerInfoService_Wrapper.obj" \
	"..\tuscany_sca\Debug\tuscany_sca.lib"

"$(OUTDIR)\CustomerInfo.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("tuscany_sca_test_CustomerInfo.dep")
!INCLUDE "tuscany_sca_test_CustomerInfo.dep"
!ELSE 
!MESSAGE Warning: cannot find "tuscany_sca_test_CustomerInfo.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "tuscany_sca_test_CustomerInfo - Win32 Release" || "$(CFG)" == "tuscany_sca_test_CustomerInfo - Win32 Debug"
SOURCE=..\..\..\runtime\core\test\CustomerInfo\CustomerInfoImpl.cpp

"$(INTDIR)\CustomerInfoImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\test\CustomerInfo\CustomerInfoImpl_CustomerInfoService_Proxy.cpp

"$(INTDIR)\CustomerInfoImpl_CustomerInfoService_Proxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\test\CustomerInfo\CustomerInfoImpl_CustomerInfoService_Wrapper.cpp

"$(INTDIR)\CustomerInfoImpl_CustomerInfoService_Wrapper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "tuscany_sca_test_CustomerInfo - Win32 Release"

"tuscany_sca - Win32 Release" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Release" 
   cd "..\tuscany_sca_test_CustomerInfo"

"tuscany_sca - Win32 ReleaseCLEAN" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Release" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_test_CustomerInfo"

!ELSEIF  "$(CFG)" == "tuscany_sca_test_CustomerInfo - Win32 Debug"

"tuscany_sca - Win32 Debug" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Debug" 
   cd "..\tuscany_sca_test_CustomerInfo"

"tuscany_sca - Win32 DebugCLEAN" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_test_CustomerInfo"

!ENDIF 


!ENDIF 

