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

/**
 * @service
 * @binding.ws
 */
class Calculator {

    /**
     * @reference
     * @binding.php Add.php   
     */
	public $add_service;

    /**
     * @reference
     * @binding.php Subtract.php   
     */
	public $sub_service;
	
    /**
     * @reference
     * @binding.php Multiply.php   
     */
	public $mul_service;	

    /**
     * @reference
     * @binding.ws Divide.wsdl
     */
	public $div_service;
	
    /**
     * Addition
     * 
     * @param float $num1 (the first number)
     * @param float $num2 (the second number)
     * @return float The result
     */
    function add($num1, $num2) {
        return $this->mul_service->add($num1, $num2);
    }

    /**
     * Subtraction
     * 
     * @param float $num1 (the first number)
     * @param float $num2 (the second number)
     * @return float The result
     */
    function sub($num1, $num2) {
        return $this->mul_service->sub($num1, $num2);
    }
    
    /**
     * Multiplication
     * 
     * @param float $num1 (the first number)
     * @param float $num2 (the second number)
     * @return float The result
     */
    function mul($num1, $num2) {
        return $this->mul_service->mul($num1, $num2);
    }
    
    /**
     * Division
     * 
     * @param float $num1 (the first number)
     * @param float $num2 (the second number)
     * @return float The result
     */
    function div($num1, $num2) {
        return $this->div_service->div($num1, $num2);
    }    
}

?>
