
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
 
echo using AXIS2C: %AXIS2C_HOME%

call vcvars32

echo Build of SDO started....
set PACKAGE=tuscany_sdo_cpp
set VERSION=0.1.incubating-M1
set SRC_PACKAGE=%PACKAGE%-%VERSION%-src
set BIN_PACKAGE=%PACKAGE%-%VERSION%-bin

set FROM_DIR=%cd%
set TO_SRC=%FROM_DIR%

set TO_BIN=%FROM_DIR%

echo Copying SDO source... %TO_SRC%\%SRC_PACKAGE%

if not exist %TO_SRC% mkdir %TO_SRC%
rmdir /S /Q %TO_SRC%\%SRC_PACKAGE%
if not exist %TO_SRC%\%SRC_PACKAGE% mkdir %TO_SRC%\%SRC_PACKAGE%
if not exist %TO_SRC%\%SRC_PACKAGE%\projects mkdir %TO_SRC%\%SRC_PACKAGE%\projects 
if not exist %TO_SRC%\%SRC_PACKAGE%\runtime mkdir %TO_SRC%\%SRC_PACKAGE%\runtime
if not exist %TO_SRC%\%SRC_PACKAGE%\deploy mkdir %TO_SRC%\%SRC_PACKAGE%\deploy
 
xcopy /t /e  %FROM_DIR%\sdo\deploy %TO_SRC%\%SRC_PACKAGE%\deploy
xcopy/s %FROM_DIR%\sdo\runtime %TO_SRC%\%SRC_PACKAGE%\runtime 

xcopy/s %FROM_DIR%\sdo\projects %TO_SRC%\%SRC_PACKAGE%\projects
copy %FROM_DIR%\sdo\INSTALL   %TO_SRC%\%SRC_PACKAGE%
copy %FROM_DIR%\sdo\LICENSE   %TO_SRC%\%SRC_PACKAGE%
copy %FROM_DIR%\sdo\COPYING   %TO_SRC%\%SRC_PACKAGE%
copy %FROM_DIR%\sdo\NOTICE    %TO_SRC%\%SRC_PACKAGE%
copy %FROM_DIR%\sdo\README    %TO_SRC%\%SRC_PACKAGE%
copy %FROM_DIR%\sdo\readme.txt  %TO_SRC%\%SRC_PACKAGE%

echo Building SDO....

cd %FROM_DIR%\sdo\projects\tuscany_sdo

call build

echo Installing SDO....

if not exist %TO_BIN% mkdir %TO_BIN%
rmdir /S /Q %TO_BIN%\%BIN_PACKAGE%
if not exist %TO_BIN%\%BIN_PACKAGE% mkdir %TO_BIN%\%BIN_PACKAGE%

xcopy/s %FROM_DIR%\sdo\deploy %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\sdo\INSTALL   %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\sdo\LICENSE   %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\sdo\COPYING   %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\sdo\NOTICE    %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\sdo\README    %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\sdo\readme.txt   %TO_BIN%\%BIN_PACKAGE%

:end
echo SDO Build complete.
endlocal