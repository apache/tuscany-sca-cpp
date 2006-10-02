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
# 
# 
# This Python code is a simple sample that provides a Python implementation of 
# the Calculator sample
# 
 

# The module-level add function
def add(val1, val2):
    result = float(val1) + float(val2)
    print "Python - CalculatorImpl.add " + str(val1) + " + " + str(val2) + " = " + str(result)
    return result

# The module-level sub function
def sub(val1, val2):
    result = float(val1) - float(val2)
    print "Python - CalculatorImpl.sub " + str(val1) + " - " + str(val2) + " = " + str(result)
    return result

# The module-level mul function
def mul(val1, val2):
    result = float(val1) * float(val2)
    print "Python - CalculatorImpl.mul " + str(val1) + " * " + str(val2) + " = " + str(result)
    return result

# The module-level div function
def div(val1, val2):
    

    print "Python - CalculatorImpl.div calling divideService to determine " + str(val1) + " / " + str(val2)

    # Use the divideService reference
    result = divideService.divide(val1, val2)

    print "Python - CalculatorImpl.div divideService returned " + str(result)

    return result

