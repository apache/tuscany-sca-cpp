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

if WANT_ALL_SAMPLES
  CPP_SAMPLES = CppCalculator
if WITH_AXIS2C
  CPP_AXIS2C_SAMPLES = CppBigBank
endif
  PYTHON_SAMPLES = PythonCalculator PythonWeatherForecast
  RUBY_SAMPLES = RubyCalculator RubyBigBank HttpdBigBank RestCalculator
endif
if WANT_CPP_SAMPLES
  CPP_SAMPLES = CppCalculator
if WITH_AXIS2C
  CPP_AXIS2C_SAMPLES = CppBigBank
endif
endif
if WANT_PYTHON_SAMPLES
  PYTHON_SAMPLES = PythonCalculator PythonWeatherForecast
endif
if WANT_RUBY_SAMPLES
  RUBY_SAMPLES = RubyCalculator RubyBigBank HttpdBigBank RestCalculator
endif
if WANT_PHP_SAMPLES
endif
SUBDIRS = ${CPP_SAMPLES} ${CPP_AXIS2C_SAMPLES} ${PYTHON_SAMPLES} ${RUBY_SAMPLES} ${PHP_SAMPLES}

EXTRA_DIST = GettingStarted.html
