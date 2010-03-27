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

from xml.etree.cElementTree import iterparse
from util import *
from httputil import *

# Element tree utility functions, used to parse SCDL documents
def parse(file):
    return map(lambda x: x, iterparse(file, events=("start", "end")))

def evt(e):
    return car(e)

def elt(e):
    return cadr(e)

def att(e):
    return elt(e).attrib

def text(e):
    return elt(e).text

def match(e, ev, tag):
    return evt(e) == ev and elt(e).tag.find("}" + tag) != -1

# Make a callable component
class component:
    def __init__(self, name, impl, svcs, refs, props):
        self.name = name
        self.impl = impl
        self.mod = None
        self.svcs = svcs
        self.refs = refs
        self.props = props
        self.proxies = ()

    def __call__(self, func, *args):
        return self.mod.__getattribute__(func)(*(args + self.proxies))

    def __repr__(self):
        return repr((self.name, self.impl, self.mod, self.svcs, self.refs, self.props, self.proxies))

def mkcomponent(name, impl, svcs, refs, props):
    return component(name, impl, svcs, refs, props)

# Return the Python module name of a component implementation
def implementation(e):
    if len(e) == 0 or match(car(e), "end", "component") == True:
        return ""
    if match(car(e), "start", "implementation.python") == False:
        return implementation(cdr(e))
    if "script" in att(car(e)):
        s = att(car(e))["script"]
        return s[0:len(s) - 3]
    return None

# Return the URI of a binding under a SCDL service or reference element
def binding(e):
    if len(e) == 0 or match(car(e), "end", "reference") == True or match(car(e), "end", "service") == True:
        return ()
    if match(car(e), "start", "binding.") == False:
        return binding(cdr(e))
    return att(car(e))["uri"]

# Return the list of references under a SCDL component element
def references(e):
    if len(e) == 0 or match(car(e), "end", "component") == True:
        return ()
    if match(car(e), "start", "reference") == False:
        return references(cdr(e))
    if "target" in att(car(e)):
        return cons(car(tokens(att(car(e))["target"])), references(cdr(e)))
    return cons(binding(e), references(cdr(e)))

# Return the list of properties under a SCDL component element
def properties(e):
    if len(e) == 0 or match(car(e), "end", "component") == True:
        return ()
    if match(car(e), "start", "property") == False:
        return properties(cdr(e))
    return cons(text(car(e)), properties(cdr(e)))

# Return the list of services under a SCDL component element
def services(e):
    if len(e) == 0 or match(car(e), "end", "component") == True:
        return ()
    if match(car(e), "start", "service") == False:
        return services(cdr(e))
    return cons(tokens(binding(e)), services(cdr(e)))

# Return the name attribute of a SCDL element
def name(e):
    return att(car(e))["name"]    

# Return the list of components under a SCDL composite element
def components(e):
    if len(e) == 0:
        return ()
    if match(car(e), "start", "component") == False:
        return components(cdr(e))
    n = name(e)
    return cons(mkcomponent(n, implementation(e), cons(("components", n), services(e)), references(e), properties(e)), components(cdr(e)))

# Find a component with a given name
def nameToComponent(name, comps):
    if comps == ():
        return None
    if car(comps).name == name:
        return car(comps)
    return nameToComponent(name, cdr(comps))

# Find the URI matching a given URI in a list of service URIs
def matchingURI(u, svcs):
    if svcs == ():
        return None
    if car(svcs) == u[0:len(car(svcs))]:
        return car(svcs)
    return matchingURI(u, cdr(svcs))

# Return the (service URI, component) pair matching a given URI
def uriToComponent(u, comps):
    if comps == ():
        return (None, None)
    m = matchingURI(u, car(comps).svcs)
    if m != None:
        return (m, car(comps))
    return uriToComponent(u, cdr(comps))

# Evaluate a reference, return a proxy to the resolved component or an
# HTTP client configured with the reference target uri
def evalReference(r, comps):
    if r.startswith("http://") or r.startswith("https://"):
        return mkclient(r)
    return nameToComponent(r, comps)

# Evaluate a component, resolve its implementation and references
def evalComponent(comp, comps):
    comp.mod = __import__(comp.impl)

    # Make a list of proxy lambda functions for the component references and properties
    # A reference proxy is the callable lambda function of the component wired to the reference
    # A property proxy is a lambda function that returns the value of the property
    comp.proxies = tuple(map(lambda r: evalReference(r, comps), comp.refs)) + tuple(map(lambda v: lambda: v, comp.props))

    return comp

# Evaluate a list of components
def evalComponents(comps):
    return tuple(map(lambda c: evalComponent(c, comps), comps))

