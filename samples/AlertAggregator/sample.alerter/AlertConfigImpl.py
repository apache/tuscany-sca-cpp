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

import xml.etree.ElementTree, os.path, datetime

def getAlertConfig ():

    # Set up some default config data
    configData = "<config xmlns=\"http://tuscany.apache.org/samples/alerter\">\n</config>"

    if os.path.isfile("config.xml"):
        # Retrieve the configuration from a file
        f = open("config.xml")
        try:
            configData = f.read()
        finally:
            f.close()
   
    configElem = xml.etree.ElementTree.XML(configData)

    return configElem

def getSourceConfig (sourceId):

    configElem = getAlertConfig()

    for sourceElem in configElem.findall("./{http://tuscany.apache.org/samples/alerter}source"):
        if sourceElem.attrib["id"] == sourceId:
            return sourceElem

    print "Source with id matching",sourceId,"not found"
    return

def updateSourceConfig (alertSourceConfigElem):

    configElem = getAlertConfig()
    sourceId = alertSourceConfigElem.attrib["id"]

    for sourceElem in configElem.findall("./{http://tuscany.apache.org/samples/alerter}source"):
        if sourceElem.attrib["id"] == sourceId:
            configElem.remove(sourceElem)
            configElem.append(alertSourceConfigElem)
            saveConfigData(configElem)
            return
    print "Source with id matching",sourceId,"not found"
    return

def addSourceConfig (alertSourceConfigElem):
    configElem = getAlertConfig()

    #check for source ID, make sure it's not the same as others and create one if it's missing
    if "id" in alertSourceConfigElem.attrib:
        configId = alertSourceConfigElem.attrib["id"]   
    else:
        configId = "0"

    gotConfigId = False

    while gotConfigId == False:

        config = getSourceConfig(configId)
        if config:
            configId = str( int(configId)+1 )
        else:
            gotConfigId = True


    alertSourceConfigElem.attrib["id"] = configId

    configElem.append(alertSourceConfigElem)
    saveConfigData(configElem)

    return configId

def saveConfigData (configElem):
    configData = xml.etree.ElementTree.tostring(configElem)

    # Save the configuration to a file
    f = open("config.xml", "w")

    if not f:
        print "Failed to open config file for writing"
    try:
        f.write(configData)
    finally:
        f.close()

def removeSourceConfig(sourceId):
    configElem = getAlertConfig()
    for sourceElem in configElem.findall("./{http://tuscany.apache.org/samples/alerter}source"):
        if sourceElem.attrib["id"] == sourceId:
            configElem.remove(sourceElem)
            saveConfigData(configElem)
            return

# # Testing
# if os.path.isfile("config.xml"):
#     os.remove("config.xml")
# data = getAlertConfig()
# print xml.etree.ElementTree.tostring(data), "\n"
#
# newsrcxml = "<source xmlns=\"http://tuscany.apache.org/samples/alerter\" type=\"rss\" id=\"0\">\n"
# newsrcxml += "<name>PSP Updates</name>\n"
# newsrcxml += "<address>http://pspupdates.qj.net</address>\n"
# newsrcxml += "<feedAddress>http://pspupdates.qj.net/rss.xml</feedAddress></source>"
#
# newsrc = xml.etree.ElementTree.XML(newsrcxml)
# ID = addSourceConfig(newsrc)
# print "Added src with ID:", ID, "\n"
#
# newsrcxml = "<source xmlns=\"http://tuscany.apache.org/samples/alerter\" type=\"rss\" id=\"1\">\n"
# newsrcxml += "<name>PSP Updates</name>\n"
# newsrcxml += "<address>http://pspupdates.qj.net!</address>\n"
# newsrcxml += "<feedAddress>http://pspupdates.qj.net/atom.xml</feedAddress></source>"
#
# newsrc = xml.etree.ElementTree.XML(newsrcxml)
# ID = addSourceConfig(newsrc)
# print "Added src with ID:", ID, "\n"
#
# data = getAlertConfig()
# print xml.etree.ElementTree.tostring(data), "\n"
#
# data = getSourceConfig("0")
# print xml.etree.ElementTree.tostring(data), "\n"
# print dir(data), "\n"
#
# name = data.find("./{http://tuscany.apache.org/samples/alerter}name")
# name.text = "Hello world!"
#
# updateSourceConfig(data)
# data = getSourceConfig("1")
# print xml.etree.ElementTree.tostring(data), "\n"
#
# removeSourceConfig("1")
# data = getAlertConfig()
# print xml.etree.ElementTree.tostring(data), "\n"
#
# ID = addSourceConfig(newsrc)
# print "Added src with ID:", ID, "\n"
#
# ID = addSourceConfig(newsrc)
# print "Added src with ID:", ID, "\n"
#
# data = getAlertConfig()
# print xml.etree.ElementTree.tostring(data), "\n"
#
#
# newsrcxml = "<source xmlns=\"http://tuscany.apache.org/samples/alerter\" type=\"rss\">\n"
# newsrcxml += "<name>PSP Updates</name>\n"
# newsrcxml += "<address>http://pspupdates.qj.net!</address>\n"
# newsrcxml += "<feedAddress>http://pspupdates.qj.net/atom.xml</feedAddress></source>"
#
# newsrc = xml.etree.ElementTree.XML(newsrcxml)
# ID = addSourceConfig(newsrc)
# print "Added src with ID:", ID, "\n"
#
# data = getAlertConfig()
# print xml.etree.ElementTree.tostring(data), "\n"
#

