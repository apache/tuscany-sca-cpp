
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

if "%TUSCANY_SDOCPP%" == "" (
echo "TUSCANY_SDOCPP not set"
goto end
)
 
echo using TUSCANY_SDOCPP: %TUSCANY_SDOCPP%

call vcvars32

echo Build of SCA started....
set PACKAGE=tuscany_sca_cpp
set VERSION=0.1.incubating-M1
set SRC_PACKAGE=%PACKAGE%-%VERSION%-src
set BIN_PACKAGE=%PACKAGE%-%VERSION%-bin

set FROM_DIR=%cd%
set TO_SRC=%FROM_DIR%

set TO_BIN=%FROM_DIR%

echo Copying SCA source...

if not exist %TO_SRC% mkdir %TO_SRC%

rmdir /S /Q %TO_SRC%\%SRC_PACKAGE%
if not exist %TO_SRC%\%SRC_PACKAGE% mkdir %TO_SRC%\%SRC_PACKAGE% 

if not exist %TO_SRC%\%SRC_PACKAGE%\projects mkdir %TO_SRC%\%SRC_PACKAGE%\projects 

xcopy/s %FROM_DIR%\sca\projects %TO_SRC%\%SRC_PACKAGE%\projects

if not exist %TO_SRC%\%SRC_PACKAGE%\runtime mkdir %TO_SRC%\%SRC_PACKAGE%\runtime 

xcopy/s %FROM_DIR%\sca\runtime %TO_SRC%\%SRC_PACKAGE%\runtime

if not exist %TO_SRC%\%SRC_PACKAGE%\tools mkdir %TO_SRC%\%SRC_PACKAGE%\tools 

xcopy/s %FROM_DIR%\sca\tools %TO_SRC%\%SRC_PACKAGE%\tools

if not exist %TO_SRC%\%SRC_PACKAGE%\samples mkdir %TO_SRC%\%SRC_PACKAGE%\samples 
if not exist %TO_SRC%\%SRC_PACKAGE%\samples\Calculator mkdir %TO_SRC%\%SRC_PACKAGE%\samples\Calculator
if not exist %TO_SRC%\%SRC_PACKAGE%\samples\ides mkdir %TO_SRC%\%SRC_PACKAGE%\samples\ides

xcopy/s %FROM_DIR%\sca\samples\Calculator %TO_SRC%\%SRC_PACKAGE%\samples\Calculator
xcopy/s %FROM_DIR%\sca\samples\ides %TO_SRC%\%SRC_PACKAGE%\samples\ides

if not exist %TO_SRC%\%SRC_PACKAGE%\xsd mkdir %TO_SRC%\%SRC_PACKAGE%\xsd 

xcopy/s %FROM_DIR%\sca\xsd %TO_SRC%\%SRC_PACKAGE%\xsd


echo Building SCA....

cd %FROM_DIR%\sca\projects\tuscany_sca

call build

echo Building scagen...

cd %FROM_DIR%\sca\tools\scagen

call ant

copy %FROM_DIR%\sca\tools\scagen\bld\scagen.jar  %FROM_DIR%\sca\deploy\bin
copy %FROM_DIR%\sca\tools\scagen\bld\scagen.cmd  %FROM_DIR%\sca\deploy\bin

echo Installing SCA....

if not exist %TO_BIN% mkdir %TO_BIN%
rmdir /S /Q %TO_BIN%\%BIN_PACKAGE%
if not exist %TO_BIN%\%BIN_PACKAGE% mkdir %TO_BIN%\%BIN_PACKAGE%

xcopy/s %FROM_DIR%\sca\deploy %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\sca\INSTALL   %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\sca\LICENSE   %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\sca\COPYING   %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\sca\NOTICE    %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\sca\README    %TO_BIN%\%BIN_PACKAGE%

echo Building calculator....

set TUSCANY_SCACPP=%FROM_DIR%\sca\deploy
cd %FROM_DIR%\sca\samples\ides\devstudio6\projects\Calculator

call build

if not exist %TO_BIN%\%BIN_PACKAGE%\samples mkdir %TO_BIN%\%BIN_PACKAGE%\samples
if not exist %TO_BIN%\%BIN_PACKAGE%\samples\ides mkdir %TO_BIN%\%BIN_PACKAGE%\samples\ides
if not exist %TO_BIN%\%BIN_PACKAGE%\samples\Calculator mkdir %TO_BIN%\%BIN_PACKAGE%\samples\Calculator

xcopy/s %FROM_DIR%\sca\samples\ides %TO_BIN%\%BIN_PACKAGE%\samples\ides
xcopy/s %FROM_DIR%\sca\samples\Calculator %TO_BIN%\%BIN_PACKAGE%\samples\Calculator

:end
echo SCA Build complete.
endlocal
