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


@setlocal

echo building Tuscany SCA C++

set config=Release
if .Debug == .%1 (
echo Building Debug version
set config=Debug
)

if "%LIBXML2_HOME%" == "" (
echo "LIBXML2_HOME not set"
goto end
)
echo using LIBXML2: %LIBXML2_HOME%

if "%AXIS2C_HOME%" == "" (
echo "AXIS2C_HOME not set"
goto end
)
echo using Axis2C: %AXIS2C_HOME%"

if "%TUSCANY_SDOCPP%" == "" (
echo "TUSCANY_SDOCPP not set"
goto end
)
echo using Tuscany SDOCPP: %TUSCANY_SDOCPP%

call vcvars32
echo building Tuscany SCA C++ core + cpp and ws extensions
cd vsexpress\tuscany_sca
call vcbuild tuscany_sca.sln "%config%|Win32"

if "%PYTHON_HOME%" == "" (
echo PYTHON_HOME not set. Skipping build of Python extension
goto pythonend
)
echo building Python extension
call vcbuild tuscany_sca_python\tuscany_sca_python.vcproj "%config%|Win32"
:pythonend

if "%RUBY_HOME%" == "" (
echo RUBY_HOME not set. Skipping build of Ruby extension
goto rubyend
)
echo building Ruby extension
call vcbuild tuscany_sca_ruby\tuscany_sca_ruby.vcproj "%config%|Win32"
:rubyend

:end

@endlocal
