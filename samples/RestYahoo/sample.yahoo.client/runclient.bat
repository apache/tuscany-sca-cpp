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

if "%TUSCANY_SCACPP%" == "" (
echo TUSCANY_SCACPP not set
goto end
)
echo using SCA installed at %TUSCANY_SCACPP%

if "%TUSCANY_SDOCPP%" == "" (
echo TUSCANY_SDOCPP not set
goto end
)
echo using SDO installed at %TUSCANY_SDOCPP%

if "%LIBCURL_HOME%" == "" (
echo LIBCURL_HOME not set
goto end
)
echo using Libcurl installed at %LIBCURL_HOME%

set PATH=%LIBCURL_HOME%\lib;%TUSCANY_SCACPP%\extensions\rest\interface\bin;%TUSCANY_SCACPP%\extensions\ruby\bin;%TUSCANY_SCACPP%\extensions\rest\reference\bin;%TUSCANY_SCACPP%\bin;%TUSCANY_SDOCPP%\bin;%PATH%
set PYTHONPATH=%TUSCANY_SCACPP%\extensions\python\bin

set TUSCANY_SCACPP_ROOT=%~d0%~p0\..\
set TUSCANY_SCACPP_COMPONENT=sample.yahoo.app.WebSearchComponent

cd %TUSCANY_SCACPP_ROOT%\sample.yahoo.client
ruby -I%TUSCANY_SCACPP%\extensions\ruby\bin YahooWebSearchClient.rb

:end
endlocal
