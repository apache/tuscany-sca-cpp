# Microsoft Developer Studio Generated NMAKE File, Based on Client.dsp
!IF "$(CFG)" == ""
CFG=Client - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Client - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Client - Win32 Release" && "$(CFG)" != "Client - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Client.mak" CFG="Client - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Client - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Client - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Client - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\calculator_client.exe"


CLEAN :
	-@erase "$(INTDIR)\CalculatorClient.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\calculator_client.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "$(TUSCANY_SDOCPP)/include" /I "$(TUSCANY_SCACPP)/include" /I "$(TUSCANY_SCACPP)/extensions/cpp/include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Client.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib tuscany_sca.lib tuscany_sca_cpp.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\calculator_client.pdb" /machine:I386 /out:"$(OUTDIR)\calculator_client.exe" /libpath:"$(TUSCANY_SDOCPP)/lib" /libpath:"$(TUSCANY_SCACPP)/lib" /libpath:"$(TUSCANY_SCACPP)/extensions/cpp/lib" 
LINK32_OBJS= \
	"$(INTDIR)\CalculatorClient.obj"

"$(OUTDIR)\calculator_client.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\calculator_client.exe"
   call ..\..\..\..\..\Calculator\deployclient.bat ..\..\..\..\..\Calculator Release
	call ..\..\..\..\..\scagen.bat ..\..\..\..\..\Calculator\sample.calculator
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "Client - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\calculator_client.exe"


CLEAN :
	-@erase "$(INTDIR)\CalculatorClient.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\calculator_client.exe"
	-@erase "$(OUTDIR)\calculator_client.ilk"
	-@erase "$(OUTDIR)\calculator_client.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(TUSCANY_SDOCPP)/include" /I "$(TUSCANY_SCACPP)/include" /I "$(TUSCANY_SCACPP)/extensions/cpp/include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Client.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib tuscany_sca.lib tuscany_sca_cpp.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\calculator_client.pdb" /debug /machine:I386 /out:"$(OUTDIR)\calculator_client.exe" /pdbtype:sept /libpath:"$(TUSCANY_SDOCPP)/lib" /libpath:"$(TUSCANY_SCACPP)/lib" /libpath:"$(TUSCANY_SCACPP)/extensions/cpp/lib" 
LINK32_OBJS= \
	"$(INTDIR)\CalculatorClient.obj"

"$(OUTDIR)\calculator_client.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\calculator_client.exe"
   call ..\..\..\..\..\Calculator\deployclient.bat ..\..\..\..\..\Calculator Debug
	call ..\..\..\..\..\scagen.bat ..\..\..\..\..\Calculator\sample.calculator
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Client.dep")
!INCLUDE "Client.dep"
!ELSE 
!MESSAGE Warning: cannot find "Client.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Client - Win32 Release" || "$(CFG)" == "Client - Win32 Debug"
SOURCE=..\..\..\..\..\Calculator\sample.calculator.client\CalculatorClient.cpp

"$(INTDIR)\CalculatorClient.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

