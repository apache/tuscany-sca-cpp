@echo off

@REM  Copyright 2006 The Apache Software Foundation or its licensors, as applicable.
@REM
@REM  Licensed under the Apache License, Version 2.0 (the "License");
@REM  you may not use this file except in compliance with the License.
@REM  You may obtain a copy of the License at
@REM
@REM     http://www.apache.org/licenses/LICENSE-2.0
@REM
@REM  Unless required by applicable law or agreed to in writing, software
@REM  distributed under the License is distributed on an "AS IS" BASIS,
@REM  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
@REM  See the License for the specific language governing permissions and
@REM  limitations under the License.



@setlocal

if "%AXIS2C_HOME%" == "" (
echo "AXIS2C_HOME not set"
goto end
)
echo using Axis2C: %AXIS2C_HOME%"

if "%TUSCANY_SDOCPP%" == "" (
echo "TUSCANY_SDOCPP not set"
set TUSCANY_SDOCPP=%cd%\deploy
)
echo using TUSCANY_SDOCPP: %TUSCANY_SDOCPP%

set PATH=%TUSCANY_SDOCPP%\bin;%AXIS2C_HOME%\lib;%PATH%


set SCATESTPATH=%cd%\projects\tuscany_sdo\sdo_test\Release
cd runtime\core\test
%SCATESTPATH%\sdo_test

:end
@endlocal
