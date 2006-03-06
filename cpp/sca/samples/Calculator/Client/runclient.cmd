@echo off
rem Runs the client after setting the SCA environment variables to use the
rem CalculatorSubsystem
setlocal
set SCA4CPP_SYSTEM_ROOT=%~d0%~p0\..\..\runtime

rem Only need to specify the subsystem (and not the moduleComponent as well) because
rem there is only one moduleComponent in the subsystem - it is a very simple sample.
set SCA4CPP_DEFAULT_MODULE=CalculatorSubsystem

rem Run the client
%~d0%~p0\debug\Client.exe %*

endlocal