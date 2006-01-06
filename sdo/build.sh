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
DEFAULT_DIR="/usr/local/apache2"

export SDO4CPP_HOME="$APFULLDIR"
export SDO4CPP="$APFULLDIR"

cd $SDO4CPP_HOME/runtime/core/src/Debug
make clean
make
cp ../commonj/sdo/*.h $SDO4CPP/include/commonj/sdo
cp libtuscany_sdo.so $SDO4CPP/lib
