# Microsoft Developer Studio Generated NMAKE File, Based on AccountClient.dsp
!IF "$(CFG)" == ""
CFG=AccountClient - Win32 Debug
!MESSAGE No configuration specified. Defaulting to AccountClient - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "AccountClient - Win32 Release" && "$(CFG)" != "AccountClient - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AccountClient.mak" CFG="AccountClient - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AccountClient - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "AccountClient - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "AccountClient - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\AccountClient.exe"


CLEAN :
	-@erase "$(INTDIR)\AccountClient.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\AccountClient.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\..\..\..\BigBank\bigbank.account" /I "$(TUSCANY_SCACPP)\include" /I "$(TUSCANY_SCACPP)\extensions\cpp\include" /I "$(TUSCANY_SDOCPP)\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AccountClient.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib tuscany_sca.lib tuscany_sca_cpp.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\AccountClient.pdb" /machine:I386 /out:"$(OUTDIR)\AccountClient.exe" /libpath:"$(TUSCANY_SCACPP)/extensions/cpp/lib" /libpath:"$(TUSCANY_SCACPP)/lib" /libpath:"$(TUSCANY_SDOCPP)/lib" 
LINK32_OBJS= \
	"$(INTDIR)\AccountClient.obj"

"$(OUTDIR)\AccountClient.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\AccountClient.exe"
   call ..\..\..\..\..\BigBank\deployclient.bat ..\..\..\..\..\BigBank Release
	call ..\..\..\..\..\scagen.bat ..\..\..\..\..\BigBank\bigbank.account
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "AccountClient - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\AccountClient.exe"


CLEAN :
	-@erase "$(INTDIR)\AccountClient.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\AccountClient.exe"
	-@erase "$(OUTDIR)\AccountClient.ilk"
	-@erase "$(OUTDIR)\AccountClient.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\..\BigBank\bigbank.account" /I "$(TUSCANY_SCACPP)\include" /I "$(TUSCANY_SCACPP)\extensions\cpp\include" /I "$(TUSCANY_SDOCPP)\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AccountClient.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib tuscany_sca.lib tuscany_sca_cpp.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\AccountClient.pdb" /debug /machine:I386 /out:"$(OUTDIR)\AccountClient.exe" /pdbtype:sept /libpath:"$(TUSCANY_SCACPP)/extensions/cpp/lib" /libpath:"$(TUSCANY_SCACPP)/lib" /libpath:"$(TUSCANY_SDOCPP)/lib" 
LINK32_OBJS= \
	"$(INTDIR)\AccountClient.obj"

"$(OUTDIR)\AccountClient.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\AccountClient.exe"
   call ..\..\..\..\..\BigBank\deployclient.bat ..\..\..\..\..\BigBank Debug
	call ..\..\..\..\..\scagen.bat ..\..\..\..\..\BigBank\bigbank.account
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("AccountClient.dep")
!INCLUDE "AccountClient.dep"
!ELSE 
!MESSAGE Warning: cannot find "AccountClient.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "AccountClient - Win32 Release" || "$(CFG)" == "AccountClient - Win32 Debug"
SOURCE=..\..\..\..\..\BigBank\bigbank.client\AccountClient.cpp

"$(INTDIR)\AccountClient.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

