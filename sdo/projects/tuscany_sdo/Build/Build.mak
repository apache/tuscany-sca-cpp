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

ALL : "sdo_test - Win32 Release" "sdo_runtime - Win32 Release" "sdo_axiom_test - Win32 Release" "sdo_axiom - Win32 Release" 

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"sdo_axiom - Win32 ReleaseCLEAN" "sdo_axiom_test - Win32 ReleaseCLEAN" "sdo_runtime - Win32 ReleaseCLEAN" "sdo_test - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe

!IF  "$(CFG)" == "Build - Win32 Release"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

!ENDIF 

MTL_PROJ=

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Build.dep")
!INCLUDE "Build.dep"
!ELSE 
!MESSAGE Warning: cannot find "Build.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Build - Win32 Release" || "$(CFG)" == "Build - Win32 Debug"

!IF  "$(CFG)" == "Build - Win32 Release"

"sdo_axiom - Win32 Release" : 
   cd "..\sdo_axiom"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_axiom.mak CFG="sdo_axiom - Win32 Release" 
   cd "..\Build"

"sdo_axiom - Win32 ReleaseCLEAN" : 
   cd "..\sdo_axiom"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_axiom.mak CFG="sdo_axiom - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Build"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

"sdo_axiom - Win32 Debug" : 
   cd "..\sdo_axiom"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_axiom.mak CFG="sdo_axiom - Win32 Debug" 
   cd "..\Build"

"sdo_axiom - Win32 DebugCLEAN" : 
   cd "..\sdo_axiom"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_axiom.mak CFG="sdo_axiom - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Build"

!ENDIF 

!IF  "$(CFG)" == "Build - Win32 Release"

"sdo_axiom_test - Win32 Release" : 
   cd "..\sdo_axiom_test"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_axiom_test.mak CFG="sdo_axiom_test - Win32 Release" 
   cd "..\Build"

"sdo_axiom_test - Win32 ReleaseCLEAN" : 
   cd "..\sdo_axiom_test"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_axiom_test.mak CFG="sdo_axiom_test - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Build"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

"sdo_axiom_test - Win32 Debug" : 
   cd "..\sdo_axiom_test"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_axiom_test.mak CFG="sdo_axiom_test - Win32 Debug" 
   cd "..\Build"

"sdo_axiom_test - Win32 DebugCLEAN" : 
   cd "..\sdo_axiom_test"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_axiom_test.mak CFG="sdo_axiom_test - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Build"

!ENDIF 

!IF  "$(CFG)" == "Build - Win32 Release"

"sdo_runtime - Win32 Release" : 
   cd "..\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Release" 
   cd "..\Build"

"sdo_runtime - Win32 ReleaseCLEAN" : 
   cd "..\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Build"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

"sdo_runtime - Win32 Debug" : 
   cd "..\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Debug" 
   cd "..\Build"

"sdo_runtime - Win32 DebugCLEAN" : 
   cd "..\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Build"

!ENDIF 

!IF  "$(CFG)" == "Build - Win32 Release"

"sdo_test - Win32 Release" : 
   cd "..\sdo_test"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_test.mak CFG="sdo_test - Win32 Release" 
   cd "..\Build"

"sdo_test - Win32 ReleaseCLEAN" : 
   cd "..\sdo_test"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_test.mak CFG="sdo_test - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Build"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

"sdo_test - Win32 Debug" : 
   cd "..\sdo_test"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_test.mak CFG="sdo_test - Win32 Debug" 
   cd "..\Build"

"sdo_test - Win32 DebugCLEAN" : 
   cd "..\sdo_test"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_test.mak CFG="sdo_test - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Build"

!ENDIF 


!ENDIF 

