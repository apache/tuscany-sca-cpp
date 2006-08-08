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


if not exist %destinationPath%\composites mkdir %destinationPath%\composites 
if not exist %destinationPath%\composites\CalculatorComposite mkdir %destinationPath%\composites\CalculatorComposite

copy %sourcePath%\CalculatorComposite\*.componentType %destinationPath%\composites\CalculatorComposite
copy %sourcePath%\CalculatorComposite\sca.composite %destinationPath%\composites\CalculatorComposite
copy %sourcePath%\CalculatorComposite\Calculator.h %destinationPath%\composites\CalculatorComposite
copy %sourcePath%\CalculatorComposite\CalculatorImpl.h %destinationPath%\composites\CalculatorComposite
copy %sourcePath%\CalculatorComposite\DivideService.h %destinationPath%\composites\CalculatorComposite
copy %sourcePath%\CalculatorComposite\DivideServiceImpl.h %destinationPath%\composites\CalculatorComposite
copy %sourcePath%\CalculatorComposite\Tuscany-model.config %destinationPath%\composites\CalculatorComposite
copy %sourcePath%\CalculatorComposite\Calculator.wsdl %destinationPath%\composites\CalculatorComposite
copy %currentPath%\Calculator\%buildMode%\Calculator.dll %destinationPath%\composites\CalculatorComposite

if not exist %destinationPath%\bin mkdir       %destinationPath%\bin
copy %currentPath%\WSClient\%buildMode%\WSClient.exe %destinationPath%\bin
if %buildMode% == Debug (
copy %currentPath%\WSClient\%buildMode%\WSClient.pdb %destinationPath%\bin
)
copy %sourcePath%\WSClient\runwsclient.cmd     %destinationPath%\bin


if not exist %destinationPath%\subsystems mkdir %destinationPath%\subsystems
if not exist %destinationPath%\subsystems\CalculatorSubsystem mkdir %destinationPath%\subsystems\CalculatorSubsystem

copy %sourcePath%\CalculatorSubsystem\sca.subsystem %destinationPath%\subsystems\CalculatorSubsystem


endlocal