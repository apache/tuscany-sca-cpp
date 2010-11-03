#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#    
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.

from util import *

# Loan data type

def firstName(loan):
    return assoc("'firstName", loan)

def lastName(loan):
    return assoc("'lastName", loan)

def ssn(loan):
    return assoc("'ssn", loan)

def amount(loan):
    return assoc("'amount", loan)

def approver(loan):
    return assoc("'approver", loan)

def approval(loan):
    return assoc("'approval", loan)

def assessor(loan):
    return assoc("'assessor", loan)

def risk(loan):
    return assoc("'risk", loan)

