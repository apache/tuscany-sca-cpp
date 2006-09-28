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

if not exist %destinationPath%                                       mkdir %destinationPath% 
if not exist %destinationPath%\packages                              mkdir %destinationPath%\packages 
if not exist %destinationPath%\packages\ws.service.interop           mkdir %destinationPath%\packages\ws.service.interop

copy %sourcePath%\ws.service.interop\TestImpl.componentType       %destinationPath%\packages\ws.service.interop
copy %sourcePath%\ws.service.interop\ws.service.interop.composite %destinationPath%\packages\ws.service.interop
copy %sourcePath%\ws.service.interop\Test.h                       %destinationPath%\packages\ws.service.interop
copy %sourcePath%\ws.service.interop\TestImpl.h                   %destinationPath%\packages\ws.service.interop
copy %sourcePath%\resources\interop.wsdl                          %destinationPath%\packages\ws.service.interop
copy %sourcePath%\resources\*.xsd                                 %destinationPath%\packages\ws.service.interop
copy %sourcePath%\resources\*.xml                                 %destinationPath%\packages\ws.service.interop
copy %sourcePath%\ws.service.interop\Debug\ws_service_interop.dll %destinationPath%\packages\ws.service.interop


if not exist %destinationPath%\bin           mkdir %destinationPath%\bin
if not exist %destinationPath%\bin\resources mkdir %destinationPath%\bin\resources

copy %sourcePath%\ws.service.interop.client\Debug\ws.service.interop.client.exe %destinationPath%\bin
copy %sourcePath%\ws.service.interop.client\Debug\ws.service.interop.client.pdb %destinationPath%\bin
copy %sourcePath%\ws.service.interop.client\runclient.bat                       %destinationPath%\bin
copy %sourcePath%\ws.service.interop\runwsserver.bat                            %destinationPath%\bin
copy %sourcePath%\resources\*.xsd                                               %destinationPath%\bin\resources
copy %sourcePath%\resources\*.xml                                               %destinationPath%\bin\resources


if not exist %destinationPath%\configuration                             mkdir %destinationPath%\configuration
if not exist %destinationPath%\configuration\ws.service.interop.solution mkdir %destinationPath%\configuration\ws.service.interop.solution

copy %sourcePath%\ws.service.interop\ws.service.interop.solution.composite %destinationPath%\configuration\ws.service.interop.solution

endlocal
