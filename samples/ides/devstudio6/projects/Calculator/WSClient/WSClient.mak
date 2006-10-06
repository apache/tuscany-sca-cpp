# Microsoft Developer Studio Generated NMAKE File, Based on WSClient.dsp
!IF "$(CFG)" == ""
CFG=WSClient - Win32 Debug
!MESSAGE No configuration specified. Defaulting to WSClient - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "WSClient - Win32 Release" && "$(CFG)" != "WSClient - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WSClient.mak" CFG="WSClient - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WSClient - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "WSClient - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WSClient - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\calculator_wsclient.exe"


CLEAN :
	-@erase "$(INTDIR)\axis2_Calculator_stub.obj"
	-@erase "$(INTDIR)\CalculatorWSClient.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\calculator_wsclient.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "$(TUSCANY_SDOCPP)/include" /I "$(AXIS2C_HOME)/include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WSClient.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib axis2_engine.lib axis2_parser.lib axis2_util.lib axiom.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\calculator_wsclient.pdb" /machine:I386 /out:"$(OUTDIR)\calculator_wsclient.exe" /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"$(AXIS2C_HOME)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\axis2_Calculator_stub.obj" \
	"$(INTDIR)\CalculatorWSClient.obj"

"$(OUTDIR)\calculator_wsclient.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\calculator_wsclient.exe"
   ..\..\..\..\..\Calculator\deploywsclient.bat ..\..\..\..\..\Calculator Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "WSClient - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\calculator_wsclient.exe"


CLEAN :
	-@erase "$(INTDIR)\axis2_Calculator_stub.obj"
	-@erase "$(INTDIR)\CalculatorWSClient.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\calculator_wsclient.exe"
	-@erase "$(OUTDIR)\calculator_wsclient.ilk"
	-@erase "$(OUTDIR)\calculator_wsclient.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "$(TUSCANY_SDOCPP)/include" /I "$(AXIS2C_HOME)/include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WSClient.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib axis2_engine.lib axis2_parser.lib axis2_util.lib axiom.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\calculator_wsclient.pdb" /debug /machine:I386 /out:"$(OUTDIR)\calculator_wsclient.exe" /pdbtype:sept /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"$(AXIS2C_HOME)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\axis2_Calculator_stub.obj" \
	"$(INTDIR)\CalculatorWSClient.obj"

"$(OUTDIR)\calculator_wsclient.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\calculator_wsclient.exe"
   ..\..\..\..\..\Calculator\deploywsclient.bat ..\..\..\..\..\Calculator  Debug
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
!IF EXISTS("WSClient.dep")
!INCLUDE "WSClient.dep"
!ELSE 
!MESSAGE Warning: cannot find "WSClient.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "WSClient - Win32 Release" || "$(CFG)" == "WSClient - Win32 Debug"
SOURCE=..\..\..\..\..\Calculator\sample.calculator.wsclient\axis2_Calculator_stub.cpp

"$(INTDIR)\axis2_Calculator_stub.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\Calculator\sample.calculator.wsclient\CalculatorWSClient.cpp

"$(INTDIR)\CalculatorWSClient.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

