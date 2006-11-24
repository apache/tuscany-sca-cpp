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
set pythonextdir=%extdir%\python
set srcdir=%rootdir%\runtime\extensions\python\src

if . == .%2 (
echo input directory not specified
goto usage
)
set inpath=%2
echo %inpath%


if not exist %deploydir% mkdir %deploydir%
if not exist %extdir% mkdir %extdir%
if not exist %pythonextdir% mkdir %pythonextdir%
if not exist %pythonextdir%\bin mkdir %pythonextdir%\bin
if not exist %pythonextdir%\lib mkdir %pythonextdir%\lib
if not exist %pythonextdir%\xsd mkdir %pythonextdir%\xsd

del %pythonextdir%\bin\sca.pyd
del %pythonextdir%\bin\sca_proxy.py

copy %inpath%\sca.pyd %pythonextdir%\bin
copy %srcdir%\tuscany\sca\python\sca_proxy.py %pythonextdir%\bin

if exist %inpath%\sca.pdb copy %inpath%\sca.pdb %pythonextdir%\bin

goto end
:usage
echo Usage: deploy <sca-root> <build-output>
:end

endlocal
