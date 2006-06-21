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
if not exist %destinationPath%\modules mkdir %destinationPath%\modules\Calculator 

copy %sourcePath%\CalculatorModule\*.componentType %destinationPath%\modules\Calculator
copy %sourcePath%\CalculatorModule\sca.module %destinationPath%\modules\Calculator
copy %sourcePath%\CalculatorModule\Calculator.h %destinationPath%\modules\Calculator
copy %sourcePath%\CalculatorModule\CalculatorImpl.h %destinationPath%\modules\Calculator
copy %sourcePath%\CalculatorModule\Tuscany-model.config %destinationPath%\modules\Calculator

copy %currentPath%\Calculator\debug\Calculator.dll %destinationPath%\modules\Calculator

copy %currentPath%\Client\Debug\Calc.exe %destinationPath%\modules\Calculator
copy %currentPath%\Client\Debug\Calc.pdb %destinationPath%\modules\Calculator


if not exist %destinationPath%\subsystems mkdir %destinationPath%\subsystems
if not exist %destinationPath%\subsystems\Calculator mkdir %destinationPath%\subsystems\Calculator

copy %sourcePath%\CalculatorSubsystem\sca.subsystem %destinationPath%\subsystems\Calculator

endlocal