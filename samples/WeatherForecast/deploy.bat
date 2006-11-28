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

rem Will copy the correct files from the source tree for packaging and deployment of the 
rem SCA sample.
setlocal
set currentPath=%~d0%~p0
set sourcePath=%currentPath%

set deploydir=%TUSCANY_SCACPP%
set samplesdir=%deploydir%\samples
set weatherdir=%samplesdir%\WeatherForecast

if . == %1. (
set destinationPath=%weatherdir%\deploy
) ELSE (
set destinationPath=%1
)

if not exist %samplesdir%                         mkdir %samplesdir%
if not exist %weatherdir%                         mkdir %weatherdir%
if not exist %destinationPath%                    mkdir %destinationPath%
if not exist %destinationPath%\sample.weather     mkdir %destinationPath%\sample.weather
copy %sourcePath%\sample.weather\*.composite      %destinationPath%\sample.weather
copy %sourcePath%\sample.weather\*.py             %destinationPath%\sample.weather
copy %sourcePath%\sample.weather\*.wsdl           %destinationPath%\sample.weather

if not exist %destinationPath%\sample.weather.client mkdir         %destinationPath%\sample.weather.client
copy %sourcePath%\sample.weather.client\weather_forecast_client.py %destinationPath%\sample.weather.client
copy %sourcePath%\sample.weather.client\runclient.bat              %destinationPath%\sample.weather.client

copy %sourcePath%\*.composite %destinationPath%

endlocal
