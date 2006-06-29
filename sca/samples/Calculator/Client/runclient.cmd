@echo off
rem Runs the client after setting the SCA environment variables to use the
rem CalculatorSubsystem
setlocal

if "%AXIS2C_HOME%" == "" (
echo "AXIS2C_HOME not set"
goto end
)
echo using Axis2C: %AXIS2C_HOME%"

if "%TUSCANY_SDOCPP%" == "" (
echo "TUSCANY_SDOCPP not set"
goto end
)
echo using TUSCANY_SDOCPP: %TUSCANY_SDOCPP%

if "%TUSCANY_SCACPP%" == "" (
echo "TUSCANY_SCACPP not set"
goto end
)
echo using TUSCANY_SCACPP: %TUSCANY_SCACPP%

set TUSCANY_SCACPP_SYSTEM_ROOT=%~d0%~p0\..\

rem Only need to specify the subsystem (and not the moduleComponent as well) because
rem there is only one moduleComponent in the subsystem - it is a very simple sample.
set TUSCANY_SCACPP_DEFAULT_MODULE=CalculatorSubsystem

rem Run the client

set PATH=%TUSCANY_SCACPP%\bin;%TUSCANY_SDOCPP%\bin;%PATH%
Calc.exe %*

endlocal
