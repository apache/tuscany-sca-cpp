# Microsoft Developer Studio Generated NMAKE File, Based on WSAccountClient.dsp
!IF "$(CFG)" == ""
CFG=WSAccountClient - Win32 Debug
!MESSAGE No configuration specified. Defaulting to WSAccountClient - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "WSAccountClient - Win32 Release" && "$(CFG)" != "WSAccountClient - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WSAccountClient.mak" CFG="WSAccountClient - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WSAccountClient - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "WSAccountClient - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "WSAccountClient - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\WSAccountClient.exe"


CLEAN :
	-@erase "$(INTDIR)\AccountWSClient.obj"
	-@erase "$(INTDIR)\axis2_Account_stub.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\WSAccountClient.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "$(AXIS2C_HOME)\include" /I "$(TUSCANY_SCACPP)\include" /I "$(TUSCANY_SCACPP)\extensions\cpp\include" /I "$(TUSCANY_SDOCPP)\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WSAccountClient.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib tuscany_sca.lib tuscany_sca_cpp.lib axis2_engine.lib axis2_parser.lib axis2_util.lib axiom.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\WSAccountClient.pdb" /machine:I386 /out:"$(OUTDIR)\WSAccountClient.exe" /libpath:"$(TUSCANY_SCACPP)/extensions/cpp/lib" /libpath:"$(TUSCANY_SCACPP)/lib" /libpath:"$(TUSCANY_SDOCPP)/lib" /libpath:"$(AXIS2C_HOME)/lib" 
LINK32_OBJS= \
	"$(INTDIR)\AccountWSClient.obj" \
	"$(INTDIR)\axis2_Account_stub.obj"

"$(OUTDIR)\WSAccountClient.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\WSAccountClient.exe"
   call ..\..\..\..\..\BigBank\deploywsclient.bat ..\..\..\..\..\BigBank Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "WSAccountClient - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\WSAccountClient.exe"


CLEAN :
	-@erase "$(INTDIR)\AccountWSClient.obj"
	-@erase "$(INTDIR)\axis2_Account_stub.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\WSAccountClient.exe"
	-@erase "$(OUTDIR)\WSAccountClient.ilk"
	-@erase "$(OUTDIR)\WSAccountClient.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(AXIS2C_HOME)\include" /I "$(TUSCANY_SCACPP)\include" /I "$(TUSCANY_SCACPP)\extensions\cpp\include" /I "$(TUSCANY_SDOCPP)\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WSAccountClient.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib tuscany_sca.lib tuscany_sca_cpp.lib axis2_engine.lib axis2_parser.lib axis2_util.lib axiom.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\WSAccountClient.pdb" /debug /machine:I386 /out:"$(OUTDIR)\WSAccountClient.exe" /pdbtype:sept /libpath:"$(TUSCANY_SCACPP)/extensions/cpp/lib" /libpath:"$(TUSCANY_SCACPP)/lib" /libpath:"$(TUSCANY_SDOCPP)/lib" /libpath:"$(AXIS2C_HOME)/lib" 
LINK32_OBJS= \
	"$(INTDIR)\AccountWSClient.obj" \
	"$(INTDIR)\axis2_Account_stub.obj"

"$(OUTDIR)\WSAccountClient.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\WSAccountClient.exe"
   call ..\..\..\..\..\BigBank\deploywsclient.bat ..\..\..\..\..\BigBank Debug
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
!IF EXISTS("WSAccountClient.dep")
!INCLUDE "WSAccountClient.dep"
!ELSE 
!MESSAGE Warning: cannot find "WSAccountClient.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "WSAccountClient - Win32 Release" || "$(CFG)" == "WSAccountClient - Win32 Debug"
SOURCE=..\..\..\..\..\BigBank\bigbank.wsclient\AccountWSClient.cpp

"$(INTDIR)\AccountWSClient.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\BigBank\bigbank.wsclient\axis2_Account_stub.cpp

"$(INTDIR)\axis2_Account_stub.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

