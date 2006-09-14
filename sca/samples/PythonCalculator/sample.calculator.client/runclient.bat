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

setlocal

if "%TUSCANY_SCACPP%" == "" (
echo "TUSCANY_SCACPP not set"
goto end
)
echo using SCA installed at %TUSCANY_SCACPP%

if "%TUSCANY_SDOCPP%" == "" (
echo "TUSCANY_SDOCPP not set"
goto end
)
echo using SDO installed at %TUSCANY_SDOCPP%

if "%AXIS2C_HOME%" == "" (
echo "AXIS2C_HOME not set"
goto end
)
echo using Axis2C installed at %AXIS2C_HOME%

set TUSCANY_SCACPP_SYSTEM_ROOT=%~d0%~p0\..\
set TUSCANY_SCACPP_DEFAULT_COMPONENT=sample.calculator.CalculatorComponent

set PATH=%TUSCANY_SCACPP%\bin;%TUSCANY_SCACPP%\extensions\python\bin;%TUSCANY_SDOCPP%\bin;%AXIS2C_HOME%\lib;%PATH%
set PYTHONPATH=%TUSCANY_SCACPP%\extensions\python\bin

python calculator_client.py add 4.7 9   
python calculator_client.py div 7.2 3.6 
python calculator_client.py mul 7 6 
    
:end
endlocal
