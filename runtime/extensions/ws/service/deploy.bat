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
set wsextdir=%extdir%\ws
set svcextdir=%wsextdir%\service
set svcsdir=%svcextdir%\services
set srcdir=%rootdir%\runtime\extensions\ws\service\axis2c\src

if . == .%2 (
echo input directory not specified
goto usage
)
set inpath=%2
echo %inpath%

if not exist %deploydir% mkdir %deploydir%
if not exist %extdir% mkdir %extdir%
if not exist %wsextdir% mkdir %wsextdir%
if not exist %svcextdir% mkdir %svcextdir%
if not exist %svcextdir%\bin mkdir %svcextdir%\bin
if not exist %svcextdir%\lib mkdir %svcextdir%\lib

set libname=tuscany_sca_ws_service

del %svcextdir%\bin\%libname%.*
del %svcextdir%\lib\%libname%.*

copy %inpath%\%libname%.lib %svcextdir%\lib
copy %inpath%\%libname%.dll %svcextdir%\bin

if exist %inpath%\%libname%.pdb copy %inpath%\%libname%.pdb %svcextdir%\bin

if not exist %svcsdir% mkdir %svcsdir%
if not exist %svcsdir%\tuscany mkdir %svcsdir%\tuscany

copy %srcdir%\axis2.xml %svcextdir%
copy %srcdir%\deploy.bat %svcextdir%
copy %srcdir%\services.xml %svcsdir%\tuscany
copy %inpath%\%libname%.dll %svcsdir%\tuscany
if exist %inpath%\%libname%.pdb copy %inpath%\%libname%.pdb %svcsdir%\tuscany

goto end
:usage
echo Usage: deploy <sca-root> <build-output>
:end

endlocal
