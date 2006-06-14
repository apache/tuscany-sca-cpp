#!/bin/sh

#  Copyright 2005 The Apache Software Foundation or its licensors, as applicable.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.


if [ x$LIBXML2_INCLUDE = x ]; then
echo "LIBXML2_INCLUDE not set"
exit;
fi
if [ x$LIBXML2_LIB = x ]; then
echo "LIBXML2_LIB not set"
exit;
fi

if [ x$AXIS2C_HOME = x ]; then
echo "AXIS2C_HOME not set"
exit;
fi
echo "Using Axis2C installed at $AXIS2C_HOME"

./autogen.sh
if [ x$TUSCANY_SDOCPP_HOME = x ]; then
TUSCANY_SDOCPP_HOME=`pwd`/deploy
fi

./configure --prefix=${TUSCANY_SDOCPP_HOME} --enable-static=no
make
make install
