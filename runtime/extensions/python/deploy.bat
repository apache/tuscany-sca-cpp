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
if not exist %pythonextdir%\include mkdir %pythonextdir%\include
if not exist %pythonextdir%\include\tuscany mkdir %pythonextdir%\include\tuscany
if not exist %pythonextdir%\include\tuscany\sca mkdir %pythonextdir%\include\tuscany\sca
if not exist %pythonextdir%\include\tuscany\sca\python mkdir %pythonextdir%\include\tuscany\sca\python
if not exist %pythonextdir%\include\tuscany\sca\python\model mkdir %pythonextdir%\include\tuscany\sca\python\model
if not exist %pythonextdir%\xsd mkdir %pythonextdir%\xsd

del %pythonextdir%\bin\tuscany_sca_python.*
del %pythonextdir%\lib\*.lib

copy %srcdir%\tuscany\sca\python\*.h %pythonextdir%\include\tuscany\sca\python
copy %srcdir%\tuscany\sca\python\model\*.h %pythonextdir%\include\tuscany\sca\python\model

copy %srcdir%\..\xsd\*.* %pythonextdir%\xsd

copy %inpath%\tuscany_sca_python.lib %pythonextdir%\lib
copy %inpath%\tuscany_sca_python.dll %pythonextdir%\bin

if exist %inpath%\tuscany_sca_python.pdb copy %inpath%\tuscany_sca_python.pdb %pythonextdir%\bin


echo Building extension to Python
set origdir=%~d0%~p0
set bindir=%pythonextdir%\bin 
cd %srcdir%\tuscany\sca\python\

rem if %pythonextdir% is a relative path, it won't be found, so set %bindir% to a full path
if not exist %bindir% set bindir=%origdir%\%pythonextdir%\bin

%PYTHON_HOME%\python sca_setup.py build
%PYTHON_HOME%\python sca_setup.py install --install-lib %bindir%
cd %origdir%

goto end
:usage
echo Usage: deploy <sca-root> <build-output>
:end

endlocal
