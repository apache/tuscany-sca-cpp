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

# Locate the customer service
customerService = sca.locateservice("Customer")

# Invoke the CRUD operations on the customer resource

customer = customerService.retrieve("2345")
print "Retrieved customer " + et.tostring(customer)

customer = et.fromstring("""<customer xmlns="http://sample.customer"><id>1234</id><firstName>Jane</firstName><lastName>Doe</lastName></customer>""")
url = customerService.create(customer)
print "Created customer " + url

customer = customerService.retrieve("1234")
print "Retrieved customer " + et.tostring(customer)

customer = customerService.retrieve(url)
print "Retrieved by url " + et.tostring(customer)

customer.find("{http://sample.customer}lastName").text="Smith"
customerService.update("1234", customer)
print "Updated customer 1234"

customer = customerService.retrieve("1234")
print "Retrieved customer " + et.tostring(customer)

customerService.delete("1234")
print "Deleted customer 1234"
