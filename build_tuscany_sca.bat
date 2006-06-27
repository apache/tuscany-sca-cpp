
@echo off

echo Build started....

call vcvars32

set FROM_DIR=c:\apache\checkout_rc1

set TO_SRC=c:\installsrc

set TO_BIN=c:\installbin

echo Copying SCA source...

if not exist %TO_SRC% mkdir %TO_SRC%
if not exist %TO_SRC%\tuscany_sca-0.1.incubating-M1 mkdir %TO_SRC%\tuscany_sca-0.1.incubating-M1 

if not exist %TO_SRC%\tuscany_sca-0.1.incubating-M1\projects mkdir %TO_SRC%\tuscany_sca-0.1.incubating-M1\projects 

xcopy/s c:\apache\checkout_rc1\sca\projects %TO_SRC%\tuscany_sca-0.1.incubating-M1\projects

if not exist %TO_SRC%\tuscany_sca-0.1.incubating-M1\runtime mkdir %TO_SRC%\tuscany_sca-0.1.incubating-M1\runtime 

xcopy/s c:\apache\checkout_rc1\sca\runtime %TO_SRC%\tuscany_sca-0.1.incubating-M1\runtime

if not exist %TO_SRC%\tuscany_sca-0.1.incubating-M1\tools mkdir %TO_SRC%\tuscany_sca-0.1.incubating-M1\tools 

xcopy/s c:\apache\checkout_rc1\sca\tools %TO_SRC%\tuscany_sca-0.1.incubating-M1\tools

if not exist %TO_SRC%\tuscany_sca-0.1.incubating-M1\samples mkdir %TO_SRC%\tuscany_sca-0.1.incubating-M1\samples 

xcopy/s c:\apache\checkout_rc1\sca\samples %TO_SRC%\tuscany_sca-0.1.incubating-M1\samples

if not exist %TO_SRC%\tuscany_sca-0.1.incubating-M1\xsd mkdir %TO_SRC%\tuscany_sca-0.1.incubating-M1\xsd 

xcopy/s c:\apache\checkout_rc1\sca\xsd %TO_SRC%\tuscany_sca-0.1.incubating-M1\xsd


rem install libxml2 to c:\apache\libs
set LIBXML2_HOME=c:\apache\libs\libxml2-2.6.19.win32

rem install iconv to c:\apache\libs
set ICONV_HOME=c:\apache\libs\iconv-1.9.1.win32

rem install axis2c
set AXIS2C_HOME=c:\axis\axis2c_92\axis2c-bin-0.92-win32

echo Assuming SDO already built.

set TUSCANY_SDOCPP=%TO_BIN%\tuscany_sdo-bin-0.1.incubating-M1\deploy

echo Building SCA....

set TUSCANY_SCACPP=c:\apache\checkout_rc1\sca\deploy

cd c:\apache\checkout_rc1\sca\projects\tuscany_sca

call build

echo Building scagen...

set JAVA_HOME=c:\jvm5\ibm5

cd c:\apache\checkout_rc1\sca\tools\scagen

call ant

echo Installing SCA....

set TUSCANY_SCACPP=%TO_BIN%\tuscany_sca-bin-0.1.incubating-M1\deploy

if not exist %TO_BIN% mkdir %TO_BIN%
if not exist %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1 mkdir %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1 
if not exist %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1\deploy mkdir %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1\deploy 
if not exist %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1\tools mkdir %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1\tools 

xcopy/s c:\apache\checkout_rc1\sca\deploy %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1\deploy
copy c:\apache\checkout_rc1\sca\INSTALL   %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1
copy c:\apache\checkout_rc1\sca\LICENSE   %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1
copy c:\apache\checkout_rc1\sca\COPYING   %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1

xcopy/s c:\apache\checkout_rc1\sca\tools\scagen\bld %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1\tools

echo Building calculator....

cd c:\apache\checkout_rc1\sca\samples\ides\devstudio6\projects\Calculator

call build

if not exist %TO_BIN% mkdir %TO_BIN%
if not exist %TO_BIN%\sca mkdir %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1 
if not exist %TO_BIN%\sca\samples mkdir %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1\samples
if not exist %TO_BIN%\sca\samples\ides mkdir %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1\samples\ides
if not exist %TO_BIN%\sca\samples\Calculator mkdir %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1\samples\Calculator

xcopy/s c:\apache\checkout_rc1\sca\samples\ides %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1\samples\ides
xcopy/s c:\apache\checkout_rc1\sca\samples\Calculator %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1\samples\Calculator
 
set TUSCANY_SCACPP_SYSTEM_ROOT=%TO_BIN%\tuscany_sca-bin-0.1.incubating-M1\samples\Calculator\deploy
set TUSCANY_SCACPP_DEFAULT_MODULE=CalculatorSubsystem/CalculatorService

cd %TO_BIN%\tuscany_sca-bin-0.1.incubating-M1\samples\Calculator\deploy\modules\Calculator

set PATH=%TUSCANY_SDOCPP%\bin;%TUSCANY_SCACPP%\bin;%PATH%

echo %TUSCANY_SDOCPP%
echo %TUSCANY_SCACPP%
echo %TUSCANY_SCACPP_SYSTEM_ROOT%

echo Testing SCA...

call Calc add 34 56

echo SCA Build complete.
