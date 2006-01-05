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
SDO4CPP_HOME="$APFULLDIR"
if [ x$SDO4CPP = x ]; then
SDO4CPP="$SDO4CPP_HOME"
fi

echo "Using SDO installed at $SDO4CPP"

cd $SDO4CPP_HOME/runtime/core/test
cd Debug
make clean
make
cd ..
export LD_LIBRARY_PATH=$SDO4CPP/lib:$LD_LIBRARY_PATH
Debug/tuscany_sdo_test
