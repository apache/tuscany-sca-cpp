# Microsoft Developer Studio Generated NMAKE File, Based on tuscany_sca_python.dsp
!IF "$(CFG)" == ""
CFG=tuscany_sca_python - Win32 Debug
!MESSAGE No configuration specified. Defaulting to tuscany_sca_python - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tuscany_sca_python - Win32 Release" && "$(CFG)" != "tuscany_sca_python - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tuscany_sca_python.mak" CFG="tuscany_sca_python - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tuscany_sca_python - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tuscany_sca_python - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "tuscany_sca_python - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\tuscany_sca_python.dll"


CLEAN :
	-@erase "$(INTDIR)\PythonExtension.obj"
	-@erase "$(INTDIR)\PythonImplementation.obj"
	-@erase "$(INTDIR)\PythonImplementationExtension.obj"
	-@erase "$(INTDIR)\PythonInterface.obj"
	-@erase "$(INTDIR)\PythonInterfaceExtension.obj"
	-@erase "$(INTDIR)\PythonReferenceBinding.obj"
	-@erase "$(INTDIR)\PythonServiceBinding.obj"
	-@erase "$(INTDIR)\PythonServiceProxy.obj"
	-@erase "$(INTDIR)\PythonServiceWrapper.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\tuscany_sca_python.dll"
	-@erase "$(OUTDIR)\tuscany_sca_python.exp"
	-@erase "$(OUTDIR)\tuscany_sca_python.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "../../../runtime/extensions/python/src" /I "../../../deploy/include" /I "$(TUSCANY_SDOCPP)/include" /I "$(PYTHON_HOME)\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_PYTHON_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca_python.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sca.lib tuscany_sdo.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\tuscany_sca_python.pdb" /machine:I386 /out:"$(OUTDIR)\tuscany_sca_python.dll" /implib:"$(OUTDIR)\tuscany_sca_python.lib" /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"$(PYTHON_HOME)\libs" 
LINK32_OBJS= \
	"$(INTDIR)\PythonImplementation.obj" \
	"$(INTDIR)\PythonInterface.obj" \
	"$(INTDIR)\PythonReferenceBinding.obj" \
	"$(INTDIR)\PythonServiceBinding.obj" \
	"$(INTDIR)\PythonExtension.obj" \
	"$(INTDIR)\PythonImplementationExtension.obj" \
	"$(INTDIR)\PythonInterfaceExtension.obj" \
	"$(INTDIR)\PythonServiceProxy.obj" \
	"$(INTDIR)\PythonServiceWrapper.obj"

"$(OUTDIR)\tuscany_sca_python.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\tuscany_sca_python.dll"
   ..\..\..\runtime\extensions\python\deploy.bat ..\..\..\ Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "tuscany_sca_python - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\tuscany_sca_python.dll"


CLEAN :
	-@erase "$(INTDIR)\PythonExtension.obj"
	-@erase "$(INTDIR)\PythonImplementation.obj"
	-@erase "$(INTDIR)\PythonImplementationExtension.obj"
	-@erase "$(INTDIR)\PythonInterface.obj"
	-@erase "$(INTDIR)\PythonInterfaceExtension.obj"
	-@erase "$(INTDIR)\PythonReferenceBinding.obj"
	-@erase "$(INTDIR)\PythonServiceBinding.obj"
	-@erase "$(INTDIR)\PythonServiceProxy.obj"
	-@erase "$(INTDIR)\PythonServiceWrapper.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\tuscany_sca_python.dll"
	-@erase "$(OUTDIR)\tuscany_sca_python.exp"
	-@erase "$(OUTDIR)\tuscany_sca_python.ilk"
	-@erase "$(OUTDIR)\tuscany_sca_python.lib"
	-@erase "$(OUTDIR)\tuscany_sca_python.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../runtime/extensions/python/src" /I "../../../deploy/include" /I "$(TUSCANY_SDOCPP)/include" /I "$(PYTHON_HOME)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_PYTHON_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca_python.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sca.lib tuscany_sdo.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\tuscany_sca_python.pdb" /debug /machine:I386 /out:"$(OUTDIR)\tuscany_sca_python.dll" /implib:"$(OUTDIR)\tuscany_sca_python.lib" /pdbtype:sept /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"$(PYTHON_HOME)\libs" 
LINK32_OBJS= \
	"$(INTDIR)\PythonImplementation.obj" \
	"$(INTDIR)\PythonInterface.obj" \
	"$(INTDIR)\PythonReferenceBinding.obj" \
	"$(INTDIR)\PythonServiceBinding.obj" \
	"$(INTDIR)\PythonExtension.obj" \
	"$(INTDIR)\PythonImplementationExtension.obj" \
	"$(INTDIR)\PythonInterfaceExtension.obj" \
	"$(INTDIR)\PythonServiceProxy.obj" \
	"$(INTDIR)\PythonServiceWrapper.obj"

"$(OUTDIR)\tuscany_sca_python.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\tuscany_sca_python.dll"
   ..\..\..\runtime\extensions\python\deploy.bat ..\..\..\ Debug
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("tuscany_sca_python.dep")
!INCLUDE "tuscany_sca_python.dep"
!ELSE 
!MESSAGE Warning: cannot find "tuscany_sca_python.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "tuscany_sca_python - Win32 Release" || "$(CFG)" == "tuscany_sca_python - Win32 Debug"
SOURCE=..\..\..\runtime\extensions\python\src\tuscany\sca\python\model\PythonImplementation.cpp

"$(INTDIR)\PythonImplementation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\python\src\tuscany\sca\python\model\PythonInterface.cpp

"$(INTDIR)\PythonInterface.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\python\src\tuscany\sca\python\model\PythonReferenceBinding.cpp

"$(INTDIR)\PythonReferenceBinding.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\python\src\tuscany\sca\python\model\PythonServiceBinding.cpp

"$(INTDIR)\PythonServiceBinding.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\python\src\tuscany\sca\python\PythonExtension.cpp

"$(INTDIR)\PythonExtension.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\python\src\tuscany\sca\python\PythonImplementationExtension.cpp

"$(INTDIR)\PythonImplementationExtension.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\python\src\tuscany\sca\python\PythonInterfaceExtension.cpp

"$(INTDIR)\PythonInterfaceExtension.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\python\src\tuscany\sca\python\PythonServiceProxy.cpp

"$(INTDIR)\PythonServiceProxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\python\src\tuscany\sca\python\PythonServiceWrapper.cpp

"$(INTDIR)\PythonServiceWrapper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

