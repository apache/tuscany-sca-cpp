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

TUSCANY_SCACPP_HOME=`pwd`

if [ x$AXISCPP_DEPLOY = x ]; then
echo "AXISCPP_DEPLOY not set"
exit;
fi
echo "Using Axis C++ installed at $AXISCPP_DEPLOY"

if [ x$TUSCANY_SDOCPP = x ]; then
echo "TUSCANY_SDOCPP not set"
exit;
fi
echo "Using SDO installed at $TUSCANY_SDOCPP"

# Build tools first as they will be used to generate cpp code
cd $TUSCANY_SCACPP_HOME/tools/scagen
ant
chmod a+x $TUSCANY_SCACPP_HOME/bin/scagen.sh

cd $TUSCANY_SCACPP_HOME
./autogen.sh
./configure --prefix=${TUSCANY_SCACPP_HOME}
make
make install
