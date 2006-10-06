#!/bin/sh

#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#    
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.

APFULLDIR=`pwd`

if [ x$AXIS2C_HOME = x ]; then
echo "AXIS2C_HOME not set"
exit;
fi
echo "Deploying to Axis2C installed at $AXIS2C_HOME"

if ! [ -d $AXIS2C_HOME/services/tuscany ]; then 
   mkdir $AXIS2C_HOME/services/tuscany
fi

if ! [ -d $AXIS2C_HOME/modules/tuscany ]; then 
   mkdir $AXIS2C_HOME/modules/tuscany
fi

cp $APFULLDIR/services/tuscany/services.xml %AXIS2C_HOME%/services/tuscany

if ! [ -f $AXIS2C_HOME/services/tuscany/libtuscany_sca_ws_service.so ]; then
   ln -s $APFULLDIR/services/tuscany/libtuscany_sca_ws_service.so $AXIS2C_HOME/services/tuscany/libtuscany_sca_ws_service.so
fi

cp $APFULLDIR/modules/tuscany/module.xml %AXIS2C_HOME%/modules/tuscany
if ! [ -f $AXIS2C_HOME/modules/tuscany/libtuscany_sca_ws_dispatcher.so ]; then
   ln -s $APFULLDIR/modules/tuscany/libtuscany_sca_ws_dispatcher.so $AXIS2C_HOME/modules/tuscany/libtuscany_sca_ws_dispatcher.so 
fi

cp $APFULLDIR/axis2.xml $AXIS2C_HOME/axis2.xml
