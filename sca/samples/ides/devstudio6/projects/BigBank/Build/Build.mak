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

ALL : "WSAccountClient - Win32 Release" "AccountClient - Win32 Release" "Accounts - Win32 Release" 

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"Accounts - Win32 ReleaseCLEAN" "AccountClient - Win32 ReleaseCLEAN" "WSAccountClient - Win32 ReleaseCLEAN" 
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

"Accounts - Win32 Release" : 
   cd "..Accounts"
   $(MAKE) /$(MAKEFLAGS) /F .\Accounts.mak CFG="Accounts - Win32 Release" 
   cd "..\Build"

"Accounts - Win32 ReleaseCLEAN" : 
   cd "..Accounts"
   $(MAKE) /$(MAKEFLAGS) /F .\Accounts.mak CFG="Accounts - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Build"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

"Accounts - Win32 Debug" : 
   cd "..Accounts"
   $(MAKE) /$(MAKEFLAGS) /F .\Accounts.mak CFG="Accounts - Win32 Debug" 
   cd "..\Build"

"Accounts - Win32 DebugCLEAN" : 
   cd "..Accounts"
   $(MAKE) /$(MAKEFLAGS) /F .\Accounts.mak CFG="Accounts - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Build"

!ENDIF 

!IF  "$(CFG)" == "Build - Win32 Release"

"AccountClient - Win32 Release" : 
   cd "..AccountClient"
   $(MAKE) /$(MAKEFLAGS) /F .\AccountClient.mak CFG="AccountClient - Win32 Release" 
   cd "..\Build"

"AccountClient - Win32 ReleaseCLEAN" : 
   cd "..AccountClient"
   $(MAKE) /$(MAKEFLAGS) /F .\AccountClient.mak CFG="AccountClient - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Build"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

"AccountClient - Win32 Debug" : 
   cd "..AccountClient"
   $(MAKE) /$(MAKEFLAGS) /F .\AccountClient.mak CFG="AccountClient - Win32 Debug" 
   cd "..\Build"

"AccountClient - Win32 DebugCLEAN" : 
   cd "..AccountClient"
   $(MAKE) /$(MAKEFLAGS) /F .\AccountClient.mak CFG="AccountClient - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Build"

!ENDIF 

!IF  "$(CFG)" == "Build - Win32 Release"

"WSAccountClient - Win32 Release" : 
   cd "..WSAccountClient"
   $(MAKE) /$(MAKEFLAGS) /F .\WSAccountClient.mak CFG="WSAccountClient - Win32 Release" 
   cd "..\Build"

"WSAccountClient - Win32 ReleaseCLEAN" : 
   cd "..WSAccountClient"
   $(MAKE) /$(MAKEFLAGS) /F .\WSAccountClient.mak CFG="WSAccountClient - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Build"

!ELSEIF  "$(CFG)" == "Build - Win32 Debug"

"WSAccountClient - Win32 Debug" : 
   cd "..WSAccountClient"
   $(MAKE) /$(MAKEFLAGS) /F .\WSAccountClient.mak CFG="WSAccountClient - Win32 Debug" 
   cd "..\Build"

"WSAccountClient - Win32 DebugCLEAN" : 
   cd "..WSAccountClient"
   $(MAKE) /$(MAKEFLAGS) /F .\WSAccountClient.mak CFG="WSAccountClient - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Build"

!ENDIF 


!ENDIF 

