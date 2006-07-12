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

if . == %1. (
set destinationPath=%sourcePath%\deploy
) ELSE (
set destinationPath=%1
)


if not exist %destinationPath%\modules mkdir %destinationPath%\modules 
if not exist %destinationPath%\modules\CalculatorModule mkdir %destinationPath%\modules\CalculatorModule

copy %sourcePath%\CalculatorModule\*.componentType %destinationPath%\modules\CalculatorModule
copy %sourcePath%\CalculatorModule\sca.module %destinationPath%\modules\CalculatorModule
copy %sourcePath%\CalculatorModule\Calculator.h %destinationPath%\modules\CalculatorModule
copy %sourcePath%\CalculatorModule\CalculatorImpl.h %destinationPath%\modules\CalculatorModule
copy %sourcePath%\CalculatorModule\DivideService.h %destinationPath%\modules\CalculatorModule
copy %sourcePath%\CalculatorModule\DivideServiceImpl.h %destinationPath%\modules\CalculatorModule
copy %sourcePath%\CalculatorModule\Tuscany-model.config %destinationPath%\modules\CalculatorModule
copy %sourcePath%\CalculatorModule\Calculator.wsdl %destinationPath%\modules\CalculatorModule
copy %currentPath%\Calculator\debug\Calculator.dll %destinationPath%\modules\CalculatorModule

if not exist %destinationPath%\bin mkdir       %destinationPath%\bin
copy %currentPath%\WSClient\Debug\WSClient.exe %destinationPath%\bin
copy %currentPath%\WSClient\Debug\WSClient.pdb %destinationPath%\bin
copy %sourcePath%\WSClient\runwsclient.cmd     %destinationPath%\bin


if not exist %destinationPath%\subsystems mkdir %destinationPath%\subsystems
if not exist %destinationPath%\subsystems\CalculatorSubsystem mkdir %destinationPath%\subsystems\CalculatorSubsystem

copy %sourcePath%\CalculatorSubsystem\sca.subsystem %destinationPath%\subsystems\CalculatorSubsystem


endlocal