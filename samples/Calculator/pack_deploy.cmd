@echo off
rem Will copy the correct files from the source tree for packaging and deployment of the 
rem SCA sample.
setlocal
set sourcePath=%~d0%~p0
if . == %1. (
set destinationPath=%sourcePath%..\runtime
) ELSE (
set destinationPath=%1
)

mkdir %destinationPath%\modules
mkdir %destinationPath%\modules\CalculatorModule
copy %sourcePath%\CalculatorModule\*.componentType %destinationPath%\modules\CalculatorModule
copy %sourcePath%\CalculatorModule\sca.module %destinationPath%\modules\CalculatorModule
copy %sourcePath%\CalculatorModule\Calculator.h %destinationPath%\modules\CalculatorModule
copy %sourcePath%\CalculatorModule\CalculatorImpl.h %destinationPath%\modules\CalculatorModule

copy %sourcePath%\CalculatorModule\debug\CalculatorModule.dll %destinationPath%\modules\CalculatorModule


mkdir %destinationPath%\subsystems
mkdir %destinationPath%\subsystems\CalculatorSubsystem
copy %sourcePath%\CalculatorSubsystem\sca.subsystem %destinationPath%\subsystems\CalculatorSubsystem


endlocal