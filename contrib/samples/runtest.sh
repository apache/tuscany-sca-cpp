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


echo "Running CppCalculator client"
cd $TUSCANY_SCACPP/samples/CppCalculator/deploy/sample.calculator.client
r=`./runclient.sh | grep "div(5,2) = 2.5"`
if [ "x$r" == "x" ]
then
  echo "CppCalculator client failed"
fi

echo "Running CppCalculator wsclient"
cd $TUSCANY_SCACPP/samples/CppCalculator/deploy/sample.calculator
(./runwsserver.sh >/dev/null)&
cd $TUSCANY_SCACPP/samples/CppCalculator/deploy/sample.calculator.wsclient
r=`./runwsclient.sh | grep "Result = 2.500e+00"`
killall -q axis2_http_server >/dev/null
if [ "x$r" == "x" ]
then
  echo "CppCalculator wsclient failed"
fi

echo "Running CppBigBank client"
cd $TUSCANY_SCACPP/samples/CppBigBank/deploy/bigbank.client
r=`./runclient.sh | grep "Balance"`
if [ "x$r" == "x" ]
then
  echo "CppBigBank client failed"
fi

echo "Running CppBigBank wsclient"
cd $TUSCANY_SCACPP/samples/CppBigBank/deploy/bigbank.account
(./runwsserver.sh >/dev/null)&
cd $TUSCANY_SCACPP/samples/CppBigBank/deploy/bigbank.wsclient
r=`./runwsclient.sh | grep "</balance>"`
killall -q axis2_http_server >/dev/null
if [ "x$r" == "x" ]
then
  echo "CppBigBank wsclient failed"
fi

echo "Running RubyCalculator client"
cd $TUSCANY_SCACPP/samples/RubyCalculator/deploy/sample.calculator.client
r=`./runclient.sh | grep "^3"`
if [ "x$r" == "x" ]
then
  echo "RubyCalculator client failed"
fi

echo "Running RubyCalculator wsclient"
cd $TUSCANY_SCACPP/samples/RubyCalculator/deploy/sample.calculator
(./runwsserver.sh >/dev/null)&
cd $TUSCANY_SCACPP/samples/RubyCalculator/deploy/sample.calculator.wsclient
r=`./runwsclient.sh | grep "^3.0"`
killall -q axis2_http_server >/dev/null
if [ "x$r" == "x" ]
then
  echo "RubyCalculator wsclient failed"
fi

echo "Running RubyBigBank client"
cd $TUSCANY_SCACPP/samples/RubyBigBank/deploy/bigbank.client
r=`./runclient.sh | grep "Balance:"`
if [ "x$r" == "x" ]
then
  echo "RubyBigBank client failed"
fi

echo "Running RubyBigBank wsclient"
cd $TUSCANY_SCACPP/samples/RubyBigBank/deploy/bigbank.account
(./runwsserver.sh >/dev/null)&
cd $TUSCANY_SCACPP/samples/RubyBigBank/deploy/bigbank.wsclient
r=`./runwsclient.sh | grep "Balance:"`
killall -q axis2_http_server >/dev/null
if [ "x$r" == "x" ]
then
  echo "RubyBigBank wsclient failed"
fi

echo "Running PythonCalculator client"
cd $TUSCANY_SCACPP/samples/PythonCalculator/deploy/sample.calculator.client
r=`./runclient.sh | grep "div ( 5 , 2 ) =  3.0"`
if [ "x$r" == "x" ]
then
  echo "PythonCalculator client failed"
fi

echo "Running PythonCalculator wsclient"
cd $TUSCANY_SCACPP/samples/PythonCalculator/deploy/sample.calculator
(./runwsserver.sh >/dev/null)&
cd $TUSCANY_SCACPP/samples/PythonCalculator/deploy/sample.calculator.wsclient
r=`./runwsclient.sh | grep "div ( 5 , 2 ) =  3.0"`
killall -q axis2_http_server >/dev/null
if [ "x$r" == "x" ]
then
  echo "PythonCalculator wsclient failed"
fi

echo "Running PythonWeatherForecast client"
cd $TUSCANY_SCACPP/samples/PythonWeatherForecast/deploy/sample.weather.client
r=`./runclient.sh | grep "Latitude: 34.090107"`
if [ "x$r" == "x" ]
then
  echo "PythonWeatherForecast client failed"
fi

echo "Running HttpdBigBank wsclient"
cd $TUSCANY_SCACPP/samples/HttpdBigBank/deploy/httpserver
./startserver.sh >/dev/null
cd $TUSCANY_SCACPP/samples/HttpdBigBank/deploy/bigbank.wsclient
r=`./runwsclient.sh | grep "Balance:"`
cd $TUSCANY_SCACPP/samples/HttpdBigBank/deploy/httpserver
./stopserver.sh >/dev/null
if [ "x$r" == "x" ]
then
  echo "HttpdBigBank wsclient failed"
fi

echo "Running RestCalculator restclient"
cd $TUSCANY_SCACPP/samples/RestCalculator/deploy/httpserver
./startserver.sh >/dev/null
cd $TUSCANY_SCACPP/samples/RestCalculator/deploy/sample.calculator.restclient
r=`./runrestclient.sh | grep "^3"`
cd $TUSCANY_SCACPP/samples/RestCalculator/deploy/httpserver
./stopserver.sh >/dev/null
if [ "x$r" == "x" ]
then
  echo "RestCalculator restclient failed"
fi

echo "Running RestCustomer restclient"
cd $TUSCANY_SCACPP/samples/RestCustomer/deploy/httpserver
./startserver.sh >/dev/null
cd $TUSCANY_SCACPP/samples/RestCustomer/deploy/sample.customer.restclient
r=`./runrestclient.sh | grep "Command  - Deleted customer 1234"`
cd $TUSCANY_SCACPP/samples/RestCustomer/deploy/httpserver
./stopserver.sh >/dev/null
if [ "x$r" == "x" ]
then
  echo "RestCustomer restclient failed"
fi

