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
echo using Axis2C: %AXIS2C_HOME%"

call vcvars32

echo Build of SCA started....
set PACKAGE=tuscany_sca_native
set VERSION=1.0-incubator-M3
set SRC_PACKAGE=%PACKAGE%-%VERSION%-src
set BIN_PACKAGE=%PACKAGE%-%VERSION%-bin

if "%TUSCANY_SDOCPP%" == "" (
echo "TUSCANY_SDOCPP not set"
set TUSCANY_SDOCPP=%CD%\..\tuscany_sdo_cpp-%VERSION%-bin
)
 
echo using TUSCANY_SDOCPP: %TUSCANY_SDOCPP%

cd ..
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
copy %FROM_DIR%\GettingStarted.html  %TO_SRC%\%SRC_PACKAGE%
copy %FROM_DIR%\build.bat  %TO_SRC%\%SRC_PACKAGE%


if not exist %TO_SRC%\%SRC_PACKAGE%\VSExpress mkdir %TO_SRC%\%SRC_PACKAGE%\VSExpress
xcopy/s %FROM_DIR%\VSExpress %TO_SRC%\%SRC_PACKAGE%\VSExpress


if not exist %TO_SRC%\%SRC_PACKAGE%\runtime mkdir %TO_SRC%\%SRC_PACKAGE%\runtime 
if not exist %TO_SRC%\%SRC_PACKAGE%\runtime\core mkdir %TO_SRC%\%SRC_PACKAGE%\runtime\core 
if not exist %TO_SRC%\%SRC_PACKAGE%\runtime\extensions mkdir %TO_SRC%\%SRC_PACKAGE%\runtime\extensions
if not exist %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\cpp mkdir %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\cpp
if not exist %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\ws mkdir %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\ws
if not exist %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\python mkdir %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\python
if not exist %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\ruby mkdir %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\ruby
if not exist %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\rest mkdir %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\rest
if not exist %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\sca mkdir %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\sca

xcopy/s %FROM_DIR%\runtime\core %TO_SRC%\%SRC_PACKAGE%\runtime\core
xcopy/s %FROM_DIR%\runtime\extensions\cpp %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\cpp
xcopy/s %FROM_DIR%\runtime\extensions\ws %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\ws
xcopy/s %FROM_DIR%\runtime\extensions\python %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\python
xcopy/s %FROM_DIR%\runtime\extensions\ruby %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\ruby
xcopy/s %FROM_DIR%\runtime\extensions\rest %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\rest
xcopy/s %FROM_DIR%\runtime\extensions\sca %TO_SRC%\%SRC_PACKAGE%\runtime\extensions\sca

if not exist %TO_SRC%\%SRC_PACKAGE%\tools mkdir %TO_SRC%\%SRC_PACKAGE%\tools 
xcopy/s %FROM_DIR%\tools %TO_SRC%\%SRC_PACKAGE%\tools

if not exist %TO_SRC%\%SRC_PACKAGE%\doc mkdir %TO_SRC%\%SRC_PACKAGE%\doc 
xcopy/s %FROM_DIR%\doc %TO_SRC%\%SRC_PACKAGE%\doc

if not exist %TO_SRC%\%SRC_PACKAGE%\samples                  mkdir %TO_SRC%\%SRC_PACKAGE%\samples 
if not exist %TO_SRC%\%SRC_PACKAGE%\samples\AlertAggregator          mkdir %TO_SRC%\%SRC_PACKAGE%\samples\AlertAggregator
if not exist %TO_SRC%\%SRC_PACKAGE%\samples\CppBigBank          mkdir %TO_SRC%\%SRC_PACKAGE%\samples\CppBigBank
if not exist %TO_SRC%\%SRC_PACKAGE%\samples\CppCalculator       mkdir %TO_SRC%\%SRC_PACKAGE%\samples\CppCalculator
if not exist %TO_SRC%\%SRC_PACKAGE%\samples\HttpdBigBank          mkdir %TO_SRC%\%SRC_PACKAGE%\samples\HttpdBigBank
if not exist %TO_SRC%\%SRC_PACKAGE%\samples\PythonCalculator mkdir %TO_SRC%\%SRC_PACKAGE%\samples\PythonCalculator
if not exist %TO_SRC%\%SRC_PACKAGE%\samples\PythonWeatherForecast mkdir %TO_SRC%\%SRC_PACKAGE%\samples\PythonWeatherForecast
if not exist %TO_SRC%\%SRC_PACKAGE%\samples\RestCalculator   mkdir %TO_SRC%\%SRC_PACKAGE%\samples\RestCalculator
if not exist %TO_SRC%\%SRC_PACKAGE%\samples\RestCustomer   mkdir %TO_SRC%\%SRC_PACKAGE%\samples\RestCustomer
if not exist %TO_SRC%\%SRC_PACKAGE%\samples\RestYahoo   mkdir %TO_SRC%\%SRC_PACKAGE%\samples\RestYahoo
if not exist %TO_SRC%\%SRC_PACKAGE%\samples\RubyBank         mkdir %TO_SRC%\%SRC_PACKAGE%\samples\RubyBank
if not exist %TO_SRC%\%SRC_PACKAGE%\samples\RubyCalculator   mkdir %TO_SRC%\%SRC_PACKAGE%\samples\RubyCalculator

xcopy/s %FROM_DIR%\samples\AlertAggregator         %TO_SRC%\%SRC_PACKAGE%\samples\AlertAggregator
xcopy/s %FROM_DIR%\samples\CppBigBank          %TO_SRC%\%SRC_PACKAGE%\samples\CppBigBank
xcopy/s %FROM_DIR%\samples\CppCalculator       %TO_SRC%\%SRC_PACKAGE%\samples\CppCalculator
xcopy/s %FROM_DIR%\samples\HttpdBigBank          %TO_SRC%\%SRC_PACKAGE%\samples\HttpdBigBank
xcopy/s %FROM_DIR%\samples\PythonCalculator %TO_SRC%\%SRC_PACKAGE%\samples\PythonCalculator
xcopy/s %FROM_DIR%\samples\PythonWeatherForecast %TO_SRC%\%SRC_PACKAGE%\samples\PythonWeatherForecast
xcopy/s %FROM_DIR%\samples\RestCalculator   %TO_SRC%\%SRC_PACKAGE%\samples\RestCalculator
xcopy/s %FROM_DIR%\samples\RestCustomer   %TO_SRC%\%SRC_PACKAGE%\samples\RestCustomer
xcopy/s %FROM_DIR%\samples\RestYahoo   %TO_SRC%\%SRC_PACKAGE%\samples\RestYahoo
xcopy/s %FROM_DIR%\samples\RubyBank         %TO_SRC%\%SRC_PACKAGE%\samples\RubyBank
xcopy/s %FROM_DIR%\samples\RubyCalculator   %TO_SRC%\%SRC_PACKAGE%\samples\RubyCalculator
copy %FROM_DIR%\samples\scagen.bat   %TO_SRC%\%SRC_PACKAGE%\samples
copy %FROM_DIR%\samples\INSTALL   %TO_SRC%\%SRC_PACKAGE%\samples
copy %FROM_DIR%\samples\LICENSE   %TO_SRC%\%SRC_PACKAGE%\samples
copy %FROM_DIR%\samples\COPYING   %TO_SRC%\%SRC_PACKAGE%\samples
copy %FROM_DIR%\samples\NOTICE    %TO_SRC%\%SRC_PACKAGE%\samples
copy %FROM_DIR%\samples\README    %TO_SRC%\%SRC_PACKAGE%\samples
copy %FROM_DIR%\samples\GettingStarted.html    %TO_SRC%\%SRC_PACKAGE%\samples

if not exist %TO_SRC%\%SRC_PACKAGE%\xsd mkdir %TO_SRC%\%SRC_PACKAGE%\xsd 
xcopy/s %FROM_DIR%\xsd %TO_SRC%\%SRC_PACKAGE%\xsd              

cd %TO_SRC%\%SRC_PACKAGE%
del /S Makefile.am

echo Creating zip file: %TO_SRC%\%SRC_PACKAGE%.zip 
jar -cMf %TO_SRC%\%SRC_PACKAGE%.zip -C %TO_SRC% %SRC_PACKAGE%


echo Building SCA Native binary distribution

set TUSCANY_SCACPP=%TO_SRC%\%SRC_PACKAGE%\deploy

cd %TO_SRC%\%SRC_PACKAGE%
call build

echo Installing SCA....

if not exist %TO_BIN% mkdir %TO_BIN%
rmdir /S /Q %TO_BIN%\%BIN_PACKAGE%

if not exist %TO_BIN%\%BIN_PACKAGE% mkdir %TO_BIN%\%BIN_PACKAGE%
xcopy/s %TUSCANY_SCACPP%  %TO_BIN%\%BIN_PACKAGE%

copy %FROM_DIR%\INSTALL   %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\LICENSE   %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\COPYING   %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\NOTICE    %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\README    %TO_BIN%\%BIN_PACKAGE%
copy %FROM_DIR%\GettingStarted.html   %TO_BIN%\%BIN_PACKAGE%

if not exist %TO_BIN%\%BIN_PACKAGE%\doc mkdir %TO_BIN%\%BIN_PACKAGE%\doc
xcopy/s %FROM_DIR%\doc    %TO_BIN%\%BIN_PACKAGE%\doc

if not exist %TO_BIN%\%BIN_PACKAGE%\samples mkdir %TO_BIN%\%BIN_PACKAGE%\samples
xcopy/s %TO_SRC%\%SRC_PACKAGE%\samples    %TO_BIN%\%BIN_PACKAGE%\samples


set TUSCANY_SCACPP=%TO_BIN%\%BIN_PACKAGE%

echo Building and deploying samples

echo Building CppCalculator....
cd %TO_BIN%\%BIN_PACKAGE%\samples\CppCalculator
call build

echo Building CppBigBank....
cd %TO_BIN%\%BIN_PACKAGE%\samples\CppBigBank
call build

echo Deploying HttpdBigBank
cd %TO_BIN%\%BIN_PACKAGE%\samples\HttpdBigBank
call deploy

echo Deploying PythonCalculator
cd %TO_BIN%\%BIN_PACKAGE%\samples\PythonCalculator
call deploy

echo Deploying PythonWeatherForecast
cd %TO_BIN%\%BIN_PACKAGE%\samples\PythonWeatherForecast
call deploy

echo Deploying RubyCalculator
cd %TO_BIN%\%BIN_PACKAGE%\samples\RubyBank
call deploy

echo Deploying RubyCalculator
cd %TO_BIN%\%BIN_PACKAGE%\samples\RubyCalculator
call deploy

echo Deploying RubyBank
cd %TO_BIN%\%BIN_PACKAGE%\samples\RubyBank
call deploy

echo Deploying RestCalculator
cd %TO_BIN%\%BIN_PACKAGE%\samples\RestCalculator
call deploy

echo Deploying RestCustomer
cd %TO_BIN%\%BIN_PACKAGE%\samples\RestCustomer
call deploy

echo Deploying RestYahoo
cd %TO_BIN%\%BIN_PACKAGE%\samples\RestYahoo
call deploy

echo Deploying AlertAggregator
cd %TO_BIN%\%BIN_PACKAGE%\samples\AlertAggregator
call deploy




cd %TO_BIN%\%BIN_PACKAGE%
del /S Makefile.am

echo Creating zip file: %TO_BIN%\%BIN_PACKAGE%.zip
jar -cMf %TO_BIN%\%BIN_PACKAGE%.zip -C %TO_BIN% %BIN_PACKAGE%

:end

echo SCA Build complete.
endlocal
