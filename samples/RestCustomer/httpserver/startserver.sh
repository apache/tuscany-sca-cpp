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

if [ x$PYTHON_LIB != x ]; then
echo "Using Python library installed at $PYTHON_LIB"
export LD_LIBRARY_PATH=$PYTHON_LIB:$LD_LIBRARY_PATH
export PATH=$PYTHON_LIB/../bin:$PATH
fi

export LD_LIBRARY_PATH=$TUSCANY_SCACPP/lib:$TUSCANY_SCACPP/extensions/python/lib:$TUSCANY_SDOCPP/lib:$AXIS2C_HOME/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$TUSCANY_SCACPP/extensions/python/lib:$PYTHONPATH

export TUSCANY_SCACPP_ROOT=$APFULLDIR/../
export TUSCANY_SCACPP_BASE_URI=http://localhost:9090

export CUSTOMER_DIR=$TUSCANY_SCACPP_ROOT/sample.customer

libsuffix=.so
UNAME=`uname -s`
if [ "x$UNAME" = "xDarwin" ]; then
    libsuffix=.dylib
fi

# Generate the mod_rest configuration
if [ ! -f conf/tuscany_sca_mod_rest.conf ]; then
  echo "LoadModule sca_rest_module $TUSCANY_SCACPP/extensions/rest/service/lib/libtuscany_sca_mod_rest$libsuffix" >conf/tuscany_sca_mod_rest.conf
  echo "TuscanyHome $TUSCANY_SCACPP" >>conf/tuscany_sca_mod_rest.conf
  echo "<Location /rest>" >>conf/tuscany_sca_mod_rest.conf
  echo "        SetHandler sca_rest_module" >>conf/tuscany_sca_mod_rest.conf
  echo "        TuscanyRoot $TUSCANY_SCACPP_ROOT" >>conf/tuscany_sca_mod_rest.conf
  echo "</Location>" >>conf/tuscany_sca_mod_rest.conf
fi

if [ ! -f conf/base.conf ]; then
  echo "DocumentRoot $APFULLDIR/htdocs" >conf/base.conf
fi

# Create logs directory
if [ ! -d logs ]; then
  mkdir logs
fi

# Start the HTTP server
echo "Starting Apache httpd"
apachectl -k start -d $APFULLDIR

