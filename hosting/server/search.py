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

# Search implementation
from util import *
from atomutil import *
from sys import debug

# Merge ratings into a list of apps
def mergeratings(entries, ratings):
    debug('search.py::mergeratings::entries', entries)

    def mergerating(app):
        debug('search.py::mergerating::app', app)
        id = (entryid(app),)
        info = content(app)
        rating = ratings.get(id)
        rates = content(rating)
        mergedentry = mkentry(title(app), car(id), author(app), updated(app), ("'info",) + (() if isNil(info) else cdr(info)) + (() if isNil(rates) else cdr(rates)))
        return mergedentry

    mergedentries = tuple(filter(lambda e: not isNil(e), map(lambda e: car(mergerating((e,))), entries)))
    debug('search.py::mergeratings::mergedentries', mergedentries)
    return mergedentries

# Search apps
def get(id, user, cache, db, apps, ratings):
    debug('search.py::get::id', id)
    q = assoc("'q", id)
    if isNil(q):
        return None

    # Run the search
    foundentries = db.get((("'regex", '("apps" .* "app.info")'), ("'textsearch", cadr(q)), ("'limit", 25)))
    debug('search.py::get::foundentries', foundentries)

    # Merge app ratings
    appentries = mergeratings(tuple(map(lambda v: car(v), () if isNil(foundentries) else foundentries)), ratings)

    results = ((("'feed", ("'title", "Search Results"), ("'id", 'search')) + appentries),)
    debug('search.py::get::results', results)
    return results

