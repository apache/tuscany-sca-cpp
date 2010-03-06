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

# Simple utility functions

# Scheme-like lists
def cons(a, b):
    return (a,) + b

def car(l):
    return l[0]
    
def cdr(l):
    return l[1:]

def cadr(l):
    return car(cdr(l))

def cddr(l):
    return cdr(cdr(l))

def caddr(l):
    return car(cddr(l))

def reverse(l):
    r = list(l)
    r.reverse()
    return tuple(r)

def isNil(l):
    return l == ()

def isSymbol(v):
    return isinstance(v, basestring) and v[0:1] == "'"

def isList(v):
    if getattr(v, '__iter__', False) == False:
        return False
    if isinstance(v, basestring) or isinstance(v, dict):
        return False
    return True

def isTaggedList(v, t):
    return isList(v) and not isNil(v) and car(v) == t

# Scheme-like associations
def assoc(k, l):
    if l == ():
        return None

    if k == car(car(l)):
        return car(l)
    return assoc(k, cdr(l))

# Currying / partial function application
def curry(f, *args):
    return lambda *a: f(*(args + a))

# Split a path into a list of segments
def tokens(path):
    return tuple(filter(lambda s: len(s) != 0, path.split("/")))

# Write a list of strings to a stream
def writeStrings(l, os):
    if l == ():
        return os
    os.write(car(l))
    return writeStrings(cdr(l), os)

