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

# Convert a particular user id to an account id
def accountid(user):
    return ("'" + user.id(), "'user.account")

# Get the user's account
def get(id, user, cache):
    account = cache.get(accountid(user))
    if isNil(account):
        return ()
    return account

# Update the user's account
def put(id, account, user, cache):
    return cache.put(accountid(user), account)

