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

setlocal

set APFULLDIR=%~d0%~p0
echo Running from %APFULLDIR%

if "%TUSCANY_SCACPP%" == "" (
echo "TUSCANY_SCACPP not set"
goto end
)
echo Using SCA installed at %TUSCANY_SCACPP%

if "%TUSCANY_SDOCPP%" == "" (
echo "TUSCANY_SDOCPP not set"
goto end
)
echo Using SDO installed at %TUSCANY_SDOCPP%

if "%AXIS2C_HOME%" == "" (
echo "AXIS2C_HOME not set"
goto end
)
echo Using Axis2C installed at %AXIS2C_HOME%

set PATH=%TUSCANY_SCACPP%\extensions\sca\reference\bin;%TUSCANY_SCACPP%\extensions\sca\service\bin;%TUSCANY_SCACPP%\extensions\ws\reference\bin;%TUSCANY_SCACPP%\extensions\ws\service\bin;%TUSCANY_SCACPP%\extensions\ruby\bin;%TUSCANY_SCACPP%\bin;%TUSCANY_SDOCPP%\bin;%AXIS2C_HOME%\lib;%PATH%

set TUSCANY_SCACPP_ROOT=%APFULLDIR%\..\

@REM Generate the mod_axis2 configuration
if not exist %APFULLDIR%\conf\mod_axis2.conf (
  echo LoadModule axis2_module %AXIS2C_HOME%/lib/mod_axis2.dll > %APFULLDIR%\conf\mod_axis2.conf
  echo ^<Location /axis2^> >> %APFULLDIR%\conf\mod_axis2.conf
  echo         SetHandler axis2_module >> %APFULLDIR%\conf\mod_axis2.conf
  echo         RepoPath %AXIS2C_HOME% >> %APFULLDIR%\conf\mod_axis2.conf
  echo         LogFile %AXIS2C_HOME%/logs/httpd.log >> %APFULLDIR%\conf\mod_axis2.conf
  echo         Axis2LogLevel AXIS2_LOG_LEVEL_DEBUG >> %APFULLDIR%\conf\mod_axis2.conf           
  echo ^</Location^> >> %APFULLDIR%\conf\mod_axis2.conf
)

@REM Create logs directory
if not exist %APFULLDIR%\logs mkdir %APFULLDIR%\logs
set TUSCANY_SCACPP_LOG=%APFULLDIR%\logs\tuscany-server.log
set TUSCANY_SCACPP_LOGGING=9

@REM Start the HTTP server
echo Starting Apache httpd
httpd -d %APFULLDIR%

