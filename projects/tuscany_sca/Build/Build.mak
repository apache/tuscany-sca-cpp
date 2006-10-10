# Microsoft Developer Studio Generated NMAKE File, Based on Build.dsp
!IF "$(CFG)" == ""
CFG=Build - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Build - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Build - Win32 Release" && "$(CFG)" != "Build - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Build.mak" CFG="Build - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Build - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "Build - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : 

!ELSE 

ALL : "tuscany_sca_ruby - Win32 Release" "tuscany_sca_python - Win32 Release" "tuscany_sca_ws_dispatcher - Win32 Release" "tuscany_sca_ws_service - Win32 Release" "tuscany_sca_ws_reference - Win32 Release" "tuscany_sca_cpp - Win32 Release" "tuscany_sca - Win32 Release" 

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"tuscany_sca - Win32 ReleaseCLEAN" "tuscany_sca_cpp - Win32 ReleaseCLEAN" "tuscany_sca_ws_reference - Win32 ReleaseCLEAN" "tuscany_sca_ws_service - Win32 ReleaseCLEAN" "tuscany_sca_ws_dispatcher - Win32 ReleaseCLEAN" "tuscany_sca_python - Win32 ReleaseCLEAN" "tuscany_sca_ruby - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

!IF  "$(CFG)" == "Build - Win32 Release"

MTL=midl.exe
MTL_PROJ=

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

MTL=midl.exe
MTL_PROJ=

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Build.dep")
!INCLUDE "Build.dep"
!ELSE 
!MESSAGE Warning: cannot find "Build.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Build - Win32 Release" || "$(CFG)" == "Build - Win32 Debug"

!IF  "$(CFG)" == "Build - Win32 Release"

"tuscany_sca - Win32 Release" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Release" 
   cd "..\Build"

"tuscany_sca - Win32 ReleaseCLEAN" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Build"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

"tuscany_sca - Win32 Debug" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Debug" 
   cd "..\Build"

"tuscany_sca - Win32 DebugCLEAN" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Build"

!ENDIF 

!IF  "$(CFG)" == "Build - Win32 Release"

"tuscany_sca_cpp - Win32 Release" : 
   cd "..\tuscany_sca_cpp"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_cpp.mak CFG="tuscany_sca_cpp - Win32 Release" 
   cd "..\Build"

"tuscany_sca_cpp - Win32 ReleaseCLEAN" : 
   cd "..\tuscany_sca_cpp"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_cpp.mak CFG="tuscany_sca_cpp - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Build"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

"tuscany_sca_cpp - Win32 Debug" : 
   cd "..\tuscany_sca_cpp"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_cpp.mak CFG="tuscany_sca_cpp - Win32 Debug" 
   cd "..\Build"

"tuscany_sca_cpp - Win32 DebugCLEAN" : 
   cd "..\tuscany_sca_cpp"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_cpp.mak CFG="tuscany_sca_cpp - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Build"

!ENDIF 

!IF  "$(CFG)" == "Build - Win32 Release"

"tuscany_sca_ws_reference - Win32 Release" : 
   cd "..\tuscany_sca_ws_reference"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_reference.mak CFG="tuscany_sca_ws_reference - Win32 Release" 
   cd "..\Build"

"tuscany_sca_ws_reference - Win32 ReleaseCLEAN" : 
   cd "..\tuscany_sca_ws_reference"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_reference.mak CFG="tuscany_sca_ws_reference - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Build"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

"tuscany_sca_ws_reference - Win32 Debug" : 
   cd "..\tuscany_sca_ws_reference"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_reference.mak CFG="tuscany_sca_ws_reference - Win32 Debug" 
   cd "..\Build"

"tuscany_sca_ws_reference - Win32 DebugCLEAN" : 
   cd "..\tuscany_sca_ws_reference"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_reference.mak CFG="tuscany_sca_ws_reference - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Build"

!ENDIF 

!IF  "$(CFG)" == "Build - Win32 Release"

"tuscany_sca_ws_service - Win32 Release" : 
   cd "..\tuscany_sca_ws_service"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_service.mak CFG="tuscany_sca_ws_service - Win32 Release" 
   cd "..\Build"

"tuscany_sca_ws_service - Win32 ReleaseCLEAN" : 
   cd "..\tuscany_sca_ws_service"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_service.mak CFG="tuscany_sca_ws_service - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Build"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

"tuscany_sca_ws_service - Win32 Debug" : 
   cd "..\tuscany_sca_ws_service"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_service.mak CFG="tuscany_sca_ws_service - Win32 Debug" 
   cd "..\Build"

"tuscany_sca_ws_service - Win32 DebugCLEAN" : 
   cd "..\tuscany_sca_ws_service"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_service.mak CFG="tuscany_sca_ws_service - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Build"

!ENDIF 

!IF  "$(CFG)" == "Build - Win32 Release"

"tuscany_sca_ws_dispatcher - Win32 Release" : 
   cd "..\tuscany_sca_ws_dispatcher"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_dispatcher.mak CFG="tuscany_sca_ws_dispatcher - Win32 Release" 
   cd "..\Build"

"tuscany_sca_ws_dispatcher - Win32 ReleaseCLEAN" : 
   cd "..\tuscany_sca_ws_dispatcher"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_dispatcher.mak CFG="tuscany_sca_ws_dispatcher - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Build"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

"tuscany_sca_ws_dispatcher - Win32 Debug" : 
   cd "..\tuscany_sca_ws_dispatcher"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_dispatcher.mak CFG="tuscany_sca_ws_dispatcher - Win32 Debug" 
   cd "..\Build"

"tuscany_sca_ws_dispatcher - Win32 DebugCLEAN" : 
   cd "..\tuscany_sca_ws_dispatcher"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_dispatcher.mak CFG="tuscany_sca_ws_dispatcher - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Build"

!ENDIF 

!IF  "$(CFG)" == "Build - Win32 Release"

"tuscany_sca_python - Win32 Release" : 
   cd "..\tuscany_sca_python"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_python.mak CFG="tuscany_sca_python - Win32 Release" 
   cd "..\Build"

"tuscany_sca_python - Win32 ReleaseCLEAN" : 
   cd "..\tuscany_sca_python"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_python.mak CFG="tuscany_sca_python - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Build"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

"tuscany_sca_python - Win32 Debug" : 
   cd "..\tuscany_sca_python"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_python.mak CFG="tuscany_sca_python - Win32 Debug" 
   cd "..\Build"

"tuscany_sca_python - Win32 DebugCLEAN" : 
   cd "..\tuscany_sca_python"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_python.mak CFG="tuscany_sca_python - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Build"

!ENDIF 

!IF  "$(CFG)" == "Build - Win32 Release"

"tuscany_sca_ruby - Win32 Release" : 
   cd "..\tuscany_sca_ruby"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ruby.mak CFG="tuscany_sca_ruby - Win32 Release" 
   cd "..\Build"

"tuscany_sca_ruby - Win32 ReleaseCLEAN" : 
   cd "..\tuscany_sca_ruby"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ruby.mak CFG="tuscany_sca_ruby - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Build"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

"tuscany_sca_ruby - Win32 Debug" : 
   cd "..\tuscany_sca_ruby"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ruby.mak CFG="tuscany_sca_ruby - Win32 Debug" 
   cd "..\Build"

"tuscany_sca_ruby - Win32 DebugCLEAN" : 
   cd "..\tuscany_sca_ruby"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ruby.mak CFG="tuscany_sca_ruby - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Build"

!ENDIF 


!ENDIF 

