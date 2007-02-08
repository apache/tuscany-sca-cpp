<?php
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
# 
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#
#
include 'SCA/SCA.php';

// Get a proxy to the local Calculator.php
$service = SCA::getService('Calculator.wsdl');

// Call the local service and write out the response
//echo "add(1.23, 4.56) = " . $service->add(1.23, 4.56) . "\n";
//echo "sub(1.23, 4.56) = " . $service->sub(1.23, 4.56) . "\n";
//echo "mul(1.23, 4.56) = " . $service->mul(1.23, 4.56) . "\n";
echo "div(1.23, 4.56) = " . $service->div(1.23, 4.56) . "\n";

?>