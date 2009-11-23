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
# This Python code is a simple sample that provides a Python 
# client for the WeatherForecast sample
 

import sys
import sca
import xml.etree.ElementTree



# The main script:

# Locate the Weather service
weatherService = sca.locateservice("WeatherComponent")

place = "San Jose"
print "\nFinding Weather Forecast for", place
weatherText = weatherService.getForecastByPlaceName(place)
print weatherText 

zip = 90210
print "\nFinding Weather Forecast for zip code", zip
weatherText = weatherService.getForecastByZipCode(zip)
print weatherText





