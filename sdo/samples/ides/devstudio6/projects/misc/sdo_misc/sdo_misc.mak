# Microsoft Developer Studio Generated NMAKE File, Based on sdo_misc.dsp
!IF "$(CFG)" == ""
CFG=sdo_misc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to sdo_misc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "sdo_misc - Win32 Release" && "$(CFG)" != "sdo_misc - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sdo_misc.mak" CFG="sdo_misc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sdo_misc - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "sdo_misc - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "sdo_misc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\sdo_misc.exe"


CLEAN :
	-@erase "$(INTDIR)\ChangeSummarySave.obj"
	-@erase "$(INTDIR)\ObjectCreation.obj"
	-@erase "$(INTDIR)\Query.obj"
	-@erase "$(INTDIR)\samples.obj"
	-@erase "$(INTDIR)\Substitutes.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\XSDLoading.obj"
	-@erase "$(OUTDIR)\sdo_misc.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /I "$(TUSCANY_SDOCPP)\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sdo_misc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\sdo_misc.pdb" /machine:I386 /out:"$(OUTDIR)\sdo_misc.exe" /libpath:"$(TUSCANY_SDOCPP)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\ChangeSummarySave.obj" \
	"$(INTDIR)\ObjectCreation.obj" \
	"$(INTDIR)\Query.obj" \
	"$(INTDIR)\samples.obj" \
	"$(INTDIR)\Substitutes.obj" \
	"$(INTDIR)\XSDLoading.obj"

"$(OUTDIR)\sdo_misc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\sdo_misc.exe"
   copy ..\..\..\..\..\misc\*.xsd ..\..\..\..\..\misc\deploy
	copy ..\..\..\..\..\misc\*.xml ..\..\..\..\..\misc\deploy
	copy Release\*.exe ..\..\..\..\..\misc\deploy\bin
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "sdo_misc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\sdo_misc.exe"


CLEAN :
	-@erase "$(INTDIR)\ChangeSummarySave.obj"
	-@erase "$(INTDIR)\ObjectCreation.obj"
	-@erase "$(INTDIR)\Query.obj"
	-@erase "$(INTDIR)\samples.obj"
	-@erase "$(INTDIR)\Substitutes.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\XSDLoading.obj"
	-@erase "$(OUTDIR)\sdo_misc.exe"
	-@erase "$(OUTDIR)\sdo_misc.ilk"
	-@erase "$(OUTDIR)\sdo_misc.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(TUSCANY_SDOCPP)\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sdo_misc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\sdo_misc.pdb" /debug /machine:I386 /out:"$(OUTDIR)\sdo_misc.exe" /pdbtype:sept /libpath:"$(TUSCANY_SDOCPP)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\ChangeSummarySave.obj" \
	"$(INTDIR)\ObjectCreation.obj" \
	"$(INTDIR)\Query.obj" \
	"$(INTDIR)\samples.obj" \
	"$(INTDIR)\Substitutes.obj" \
	"$(INTDIR)\XSDLoading.obj"

"$(OUTDIR)\sdo_misc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\sdo_misc.exe"
   copy ..\..\..\..\..\misc\*.xsd ..\..\..\..\..\misc\deploy
	copy ..\..\..\..\..\misc\*.xml ..\..\..\..\..\misc\deploy
	copy Debug\*.exe ..\..\..\..\..\misc\deploy\bin
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("sdo_misc.dep")
!INCLUDE "sdo_misc.dep"
!ELSE 
!MESSAGE Warning: cannot find "sdo_misc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "sdo_misc - Win32 Release" || "$(CFG)" == "sdo_misc - Win32 Debug"
SOURCE=..\..\..\..\..\misc\ChangeSummarySave.cpp

"$(INTDIR)\ChangeSummarySave.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\misc\ObjectCreation.cpp

"$(INTDIR)\ObjectCreation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\misc\Query.cpp

"$(INTDIR)\Query.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\misc\samples.cpp

"$(INTDIR)\samples.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\misc\Substitutes.cpp

"$(INTDIR)\Substitutes.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\misc\XSDLoading.cpp

"$(INTDIR)\XSDLoading.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

