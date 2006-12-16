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

if [ x$TUSCANY_SCACPP = x ]; then
echo "TUSCANY_SCACPP not set"
exit;
fi
echo "Using SCA installed at $TUSCANY_SCACPP"

if [ x$TUSCANY_SDOCPP = x ]; then
echo "TUSCANY_SDOCPP not set"
exit;
fi
echo "Using SDO installed at $TUSCANY_SDOCPP"

if [ x$AXIS2C_HOME = x ]; then
echo "AXIS2C_HOME not set"
exit;
fi
echo "Using Axis2C installed at $AXIS2C_HOME"

if [ x$PYTHON_LIB != x ]; then
echo "Using Python library installed at $PYTHON_LIB"
export LD_LIBRARY_PATH=$PYTHON_LIB:$LD_LIBRARY_PATH
export PATH=$PYTHON_LIB/../bin:$PATH
fi

export LD_LIBRARY_PATH=$TUSCANY_SCACPP/lib:$TUSCANY_SCACPP/extensions/python/lib:$TUSCANY_SDOCPP/lib:$AXIS2C_HOME/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$TUSCANY_SCACPP/extensions/python/lib:$PYTHONPATH

export TUSCANY_SCACPP_SYSTEM_ROOT=$APFULLDIR/../
export TUSCANY_SCACPP_BASE_URI=http://localhost:9090/axis2/services/

cd $AXIS2C_HOME/bin
./axis2_http_server
