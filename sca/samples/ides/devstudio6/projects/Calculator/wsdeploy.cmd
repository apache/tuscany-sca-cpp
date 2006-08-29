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
if not exist %destinationPath%\packages\CalculatorComposite mkdir %destinationPath%\packages\CalculatorComposite

copy %sourcePath%\CalculatorComposite\*.componentType %destinationPath%\packages\CalculatorComposite
copy %sourcePath%\CalculatorComposite\*.composite %destinationPath%\packages\CalculatorComposite
copy %sourcePath%\CalculatorComposite\Calculator.h %destinationPath%\packages\CalculatorComposite
copy %sourcePath%\CalculatorComposite\CalculatorImpl.h %destinationPath%\packages\CalculatorComposite
copy %sourcePath%\CalculatorComposite\DivideService.h %destinationPath%\packages\CalculatorComposite
copy %sourcePath%\CalculatorComposite\DivideServiceImpl.h %destinationPath%\packages\CalculatorComposite
copy %sourcePath%\CalculatorComposite\Calculator.wsdl %destinationPath%\packages\CalculatorComposite
copy %currentPath%\Calculator\%buildMode%\CalculatorComposite.dll %destinationPath%\packages\CalculatorComposite

if not exist %destinationPath%\bin mkdir       %destinationPath%\bin
copy %currentPath%\WSClient\%buildMode%\WSClient.exe %destinationPath%\bin
if %buildMode% == Debug (
copy %currentPath%\WSClient\%buildMode%\WSClient.pdb %destinationPath%\bin
)
copy %sourcePath%\WSClient\runwsclient.cmd     %destinationPath%\bin


if not exist %destinationPath%\configuration mkdir %destinationPath%\configuration
if not exist %destinationPath%\configuration\CalculatorSubsystem mkdir %destinationPath%\configuration\CalculatorSubsystem

copy %sourcePath%\CalculatorSubsystem\*.composite %destinationPath%\configuration\CalculatorSubsystem


endlocal
