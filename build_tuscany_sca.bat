
@echo off
setlocal
if "%LIBXML2_HOME%" == "" (
echo "LIBXML2_HOME not set"
goto end
)
echo using LIBXML2: %LIBXML2_HOME%

if "%ICONV_HOME%" == "" (
echo "ICONV_HOME not set"
goto end
)
echo using ICONV: %ICONV_HOME%"

if "%AXIS2C_HOME%" == "" (
echo "AXIS2C_HOME not set"
goto end
)
echo using Axis2C: %AXIS2C_HOME%"

if "%TUSCANY_SDOCPP%" == "" (
echo "TUSCANY_SDOCPP not set"
set TUSCANY_SDOCPP=%cd%\sdo
goto end
)
 
echo using TUSCANY_SDOCPP: %TUSCANY_SDOCPP%

call vcvars32

echo Build of SCA started....
set PACKAGE=tuscany_sca_cpp
set VERSION=0.1.incubating-M1
set SRC_PACKAGE=%PACKAGE%-%VERSION%-src
set BIN_PACKAGE=%PACKAGE%-%VERSION%-bin

set FROM_DIR=%cd%\sca
set TO_SRC=%cd%

set TO_BIN=%cd%

echo Copying SCA source...

if not exist %TO_SRC% mkdir %TO_SRC%

rmdir /S /Q %TO_SRC%\%SRC_PACKAGE%
if not exist %TO_SRC%\%SRC_PACKAGE% mkdir %TO_SRC%\%SRC_PACKAGE% 
copy %FROM_DIR%\INSTALL   %TO_SRC%\%SRC_PACKAGE%
copy %FROM_DIR%\LICENSE   %TO_SRC%\%SRC_PACKAGE%
copy %FROM_DIR%\COPYING   %TO_SRC%\%SRC_PACKAGE%
copy %FROM_DIR%\NOTICE    %TO_SRC%\%SRC_PACKAGE%
copy %FROM_DIR%\README    %TO_SRC%\%SRC_PACKAGE%
copy %FROM_DIR%\readme.txt  %TO_SRC%\%SRC_PACKAGE%

if not exist %TO_SRC%\%SRC_PACKAGE%\deploy mkdir %TO_SRC%\%SRC_PACKAGE%\deploy
xcopy /t /e  %FROM_DIR%\deploy %TO_SRC%\%SRC_PACKAGE%\deploy

if not exist %TO_SRC%\%SRC_PACKAGE%\projects mkdir %TO_SRC%\%SRC_PACKAGE%\projects 
xcopy/s %FROM_DIR%\projects %TO_SRC%\%SRC_PACKAGE%\projects

if not exist %TO_SRC%\%SRC_PACKAGE%\projectsvc7 mkdir %TO_SRC%\%SRC_PACKAGE%\projectsvc7
xcopy/s %FROM_DIR%\projectsvc7 %TO_SRC%\%SRC_PACKAGE%\projectsvc7

if not exist %TO_SRC%\%SRC_PACKAGE%\runtime mkdir %TO_SRC%\%SRC_PACKAGE%\runtime 
xcopy/s %FROM_DIR%\runtime %TO_SRC%\%SRC_PACKAGE%\runtime

if not exist %TO_SRC%\%SRC_PACKAGE%\tools mkdir %TO_SRC%\%SRC_PACKAGE%\tools 
xcopy/s %FROM_DIR%\tools %TO_SRC%\%SRC_PACKAGE%\tools

if not exist %TO_SRC%\%SRC_PACKAGE%\samples mkdir %TO_SRC%\%SRC_PACKAGE%\samples 
if not exist %TO_SRC%\%SRC_PACKAGE%\samples\Calculator mkdir %TO_SRC%\%SRC_PACKAGE%\samples\Calculator
if not exist %TO_SRC%\%SRC_PACKAGE%\samples\ides mkdir %TO_SRC%\%SRC_PACKAGE%\samples\ides

xcopy/s %FROM_DIR%\samples\Calculator %TO_SRC%\%SRC_PACKAGE%\samples\Calculator
xcopy/s %FROM_DIR%\samples\ides %TO_SRC%\%SRC_PACKAGE%\samples\ides
copy %FROM_DIR%\samples\INSTALL   %TO_SRC%\%SRC_PACKAGE%\samples
copy %FROM_DIR%\samples\LICENSE   %TO_SRC%\%SRC_PACKAGE%\samples
copy %FROM_DIR%\samples\COPYING   %TO_SRC%\%SRC_PACKAGE%\samples
copy %FROM_DIR%\samples\NOTICE    %TO_SRC%\%SRC_PACKAGE%\samples
copy %FROM_DIR%\samples\README    %TO_SRC%\%SRC_PACKAGE%\samples

if not exist %TO_SRC%\%SRC_PACKAGE%\xsd mkdir %TO_SRC%\%SRC_PACKAGE%\xsd 
xcopy/s %FROM_DIR%\xsd %TO_SRC%\%SRC_PACKAGE%\xsd


echo Building SCA....

cd %FROM_DIR%\projects\tuscany_sca

call build

echo Building scagen...

cd %FROM_DIR%\tools\scagen

call ant

copy %FROM_DIR%\tools\scagen\bld\scagen.jar  %FROM_DIR%\deploy\bin
copy %FROM_DIR%\tools\scagen\bld\scagen.cmd  %FROM_DIR%\deploy\bin

echo Installing SCA....

if not exist %TO_BIN% mkdir %TO_BIN%
rmdir /S /Q %TO_BIN%\%BIN_PACKAGE%
if not exist %TO_BIN%\%BIN_PACKAGE% mkdir %TO_BIN%\%BIN_PACKAGE%

xcopy/s %FROM_DIR%\deploy %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\INSTALL   %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\LICENSE   %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\COPYING   %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\NOTICE    %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\README    %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\readme.txt   %TO_BIN%\%BIN_PACKAGE%

echo Building calculator....

set TUSCANY_SCACPP=%FROM_DIR%\deploy
cd %FROM_DIR%\samples\ides\devstudio6\projects\Calculator

call build

if not exist %TO_BIN%\%BIN_PACKAGE%\samples mkdir %TO_BIN%\%BIN_PACKAGE%\samples
if not exist %TO_BIN%\%BIN_PACKAGE%\samples\ides mkdir %TO_BIN%\%BIN_PACKAGE%\samples\ides
if not exist %TO_BIN%\%BIN_PACKAGE%\samples\Calculator mkdir %TO_BIN%\%BIN_PACKAGE%\samples\Calculator

xcopy/s %FROM_DIR%\samples\ides %TO_BIN%\%BIN_PACKAGE%\samples\ides
xcopy/s %FROM_DIR%\samples\Calculator %TO_BIN%\%BIN_PACKAGE%\samples\Calculator
copy %FROM_DIR%\samples\INSTALL   %TO_BIN%\%BIN_PACKAGE%\samples
copy %FROM_DIR%\samples\LICENSE   %TO_BIN%\%BIN_PACKAGE%\samples
copy %FROM_DIR%\samples\COPYING   %TO_BIN%\%BIN_PACKAGE%\samples
copy %FROM_DIR%\samples\NOTICE    %TO_BIN%\%BIN_PACKAGE%\samples
copy %FROM_DIR%\samples\README    %TO_BIN%\%BIN_PACKAGE%\samples

:end
echo SCA Build complete.
endlocal
