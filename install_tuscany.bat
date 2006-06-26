
@echo off

echo Build started....

echo Copying the source tree...

echo Copying SCA source...


if not exist c:\installsrc mkdir c:\installsrc
if not exist c:\installsrc\tuscany_sdo-0.1.incubating-M1 mkdir c:\installsrc\tuscany_sdo-0.1.incubating-M1 
if not exist c:\installsrc mkdir c:\installsrc
if not exist c:\installsrc\tuscany_sca-0.1.incubating-M1 mkdir c:\installsrc\tuscany_sca-0.1.incubating-M1 

if not exist c:\installsrc\tuscany_sca-0.1.incubating-M1\projects mkdir c:\installsrc\tuscany_sca-0.1.incubating-M1\projects 

xcopy/s c:\apache\checkout_rc1\sca\projects c:\installsrc\tuscany_sca-0.1.incubating-M1\projects

if not exist c:\installsrc\tuscany_sca-0.1.incubating-M1\runtime mkdir c:\installsrc\tuscany_sca-0.1.incubating-M1\runtime 

xcopy/s c:\apache\checkout_rc1\sca\runtime c:\installsrc\tuscany_sca-0.1.incubating-M1\runtime

if not exist c:\installsrc\tuscany_sca-0.1.incubating-M1\tools mkdir c:\installsrc\tuscany_sca-0.1.incubating-M1\tools 

xcopy/s c:\apache\checkout_rc1\sca\tools c:\installsrc\tuscany_sca-0.1.incubating-M1\tools

if not exist c:\installsrc\tuscany_sca-0.1.incubating-M1\samples mkdir c:\installsrc\tuscany_sca-0.1.incubating-M1\samples 

xcopy/s c:\apache\checkout_rc1\sca\samples c:\installsrc\tuscany_sca-0.1.incubating-M1\samples

if not exist c:\installsrc\tuscany_sca-0.1.incubating-M1\xsd mkdir c:\installsrc\tuscany_sca-0.1.incubating-M1\xsd 

xcopy/s c:\apache\checkout_rc1\sca\xsd c:\installsrc\tuscany_sca-0.1.incubating-M1\xsd

echo Copying SDO source...

if not exist c:\installsrc\tuscany_sdo-0.1.incubating-M1\runtime mkdir c:\installsrc\tuscany_sdo-0.1.incubating-M1\runtime
 
xcopy/s c:\apache\checkout_rc1\sdo\runtime c:\installsrc\tuscany_sdo-0.1.incubating-M1\runtime 

if not exist c:\installsrc\tuscany_sdo-0.1.incubating-M1\projects mkdir c:\installsrc\tuscany_sdo-0.1.incubating-M1\projects
 
xcopy/s c:\apache\checkout_rc1\sdo\projects c:\installsrc\tuscany_sdo-0.1.incubating-M1\projects

copy c:\apache\checkout_rc1\sca\INSTALL   c:\installsrc\tuscany_sca-0.1.incubating-M1
copy c:\apache\checkout_rc1\sca\LICENSE   c:\installsrc\tuscany_sca-0.1.incubating-M1
copy c:\apache\checkout_rc1\sca\COPYING   c:\installsrc\tuscany_sca-0.1.incubating-M1

copy c:\apache\checkout_rc1\sdo\INSTALL   c:\installsrc\tuscany_sdo-0.1.incubating-M1
copy c:\apache\checkout_rc1\sdo\LICENSE   c:\installsrc\tuscany_sdo-0.1.incubating-M1
copy c:\apache\checkout_rc1\sdo\COPYING   c:\installsrc\tuscany_sdo-0.1.incubating-M1

rem install libxml2 to c:\apache\libs
set LIBXML2_HOME=c:\apache\libs\libxml2-2.6.19.win32

rem install iconv to c:\apache\libs
set ICONV_HOME=c:\apache\libs\iconv-1.9.1.win32

rem install axis2c
set AXIS2C_HOME=c:\axis\axis2c_92\axis2c-bin-0.92-win32

echo Building SDO....

set TUSCANY_SDOCPP=c:\apache\checkout_rc1\sdo\deploy

cd c:\apache\checkout_rc1\sdo\projects\tuscany_sdo

call build

echo Installing SDO....

if not exist c:\install mkdir c:\install
if not exist c:\install\tuscany_sdo-bin-0.1.incubating-M1 mkdir c:\install\tuscany_sdo-bin-0.1.incubating-M1 
if not exist c:\install\tuscany_sdo-bin-0.1.incubating-M1\deploy mkdir c:\install\tuscany_sdo-bin-0.1.incubating-M1\deploy 

xcopy/s c:\apache\checkout_rc1\sdo\deploy c:\install\tuscany_sdo-bin-0.1.incubating-M1\deploy
copy c:\apache\checkout_rc1\sdo\INSTALL   c:\install\tuscany_sdo-bin-0.1.incubating-M1
copy c:\apache\checkout_rc1\sdo\LICENSE   c:\install\tuscany_sdo-bin-0.1.incubating-M1
copy c:\apache\checkout_rc1\sdo\COPYING   c:\install\tuscany_sdo-bin-0.1.incubating-M1


set TUSCANY_SDOCPP=c:\install\tuscany_sdo-bin-0.1.incubating-M1\deploy

echo Building SCA....

set TUSCANY_SCACPP=c:\apache\checkout_rc1\sca\deploy

cd c:\apache\checkout_rc1\sca\projects\tuscany_sca

call build

echo Building scagen...

cd c:\apache\checkout_rc1\sca\tools\scagen

call ant

echo Installing SCA....

set TUSCANY_SCACPP=c:\install\tuscany_sca-bin-0.1.incubating-M1\deploy

if not exist c:\install mkdir c:\install
if not exist c:\install\tuscany_sca-bin-0.1.incubating-M1 mkdir c:\install\tuscany_sca-bin-0.1.incubating-M1 
if not exist c:\install\tuscany_sca-bin-0.1.incubating-M1\deploy mkdir c:\install\tuscany_sca-bin-0.1.incubating-M1\deploy 
if not exist c:\install\tuscany_sca-bin-0.1.incubating-M1\tools mkdir c:\install\tuscany_sca-bin-0.1.incubating-M1\tools 

xcopy/s c:\apache\checkout_rc1\sca\deploy c:\install\tuscany_sca-bin-0.1.incubating-M1\deploy
copy c:\apache\checkout_rc1\sca\INSTALL   c:\install\tuscany_sca-bin-0.1.incubating-M1
copy c:\apache\checkout_rc1\sca\LICENSE   c:\install\tuscany_sca-bin-0.1.incubating-M1
copy c:\apache\checkout_rc1\sca\COPYING   c:\install\tuscany_sca-bin-0.1.incubating-M1

xcopy/s c:\apache\checkout_rc1\sca\tools\scagen\bld c:\install\tuscany_sca-bin-0.1.incubating-M1\tools

echo Building calculator....

cd c:\apache\checkout_rc1\sca\samples\ides\devstudio6\projects\Calculator

call build

if not exist c:\install mkdir c:\install
if not exist c:\install\sca mkdir c:\install\tuscany_sca-bin-0.1.incubating-M1 
if not exist c:\install\sca\samples mkdir c:\install\tuscany_sca-bin-0.1.incubating-M1\samples
if not exist c:\install\sca\samples\ides mkdir c:\install\tuscany_sca-bin-0.1.incubating-M1\samples\ides
if not exist c:\install\sca\samples\Calculator mkdir c:\install\tuscany_sca-bin-0.1.incubating-M1\samples\Calculator

xcopy/s c:\apache\checkout_rc1\sca\samples\ides c:\install\tuscany_sca-bin-0.1.incubating-M1\samples\ides
xcopy/s c:\apache\checkout_rc1\sca\samples\Calculator c:\install\tuscany_sca-bin-0.1.incubating-M1\samples\Calculator
 
set TUSCANY_SCACPP_SYSTEM_ROOT=c:\install\tuscany_sca-bin-0.1.incubating-M1\samples\Calculator\deploy
set TUSCANY_SCACPP_DEFAULT_MODULE=CalculatorSubsystem/CalculatorService

cd c:\install\tuscany_sca-bin-0.1.incubating-M1\samples\Calculator\deploy\modules\Calculator

set PATH=%TUSCANY_SDOCPP%\bin;%TUSCANY_SCACPP%\bin;%PATH%

echo %TUSCANY_SDOCPP%
echo %TUSCANY_SCACPP%
echo %TUSCANY_SCACPP_SYSTEM_ROOT%

echo Testing SCA...

call Calc add 34 56



echo Build complete.
