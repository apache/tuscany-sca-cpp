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

TUSCANY_SDOCPP_HOME=`pwd`

if [ x$AXIS2C_HOME = x ]; then
echo "AXIS2C_HOME not set"
exit;
fi
echo "Using Axis2C installed at $AXIS2C_HOME"

if [ x$LIBXML2_INCLUDE = x ]; then
echo "LIBXML2_INCLUDE not set"
exit;
fi
if [ x$LIBXML2_LIB = x ]; then
echo "LIBXML2_LIB not set"
exit;
fi
echo "Using libxml2 installed at $LIBXML2_LIB, $LIBXML2_INCLUDE"

cd ${TUSCANY_SDOCPP_HOME}/samples
./autogen.sh
./configure --prefix=${TUSCANY_SDOCPP_HOME}/deploy --enable-static=no

cd $TUSCANY_SDOCPP_HOME
./autogen.sh
./configure --prefix=${TUSCANY_SDOCPP_HOME}/deploy --enable-static=no

make bindist



