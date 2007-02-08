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

import feedparser, datetime, xml.etree.ElementTree, re

def getNewAlerts(rssaddress, lastchecktimestamp):

    print "RSSCheckerImpl.getNewAlerts() called", rssaddress, lastchecktimestamp

    #Get and parse the RSS/Atom data
    d = feedparser.parse(rssaddress)

    newalertsxml = "<alerts xmlns=\"http://tuscany.apache.org/samples/alerter\">\n"

    lastcheckdate = datetime.datetime.min
    if lastchecktimestamp:
        lastcheckdate = datetime.datetime.strptime(lastchecktimestamp, "%Y-%m-%dT%H:%M:%S")

    for entry in d.entries:

        (year, month, day, hour, minute, second, millisecond, microsecond, tzinfo) = entry.date_parsed
        entrydate = datetime.datetime(year, month, day, hour, minute, second)

        if (entrydate > lastcheckdate) :

            newalertsxml += "<alert><title>" + stripXML(entry.title) + "</title>\n"
            newalertsxml += "<address>" + entry.link + "</address>\n"
            newalertsxml += "<date>" + entrydate.isoformat() + "</date>\n"
            newalertsxml += "<summary>" + stripXML(entry.description) + "</summary></alert>\n"
    newalertsxml += "</alerts>"    

    return xml.etree.ElementTree.XML(newalertsxml)

def stripXML(data):
    elementsRemoved = re.sub("<.*?>","", data)
    entitiesRemoved = re.sub("&.*?;", " ", elementsRemoved)
    asciiEncoded = entitiesRemoved.encode('ASCII', 'replace')
    returnData = asciiEncoded.replace('&', 'and')
    return returnData


# Testing
# print xml.etree.ElementTree.tostring(getNewAlerts("http://www.engadget.com/rss.xml", "2007-02-07T17:11:14"))
#
#
# today = datetime.datetime.now().replace(hour=0, minute=0, second=0, microsecond=0)
# data2 = getNewAlerts("http://newsrss.bbc.co.uk/rss/newsonline_uk_edition/front_page/rss.xml", today.isoformat())
# print xml.etree.ElementTree.tostring(data2)
#
# print "1st call returned", len(data.findall("./{http://tuscany.apache.org/samples/alerter}alert")), "elements"
# print "2nd call returned", len(data2.findall("./{http://tuscany.apache.org/samples/alerter}alert")), "elements"


