# Microsoft Developer Studio Generated NMAKE File, Based on Calculator.dsp
!IF "$(CFG)" == ""
CFG=Calculator - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Calculator - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Calculator - Win32 Release" && "$(CFG)" != "Calculator - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Calculator.mak" CFG="Calculator - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Calculator - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Calculator - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "Calculator - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Calculator.dll"

!ELSE 

ALL : "Client - Win32 Release" "$(OUTDIR)\Calculator.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"Client - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\CalculatorImpl.obj"
	-@erase "$(INTDIR)\CalculatorImpl_CalculatorService_Proxy.obj"
	-@erase "$(INTDIR)\CalculatorImpl_CalculatorService_Wrapper.obj"
	-@erase "$(INTDIR)\CalculatorImpl_divideService_Proxy.obj"
	-@erase "$(INTDIR)\DivideImpl.obj"
	-@erase "$(INTDIR)\DivideImpl_DivideService_Proxy.obj"
	-@erase "$(INTDIR)\DivideImpl_DivideService_Wrapper.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Calculator.dll"
	-@erase "$(OUTDIR)\Calculator.exp"
	-@erase "$(OUTDIR)\Calculator.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "$(TUSCANY_SDOCPP)/include" /I "$(TUSCANY_SCACPP)/include" /I "$(TUSCANY_SCACPP)/extensions/cpp/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CALCULATOR_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Calculator.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sca_cpp.lib tuscany_sca.lib tuscany_sdo.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\Calculator.pdb" /machine:I386 /out:"$(OUTDIR)\Calculator.dll" /implib:"$(OUTDIR)\Calculator.lib" /libpath:"$(TUSCANY_SDOCPP)/lib" /libpath:"$(TUSCANY_SCACPP)/lib" /libpath:"$(TUSCANY_SCACPP)/extensions/cpp/lib" 
LINK32_OBJS= \
	"$(INTDIR)\CalculatorImpl.obj" \
	"$(INTDIR)\CalculatorImpl_CalculatorService_Proxy.obj" \
	"$(INTDIR)\CalculatorImpl_CalculatorService_Wrapper.obj" \
	"$(INTDIR)\CalculatorImpl_divideService_Proxy.obj" \
	"$(INTDIR)\DivideImpl.obj" \
	"$(INTDIR)\DivideImpl_DivideService_Proxy.obj" \
	"$(INTDIR)\DivideImpl_DivideService_Wrapper.obj"

"$(OUTDIR)\Calculator.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "Client - Win32 Release" "$(OUTDIR)\Calculator.dll"
   ..\..\..\..\..\Calculator\deploy.bat ..\..\..\..\..\Calculator Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "Calculator - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Calculator.dll"

!ELSE 

ALL : "Client - Win32 Debug" "$(OUTDIR)\Calculator.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"Client - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\CalculatorImpl.obj"
	-@erase "$(INTDIR)\CalculatorImpl_CalculatorService_Proxy.obj"
	-@erase "$(INTDIR)\CalculatorImpl_CalculatorService_Wrapper.obj"
	-@erase "$(INTDIR)\CalculatorImpl_divideService_Proxy.obj"
	-@erase "$(INTDIR)\DivideImpl.obj"
	-@erase "$(INTDIR)\DivideImpl_DivideService_Proxy.obj"
	-@erase "$(INTDIR)\DivideImpl_DivideService_Wrapper.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Calculator.dll"
	-@erase "$(OUTDIR)\Calculator.exp"
	-@erase "$(OUTDIR)\Calculator.ilk"
	-@erase "$(OUTDIR)\Calculator.lib"
	-@erase "$(OUTDIR)\Calculator.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(TUSCANY_SDOCPP)/include" /I "$(TUSCANY_SCACPP)/include" /I "$(TUSCANY_SCACPP)/extensions/cpp/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CALCULATOR_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Calculator.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sca_cpp.lib tuscany_sca.lib tuscany_sdo.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\Calculator.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Calculator.dll" /implib:"$(OUTDIR)\Calculator.lib" /pdbtype:sept /libpath:"$(TUSCANY_SDOCPP)/lib" /libpath:"$(TUSCANY_SCACPP)/lib" /libpath:"$(TUSCANY_SCACPP)/extensions/cpp/lib" 
LINK32_OBJS= \
	"$(INTDIR)\CalculatorImpl.obj" \
	"$(INTDIR)\CalculatorImpl_CalculatorService_Proxy.obj" \
	"$(INTDIR)\CalculatorImpl_CalculatorService_Wrapper.obj" \
	"$(INTDIR)\CalculatorImpl_divideService_Proxy.obj" \
	"$(INTDIR)\DivideImpl.obj" \
	"$(INTDIR)\DivideImpl_DivideService_Proxy.obj" \
	"$(INTDIR)\DivideImpl_DivideService_Wrapper.obj"

"$(OUTDIR)\Calculator.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "Client - Win32 Debug" "$(OUTDIR)\Calculator.dll"
   ..\..\..\..\..\Calculator\deploy.bat ..\..\..\..\..\Calculator Debug
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
!IF EXISTS("Calculator.dep")
!INCLUDE "Calculator.dep"
!ELSE 
!MESSAGE Warning: cannot find "Calculator.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Calculator - Win32 Release" || "$(CFG)" == "Calculator - Win32 Debug"
SOURCE=..\..\..\..\..\Calculator\sample.calculator\CalculatorImpl.cpp

"$(INTDIR)\CalculatorImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\Calculator\sample.calculator\CalculatorImpl_CalculatorService_Proxy.cpp

"$(INTDIR)\CalculatorImpl_CalculatorService_Proxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\Calculator\sample.calculator\CalculatorImpl_CalculatorService_Wrapper.cpp

"$(INTDIR)\CalculatorImpl_CalculatorService_Wrapper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\Calculator\sample.calculator\CalculatorImpl_divideService_Proxy.cpp

"$(INTDIR)\CalculatorImpl_divideService_Proxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\Calculator\sample.calculator\DivideImpl.cpp

"$(INTDIR)\DivideImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\Calculator\sample.calculator\DivideImpl_DivideService_Proxy.cpp

"$(INTDIR)\DivideImpl_DivideService_Proxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\Calculator\sample.calculator\DivideImpl_DivideService_Wrapper.cpp

"$(INTDIR)\DivideImpl_DivideService_Wrapper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "Calculator - Win32 Release"

"Client - Win32 Release" : 
   cd "..\Client"
   $(MAKE) /$(MAKEFLAGS) /F .\Client.mak CFG="Client - Win32 Release" 
   cd "..\Calculator"

"Client - Win32 ReleaseCLEAN" : 
   cd "..\Client"
   $(MAKE) /$(MAKEFLAGS) /F .\Client.mak CFG="Client - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Calculator"

!ELSEIF  "$(CFG)" == "Calculator - Win32 Debug"

"Client - Win32 Debug" : 
   cd "..\Client"
   $(MAKE) /$(MAKEFLAGS) /F .\Client.mak CFG="Client - Win32 Debug" 
   cd "..\Calculator"

"Client - Win32 DebugCLEAN" : 
   cd "..\Client"
   $(MAKE) /$(MAKEFLAGS) /F .\Client.mak CFG="Client - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Calculator"

!ENDIF 


!ENDIF 

