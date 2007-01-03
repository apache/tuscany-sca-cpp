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

import xml.etree.ElementTree

def search(searchText):

    print "Python - YahooWebSearchImpl.py running Yahoo search for:", searchText, "with ID:", yahooAppID

    # Use the yahooWebSearchService reference to call the Yahoo REST service
    # also use the yahooAppID property from the composite definition
    xmlResult = yahooWebSearchService.webSearch(appid = yahooAppID, query = searchText, results = 5)    

    print "Python - YahooWebSearchImpl.py formatting result from Yahoo:", xmlResult

    return formatResults(xmlResult, searchText)


def formatResults(results, searchText):

   formatted = "Failed to run Yahoo search service for \"" + str(searchText) + "\""

   ns = "{urn:yahoo:srch}"
   pathAndNS = "./" + ns

   formatted = "\n\nYahoo Web Search Results for \""+ str(searchText) +"\"\n" 
   formatted += results.attrib["totalResultsAvailable"]+ " results available. First "+ results.attrib["totalResultsReturned"]+ " below\n\n"
   pos = 1

   for node in results.findall(".//"+ns+"Result"):
      formatted += str(pos) + ". "+ node.findtext(pathAndNS+"Title") +"\n\t"
      formatted += node.findtext(pathAndNS+"Summary") +"\n\t"
      formatted += node.findtext(pathAndNS+"Url") +"\n\n"
      pos += 1

   return formatted
