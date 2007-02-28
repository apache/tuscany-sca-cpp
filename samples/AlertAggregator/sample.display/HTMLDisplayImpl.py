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


import xml.etree.ElementTree, datetime, os.path, os

ns = "./{http://tuscany.apache.org/samples/alerter}"

def getSavedAlerts():

    # Set up some default config data
    alertsData = "<alerts xmlns=\"http://tuscany.apache.org/samples/alerter\">\n</alerts>"

    if os.path.isfile(os.environ['TUSCANY_SCACPP_ROOT']+"/sample.display/alerts.xml"):
        # Retrieve the configuration from a file
        f = open(os.environ['TUSCANY_SCACPP_ROOT']+"/sample.display/alerts.xml")
        try:
            alertsData = f.read()
        finally:
            f.close()
   
    alertsElem = xml.etree.ElementTree.XML(alertsData)

    return alertsElem

def saveAlerts(alertsElem):
    alertsData = xml.etree.ElementTree.tostring(alertsElem)

    # Save the configuration to a file
    f = open(os.environ['TUSCANY_SCACPP_ROOT']+"/sample.display/alerts.xml", "w")

    if not f:
        print "Failed to open alerts file for writing"
    try:
        f.write(alertsData)
    finally:
        f.close()


def updateAllAlertsHTMLTable ():

    alertsElem = getSavedAlerts()

    # Use the alertService reference
    newAlertsElem = alertService.getAllNewAlerts()
    #newAlertsElem = getAllNewAlerts(); # For testing

    for alert in newAlertsElem.findall(ns+"alert"):

        alert.attrib["unread"]="True"
        alertsElem.append(alert)          

    return generateHTMLTable(alertsElem)


def updateSourceAlertsHTMLTable (sourceId):

    alertsElem = getSavedAlerts()

    # Use the alertService reference
    newAlertsElem = alertService.getNewAlerts(sourceId);
    #newAlertsElem = getAllNewAlerts(); # For testing

    for alert in newAlertsElem.findall(ns+"alert"):

        alert.attrib["unread"]="True"
        alertsElem.append(alert)        

    return generateHTMLTable(alertsElem)



def generateHTMLTable (alertsElem):

    returnData = "<TABLE border=\"0\">"    

    # If we have more than x alerts, clear out any that have been read
    # x is determined by the showNumberOfReadAlerts property
    alerts = alertsElem.findall(ns+"alert")
    if len(alerts) > int(showNumberOfReadAlerts):
        for alert in alerts:
            if alert.attrib["unread"]!="True":
                alertsElem.remove(alert)    

    alertIDNumber = 0
    for alert in alertsElem.findall(ns+"alert"):
        
        date = datetime.datetime.strptime(alert.find(ns+"date").text, "%Y-%m-%dT%H:%M:%S")
        alertID = "alert_"+str(alertIDNumber)
        alert.attrib["id"]= alertID
        alertIDNumber += 1

        returnData += "<TR class=\"source_"
        returnData += alert.attrib["sourceid"]
        returnData += " clickable\" onclick=\"displayAlert('"
        address = alert.find(ns+"address").text
        if address != None:
            returnData += address
        returnData += "', '"+alertID+"')\"><TD><SPAN id=\""+alertID+"\" class=\""
        if alert.attrib["unread"] == "True":
            returnData += "unread_title"
        else:
            returnData += "read_title"
        returnData += "\">"
        title = alert.find(ns+"title").text
        noOfChars = 75-len(title)
        
        if noOfChars>0:
            returnData += title
            returnData += "</SPAN><SPAN class=\"summary\"> - "
            returnData += alert.find(ns+"summary").text[0:noOfChars]
            returnData += "...</SPAN></TD><TD>"            
        else:
            returnData += title[0:80]
            returnData += "</SPAN></TD><TD>"            
        
        returnData += date.strftime("%d/%m/%Y %I:%M %p")
        returnData += "</TD></TR>\n"        

    returnData += "</TABLE>" 

    saveAlerts(alertsElem)
    #print xml.etree.ElementTree.tostring(alertsElem)
    return returnData

def readAlert (alertID):

    alertsElem = getSavedAlerts()

    returnData = ""
    for alert in alertsElem.findall(ns+"alert"):

        if alert.attrib["id"]==alertID:
            alert.attrib["unread"] = "False"

            saveAlerts(alertsElem)

            srcType = getSourceType(alert.attrib["sourceid"])

            if srcType=="pop":
                returnData += "<PRE>"
                returnData += alert.find(ns+"summary").text
                returnData += "</PRE>"
                return returnData

    return

def getSourceType(sourceid):
    # Use the alertService reference
    alertSources = alertService.getAlertSources()
    #alertSources = getAlertSources() #testing

    for source in alertSources.findall(ns+"source"):

        if sourceid == source.attrib["id"]:
            return source.attrib["type"]

    return "none"


def getAlertSourcesHTMLTable():

    returnData = "<TABLE border=\"0\">\n"    

    # Use the alertService reference
    alertSources = alertService.getAlertSources()
    #alertSources = getAlertSources() #testing

    srcIdList = []

    for source in alertSources.findall(ns+"source"):

        srcId = source.attrib["id"]
        srcIdList.append(srcId)
        srcType = source.attrib["type"]        

        # Write out the source data row
        returnData += "<TR CLASS=\"source_"
        returnData += srcId
        returnData += "\"><TD CLASS=\"clickable\" ONCLICK=\"displayAlert('"
        returnData += source.find(ns+"address").text
        returnData += "', '')\">"
        returnData += "<IMG SRC=\""
        returnData += srcType
        returnData += ".png\"/>&nbsp;&nbsp;"
        returnData += source.find(ns+"name").text
        returnData += "</TD><TD CLASS=\"clickable link\" ONCLICK=\"getAlerts('"
        returnData += srcId
        returnData += "')\">Update Alerts</TD><TD CLASS=\"clickable link\" ONCLICK=\"showEditSource('"
        returnData += srcId
        returnData += "')\">Edit Source</TD><TD CLASS=\"clickable link\" ONCLICK=\"deleteSource('"
        returnData += srcId
        returnData += "')\">Delete Source</TD></TR>\n"

        # Now write out the row that gets shown when "edit" is pressed
        returnData += "<TR ID=\"edit_source_"
        returnData += srcId
        returnData += "\" CLASS=\"hidden source_"
        returnData += srcId
        returnData += "\"><TD COLSPAN=\"4\"><TABLE CLASS=\"sourceDetailsTable\"><TR><TD>Source name:</TD><TD><INPUT ID=\"source_"
        returnData += srcId
        returnData += "_name\" TYPE=\"TEXT\" SIZE=\"50\" VALUE=\""
        returnData += source.find(ns+"name").text
        returnData += "\"/></TR><TR><TD>Source address:</TD><TD><INPUT ID=\"source_"
        returnData += srcId
        returnData += "_address\" TYPE=\"TEXT\" SIZE=\"50\" VALUE=\""
        returnData += source.find(ns+"address").text
        returnData += "\"/></TR>"
        if srcType=="rss":
            returnData += "<TR><TD>Feed address:</TD><TD><INPUT ID=\"source_"
            returnData += srcId
            returnData += "_feedAddress\" TYPE=\"TEXT\" SIZE=\"50\" VALUE=\""
            returnData += source.find(ns+"feedAddress").text
            returnData += "\"/></TR>"
        elif srcType=="pop":
            returnData += "<TR><TD>POP3 server:</TD><TD><INPUT ID=\"source_"
            returnData += srcId
            returnData += "_popServer\" TYPE=\"TEXT\" SIZE=\"50\" VALUE=\""
            returnData += source.find(ns+"popServer").text
            returnData += "\"/></TR>"
            returnData += "<TR><TD>Username:</TD><TD><INPUT ID=\"source_"
            returnData += srcId
            returnData += "_popUsername\" TYPE=\"TEXT\" SIZE=\"50\" VALUE=\""
            returnData += source.find(ns+"popUsername").text
            returnData += "\"/></TD></TR>"
            returnData += "<TR><TD>Password:</TD><TD><INPUT ID=\"source_"
            returnData += srcId
            returnData += "_popPassword\" TYPE=\"PASSWORD\" SIZE=\"50\" VALUE=\""
            returnData += source.find(ns+"popPassword").text
            returnData += "\"/></TD></TR>"
        returnData += "<TR><TD><INPUT ID=\"source_"
        returnData += srcId
        returnData += "_type\" TYPE=\"HIDDEN\" VALUE=\""
        returnData += srcType
        returnData += "\"/><INPUT TYPE=\"BUTTON\" VALUE=\"Update\" ONCLICK=\"updateSource('"
        returnData += srcId
        returnData += "')\"><INPUT TYPE=\"BUTTON\" VALUE=\"Cancel\" ONCLICK=\"hideEditSource('"
        returnData += srcId
        returnData += "')\"></TR></TABLE></TD></TR>"

    # Get the first unused srcId
    srcId = "0"
    while srcId in srcIdList:
        srcId = str(int(srcId)+1)

    # Now write out the "add new source" row
    returnData += "<TR CLASS=\"source_"
    returnData += srcId
    returnData += "\"><TD COLSPAN=\"4\" CLASS=\"clickable link\" ONCLICK=\"showAddNewSource('"
    returnData += srcId
    returnData += "')\">Add new Alert Source</TD></TR>"
    # Add the (initially hidden) new source details row
    returnData += "<TR ID=\"add_source_"
    returnData += srcId
    returnData += "\"CLASS=\"hidden source_"
    returnData += srcId
    returnData += "\"><TD COLSPAN=\"4\"><TABLE CLASS=\"sourceDetailsTable\"><TR><TD>Source name:</TD><TD><INPUT ID=\"source_"
    returnData += srcId
    returnData += "_name\" TYPE=\"TEXT\" SIZE=\"50\"/></TD></TR><TR><TD>Source address:</TD><TD><INPUT ID=\"source_"
    returnData += srcId
    returnData += "_address\" TYPE=\"TEXT\" SIZE=\"50\"/></TD></TR><TR><TD>Source type:</TD><TD><SELECT ID=\"source_"
    returnData += srcId
    returnData += "_type\" ONCHANGE=\"showSourceType('"
    returnData += srcId
    returnData += "')\"><OPTION value=\"rss\" selected=\"selected\">RSS/Atom feed</OPTION>"
    returnData += "<OPTION value=\"pop\">POP3 Email</OPTION></SELECT></TD></TR>"
    # Section for RSS feeds
    returnData += "<TR ID=\"add_rss_source\"><TD COLSPAN=\"2\"><TABLE CLASS=\"sourceDetailsTable\"><TR><TD>Feed address:</TD><TD><INPUT ID=\"source_"
    returnData += srcId
    returnData += "_feedAddress\" TYPE=\"TEXT\" SIZE=\"50\"/></TD></TR></TABLE></TD></TR>"
    # Section for POP3 email
    returnData += "<TR ID=\"add_pop_source\" CLASS=\"hidden\"><TD COLSPAN=\"2\"><TABLE CLASS=\"sourceDetailsTable\"><TR><TD>POP3 server:</TD><TD><INPUT ID=\"source_"
    returnData += srcId
    returnData += "_popServer\" TYPE=\"TEXT\" SIZE=\"50\"/></TR>"
    returnData += "<TR><TD>Username:</TD><TD><INPUT ID=\"source_"
    returnData += srcId
    returnData += "_popUsername\" TYPE=\"TEXT\" SIZE=\"50\"/></TD></TR>"
    returnData += "<TR><TD>Password:</TD><TD><INPUT ID=\"source_"
    returnData += srcId
    returnData += "_popPassword\" TYPE=\"PASSWORD\" SIZE=\"50\"/></TD></TR></TABLE></TD></TR>"
    # Last row for buttons
    returnData += "<TR><TD><INPUT TYPE=\"BUTTON\" VALUE=\"Add\" ONCLICK=\"addSource('"
    returnData += srcId
    returnData += "')\"><INPUT TYPE=\"BUTTON\" VALUE=\"Cancel\" ONCLICK=\"hideAddNewSource('"
    returnData += srcId
    returnData += "')\"></TD></TR></TABLE></TD></TR>"
    returnData += "</TABLE>\n"
    return returnData


def deleteAlertSource (sourceId):
    # Use the alertService reference    
    alertService.removeAlertSource(sourceId)

    # Remove all alerts with this sourceid
    for alert in alertsElem.findall(ns+"alert"):
        if alert.attrib["sourceid"]==sourceId:
            alertsElem.remove(alert)

def addAlertSource(alertSourceElem):
    if xml.etree.ElementTree.iselement(alertSourceElem):
        # Use the alertService reference    
        alertService.addAlertSource(alertSourceElem)
    return

def updateAlertSource(alertSourceElem):

    if xml.etree.ElementTree.iselement(alertSourceElem):

        # Get the sourceid
        sourceId = alertSourceElem.attrib["id"]

        # Remove all alerts with this sourceid - they may have changed
        for alert in alertsElem.findall(ns+"alert"):
            if alert.attrib["sourceid"]==sourceId:
                alertsElem.remove(alert)

        # Use the alertService reference to update the config
        alertService.updateAlertSource(alertSourceElem)

    return


# # For testing
# showNumberOfReadAlerts = 3
#
# def getAllNewAlerts ():
#
#     data = "<alerts xmlns=\"http://tuscany.apache.org/samples/alerter\">"
#
#     data += "<alert sourceid=\"0\" id=\"alert_0\"><title>Arrests over 'terror kidnap plot'</title>"
#     data += "<summary>Anti-terror police arrest eight people in Birmingham over an alleged plot to kidnap a member of the armed forces.</summary>"
#     data += "<address>http://news.bbc.co.uk/go/rss/-/1/hi/uk/6315989.stm</address><date>2007-01-31T15:34:18</date></alert>"
#
#     data += "</alerts>"
#
#     return xml.etree.ElementTree.XML(data)
#
# print getAlertsHTMLTable () + "\n"
#
# readAlert("http://news.bbc.co.uk/go/rss/-/1/hi/uk/6315989.stm")
#
# print getAlertsHTMLTable () + "\n"
# 
# def getAlertSources ():
#
#     data = "<config xmlns=\"http://tuscany.apache.org/samples/alerter\">"
#     data += "<source id=\"0\" type=\"rss\"><name>BBC News</name>"
#     data += "<address>http://news.bbc.co.uk/</address>"
#     data += "<lastChecked>2007-02-02T10:08:40</lastChecked>"
#     data += "<feedAddress>http://newsrss.bbc.co.uk/rss/newsonline_uk_edition/front_page/rss.xml</feedAddress></source>"
#     data += "</config>"
#     return xml.etree.ElementTree.XML(data)
#
# print getAlertSourcesHTMLTable()
