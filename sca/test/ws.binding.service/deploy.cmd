@echo off

@REM  Copyright 2006 The Apache Software Foundation or its licensors, as applicable.
@REM
@REM  Licensed under the Apache License, Version 2.0 (the "License");
@REM  you may not use this file except in compliance with the License.
@REM  You may obtain a copy of the License at
@REM
@REM     http://www.apache.org/licenses/LICENSE-2.0
@REM
@REM  Unless required by applicable law or agreed to in writing, software
@REM  distributed under the License is distributed on an "AS IS" BASIS,
@REM  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
@REM  See the License for the specific language governing permissions and
@REM  limitations under the License.

rem Will copy the correct files from the source tree for packaging and deployment of the 
rem SCA sample.
setlocal
set currentPath=%~d0%~p0
set sourcePath=%currentPath%


set destinationPath=%sourcePath%\deploy

if not exist %destinationPath%\packages mkdir %destinationPath%\packages 
if not exist %destinationPath%\packages\ws.binding.service mkdir %destinationPath%\packages\ws.binding.service

copy %sourcePath%\ws.binding.service\TestImpl.componentType       %destinationPath%\packages\ws.binding.service
copy %sourcePath%\ws.binding.service\ws.binding.service.composite %destinationPath%\packages\ws.binding.service
copy %sourcePath%\ws.binding.service\Test.h                       %destinationPath%\packages\ws.binding.service
copy %sourcePath%\ws.binding.service\TestImpl.h                   %destinationPath%\packages\ws.binding.service
copy %sourcePath%\ws.binding.service\Test.wsdl                    %destinationPath%\packages\ws.binding.service
copy %sourcePath%\ws.binding.service\Debug\ws.binding.service.dll %destinationPath%\packages\ws.binding.service

if not exist %destinationPath%\bin mkdir %destinationPath%\bin
copy %sourcePath%\ws.binding.service.client\Debug\ws.binding.service.client.exe %destinationPath%\bin
copy %sourcePath%\ws.binding.service.client\Debug\ws.binding.service.client.pdb %destinationPath%\bin
copy %sourcePath%\ws.binding.service.client\runclient.bat                       %destinationPath%\bin


if not exist %destinationPath%\configuration mkdir %destinationPath%\configuration
if not exist %destinationPath%\configuration\ws.binding.service.solution mkdir %destinationPath%\configuration\ws.binding.service.solution

copy %sourcePath%\ws.binding.service\ws.binding.service.solution.composite %destinationPath%\configuration\ws.binding.service.solution


if not exist %AXIS2C_HOME%\services\ws.binding.service mkdir %AXIS2C_HOME%\services\ws.binding.service

copy %sourcePath%\ws.binding.service\services.xml                          %AXIS2C_HOME%\services\ws.binding.service
copy %TUSCANY_SCACPP%\extensions\ws\service\bin\tuscany_sca_ws_service.dll %AXIS2C_HOME%\services\ws.binding.service


endlocal
