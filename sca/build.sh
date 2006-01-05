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

APFULLDIR=`pwd`
BIN_DIR="$APFULLDIR/bin"
LIB_DIR="$APFULLDIR/lib"
INCLUDE_DIR="$APFULLDIR/include"

SCA4CPP_HOME="$APFULLDIR"
SCA4CPP="$APFULLDIR"

if [ x$AXISCPP_DEPLOY = x ]; then
echo AXISCPP_DEPLOY not set
exit;
fi
echo "Using Axis C++ installed at $AXISCPP_DEPLOY"

if [ x$SDO4CPP = x ]; then
SDO4CPP="$SCA4CPP/../sdo"
fi
echo "Using SDO installed at $SDO4CPP"

cd $SCA4CPP_HOME/runtime/core/src
cd Debug
make clean
make

cp libtuscany_sca.so $SCA4CPP/lib

cd ..

cp osoa/sca/*.h $SCA4CPP/include/osoa/sca
cp tuscany/sca/core/*.h $SCA4CPP/include/tuscany/sca/core
cp tuscany/sca/util/*.h $SCA4CPP/include/tuscany/sca/util
cp tuscany/sca/model/*.h $SCA4CPP/include/tuscany/sca/model
cp tuscany/sca/ws/*.h $SCA4CPP/include/tuscany/sca/ws

cd $SCA4CPP_HOME/runtime/axis_binding/handler/src
cd Debug
make clean
make
cp libtuscany_sca_axis_handler.so $SCA4CPP/lib

cd $SCA4CPP_HOME/runtime/axis_binding/wrapper/src
cd Debug
make clean
make
cp libtuscany_sca_axis_wrapper.so $SCA4CPP/lib


cd $SCA4CPP_HOME/tools/scagen
ant
chmod a+x $SCA4CPP/bin/scagen.sh
