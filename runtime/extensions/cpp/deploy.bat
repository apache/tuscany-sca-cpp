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
set cppextdir=%extdir%\cpp
set srcdir=%rootdir%\runtime\extensions\cpp\src
set toolsdir=%rootdir%\runtime\extensions\cpp\tools

if . == .%2 (
echo input directory not specified
goto usage
)
set inpath=%2
echo %inpath%


if not exist %deploydir% mkdir %deploydir%
if not exist %extdir% mkdir %extdir%
if not exist %cppextdir% mkdir %cppextdir%
if not exist %cppextdir%\module mkdir %cppextdir%\module
if not exist %cppextdir%\bin mkdir %cppextdir%\bin
if not exist %cppextdir%\lib mkdir %cppextdir%\lib
if not exist %cppextdir%\include mkdir %cppextdir%\include
if not exist %cppextdir%\include\osoa mkdir %cppextdir%\include\osoa
if not exist %cppextdir%\include\osoa\sca mkdir %cppextdir%\include\osoa\sca
if not exist %cppextdir%\include\tuscany mkdir %cppextdir%\include\tuscany
if not exist %cppextdir%\include\tuscany\sca mkdir %cppextdir%\include\tuscany\sca
if not exist %cppextdir%\include\tuscany\sca\cpp mkdir %cppextdir%\include\tuscany\sca\cpp
if not exist %cppextdir%\include\tuscany\sca\cpp\model mkdir %cppextdir%\include\tuscany\sca\cpp\model
if not exist %cppextdir%\xsd mkdir %cppextdir%\xsd

del %cppextdir%\bin\tuscany_sca_cpp.*
del %cppextdir%\lib\*.lib

copy %srcdir%\osoa\sca\*.h %cppextdir%\include\osoa\sca
copy %srcdir%\tuscany\sca\cpp\*.h %cppextdir%\include\tuscany\sca\cpp
copy %srcdir%\tuscany\sca\cpp\model\*.h %cppextdir%\include\tuscany\sca\cpp\model

copy %srcdir%\..\xsd\*.* %cppextdir%\xsd

copy %inpath%\tuscany_sca_cpp.lib %cppextdir%\lib
copy %inpath%\tuscany_sca_cpp.dll %cppextdir%\bin
copy %inpath%\tuscany_sca_cpp.dll %cppextdir%\module

if exist %inpath%\tuscany_sca_cpp.pdb copy %inpath%\tuscany_sca_cpp.pdb %cppextdir%\bin
if exist %inpath%\tuscany_sca_cpp.pdb copy %inpath%\tuscany_sca_cpp.pdb %cppextdir%\module

copy %toolsdir%\scagen\bld\scagen.jar  %cppextdir%\bin
copy %toolsdir%\scagen\scagen.bat  %cppextdir%\bin

goto end
:usage
echo Usage: deploy <sca-root> <build-output>
:end

endlocal
