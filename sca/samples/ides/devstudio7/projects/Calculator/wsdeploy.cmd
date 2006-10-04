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

rem Will copy the correct files from the source tree for packaging and deployment of the 
rem SCA sample.
setlocal
set currentPath=%~d0%~p0
set sourcePath=%currentPath%..\..\..\..\Calculator

set buildMode=Release
if .Debug == .%1 (
set buildMode=Debug
)


if . == %2. (
set destinationPath=%sourcePath%\deploy
) ELSE (
set destinationPath=%2
)

if not exist %destinationPath%\packages mkdir %destinationPath%\packages 
if not exist %destinationPath%\packages\sample.calculator mkdir %destinationPath%\packages\sample.calculator

copy %sourcePath%\sample.calculator\*.componentType %destinationPath%\packages\sample.calculator
copy %sourcePath%\sample.calculator\*.composite %destinationPath%\packages\sample.calculator
copy %sourcePath%\sample.calculator\Calculator.h %destinationPath%\packages\sample.calculator
copy %sourcePath%\sample.calculator\CalculatorImpl.h %destinationPath%\packages\sample.calculator
copy %sourcePath%\sample.calculator\Divide.h %destinationPath%\packages\sample.calculator
copy %sourcePath%\sample.calculator\DivideImpl.h %destinationPath%\packages\sample.calculator
copy %sourcePath%\sample.calculator\Calculator.wsdl %destinationPath%\packages\sample.calculator
copy %currentPath%\Calculator\%buildMode%\Calculator.dll %destinationPath%\packages\sample.calculator

if not exist %destinationPath%\bin mkdir       %destinationPath%\bin
copy %currentPath%\WSClient\%buildMode%\calculator_wsclient.exe %destinationPath%\bin
if %buildMode% == Debug (
copy %currentPath%\WSClient\%buildMode%\calculator_wsclient.pdb %destinationPath%\bin
)
copy %sourcePath%\sample.calculator.wsclient\runwsclient.bat    %destinationPath%\bin
copy %sourcePath%\sample.calculator\runwsserver.bat             %destinationPath%\bin


if not exist %destinationPath%\configuration mkdir %destinationPath%\configuration
if not exist %destinationPath%\configuration\sample.calculator.solution mkdir %destinationPath%\configuration\sample.calculator.solution

copy %sourcePath%\sample.calculator.solution\*.composite %destinationPath%\configuration\sample.calculator.solution


endlocal
