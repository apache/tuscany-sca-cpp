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

SCA_PHP_EXTENSION_HOME=`pwd`

if [ x$TUSCANY_SCACPP = x ]; then
echo "TUSCANY_SCACPP not set"
exit;
fi

echo "Using SCA installed at $TUSCANY_SCACPP"

if [ x$PHP_LIB = x ]; then
echo "PHP_LIB not set."
exit;
elif [ x$PHP_INCLUDE = x ]; then
echo "PHP_INCLUDE not set."
exit;
elif [ x$PHP_SCA_SDO_INCLUDE = x ]; then
echo "PHP_SCA_SDO_INCLUDE not set."
exit;
elif [ x$PHP_SCA_SDO_LIB = x ]; then
echo "PHP_SCA_SDO_LIB not set."
exit;
fi
echo "Building PHP extension with PHP installed at $PHP_LIB, $PHP_INCLUDE"
echo "and PHP SCA and SDO installed at $PHP_SCA_SDO_LIB, $PHP_SCA_SDO_INCLUDE"

#cd ${TUSCANY_SCACPP_HOME}/samples
#./autogen.sh

cd SCA_PHP_EXTENSION_HOME
./autogen.sh


./configure --prefix=${TUSCANY_SCACPP}/extensions/php
make
make install

