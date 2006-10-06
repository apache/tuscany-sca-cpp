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

rem Will copy the correct files from the source tree for packaging and deployment of the 
rem SCA Calculator sample.

setlocal

if . == .%1 (
echo source path not specified
goto usage
)
set srcdir=%1

if . == .%2 (
echo input directory not specified
goto usage
)
set inpath=%2
echo %inpath%

set package=sample.calculator.client

set deploydir=%TUSCANY_SCACPP%
set samplesdir=%deploydir%\samples
set calcdir=%samplesdir%\Calculator
set calcdeploydir=%calcdir%\deploy
set clientdir=%calcdeploydir%\%package%

if not exist %samplesdir% mkdir %samplesdir%
if not exist %calcdir% mkdir %calcdir%
if not exist %calcdeploydir% mkdir %calcdeploydir%
if not exist %clientdir% mkdir %clientdir% 

set clientdirsrc=%srcdir%\%package%
copy %clientdirsrc%\runclient.bat %clientdir% 

set library=calculator_client
copy %inpath%\%library%.exe %clientdir%

if exist %inpath%\%library%.pdb copy %inpath%\%library%.pdb %clientdir%

goto end
:usage
echo Usage: deploy <sample-src> <build-output>
:end

endlocal
