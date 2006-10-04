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
set sourcePath=%currentPath%..\..\..\..\BigBank

set buildMode=Release
if .Debug == .%1 (
set buildMode=Debug
)


if . == %2. (
set destinationPath=%sourcePath%\deploy
) ELSE (
set destinationPath=%2
)

if not exist %destinationPath%                        mkdir %destinationPath%
if not exist %destinationPath%\packages               mkdir %destinationPath%\packages
if not exist %destinationPath%\packages\Accounts      mkdir %destinationPath%\packages\bigbank.account
if not exist %destinationPath%\configuration          mkdir %destinationPath%\configuration
if not exist %destinationPath%\configuration\Accounts mkdir %destinationPath%\configuration\bigbank.accountmanagement
if not exist %destinationPath%\bin                    mkdir %destinationPath%\bin

copy %currentPath%\Accounts\%buildMode%\Account.dll     %destinationPath%\packages\bigbank.account
if exist %currentPath%\Accounts\%buildMode%\Account.pdb copy %currentPath%\Accounts\%buildMode%\Account.pdb %destinationPath%\packages\bigbank.account

copy %sourcePath%\bigbank.account\*.h             %destinationPath%\packages\bigbank.account
copy %sourcePath%\bigbank.account\*.composite     %destinationPath%\packages\bigbank.account
copy %sourcePath%\bigbank.account\*.componentType %destinationPath%\packages\bigbank.account
copy %sourcePath%\bigbank.account\*.wsdl          %destinationPath%\packages\bigbank.account
copy %sourcePath%\bigbank.account\*.xsd           %destinationPath%\packages\bigbank.account

copy %sourcePath%\bigbank.accountmanagement\*.composite   %destinationPath%\configuration\bigbank.accountmanagement

copy %currentPath%\AccountClient\%buildMode%\AccountClient.exe     %destinationPath%\bin
if exist %currentPath%\AccountClient\%buildMode%\AccountClient.pdb copy %currentPath%\AccountClient\%buildMode%\AccountClient.pdb     %destinationPath%\bin
copy %sourcePath%\bigbank.client\*.bat                       %destinationPath%\bin


