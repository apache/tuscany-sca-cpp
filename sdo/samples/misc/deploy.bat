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
echo sample root not specified
goto usage
)
set deploydir=%TUSCANY_SDOCPP%\samples\misc\deploy
set srcdir=%1

if . == .%2 (
echo input directory not specified
goto usage
)
set inpath=%2
echo %inpath%

if not exist %TUSCANY_SDOCPP%\samples mkdir %TUSCANY_SDOCPP%\samples
if not exist %TUSCANY_SDOCPP%\samples\misc mkdir %TUSCANY_SDOCPP%\samples\misc
if not exist %TUSCANY_SDOCPP%\samples\misc\deploy mkdir %TUSCANY_SDOCPP%\samples\misc\deploy
if not exist %TUSCANY_SDOCPP%\samples\misc\deploy\bin mkdir %TUSCANY_SDOCPP%\samples\misc\deploy\bin
copy %srcdir%\*.xsd %deploydir%
copy %srcdir%\*.xml %deploydir%

copy %inpath%\sdo_misc.exe %deploydir%\bin
if exist %inpath%\sdo_misc.pdb copy %inpath%\sdo_misc.pdb %deploydir%\bin

goto end
:usage
echo Usage: deploy <sample-root> <build-output> 
:end

endlocal
