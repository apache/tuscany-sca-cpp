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
def accountid(uid):
    return ('accounts', uid, 'user.account')

# Get a user account
def get(id, user, cache):
    debug('accounts.py::get::id', id)

    # Default to the current user's account
    uid = user.get(()) if isNull(id) else car(id)

    # Get the account
    account = cache.get(accountid(uid))
    if isNull(account):
        return mkentry(uid, uid, uid, now(), ())

    # Strip private info from account
    if uid != user.get(()) and user.get(()) != 'admin':
        desc = assoc("'description", content(account))
        return mkentry(title(account), uid, uid, now(), ("'account",) + (() if isNull(desc) else (desc,)))

    return account

# Update a user account
def put(id, account, user, cache):
    debug('accounts.py::put::id', id)
    debug('accounts.py::put::account', account)

    # Default to the current user's account
    uid = user.get(()) if isNull(id) else car(id)

    # Only the admin can create or update somebody else's account
    if uid != user.get(()) and user.get(()) != 'admin':
        debug('accounts.py::put', 'not owner or admin', user.get(()))
        return False

    # Get existing account
    eaccount = cache.get(accountid(uid))
    if isNull(eaccount) and user.get(()) != 'admin':
        # Only the admin can create a new account
        debug('accounts.py::put', 'account not found', (uid,))
        return False

    # Merge new account info
    email = assoc("'email", content(account)) if isNull(eaccount) else assoc("'email", content(eaccount))
    desc = assoc("'description", content(account))
    accountentry = mkentry(title(account), uid, uid, now(), ("'account",) + (() if isNull(email) else (email,)) + (() if isNull(desc) else (desc,)))

    return cache.put(accountid(uid), accountentry)

def delete(id, user, cache):
    debug('accounts.py::delete::id', id)
    uid = car(id)

    # Only the admin can delete an account
    if user.get(()) != 'admin':
        debug('accounts.py::delete', 'not admin', user.get(()))
        return False

    # Get the requested id
    account = cache.get(accountid(uid))
    if isNull(account):
        debug('accounts.py::delete', 'account not found', (uid,))
        return False

    # Delete the account
    return cache.delete(accountid(uid))

