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
import xml.etree.ElementTree, datetime

def getAllNewAlerts ():

    returnXML = "<alerts xmlns=\"http://tuscany.apache.org/samples/alerter\">\n</alerts>"
    returnElem = xml.etree.ElementTree.XML(returnXML)

    # Use the alertConfigService to get the configuration
    configElem = alertConfigService.getAlertConfig()

    for sourceElem in configElem.findall("./{http://tuscany.apache.org/samples/alerter}source"):

        sourceid = sourceElem.attrib["id"]

        newAlerts = getAlerts(sourceElem)
        if xml.etree.ElementTree.iselement(newAlerts):
            # Add the sourceid to each alert and append to the entire list
            for alert in newAlerts.findall("./{http://tuscany.apache.org/samples/alerter}alert"):
                alert.attrib["sourceid"] = sourceid
                returnElem.append(alert)

    return returnElem

def getAlerts (sourceElem):

    lastcheckedElem = sourceElem.find("./{http://tuscany.apache.org/samples/alerter}lastChecked")
    if not xml.etree.ElementTree.iselement(lastcheckedElem):
        lastcheckedElem = xml.etree.ElementTree.SubElement(sourceElem, "lastChecked")
        lastcheckedElem.text = ""

    if sourceElem.attrib["type"] == "rss":

        feedaddress = sourceElem.find("./{http://tuscany.apache.org/samples/alerter}feedAddress").text

        if feedaddress:
            # Use the rssCheckerService to get new articles for this config
            newAlerts = rssCheckerService.getNewAlerts(feedaddress, lastcheckedElem.text)

            lastcheckedElem.text = datetime.datetime.now().replace(microsecond=0).isoformat()
            alertConfigService.updateSourceConfig(sourceElem)
            return newAlerts            

    elif sourceElem.attrib["type"] == "pop":

        popserver = sourceElem.find("./{http://tuscany.apache.org/samples/alerter}popServer").text
        popusername = sourceElem.find("./{http://tuscany.apache.org/samples/alerter}popUsername").text
        poppassword = sourceElem.find("./{http://tuscany.apache.org/samples/alerter}popPassword").text

        if popserver and popusername and poppassword:
            # Use the popCheckerService to get new emails for this config
            newAlerts = popCheckerService.getNewAlerts(popserver, popusername, poppassword, lastcheckedElem.text)

            lastcheckedElem.text = datetime.datetime.now().replace(microsecond=0).isoformat()
            alertConfigService.updateSourceConfig(sourceElem)
            return newAlerts            

    else:
        print "Source type not supported: ", sourceElem.attrib["type"]

    return None
        

def getNewAlerts (sourceId):
    # Use the alertConfigService to get the configuration
    sourceConfigElem = alertConfigService.getSourceConfig(sourceId)

    # Get the alerts
    newAlerts = getAlerts(sourceConfigElem)

    if xml.etree.ElementTree.iselement(newAlerts):
        # Add the sourceid to each alert
        for alert in newAlerts.findall("./{http://tuscany.apache.org/samples/alerter}alert"):
            alert.attrib["sourceid"] = sourceId
        
    return newAlerts

def addAlertSource (sourceElem):    
    # Add the source directly to the configuration
    return alertConfigService.addSourceConfig(sourceElem)

def updateAlertSource (sourceElem):
    # Update the source in the configuration
    return alertConfigService.updateSourceConfig(sourceElem)

def removeAlertSource (sourceId):
    # Remove the source from the configuration
    alertConfigService.removeSourceConfig(sourceId)

def getAlertSources ():
    # Get the config for all the sources
    return alertConfigService.getAlertConfig()

# # Testing
# import AlertConfigImpl as alertConfigService
# import RSSCheckerImpl as rssCheckerService
# import POPCheckerImpl as popCheckerService
#
# print xml.etree.ElementTree.tostring(getAlertSources())
# print xml.etree.ElementTree.tostring(getAllNewAlerts())
