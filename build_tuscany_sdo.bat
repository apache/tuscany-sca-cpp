@echo off

@REM  Licensed to the Apache Software Foundation (ASF) under one
@REM  or more contributor license agreements.  See the NOTICE file
@REM  distributed with this work for additional information
@REM  regarding copyright ownership.  The ASF licenses this file
@REM  to you under the Apache License, Version 2.0 (the
@REM  "License"); you may not use this file except in compliance
@REM  with the License.  You may obtain a copy of the License at
@REM  
@REM    http://www.apache.org/licenses/LICENSE-2.0
@REM    
@REM  Unless required by applicable law or agreed to in writing,
@REM  software distributed under the License is distributed on an
@REM  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
@REM  KIND, either express or implied.  See the License for the
@REM  specific language governing permissions and limitations
@REM  under the License.


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
set VERSION=1.0-incubator-M2
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
copy %FROM_DIR%\sdotest.bat  %TO_SRC%\%SRC_PACKAGE%


if not exist %TO_SRC%\%SRC_PACKAGE%\samples mkdir %TO_SRC%\%SRC_PACKAGE%\samples 
if not exist %TO_SRC%\%SRC_PACKAGE%\samples\misc mkdir %TO_SRC%\%SRC_PACKAGE%\samples\misc
if not exist %TO_SRC%\%SRC_PACKAGE%\samples\ides mkdir %TO_SRC%\%SRC_PACKAGE%\samples\ides
copy %FROM_DIR%\samples\INSTALL   %TO_SRC%\%SRC_PACKAGE%\samples
copy %FROM_DIR%\samples\LICENSE   %TO_SRC%\%SRC_PACKAGE%\samples
copy %FROM_DIR%\samples\COPYING   %TO_SRC%\%SRC_PACKAGE%\samples
copy %FROM_DIR%\samples\NOTICE    %TO_SRC%\%SRC_PACKAGE%\samples
copy %FROM_DIR%\samples\README    %TO_SRC%\%SRC_PACKAGE%\samples
copy %FROM_DIR%\samples\GettingStarted.html    %TO_SRC%\%SRC_PACKAGE%\samples

xcopy /s /e %FROM_DIR%\samples\misc %TO_SRC%\%SRC_PACKAGE%\samples\misc
xcopy/s %FROM_DIR%\samples\ides %TO_SRC%\%SRC_PACKAGE%\samples\ides

cd %TO_SRC%\%SRC_PACKAGE%
del /S Makefile.am

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


echo Building samples

set TUSCANY_SDOCPP=%FROM_DIR%\deploy
cd %FROM_DIR%\samples\ides\devstudio6\projects\misc

call build

if not exist %TO_BIN%\%BIN_PACKAGE%\samples mkdir %TO_BIN%\%BIN_PACKAGE%\samples
if not exist %TO_BIN%\%BIN_PACKAGE%\samples\ides mkdir %TO_BIN%\%BIN_PACKAGE%\samples\ides
if not exist %TO_BIN%\%BIN_PACKAGE%\samples\misc mkdir %TO_BIN%\%BIN_PACKAGE%\samples\misc

xcopy/s %FROM_DIR%\samples\ides %TO_BIN%\%BIN_PACKAGE%\samples\ides
xcopy/s /Y %FROM_DIR%\samples\misc %TO_BIN%\%BIN_PACKAGE%\samples\misc
copy %FROM_DIR%\samples\INSTALL   %TO_BIN%\%BIN_PACKAGE%\samples
copy %FROM_DIR%\samples\LICENSE   %TO_BIN%\%BIN_PACKAGE%\samples
copy %FROM_DIR%\samples\COPYING   %TO_BIN%\%BIN_PACKAGE%\samples
copy %FROM_DIR%\samples\NOTICE    %TO_BIN%\%BIN_PACKAGE%\samples
copy %FROM_DIR%\samples\README    %TO_BIN%\%BIN_PACKAGE%\samples
copy %FROM_DIR%\samples\GettingStarted.html    %TO_BIN%\%BIN_PACKAGE%\samples

cd %TO_BIN%\%BIN_PACKAGE%
del /S Makefile.am

echo Creating zip file: %TO_SRC%\%SRC_PACKAGE%.zip 
jar -cMf %TO_SRC%\%SRC_PACKAGE%.zip -C %TO_SRC% %SRC_PACKAGE%
echo Creating zip file: %TO_BIN%\%BIN_PACKAGE%.zip
jar -cMf %TO_BIN%\%BIN_PACKAGE%.zip -C %TO_BIN% %BIN_PACKAGE%

:end
echo SDO Build complete.
endlocal
