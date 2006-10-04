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
export SDO4CPP_HOME="$APFULLDIR"
if [ x$TUSCANY_SDOCPP = x ]; then
export TUSCANY_SDOCPP="$SDO4CPP_HOME"
fi

echo "Using SDO installed at $TUSCANY_SDOCPP"

if [ `uname -s` = Darwin ]
then
  export DYLD_LIBRARY_PATH=$TUSCANY_SDOCPP/lib:$DYLD_LIBRARY_PATH
else
  export LD_LIBRARY_PATH=$TUSCANY_SDOCPP/lib:$LD_LIBRARY_PATH
fi
cd runtime/core/test
./tuscany_sdo_test
