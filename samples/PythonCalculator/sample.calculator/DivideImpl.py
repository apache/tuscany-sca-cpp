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
# the Divide Service used in the Calculator sample
# 
# Either use the divide function in the DivideClass class (using classes allows 
# composite scoping to be used)
# e.g. in CalculatorComposite.composite use the line:
#   <implementation.python module="DivideImpl" class="DivideClass"/>
# 
# Or just use the module-level divide function
# e.g. in CalculatorComposite.composite use the line:
#   <implementation.python module="DivideImpl" />
# 
 

class DivideClass:
    "A class to handle dividing"

    def __init__(self):
        print "Python - DivideImpl.DivideClass constructor"

    # The class-level divide function
    def divide(self, val1, val2):
        result = float(val1) / float(val2)

        print "Python - DivideImpl.DivideClass.divide " + str( val1 ) + " / " + str(val2) + " = " + str(result)

        # Use the doRounding property
        if doRounding:
           result = round(result)
           print "Python - DivideImpl.DivideClass.divide is rounding the result to " + str(result)

        return result

# The module-level divide function
def divide(val1, val2):
    result = float(val1) / float(val2)
    print "Python - DivideImpl.divide " + str(val1) + " / " + str(val2) + " = " + str(result)

    # Use the doRounding property
    if doRounding:
        result = round(result)
        print "Python - DivideImpl.divide is rounding the result to " + str(result)

    return result

