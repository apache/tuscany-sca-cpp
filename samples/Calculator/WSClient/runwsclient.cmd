@echo off
setlocal

if "%AXIS2C_HOME%" == "" (
echo "AXIS2C_HOME not set"
goto end
)
echo using Axis2C: %AXIS2C_HOME%"

rem Run the client
WSClient.exe %*

endlocal
