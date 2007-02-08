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
set alertdir=%samplesdir%\AlertAggregator

if . == %1. (
set destinationPath=%alertdir%\deploy
) ELSE (
set destinationPath=%1
)

if not exist %samplesdir%                                mkdir %samplesdir%
if not exist %alertdir%                                  mkdir %alertdir%
if not exist %destinationPath%                           mkdir %destinationPath%
if not exist %destinationPath%\sample.alerter            mkdir %destinationPath%\sample.alerter
if not exist %destinationPath%\sample.display            mkdir %destinationPath%\sample.display
if not exist %destinationPath%\httpserver                mkdir %destinationPath%\httpserver
if not exist %destinationPath%\httpserver\conf           mkdir %destinationPath%\httpserver\conf
if not exist %destinationPath%\httpserver\htdocs         mkdir %destinationPath%\httpserver\htdocs

copy %sourcePath%\sample.alerter\*.composite     %destinationPath%\sample.alerter
copy %sourcePath%\sample.alerter\*.py            %destinationPath%\sample.alerter
copy %sourcePath%\sample.alerter\*.rb            %destinationPath%\sample.alerter
copy %sourcePath%\sample.alerter\*.xsd           %destinationPath%\sample.alerter

copy %sourcePath%\sample.display\*.composite     %destinationPath%\sample.display
copy %sourcePath%\sample.display\*.py            %destinationPath%\sample.display
copy %sourcePath%\sample.display\*.xsd           %destinationPath%\sample.display

copy %sourcePath%\httpserver\*.bat         %destinationPath%\httpserver
copy %sourcePath%\httpserver\conf\*.types  %destinationPath%\httpserver\conf
copy %sourcePath%\httpserver\conf\*.conf   %destinationPath%\httpserver\conf
copy %sourcePath%\httpserver\htdocs\*.html %destinationPath%\httpserver\htdocs
copy %sourcePath%\httpserver\htdocs\*.css %destinationPath%\httpserver\htdocs
copy %sourcePath%\httpserver\htdocs\*.png %destinationPath%\httpserver\htdocs

copy %sourcePath%\*.composite %destinationPath%
copy %sourcePath%\*.xml       %destinationPath%

endlocal
