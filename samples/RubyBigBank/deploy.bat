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
set bbdir=%samplesdir%\RubyBigBank


if . == %1. (
set destinationPath=%bbdir%\deploy
) ELSE (
set destinationPath=%1
)

if not exist %samplesdir%                       mkdir %samplesdir%
if not exist %bbdir%                            mkdir %bbdir%
if not exist %destinationPath%                  mkdir %destinationPath%
if not exist %destinationPath%\bigbank.account  mkdir %destinationPath%\bigbank.account
if not exist %destinationPath%\bigbank.client   mkdir %destinationPath%\bigbank.client
if not exist %destinationPath%\bigbank.wsclient mkdir %destinationPath%\bigbank.wsclient

copy %sourcePath%\bigbank.account\*.rb        %destinationPath%\bigbank.account
copy %sourcePath%\bigbank.account\*.composite %destinationPath%\bigbank.account
copy %sourcePath%\bigbank.account\*.wsdl      %destinationPath%\bigbank.account
copy %sourcePath%\bigbank.account\*.xsd       %destinationPath%\bigbank.account
copy %sourcePath%\bigbank.account\*.bat       %destinationPath%\bigbank.account

copy %sourcePath%\bigbank.client\*.rb         %destinationPath%\bigbank.client
copy %sourcePath%\bigbank.client\*.bat        %destinationPath%\bigbank.client

copy %sourcePath%\bigbank.wsclient\*.rb        %destinationPath%\bigbank.wsclient
copy %sourcePath%\bigbank.wsclient\*.bat       %destinationPath%\bigbank.wsclient
copy %sourcePath%\bigbank.wsclient\*.composite %destinationPath%\bigbank.wsclient
copy %sourcePath%\bigbank.wsclient\*.wsdl      %destinationPath%\bigbank.wsclient

copy %sourcePath%\*.composite   %destinationPath%

endlocal
