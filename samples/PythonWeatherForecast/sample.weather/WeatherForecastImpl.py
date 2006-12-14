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
# This Python code is a simple sample that calls a service that returns an 
# SDO which has been converted into an ElementTree Element object. The Element
# object is then converted into formatted text
 
import xml.etree.ElementTree

def getForecastByPlaceName(placeName):

    print "Python - WeatherForecastImpl.getForecastByPlaceName:", placeName

    xmlResult = weatherWebService.GetWeatherByPlaceName(placeName)    

    print "Python - Formatting result from weatherService:", xmlResult

    return formatWeather(xmlResult, placeName)


def getForecastByZipCode(zipCode):

    print "Python - WeatherForecastImpl.getForecastByZipCode:", zipCode

    xmlResult = weatherWebService.GetWeatherByZipCode(zipCode)    

    print "Python - Formatting result from weatherService:", xmlResult

    return formatWeather(xmlResult, zipCode)


def formatWeather(weather, name):

    result = "Could not retrieve Weather Forecast for " + str(name)

    pathAndNS = "./{http://www.webservicex.net}"

    if weather.findtext(pathAndNS+"PlaceName"):
    
        result = "\n\nWeather Forecast for "+ weather.findtext(pathAndNS+"PlaceName")+ ", "+ weather.findtext(pathAndNS+"StateCode")+"\n"
        result += "Latitude: "+ weather.findtext(pathAndNS+"Latitude")+ " Longitude: "+ weather.findtext(pathAndNS+"Longitude")+"\n\n"
    
        for node in weather.findall(".//{http://www.webservicex.net}WeatherData"):
            if node.findtext(pathAndNS+"Day"): result += node.findtext(pathAndNS+"Day")
            if node.findtext(pathAndNS+"MaxTemperatureC"): result += ":\tMax "+ node.findtext(pathAndNS+"MaxTemperatureC")
            if node.findtext(pathAndNS+"MinTemperatureC"): result += "C. Min "+ node.findtext(pathAndNS+"MinTemperatureC")+ "C.\n"

    return result
