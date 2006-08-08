# Microsoft Developer Studio Generated NMAKE File, Based on tuscany_sca_test.dsp
!IF "$(CFG)" == ""
CFG=tuscany_sca_test - Win32 Debug
!MESSAGE No configuration specified. Defaulting to tuscany_sca_test - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tuscany_sca_test - Win32 Release" && "$(CFG)" != "tuscany_sca_test - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tuscany_sca_test.mak" CFG="tuscany_sca_test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tuscany_sca_test - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "tuscany_sca_test - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "tuscany_sca_test - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\tuscany_sca_test.exe"

!ELSE 

ALL : "tuscany_sca_ws_service - Win32 Release" "tuscany_sca_test_MyValue - Win32 Release" "tuscany_sca_test_CustomerInfo - Win32 Release" "tuscany_sca - Win32 Release" "$(OUTDIR)\tuscany_sca_test.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"tuscany_sca - Win32 ReleaseCLEAN" "tuscany_sca_test_CustomerInfo - Win32 ReleaseCLEAN" "tuscany_sca_test_MyValue - Win32 ReleaseCLEAN" "tuscany_sca_ws_service - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\TestSCA.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\tuscany_sca_test.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "../../../runtime/core/src" /I "$(TUSCANY_SDOCPP)/include" /I "../../../runtime/core/test/MyValue" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca_test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sca.lib tuscany_sdo.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\tuscany_sca_test.pdb" /machine:I386 /out:"$(OUTDIR)\tuscany_sca_test.exe" /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\TestSCA.obj" \
	"..\tuscany_sca\Release\tuscany_sca.lib" \
	"..\tuscany_sca_test_CustomerInfo\Release\CustomerInfo.lib" \
	"..\tuscany_sca_test_MyValue\Release\MyValue.lib" \
	"..\tuscany_sca_ws_service\Release\tuscany_sca_ws_service.lib"

"$(OUTDIR)\tuscany_sca_test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "tuscany_sca_ws_service - Win32 Release" "tuscany_sca_test_MyValue - Win32 Release" "tuscany_sca_test_CustomerInfo - Win32 Release" "tuscany_sca - Win32 Release" "$(OUTDIR)\tuscany_sca_test.exe"
   copy ..\tuscany_sca_test_MyValue\Release\MyValue.dll testSCASystem\composites\MyValueServiceComposite
	copy ..\tuscany_sca_test_CustomerInfo\Release\CustomerInfo.dll testSCASystem\composites\MyValueServiceComposite
	copy..\tuscany_sca\Release\*.dll Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "tuscany_sca_test - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\tuscany_sca_test.exe"

!ELSE 

ALL : "tuscany_sca_ws_service - Win32 Debug" "tuscany_sca_test_MyValue - Win32 Debug" "tuscany_sca_test_CustomerInfo - Win32 Debug" "tuscany_sca - Win32 Debug" "$(OUTDIR)\tuscany_sca_test.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"tuscany_sca - Win32 DebugCLEAN" "tuscany_sca_test_CustomerInfo - Win32 DebugCLEAN" "tuscany_sca_test_MyValue - Win32 DebugCLEAN" "tuscany_sca_ws_service - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\TestSCA.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\tuscany_sca_test.exe"
	-@erase "$(OUTDIR)\tuscany_sca_test.ilk"
	-@erase "$(OUTDIR)\tuscany_sca_test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../runtime/core/src" /I "$(TUSCANY_SDOCPP)/include" /I "../../../runtime/core/test/MyValue" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca_test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sca.lib tuscany_sdo.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\tuscany_sca_test.pdb" /debug /machine:I386 /out:"$(OUTDIR)\tuscany_sca_test.exe" /pdbtype:sept /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\TestSCA.obj" \
	"..\tuscany_sca\Debug\tuscany_sca.lib" \
	"..\tuscany_sca_test_CustomerInfo\Debug\CustomerInfo.lib" \
	"..\tuscany_sca_test_MyValue\Debug\MyValue.lib" \
	"..\tuscany_sca_ws_service\Debug\tuscany_sca_ws_service.lib"

"$(OUTDIR)\tuscany_sca_test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "tuscany_sca_ws_service - Win32 Debug" "tuscany_sca_test_MyValue - Win32 Debug" "tuscany_sca_test_CustomerInfo - Win32 Debug" "tuscany_sca - Win32 Debug" "$(OUTDIR)\tuscany_sca_test.exe"
   copy ..\tuscany_sca_test_MyValue\Debug\MyValue.dll testSCASystem\composites\MyValueServiceComposite
	copy ..\tuscany_sca_test_MyValue\Debug\MyValue.pdb testSCASystem\composites\MyValueServiceComposite
	copy ..\tuscany_sca_test_CustomerInfo\Debug\CustomerInfo.dll testSCASystem\composites\MyValueServiceComposite
	copy ..\tuscany_sca_test_CustomerInfo\Debug\CustomerInfo.pdb testSCASystem\composites\MyValueServiceComposite
	copy..\tuscany_sca\Debug\*.dll Debug
	copy..\tuscany_sca\Debug\*.pdb Debug
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("tuscany_sca_test.dep")
!INCLUDE "tuscany_sca_test.dep"
!ELSE 
!MESSAGE Warning: cannot find "tuscany_sca_test.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "tuscany_sca_test - Win32 Release" || "$(CFG)" == "tuscany_sca_test - Win32 Debug"
SOURCE=..\..\..\runtime\core\test\src\TestSCA.cpp

"$(INTDIR)\TestSCA.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "tuscany_sca_test - Win32 Release"

"tuscany_sca - Win32 Release" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Release" 
   cd "..\tuscany_sca_test"

"tuscany_sca - Win32 ReleaseCLEAN" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Release" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_test"

!ELSEIF  "$(CFG)" == "tuscany_sca_test - Win32 Debug"

"tuscany_sca - Win32 Debug" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Debug" 
   cd "..\tuscany_sca_test"

"tuscany_sca - Win32 DebugCLEAN" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_test"

!ENDIF 

!IF  "$(CFG)" == "tuscany_sca_test - Win32 Release"

"tuscany_sca_test_CustomerInfo - Win32 Release" : 
   cd "..\tuscany_sca_test_CustomerInfo"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_test_CustomerInfo.mak CFG="tuscany_sca_test_CustomerInfo - Win32 Release" 
   cd "..\tuscany_sca_test"

"tuscany_sca_test_CustomerInfo - Win32 ReleaseCLEAN" : 
   cd "..\tuscany_sca_test_CustomerInfo"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_test_CustomerInfo.mak CFG="tuscany_sca_test_CustomerInfo - Win32 Release" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_test"

!ELSEIF  "$(CFG)" == "tuscany_sca_test - Win32 Debug"

"tuscany_sca_test_CustomerInfo - Win32 Debug" : 
   cd "..\tuscany_sca_test_CustomerInfo"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_test_CustomerInfo.mak CFG="tuscany_sca_test_CustomerInfo - Win32 Debug" 
   cd "..\tuscany_sca_test"

"tuscany_sca_test_CustomerInfo - Win32 DebugCLEAN" : 
   cd "..\tuscany_sca_test_CustomerInfo"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_test_CustomerInfo.mak CFG="tuscany_sca_test_CustomerInfo - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_test"

!ENDIF 

!IF  "$(CFG)" == "tuscany_sca_test - Win32 Release"

"tuscany_sca_test_MyValue - Win32 Release" : 
   cd "..\tuscany_sca_test_MyValue"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_test_MyValue.mak CFG="tuscany_sca_test_MyValue - Win32 Release" 
   cd "..\tuscany_sca_test"

"tuscany_sca_test_MyValue - Win32 ReleaseCLEAN" : 
   cd "..\tuscany_sca_test_MyValue"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_test_MyValue.mak CFG="tuscany_sca_test_MyValue - Win32 Release" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_test"

!ELSEIF  "$(CFG)" == "tuscany_sca_test - Win32 Debug"

"tuscany_sca_test_MyValue - Win32 Debug" : 
   cd "..\tuscany_sca_test_MyValue"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_test_MyValue.mak CFG="tuscany_sca_test_MyValue - Win32 Debug" 
   cd "..\tuscany_sca_test"

"tuscany_sca_test_MyValue - Win32 DebugCLEAN" : 
   cd "..\tuscany_sca_test_MyValue"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_test_MyValue.mak CFG="tuscany_sca_test_MyValue - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_test"

!ENDIF 

!IF  "$(CFG)" == "tuscany_sca_test - Win32 Release"

"tuscany_sca_ws_service - Win32 Release" : 
   cd "..\tuscany_sca_ws_service"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_service.mak CFG="tuscany_sca_ws_service - Win32 Release" 
   cd "..\tuscany_sca_test"

"tuscany_sca_ws_service - Win32 ReleaseCLEAN" : 
   cd "..\tuscany_sca_ws_service"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_service.mak CFG="tuscany_sca_ws_service - Win32 Release" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_test"

!ELSEIF  "$(CFG)" == "tuscany_sca_test - Win32 Debug"

"tuscany_sca_ws_service - Win32 Debug" : 
   cd "..\tuscany_sca_ws_service"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_service.mak CFG="tuscany_sca_ws_service - Win32 Debug" 
   cd "..\tuscany_sca_test"

"tuscany_sca_ws_service - Win32 DebugCLEAN" : 
   cd "..\tuscany_sca_ws_service"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_service.mak CFG="tuscany_sca_ws_service - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_test"

!ENDIF 


!ENDIF 

