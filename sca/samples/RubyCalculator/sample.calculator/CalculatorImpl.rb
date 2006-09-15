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

class CalculatorImpl

  attr_writer :divideService

  def initialize()
    print "Ruby - CalculatorImpl.initialize\n"
  end
	
  def div(arg1, arg2)
    print "Ruby - CalculatorImpl.div\n"
    @divideService.divide(arg1.to_f, arg2.to_f)
  end

  def add(arg1, arg2)
    print "Ruby - CalculatorImpl.add\n"
    arg1.to_f + arg2.to_f
  end

  def sub(arg1, arg2)
    print "Ruby - CalculatorImpl.sub\n"
    arg1.to_f - arg2.to_f
  end

  def mul(arg1, arg2)
    print "Ruby - CalculatorImpl.mul\n"
    arg1.to_f * arg2.to_f
  end

end