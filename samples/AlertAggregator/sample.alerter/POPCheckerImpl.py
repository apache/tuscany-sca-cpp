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

import poplib, email, datetime, re, xml.etree.ElementTree
                                                
def getNewAlerts(popserver, username, password, lastchecktimestamp):

    print "POPCheckerImpl getting new POP e-mail alerts\n"

    alertsXML = "<alerts xmlns=\"http://tuscany.apache.org/samples/alerter\">\n"

    # initially set lastchecked to the epoch before trying to use the lastchecked string
    lastcheckdate = datetime.datetime.min
    if lastchecktimestamp:
        lastcheckdate = datetime.datetime.strptime(lastchecktimestamp, "%Y-%m-%dT%H:%M:%S")

    numberOfNewEmails = 0

    mail = poplib.POP3(popserver)
    mail.user(username)
    mail.pass_(password)
    msgCount, inboxSize = mail.stat()    

    for msgNum in range(msgCount):

        print "getting msg", msgNum+1, "of", msgCount

        data = ""
        for line in mail.retr(msgNum+1)[1]:
            data += str(line)
            data += "\n"

        msg = email.message_from_string(data)

        # date is of form "Sun, 21 Jan 2007 13:51:53 -0500"
        msgDateString = msg.get("Date")        
        msgDateString, tz = msgDateString.rsplit(None, 1) 

        timezoneadjust = datetime.timedelta(0, (int(tz)/100)*60*60 + int(tz[3:5])*60)
        
        msgdate = datetime.datetime.strptime(msgDateString, "%a, %d %b %Y %H:%M:%S") + timezoneadjust        

        if msgdate >= lastcheckdate:
            msgTo = msg.get("To")
            msgFrom = msg.get("From")
            msgSubject = msg.get("Subject")

            msgBody = ""
            if msg.is_multipart():
                for msgPart in msg.get_payload():
                    if msgPart.get_content_type() == "text/plain" or (msgPart.get_content_type() == "text/html" and msgBody == ""):
                        msgBody = msgPart.get_payload()
            else:
                msgBody = msg.get_payload()

            alertsXML += "<alert><title>" + encodeXML(msgFrom) + " - "+encodeXML(msgSubject)+"</title>\n"
            alertsXML += "<address></address>\n"
            alertsXML += "<date>" + msgdate.isoformat() + "</date>\n"
            alertsXML += "<summary>From: " + encodeXML(msgFrom) 
            alertsXML += "\nTo: "+encodeXML(msgTo)
            alertsXML += "\nSubject: "+encodeXML(msgSubject)
            alertsXML += "\nDate: " + msgdate.isoformat()
            alertsXML += "\n\n" + stripXML(msgBody) + "</summary></alert>\n"


    mail.quit()

    alertsXML += "</alerts>"    

    return xml.etree.ElementTree.XML(alertsXML)


def encodeXML(data):
    data = re.sub("<","[", data)
    data = re.sub(">","]", data)
    return data

def stripXML(data):
    elementsRemoved = re.sub("<.*?>","", data)
    entitiesRemoved = re.sub("&.*?;", " ", elementsRemoved)
    asciiEncoded = entitiesRemoved.encode('ASCII', 'replace')
    returnData = asciiEncoded.replace('&', 'and')
    return returnData



# Testing

# print getNewAlerts("mail.eclipse.co.uk", "andrew@borley-hall.eclipse.co.uk", "app73sauc3", "2007-01-29T02:15:53")
