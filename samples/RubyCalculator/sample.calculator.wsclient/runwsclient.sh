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

TEST_SYSTEM=$APFULLDIR/../

export LD_LIBRARY_PATH=$TUSCANY_SCACPP/lib:$TUSCANY_SDOCPP/lib:$AXIS2C_HOME/lib:$LD_LIBRARY_PATH

export TUSCANY_SCACPP_SYSTEM_ROOT=$TEST_SYSTEM
export TUSCANY_SCACPP_DEFAULT_COMPONENT=sample.calculator.CalculatorWSClientComponent

cd $TUSCANY_SCACPP_SYSTEM_ROOT/packages/sample.calculator.wsclient
ruby -I$TUSCANY_SCACPP/extensions/ruby/lib CalculatorWSClient.rb
