@echo off
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

if not exist %destinationPath%\bin mkdir %destinationPath%\bin
copy %currentPath%\Client\Debug\Calc.exe %destinationPath%\bin
copy %currentPath%\Client\Debug\Calc.pdb %destinationPath%\bin
copy %sourcePath%\Client\runclient.cmd   %destinationPath%\bin


if not exist %destinationPath%\subsystems mkdir %destinationPath%\subsystems
if not exist %destinationPath%\subsystems\CalculatorSubsystem mkdir %destinationPath%\subsystems\CalculatorSubsystem

copy %sourcePath%\CalculatorSubsystem\sca.subsystem %destinationPath%\subsystems\CalculatorSubsystem

endlocal