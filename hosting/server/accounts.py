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

# Accounts collection implementation
from util import *
from atomutil import *
from sys import debug

# Convert a particular user id to an account id
def accountid(user):
    return ('accounts', user.get(()), 'user.account')

# Get the current user's account
def get(id, user, cache):
    debug('accounts.py::get::id', id)
    account = cache.get(accountid(user))
    if isNil(account):
        return mkentry(user.get(()), user.get(()), user.get(()), now(), ())
    return account

# Update the user's account
def put(id, account, user, cache):
    debug('accounts.py::put::id', id)
    debug('accounts.py::put::account', account)

    accountentry = mkentry(title(account), user.get(()), user.get(()), now(), content(account))
    return cache.put(accountid(user), accountentry)

