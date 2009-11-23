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

/**
 * @service
 * @binding.ws
 */
class Divide {

    /**
     * @reference 
     * @binding.tuscany cppDivideService
     */
    public $another_divide;
    
    /**
     * @reference 
     * @binding.tuscany add_service
     */
    public $add_service;    
    
    /**
     * @reference 
     * @binding.tuscany subtract_service
     */
    public $subtract_service;    
    
    /**
     * @reference 
     * @binding.tuscany multiply_service
     */
    public $multiply_service;   

    /**
     * Division
     *
     * @param float $num1 (the first number)
     * @param float $num2 (the second number)
     * @return float The result
     */
    function div($num1, $num2) {

        //$this->add_service->add(12.0, 27.0);
        //$this->subtract_service->sub(12.0, 27.0);        
        $this->multiply_service->mul(12.0, 27.0);
        
        $result = $this->another_divide->div($num1, $num2);

        return $result;
    }

}
  
?>
