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
set srcdir=%rootdir%\runtime\core\src

if . == .%2 (
echo input directory not specified
goto usage
)
set inpath=%2
echo %inpath%


if not exist %deploydir% mkdir %deploydir%
if not exist %deploydir%\bin mkdir %deploydir%\bin
if not exist %deploydir%\lib mkdir %deploydir%\lib
if not exist %deploydir%\include mkdir %deploydir%\include
if not exist %deploydir%\include\tuscany mkdir %deploydir%\include\tuscany
if not exist %deploydir%\include\tuscany\sca mkdir %deploydir%\include\tuscany\sca
if not exist %deploydir%\include\tuscany\sca\core mkdir %deploydir%\include\tuscany\sca\core 
if not exist %deploydir%\include\tuscany\sca\extension mkdir %deploydir%\include\tuscany\sca\extension
if not exist %deploydir%\include\tuscany\sca\model mkdir %deploydir%\include\tuscany\sca\model 
if not exist %deploydir%\include\tuscany\sca\util mkdir %deploydir%\include\tuscany\sca\util
if not exist %deploydir%\xsd mkdir %deploydir%\xsd

del %deploydir%\bin\tuscany_sca.*
del %deploydir%\lib\*.lib

copy %srcdir%\tuscany\sca\*.h %deploydir%\include\tuscany\sca
copy %srcdir%\tuscany\sca\core\*.h %deploydir%\include\tuscany\sca\core
copy %srcdir%\tuscany\sca\extension\*.h %deploydir%\include\tuscany\sca\extension
copy %srcdir%\tuscany\sca\model\*.h %deploydir%\include\tuscany\sca\model
copy %srcdir%\tuscany\sca\util\*.h %deploydir%\include\tuscany\sca\util

copy %rootdir%\xsd\*.* %deploydir%\xsd

copy %inpath%\tuscany_sca.lib %deploydir%\lib
copy %inpath%\tuscany_sca.dll %deploydir%\bin

if exist %inpath%\tuscany_sca.pdb copy %inpath%\tuscany_sca.pdb %deploydir%\bin

goto end
:usage
echo Usage: deploy <sca-root> <build-output>
:end

endlocal
