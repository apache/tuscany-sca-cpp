# Microsoft Developer Studio Generated NMAKE File, Based on tuscany_sca_cpp.dsp
!IF "$(CFG)" == ""
CFG=tuscany_sca_cpp - Win32 Debug
!MESSAGE No configuration specified. Defaulting to tuscany_sca_cpp - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tuscany_sca_cpp - Win32 Release" && "$(CFG)" != "tuscany_sca_cpp - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tuscany_sca_cpp.mak" CFG="tuscany_sca_cpp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tuscany_sca_cpp - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tuscany_sca_cpp - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "tuscany_sca_cpp - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\tuscany_sca_cpp.dll"

!ELSE 

ALL : "tuscany_sca - Win32 Release" "$(OUTDIR)\tuscany_sca_cpp.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"tuscany_sca - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ComponentContext.obj"
	-@erase "$(INTDIR)\ComponentContextImpl.obj"
	-@erase "$(INTDIR)\CompositeContext.obj"
	-@erase "$(INTDIR)\CompositeContextImpl.obj"
	-@erase "$(INTDIR)\CPPExtension.obj"
	-@erase "$(INTDIR)\CPPImplementation.obj"
	-@erase "$(INTDIR)\CPPImplementationExtension.obj"
	-@erase "$(INTDIR)\CPPInterface.obj"
	-@erase "$(INTDIR)\CPPInterfaceExtension.obj"
	-@erase "$(INTDIR)\CPPReferenceBinding.obj"
	-@erase "$(INTDIR)\CPPServiceBinding.obj"
	-@erase "$(INTDIR)\CPPServiceProxy.obj"
	-@erase "$(INTDIR)\CPPServiceWrapper.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\tuscany_sca_cpp.dll"
	-@erase "$(OUTDIR)\tuscany_sca_cpp.exp"
	-@erase "$(OUTDIR)\tuscany_sca_cpp.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "../../../runtime/extensions/cpp/src" /I "../../../deploy/include" /I "$(TUSCANY_SDOCPP)/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_CPP_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca_cpp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=tuscany_sca.lib tuscany_sdo.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\tuscany_sca_cpp.pdb" /machine:I386 /out:"$(OUTDIR)\tuscany_sca_cpp.dll" /implib:"$(OUTDIR)\tuscany_sca_cpp.lib" /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\ComponentContext.obj" \
	"$(INTDIR)\CompositeContext.obj" \
	"$(INTDIR)\CPPImplementation.obj" \
	"$(INTDIR)\CPPInterface.obj" \
	"$(INTDIR)\CPPReferenceBinding.obj" \
	"$(INTDIR)\CPPServiceBinding.obj" \
	"$(INTDIR)\ComponentContextImpl.obj" \
	"$(INTDIR)\CompositeContextImpl.obj" \
	"$(INTDIR)\CPPExtension.obj" \
	"$(INTDIR)\CPPImplementationExtension.obj" \
	"$(INTDIR)\CPPInterfaceExtension.obj" \
	"$(INTDIR)\CPPServiceProxy.obj" \
	"$(INTDIR)\CPPServiceWrapper.obj" \
	"..\tuscany_sca\Release\tuscany_sca.lib"

"$(OUTDIR)\tuscany_sca_cpp.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "tuscany_sca - Win32 Release" "$(OUTDIR)\tuscany_sca_cpp.dll"
   ..\..\..\runtime\extensions\cpp\deploy.bat ..\..\..\ Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "tuscany_sca_cpp - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\tuscany_sca_cpp.dll"

!ELSE 

ALL : "tuscany_sca - Win32 Debug" "$(OUTDIR)\tuscany_sca_cpp.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"tuscany_sca - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ComponentContext.obj"
	-@erase "$(INTDIR)\ComponentContextImpl.obj"
	-@erase "$(INTDIR)\CompositeContext.obj"
	-@erase "$(INTDIR)\CompositeContextImpl.obj"
	-@erase "$(INTDIR)\CPPExtension.obj"
	-@erase "$(INTDIR)\CPPImplementation.obj"
	-@erase "$(INTDIR)\CPPImplementationExtension.obj"
	-@erase "$(INTDIR)\CPPInterface.obj"
	-@erase "$(INTDIR)\CPPInterfaceExtension.obj"
	-@erase "$(INTDIR)\CPPReferenceBinding.obj"
	-@erase "$(INTDIR)\CPPServiceBinding.obj"
	-@erase "$(INTDIR)\CPPServiceProxy.obj"
	-@erase "$(INTDIR)\CPPServiceWrapper.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\tuscany_sca_cpp.dll"
	-@erase "$(OUTDIR)\tuscany_sca_cpp.exp"
	-@erase "$(OUTDIR)\tuscany_sca_cpp.ilk"
	-@erase "$(OUTDIR)\tuscany_sca_cpp.lib"
	-@erase "$(OUTDIR)\tuscany_sca_cpp.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../runtime/extensions/cpp/src" /I "../../../deploy/include" /I "$(TUSCANY_SDOCPP)/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_CPP_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca_cpp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=tuscany_sca.lib tuscany_sdo.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\tuscany_sca_cpp.pdb" /debug /machine:I386 /out:"$(OUTDIR)\tuscany_sca_cpp.dll" /implib:"$(OUTDIR)\tuscany_sca_cpp.lib" /pdbtype:sept /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\ComponentContext.obj" \
	"$(INTDIR)\CompositeContext.obj" \
	"$(INTDIR)\CPPImplementation.obj" \
	"$(INTDIR)\CPPInterface.obj" \
	"$(INTDIR)\CPPReferenceBinding.obj" \
	"$(INTDIR)\CPPServiceBinding.obj" \
	"$(INTDIR)\ComponentContextImpl.obj" \
	"$(INTDIR)\CompositeContextImpl.obj" \
	"$(INTDIR)\CPPExtension.obj" \
	"$(INTDIR)\CPPImplementationExtension.obj" \
	"$(INTDIR)\CPPInterfaceExtension.obj" \
	"$(INTDIR)\CPPServiceProxy.obj" \
	"$(INTDIR)\CPPServiceWrapper.obj" \
	"..\tuscany_sca\Debug\tuscany_sca.lib"

"$(OUTDIR)\tuscany_sca_cpp.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "tuscany_sca - Win32 Debug" "$(OUTDIR)\tuscany_sca_cpp.dll"
   ..\..\..\runtime\extensions\cpp\deploy.bat ..\..\..\ Debug
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("tuscany_sca_cpp.dep")
!INCLUDE "tuscany_sca_cpp.dep"
!ELSE 
!MESSAGE Warning: cannot find "tuscany_sca_cpp.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "tuscany_sca_cpp - Win32 Release" || "$(CFG)" == "tuscany_sca_cpp - Win32 Debug"
SOURCE=..\..\..\runtime\extensions\cpp\src\osoa\sca\ComponentContext.cpp

"$(INTDIR)\ComponentContext.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\cpp\src\osoa\sca\CompositeContext.cpp

"$(INTDIR)\CompositeContext.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\model\CPPImplementation.cpp

"$(INTDIR)\CPPImplementation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\model\CPPInterface.cpp

"$(INTDIR)\CPPInterface.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\model\CPPReferenceBinding.cpp

"$(INTDIR)\CPPReferenceBinding.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\model\CPPServiceBinding.cpp

"$(INTDIR)\CPPServiceBinding.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\ComponentContextImpl.cpp

"$(INTDIR)\ComponentContextImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CompositeContextImpl.cpp

"$(INTDIR)\CompositeContextImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CPPExtension.cpp

"$(INTDIR)\CPPExtension.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CPPImplementationExtension.cpp

"$(INTDIR)\CPPImplementationExtension.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CPPInterfaceExtension.cpp

"$(INTDIR)\CPPInterfaceExtension.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CPPServiceProxy.cpp

"$(INTDIR)\CPPServiceProxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CPPServiceWrapper.cpp

"$(INTDIR)\CPPServiceWrapper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "tuscany_sca_cpp - Win32 Release"

"tuscany_sca - Win32 Release" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Release" 
   cd "..\tuscany_sca_cpp"

"tuscany_sca - Win32 ReleaseCLEAN" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Release" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_cpp"

!ELSEIF  "$(CFG)" == "tuscany_sca_cpp - Win32 Debug"

"tuscany_sca - Win32 Debug" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Debug" 
   cd "..\tuscany_sca_cpp"

"tuscany_sca - Win32 DebugCLEAN" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_cpp"

!ENDIF 


!ENDIF 

