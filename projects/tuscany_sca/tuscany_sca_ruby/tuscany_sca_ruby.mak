# Microsoft Developer Studio Generated NMAKE File, Based on tuscany_sca_ruby.dsp
!IF "$(CFG)" == ""
CFG=tuscany_sca_ruby - Win32 Debug
!MESSAGE No configuration specified. Defaulting to tuscany_sca_ruby - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tuscany_sca_ruby - Win32 Release" && "$(CFG)" != "tuscany_sca_ruby - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tuscany_sca_ruby.mak" CFG="tuscany_sca_ruby - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tuscany_sca_ruby - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tuscany_sca_ruby - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "tuscany_sca_ruby - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\tuscany_sca_ruby.dll"

!ELSE 

ALL : "tuscany_sca - Win32 Release" "$(OUTDIR)\tuscany_sca_ruby.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"tuscany_sca - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\RubyCompositeContext.obj"
	-@erase "$(INTDIR)\RubyExtension.obj"
	-@erase "$(INTDIR)\RubyImplementation.obj"
	-@erase "$(INTDIR)\RubyImplementationExtension.obj"
	-@erase "$(INTDIR)\RubyReferenceBinding.obj"
	-@erase "$(INTDIR)\RubyServiceBinding.obj"
	-@erase "$(INTDIR)\RubyServiceProxy.obj"
	-@erase "$(INTDIR)\RubyServiceWrapper.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\tuscany_sca_ruby.dll"
	-@erase "$(OUTDIR)\tuscany_sca_ruby.exp"
	-@erase "$(OUTDIR)\tuscany_sca_ruby.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "../../../runtime/extensions/ruby/src" /I "../../../deploy/include" /I "$(TUSCANY_SDOCPP)/include" /I "$(RUBY_HOME)\lib\ruby\1.8\i386-mswin32" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_RUBY_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca_ruby.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sca.lib tuscany_sdo.lib msvcrt-ruby18.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\tuscany_sca_ruby.pdb" /machine:I386 /out:"$(OUTDIR)\tuscany_sca_ruby.dll" /implib:"$(OUTDIR)\tuscany_sca_ruby.lib" /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"$(RUBY_HOME)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\RubyImplementation.obj" \
	"$(INTDIR)\RubyReferenceBinding.obj" \
	"$(INTDIR)\RubyServiceBinding.obj" \
	"$(INTDIR)\RubyCompositeContext.obj" \
	"$(INTDIR)\RubyExtension.obj" \
	"$(INTDIR)\RubyImplementationExtension.obj" \
	"$(INTDIR)\RubyServiceProxy.obj" \
	"$(INTDIR)\RubyServiceWrapper.obj" \
	"..\tuscany_sca\Release\tuscany_sca.lib"

"$(OUTDIR)\tuscany_sca_ruby.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "tuscany_sca - Win32 Release" "$(OUTDIR)\tuscany_sca_ruby.dll"
   ..\..\..\runtime\extensions\ruby\deploy.bat ..\..\..\ Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "tuscany_sca_ruby - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\tuscany_sca_ruby.dll"

!ELSE 

ALL : "tuscany_sca - Win32 Debug" "$(OUTDIR)\tuscany_sca_ruby.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"tuscany_sca - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\RubyCompositeContext.obj"
	-@erase "$(INTDIR)\RubyExtension.obj"
	-@erase "$(INTDIR)\RubyImplementation.obj"
	-@erase "$(INTDIR)\RubyImplementationExtension.obj"
	-@erase "$(INTDIR)\RubyReferenceBinding.obj"
	-@erase "$(INTDIR)\RubyServiceBinding.obj"
	-@erase "$(INTDIR)\RubyServiceProxy.obj"
	-@erase "$(INTDIR)\RubyServiceWrapper.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\tuscany_sca_ruby.dll"
	-@erase "$(OUTDIR)\tuscany_sca_ruby.exp"
	-@erase "$(OUTDIR)\tuscany_sca_ruby.ilk"
	-@erase "$(OUTDIR)\tuscany_sca_ruby.lib"
	-@erase "$(OUTDIR)\tuscany_sca_ruby.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../runtime/extensions/ruby/src" /I "../../../deploy/include" /I "$(TUSCANY_SDOCPP)/include" /I "$(RUBY_HOME)\lib\ruby\1.8\i386-mswin32" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_RUBY_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca_ruby.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sca.lib tuscany_sdo.lib msvcrt-ruby18.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\tuscany_sca_ruby.pdb" /debug /machine:I386 /out:"$(OUTDIR)\tuscany_sca_ruby.dll" /implib:"$(OUTDIR)\tuscany_sca_ruby.lib" /pdbtype:sept /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"$(RUBY_HOME)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\RubyImplementation.obj" \
	"$(INTDIR)\RubyReferenceBinding.obj" \
	"$(INTDIR)\RubyServiceBinding.obj" \
	"$(INTDIR)\RubyCompositeContext.obj" \
	"$(INTDIR)\RubyExtension.obj" \
	"$(INTDIR)\RubyImplementationExtension.obj" \
	"$(INTDIR)\RubyServiceProxy.obj" \
	"$(INTDIR)\RubyServiceWrapper.obj" \
	"..\tuscany_sca\Debug\tuscany_sca.lib"

"$(OUTDIR)\tuscany_sca_ruby.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "tuscany_sca - Win32 Debug" "$(OUTDIR)\tuscany_sca_ruby.dll"
   ..\..\..\runtime\extensions\ruby\deploy.bat ..\..\..\ Debug
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("tuscany_sca_ruby.dep")
!INCLUDE "tuscany_sca_ruby.dep"
!ELSE 
!MESSAGE Warning: cannot find "tuscany_sca_ruby.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "tuscany_sca_ruby - Win32 Release" || "$(CFG)" == "tuscany_sca_ruby - Win32 Debug"
SOURCE=..\..\..\runtime\extensions\ruby\src\tuscany\sca\ruby\model\RubyImplementation.cpp

"$(INTDIR)\RubyImplementation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\ruby\src\tuscany\sca\ruby\model\RubyReferenceBinding.cpp

"$(INTDIR)\RubyReferenceBinding.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\ruby\src\tuscany\sca\ruby\model\RubyServiceBinding.cpp

"$(INTDIR)\RubyServiceBinding.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\ruby\src\tuscany\sca\ruby\RubyCompositeContext.cpp

"$(INTDIR)\RubyCompositeContext.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\ruby\src\tuscany\sca\ruby\RubyExtension.cpp

"$(INTDIR)\RubyExtension.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\ruby\src\tuscany\sca\ruby\RubyImplementationExtension.cpp

"$(INTDIR)\RubyImplementationExtension.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\ruby\src\tuscany\sca\ruby\RubyServiceProxy.cpp

"$(INTDIR)\RubyServiceProxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\ruby\src\tuscany\sca\ruby\RubyServiceWrapper.cpp

"$(INTDIR)\RubyServiceWrapper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "tuscany_sca_ruby - Win32 Release"

"tuscany_sca - Win32 Release" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Release" 
   cd "..\tuscany_sca_ruby"

"tuscany_sca - Win32 ReleaseCLEAN" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Release" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_ruby"

!ELSEIF  "$(CFG)" == "tuscany_sca_ruby - Win32 Debug"

"tuscany_sca - Win32 Debug" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Debug" 
   cd "..\tuscany_sca_ruby"

"tuscany_sca - Win32 DebugCLEAN" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_ruby"

!ENDIF 


!ENDIF 

