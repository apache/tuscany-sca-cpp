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

if . == .%1 (
echo sca root not specified
goto usage
)
set rootdir=%1
set deploydir=%rootdir%\deploy
set extdir=%deploydir%\extensions
set restextdir=%extdir%\rest
set refextdir=%restextdir%\reference
set srcdir=%rootdir%\runtime\extensions\rest\reference

if . == .%2 (
echo input directory not specified
goto usage
)
set inpath=%2
echo %inpath%

call %srcdir%\..\deploy.bat %rootdir%

if not exist %deploydir% mkdir %deploydir%
if not exist %extdir% mkdir %extdir%
if not exist %restextdir% mkdir %restextdir%
if not exist %refextdir% mkdir %refextdir%
if not exist %refextdir%\module mkdir %refextdir%\module
if not exist %refextdir%\bin mkdir %refextdir%\bin
if not exist %refextdir%\lib mkdir %refextdir%\lib

set libname=tuscany_sca_rest_reference

del %refextdir%\bin\%libname%.*
del %refextdir%\lib\%libname%.*

copy %inpath%\%libname%.lib %refextdir%\lib
copy %inpath%\%libname%.dll %refextdir%\bin
copy %inpath%\%libname%.dll %refextdir%\module

if exist %inpath%\%libname%.pdb copy %inpath%\%libname%.pdb %refextdir%\bin
if exist %inpath%\%libname%.pdb copy %inpath%\%libname%.pdb %refextdir%\module

goto end
:usage
echo Usage: deploy <sca-root> <build-output>
:end

endlocal
