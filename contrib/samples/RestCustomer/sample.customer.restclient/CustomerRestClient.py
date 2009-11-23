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
# This Python code is a simple sample that provides a Python 
# client for the REST Customer sample
 

from xml.etree import ElementTree as et
import sca

# Locate the customer resource service
customerResource = sca.locateservice("CustomerResource")

# Show how to invoke CRUD operations on the customer resource
# The CRUD operations translate to HTTP POST, GET, PUT and DELETE
# according to the REST pattern

customer = customerResource.retrieve("2345")
print "Rest - Retrieved customer " + et.tostring(customer)

customer = et.fromstring("""<customer xmlns="http://sample.customer"><id>1234</id><firstName>Jane</firstName><lastName>Doe</lastName></customer>""")
url = customerResource.create(customer)
print "Rest - Created customer " + url

customer = customerResource.retrieve("1234")
print "Rest - Retrieved customer " + et.tostring(customer)

customer = customerResource.retrieve(url)
print "Rest - Retrieved by url " + et.tostring(customer)

customer.find("{http://sample.customer}lastName").text="Smith"
customerResource.update("1234", customer)
print "Rest - Updated customer 1234"

customer = customerResource.retrieve("1234")
print "Rest - Retrieved customer " + et.tostring(customer)

customerResource.delete("1234")
print "Rest - Deleted customer 1234"

# Also show how to use REST binding to invoke remote commands
# using HTTP GET and XML over HTTP POST, the REST binding
# uses that command pattern when you don't declare a REST interface
# on your SCA reference

# Locate the customer command service
customerCommand = sca.locateservice("CustomerCommand")

# Invoke operations on the customer command service
customer = customerCommand.retrieve("2345")
print "Command  - Retrieved customer " + et.tostring(customer)

customer = et.fromstring("""<customer xmlns="http://sample.customer"><id>1234</id><firstName>Jane</firstName><lastName>Doe</lastName></customer>""")
url = customerCommand.create(customer)
print "Command  - Created customer " + url

customer = customerCommand.retrieve("1234")
print "Command  - Retrieved customer " + et.tostring(customer)

customer.find("{http://sample.customer}lastName").text="Smith"
customerCommand.update("1234", customer)
print "Command  - Updated customer 1234"

customer = customerCommand.retrieve("1234")
print "Command  - Retrieved customer " + et.tostring(customer)

customerCommand.delete("1234")
print "Command  - Deleted customer 1234"
