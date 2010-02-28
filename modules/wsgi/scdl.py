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

# SCDL parsing functions

from util import *

def mkcomponent(name, impl, svcs, refs):
    return (name, impl, svcs, refs)

def implementation(e):
    if len(e) == 0 or match(car(e), "end", "component") == True:
        return ""
    if match(car(e), "start", "implementation.python") == False:
        return implementation(cdr(e))
    if "script" in att(car(e)):
        s = att(car(e))["script"]
        return s[0:len(s) - 3]
    return None

def binding(e):
    if len(e) == 0 or match(car(e), "end", "reference") == True or match(car(e), "end", "service") == True:
        return ()
    if match(car(e), "start", "binding.") == False:
        return binding(cdr(e))
    return tokens(att(car(e))["uri"])

def references(e):
    if len(e) == 0 or match(car(e), "end", "component") == True:
        return ()
    if match(car(e), "start", "reference") == False:
        return references(cdr(e))
    if "target" in att(car(e)):
        return (att(car(e))["target"],) + references(cdr(e))
    return cons(binding(e), references(cdr(e)))

def services(e):
    if len(e) == 0 or match(car(e), "end", "component") == True:
        return ()
    if match(car(e), "start", "service") == False:
        return services(cdr(e))
    return cons(binding(e), services(cdr(e)))

def name(e):
    return att(car(e))["name"]    

def components(e):
    if len(e) == 0:
        return ()
    if match(car(e), "start", "component") == False:
        return components(cdr(e))
    n = name(e)
    return cons(mkcomponent(n, implementation(e), cons(("components", n), services(e)), references(e)), components(cdr(e)))

def nameToComponent(n, c):
    if c == ():
        return None
    if car(car(c)) == n:
        return car(c)
    return nameToComponent(n, cdr(c))

def matchingURI(u, s):
    if s == ():
        return None
    if car(s) == u[0:len(car(s))]:
        return car(s)
    return matchingURI(u, cdr(s))

def uriToComponent(u, c):
    if c == ():
        return (None, None)
    m = matchingURI(u, caddr(car(c)))
    if m != None:
        return (m, car(c))
    return uriToComponent(u, cdr(c))
