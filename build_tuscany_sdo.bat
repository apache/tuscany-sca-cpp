@echo off


@REM  Copyright 2006 The Apache Software Foundation or its licensors, as applicable.
@REM
@REM  Licensed under the Apache License, Version 2.0 (the "License");
@REM  you may not use this file except in compliance with the License.
@REM  You may obtain a copy of the License at
@REM
@REM     http://www.apache.org/licenses/LICENSE-2.0
@REM
@REM  Unless required by applicable law or agreed to in writing, software
@REM  distributed under the License is distributed on an "AS IS" BASIS,
@REM  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
@REM  See the License for the specific language governing permissions and
@REM  limitations under the License.

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

set FROM_DIR=%cd%\sdo
set TO_SRC=%cd%
set TO_BIN=%cd%

echo Copying SDO source... %TO_SRC%\%SRC_PACKAGE%

if not exist %TO_SRC% mkdir %TO_SRC%
rmdir /S /Q %TO_SRC%\%SRC_PACKAGE%
if not exist %TO_SRC%\%SRC_PACKAGE% mkdir %TO_SRC%\%SRC_PACKAGE%
if not exist %TO_SRC%\%SRC_PACKAGE%\projects mkdir %TO_SRC%\%SRC_PACKAGE%\projects
if not exist %TO_SRC%\%SRC_PACKAGE%\projectsvc7 mkdir %TO_SRC%\%SRC_PACKAGE%\projectsvc7 
if not exist %TO_SRC%\%SRC_PACKAGE%\runtime mkdir %TO_SRC%\%SRC_PACKAGE%\runtime
if not exist %TO_SRC%\%SRC_PACKAGE%\deploy mkdir %TO_SRC%\%SRC_PACKAGE%\deploy
if not exist %TO_SRC%\%SRC_PACKAGE%\doc mkdir %TO_SRC%\%SRC_PACKAGE%\doc
 
xcopy /t /e  %FROM_DIR%\deploy %TO_SRC%\%SRC_PACKAGE%\deploy
xcopy/s %FROM_DIR%\runtime %TO_SRC%\%SRC_PACKAGE%\runtime 
xcopy/s %FROM_DIR%\projectsvc7 %TO_SRC%\%SRC_PACKAGE%\projectsvc7
xcopy/s %FROM_DIR%\doc %TO_SRC%\%SRC_PACKAGE%\doc

xcopy/s %FROM_DIR%\projects %TO_SRC%\%SRC_PACKAGE%\projects
copy %FROM_DIR%\INSTALL   %TO_SRC%\%SRC_PACKAGE%
copy %FROM_DIR%\LICENSE   %TO_SRC%\%SRC_PACKAGE%
copy %FROM_DIR%\COPYING   %TO_SRC%\%SRC_PACKAGE%
copy %FROM_DIR%\NOTICE    %TO_SRC%\%SRC_PACKAGE%
copy %FROM_DIR%\README    %TO_SRC%\%SRC_PACKAGE%
copy %FROM_DIR%\GettingStarted.html  %TO_SRC%\%SRC_PACKAGE%
copy %FROM_DIR%\build.bat  %TO_SRC%\%SRC_PACKAGE%

echo Building SDO....

cd %FROM_DIR%
call build

echo Installing SDO....

if not exist %TO_BIN% mkdir %TO_BIN%
rmdir /S /Q %TO_BIN%\%BIN_PACKAGE%
if not exist %TO_BIN%\%BIN_PACKAGE% mkdir %TO_BIN%\%BIN_PACKAGE%
if not exist %TO_BIN%\%BIN_PACKAGE%\doc mkdir %TO_BIN%\%BIN_PACKAGE%\doc

xcopy/s %FROM_DIR%\deploy %TO_BIN%\%BIN_PACKAGE%
xcopy/s %FROM_DIR%\doc %TO_BIN%\%BIN_PACKAGE%\doc
copy %FROM_DIR%\INSTALL   %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\LICENSE   %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\COPYING   %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\NOTICE    %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\README    %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\GettingStarted.html   %TO_BIN%\%BIN_PACKAGE%

:end
echo SDO Build complete.
endlocal
