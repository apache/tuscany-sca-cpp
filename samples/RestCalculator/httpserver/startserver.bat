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

if "%HTTPD_HOME%" == "" (
echo "HTTPD_HOME not set"
goto end
)
echo Using HTTPD installed at %HTTPD_HOME%

set PATH=%TUSCANY_SCACPP%\extensions\ruby\bin;%TUSCANY_SCACPP%\extensions\rest\interface\bin;%TUSCANY_SCACPP%\extensions\rest\service\bin;%TUSCANY_SCACPP%\bin;%TUSCANY_SDOCPP%\bin;%HTTPD_HOME%\bin;%PATH%

set TUSCANY_SCACPP_ROOT=%APFULLDIR%\..\
set TUSCANY_SCACPP_BASE_URI=http://localhost:9090/rest/

@REM Generate the tuscany_sca_mod_rest configuration
if not exist %APFULLDIR%\conf\tuscany_sca_mod_rest.conf (
  echo LoadModule sca_rest_module %TUSCANY_SCACPP%/extensions/rest\service/bin/tuscany_sca_mod_rest.dll > %APFULLDIR%\conf\tuscany_sca_mod_rest.conf
  echo TuscanyHome %TUSCANY_SCACPP% >> %APFULLDIR%\conf\tuscany_sca_mod_rest.conf
  echo ^<Location /rest^> >> %APFULLDIR%\conf\tuscany_sca_mod_rest.conf
  echo        SetHandler sca_rest_module >> %APFULLDIR%\conf\tuscany_sca_mod_rest.conf
  echo        TuscanyRoot %TUSCANY_SCACPP_ROOT% >> %APFULLDIR%\conf\tuscany_sca_mod_rest.conf
  echo ^</Location^> >> %APFULLDIR%\conf\tuscany_sca_mod_rest.conf
)

@REM Generate the base HTTPD configuration
if not exist %APFULLDIR%\conf\base.conf (
  echo LoadModule mime_module %HTTPD_HOME%\modules\mod_mime.so > %APFULLDIR%\conf\base.conf
  echo LoadModule dir_module %HTTPD_HOME%\modules\mod_dir.so >> %APFULLDIR%\conf\base.conf
  echo DocumentRoot %APFULLDIR%\htdocs >> %APFULLDIR%\conf\base.conf
)

@REM Create logs directory
if not exist %APFULLDIR%\logs mkdir %APFULLDIR%\logs
set TUSCANY_SCACPP_LOG=%APFULLDIR%\logs\tuscany-server.log
set TUSCANY_SCACPP_LOGGING=9

@REM Start the HTTP server
echo Starting Apache httpd
httpd -d %APFULLDIR%
