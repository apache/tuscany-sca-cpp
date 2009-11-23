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

require 'net/pop'
require "rexml/document"
include REXML

# class POPChecker
#
#     def initialize()
#         print "POPCheckerImpl.initialize\n"
#     end

def getNewAlerts(popserver, username, password, lastchecked)

    print "POPCheckerImpl getting new POP e-mail alerts\n"

    alertsXML = "<alerts xmlns=\"http://tuscany.apache.org/samples/alerter\">\n"

    # initially set lastchecked to the epoch before trying to use the lastchecked string
    lastcheckedtime = Time.at(0)
    if lastchecked != ''
        # lastchecked (if provided) is of form 2007-02-17T23:34:56
        year , month, day, hour, min, sec = lastchecked.split(/[-T:]/)
        lastcheckedtime = Time.gm(year , month, day, hour, min, sec, nil)
    end

    numberOfEmails = 0

    Net::POP3.start(popserver, 110, username, password) do |pop|

        if !pop.mails.empty?
            pop.each_mail do |m|               
                msg = m.pop

                header, body = msg.split("\r\n\r\n", 2)                
                date = getMessageField("Date", header)
                # date is of form "Sun, 21 Jan 2007 13:51:53 -0500"
                parts = date.split(' ')
                timeparts = parts[4].split(':') 
                timezoneSecs = (parts[5].to_i()/100)*60*60 + (parts[5].slice(-2,0).to_i())*60

                msgtime = Time.gm(timeparts[2], timeparts[1], timeparts[0], parts[1], parts[2], parts[3], nil, nil, nil, nil) + timezoneSecs                 


                if msgtime >= lastcheckedtime
                    from = getMessageField("From", header)
                    subject = getMessageField("Subject", header)

                    alertsXML += "<alert><title>From: " + encodeXML(from) + "\nSubject: "+encodeXML(subject)+"</title>\n"
                    alertsXML += "<address></address>\n"
                    alertsXML += "<date>" + msgtime.strftime("%Y-%m-%dT%H:%M:%S") + "</date>\n"
                    alertsXML += "<summary>" + stripXML(body) + "</summary></alert>\n"

                    numberOfEmails += 1
                end

            end
        end
    end
    alertsXML += "</alerts>"

    # Print XML
    # puts alertsXML
    print "POPCheckerImpl retrieved "+numberOfEmails.to_s+" new POP e-mail alerts\n"

    # Return data as REXML document
    result = Document.new(alertsXML)

    return result
end


def getMessageField(field, message)

    # Use a regex to get the field
    pattern = '^'+field+': (.*?)$'

    re = Regexp.new(pattern)
    re =~ message
    
    if $1 == nil        
        return ''
    end

    return $1
end

def encodeXML(data)
    data = data.gsub(/</,'&lt;')
    data = data.gsub(/>/,'&gt;')
    return data
end

def stripXML(data)
    data = data.gsub(/<.*?>/m,'')
    data = data.gsub(/&.*?;/, ' ')
    data = data.gsub(/&/, 'and')
    data = encodeXML(data)
    return data
end



# # Testing
# pop = POPChecker.new()
# print pop.getNewAlerts("mail.eclipse.co.uk", "andrew@borley-hall.eclipse.co.uk", "app73sauc3", "2007-01-29T02:15:53")
