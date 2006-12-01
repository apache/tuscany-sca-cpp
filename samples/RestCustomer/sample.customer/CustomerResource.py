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
# This Python code implements a simple SCA component that
# manages Customer resources.
# 

import sys
import os
import shutil
from xml.etree import ElementTree as et

qname = "{http://sample.customer}id"
dir = os.environ["CUSTOMER_DIR"]

class CustomerResourceImpl:
    "A class that manages Customer resources"
    
    def create(self, customer):
        id = customer.findtext(qname)
        print "Python - CustomerResourceImpl.create " + id
        f = open(dir + "/" + id + ".xml", "w")
        f.write(et.tostring(customer))
        f.close()
		return id

    def retrieve(self, id):
        print "Python - CustomerResourceImpl.retrieve " + id
        f = open(dir + "/" + id + ".xml", "r")
        customer = et.fromstring(f.read())
        f.close()
        return customer

    def update(self, customer):
        id = customer.findtext(qname)
        print "Python - CustomerResourceImpl.update " + id
        f = open(dir + "/" + id + ".xml", "w")
        f.write(et.tostring(customer))
        f.close()

    def delete(self, id):
        print "Python - CustomerResourceImpl.delete " + id
        shutil.copyfile(dir + "/" + id + ".xml", dir + "/" + id + "-bak.xml")
        os.remove(dir + "/" + id + ".xml")
