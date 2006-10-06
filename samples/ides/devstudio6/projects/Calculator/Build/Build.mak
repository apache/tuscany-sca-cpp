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

ALL : "WSClient - Win32 Release" "Client - Win32 Release" "Calculator - Win32 Release" 

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"Calculator - Win32 ReleaseCLEAN" "Client - Win32 ReleaseCLEAN" "WSClient - Win32 ReleaseCLEAN" 
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

"Calculator - Win32 Release" : 
   cd "..\Calculator"
   $(MAKE) /$(MAKEFLAGS) /F .\Calculator.mak CFG="Calculator - Win32 Release" 
   cd "..\Build"

"Calculator - Win32 ReleaseCLEAN" : 
   cd "..\Calculator"
   $(MAKE) /$(MAKEFLAGS) /F .\Calculator.mak CFG="Calculator - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Build"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

"Calculator - Win32 Debug" : 
   cd "..\Calculator"
   $(MAKE) /$(MAKEFLAGS) /F .\Calculator.mak CFG="Calculator - Win32 Debug" 
   cd "..\Build"

"Calculator - Win32 DebugCLEAN" : 
   cd "..\Calculator"
   $(MAKE) /$(MAKEFLAGS) /F .\Calculator.mak CFG="Calculator - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Build"

!ENDIF 

!IF  "$(CFG)" == "Build - Win32 Release"

"Client - Win32 Release" : 
   cd "..\Client"
   $(MAKE) /$(MAKEFLAGS) /F .\Client.mak CFG="Client - Win32 Release" 
   cd "..\Build"

"Client - Win32 ReleaseCLEAN" : 
   cd "..\Client"
   $(MAKE) /$(MAKEFLAGS) /F .\Client.mak CFG="Client - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Build"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

"Client - Win32 Debug" : 
   cd "..\Client"
   $(MAKE) /$(MAKEFLAGS) /F .\Client.mak CFG="Client - Win32 Debug" 
   cd "..\Build"

"Client - Win32 DebugCLEAN" : 
   cd "..\Client"
   $(MAKE) /$(MAKEFLAGS) /F .\Client.mak CFG="Client - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Build"

!ENDIF 

!IF  "$(CFG)" == "Build - Win32 Release"

"WSClient - Win32 Release" : 
   cd "..\WSClient"
   $(MAKE) /$(MAKEFLAGS) /F .\WSClient.mak CFG="WSClient - Win32 Release" 
   cd "..\Build"

"WSClient - Win32 ReleaseCLEAN" : 
   cd "..\WSClient"
   $(MAKE) /$(MAKEFLAGS) /F .\WSClient.mak CFG="WSClient - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Build"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

"WSClient - Win32 Debug" : 
   cd "..\WSClient"
   $(MAKE) /$(MAKEFLAGS) /F .\WSClient.mak CFG="WSClient - Win32 Debug" 
   cd "..\Build"

"WSClient - Win32 DebugCLEAN" : 
   cd "..\WSClient"
   $(MAKE) /$(MAKEFLAGS) /F .\WSClient.mak CFG="WSClient - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Build"

!ENDIF 


!ENDIF 

