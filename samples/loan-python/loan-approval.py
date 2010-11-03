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

# Loan approval service implementation
import uuid
import sys
from util import *
from loan import *

loansId = ("loans", "1234")

# Return the list of loans
def loans(cache):
    l = cache("get", loansId)
    if l is None:
        return ()
    return l

# Post a new loan request
def post(collection, item, cache):
    id = str(uuid.uuid1())
    loans = cons(cons(id, cdr(item)), loans(cache))
    cache("put", loansId, loans)
    return (id,)

# Return the person currently processing a loan request
def processor(l):
    if approver(l) is not None:
        return approver(l)
    return assessor(l)

# Return a list of loans that match a given criteria
def get(r, cache):
    # All the loans
    if r == ():
        return loans(cache)
    # Loans that need approval
    if car(r) == "needApproval":
        return filter(lambda l: (amount(l) >= 10000 or risk(l) == "high") and approval(l) is None, loans(cache))
    # Loans that need a risk assessment
    if car(r) == "needAssessment":
        return filter(lambda : amount(l) < 10000 and risk(l) is None, loans(cache))
    # Loans currently under approval
    if car(r) == "underApproval":
        return filter(lambda l: approver(l) is not None, loans(cache))
    # Loans currently under assessment
    if car(r) == "underAssessment":
        return filter(lambda l: assessor(l) is not None, loans(cache))
    # Loan requests that are unassigned
    if car(r) == "unassigned":
        return filter(lambda l: processor(l) is None, loans(cache))
    # Loan requests that are assigned and getting processed
    if car(r) == "assigned":
        return filter(lambda l: processor(l) == cadr(r), loans(cache))
    # Approved loans
    if car(r) == "approved":
        return filter(lambda l: approval(l) == true, loans(cache))
    # Denied loans
    if car(r) == "denied":
        return filter(lambda l: approval(l) == false, loans(cache))
    # A particular loan
    return filter(lambda l: id(l) == r, loans(cache))

