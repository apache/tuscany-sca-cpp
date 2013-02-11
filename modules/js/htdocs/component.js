/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.    
 *
 * The JSON-RPC client code is based on Jan-Klaas' JavaScript
 * o lait library (jsolait).
 *
 * $Id: jsonrpc.js,v 1.36.2.3 2006/03/08 15:09:37 mclark Exp $
 *
 * Copyright (c) 2003-2004 Jan-Klaas Kollhof
 * Copyright (c) 2005 Michael Clark, Metaparadigm Pte Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 */

/**
 * Client component wiring API, supporting JSON and ATOM bindings.
 */

var JSONClient = {};

/**
 * Escape a character.
 */
JSONClient.escapeJSONChar = function(c) {
    if(c == "\"" || c == "\\") return "\\" + c;
    if(c == "\b") return "\\b";
    if(c == "\f") return "\\f";
    if(c == "\n") return "\\n";
    if(c == "\r") return "\\r";
    if(c == "\t") return "\\t";
    var hex = c.charCodeAt(0).toString(16);
    if(hex.length == 1) return "\\u000" + hex;
    if(hex.length == 2) return "\\u00" + hex;
    if(hex.length == 3) return "\\u0" + hex;
    return "\\u" + hex;
};

/**
 * Encode a string into JSON format.
 */
JSONClient.escapeJSONString = function(s) {
    // The following should suffice but Safari's regex is broken (doesn't support callback substitutions)
    // return "\"" + s.replace(/([^\u0020-\u007f]|[\\\"])/g, JSONClient.escapeJSONChar) + "\"";

    // Rather inefficient way to do it
    var parts = s.split("");
    for(var i = 0; i < parts.length; i++) {
        var c = parts[i];
        if(c == '"' || c == '\\' || c.charCodeAt(0) < 32 || c.charCodeAt(0) >= 128)
            parts[i] = JSONClient.escapeJSONChar(parts[i]);
    }
    return "\"" + parts.join("") + "\"";
};

/**
 * Marshall objects to JSON format.
 */
JSONClient.toJSON = function(o) {
    if(o == null)
        return "null";
    if(o.constructor == String)
        return JSONClient.escapeJSONString(o);
    if(o.constructor == Number)
        return o.toString();
    if(o.constructor == Boolean)
        return o.toString();
    if(o.constructor == Date)
        return '{javaClass: "java.util.Date", time: ' + o.valueOf() +'}';
    if(o.constructor == Array) {
        var v = [];
        for(var i = 0; i < o.length; i++)
            v.push(JSONClient.toJSON(o[i]));
        return "[" + v.join(", ") + "]";
    }
    var v = [];
    for(attr in o) {
        if(o[attr] == null)
            v.push("\"" + attr + "\": null");
        else if(typeof o[attr] == "function")
            ; // Skip
        else
            v.push(JSONClient.escapeJSONString(attr) + ": " + JSONClient.toJSON(o[attr]));
    }
    return "{" + v.join(", ") + "}";
};

/**
 * Construct an HTTPBindingClient.
 */
function HTTPBindingClient(name, uri, domain) {
    this.name = name;
    this.domain = domain;
    this.uri = uri;
    this.apply = this.createApplyMethod();
}

/**
 * HTTPBindingClient implementation
 */

/**
 * Generate client proxy apply method.
 */
HTTPBindingClient.prototype.createApplyMethod = function() {
    var fn = function() {
        var methodName = arguments[0];
        var args = [];
        for(var i = 1; i < arguments.length; i++)
            args.push(arguments[i]);

        var cb = null;
        if(typeof args[args.length - 1] == "function")
            cb = args.pop();

        var req = HTTPBindingClient.makeJSONRequest(methodName, args, cb);
        return fn.client.jsonApply(req);
    };
    fn.client = this;
    return fn;
};

/**
 * JSON-RPC request counter.
 */
HTTPBindingClient.jsonrpcID = 1;

/**
 * Make a JSON-RPC request.
 */
HTTPBindingClient.makeJSONRequest = function(methodName, args, cb) {
    var req = {};
    req.id = HTTPBindingClient.jsonrpcID++;
    if(cb)
        req.cb = cb;
    var obj = {};
    obj.id = req.id;
    obj.method = methodName;
    obj.params = args;
    req.data = JSONClient.toJSON(obj);
    return req;
};

/**
 * Return the JSON result from an XMLHttpRequest.
 */
HTTPBindingClient.jsonResult = function(http) {
    // Get the charset
    function httpCharset(http) {
        try {
            var contentType = http.getResponseHeader("Content-Type");
            var parts = contentType.split(/\s*;\s*/);
            for(var i = 0; i < parts.length; i++) {
                if(parts[i].substring(0, 8) == "charset=")
                    return parts[i].substring(8, parts[i].length);
            }
        } catch (e) {}
        return "UTF-8";
    }
    if(!HTTPBindingClient.charset)
        HTTPBindingClient.charset = httpCharset(http);

    // Unmarshall the JSON response
    var obj;
    eval("obj = " + http.responseText);
    if(obj.error)
        throw new HTTPBindingClient.Exception(obj.error.code, obj.error.msg);
    var res = obj.result;
    return res;
};

/**
 * Schedule async requests, limiting the number of concurrent running requests.
 */
HTTPBindingClient.queuedRequests = new Array();
HTTPBindingClient.runningRequests = new Array();
HTTPBindingClient.concurrentRequests = 2;

HTTPBindingClient.scheduleAsyncRequest = function(f, cancelable) {
    //debug('schedule async request, running ' + HTTPBindingClient.runningRequests.length + ' queued ' + HTTPBindingClient.queuedRequests.length);

    // Queue the request function
    var req = new Object();
    req.f = f;
    req.cancelable = cancelable;
    req.canceled = false;
    HTTPBindingClient.queuedRequests.push(req);

    // Execute any requests in the queue
    setTimeout(function() {
        HTTPBindingClient.runAsyncRequests(true);
    }, 0);
    return true;
};

HTTPBindingClient.forgetRequest = function(req) {
    req.http = null;

    // Remove a request from the list of running requests
    for (i in HTTPBindingClient.runningRequests) {
        if (HTTPBindingClient.runningRequests[i] == req) {
            HTTPBindingClient.runningRequests.splice(i, 1);
            //debug('forget async request, running ' + HTTPBindingClient.runningRequests.length + ' queued ' + HTTPBindingClient.queuedRequests.length);
            return true;
        }
    }
    return false;
};

HTTPBindingClient.cancelRequests = function() {
    //debug('cancel async requests, running ' + HTTPBindingClient.runningRequests.length + ' queued ' + HTTPBindingClient.queuedRequests.length);

    // Cancel any cancelable in flight HTTP requests
    for (i in HTTPBindingClient.queuedRequests) {
        var req = HTTPBindingClient.queuedRequests[i];
        if (req.cancelable)
            req.canceled = true;
    }
    for (i in HTTPBindingClient.runningRequests) {
        var req = HTTPBindingClient.runningRequests[i];
        if (req.cancelable) {
            req.canceled = true;
            if (req.http) {
                req.http.abort();
                req.http = null;
                //debug('abort async request, running ' + HTTPBindingClient.runningRequests.length + ' queued ' + HTTPBindingClient.queuedRequests.length);
            }
        }
    }

    // Flush the queue
    setTimeout(function() {
        HTTPBindingClient.runAsyncRequests(true);
    }, 0);
}

HTTPBindingClient.runAsyncRequests = function(fromui) {
    // Stop now if we already have enough requests running or there's no request in the queue
    if(HTTPBindingClient.runningRequests.length >= HTTPBindingClient.concurrentRequests || HTTPBindingClient.queuedRequests.length == 0)
        return true;

    // Run the first request in the queue
    var req = HTTPBindingClient.queuedRequests.shift();
    if (!req.canceled) {
        HTTPBindingClient.runningRequests.push(req);
        //debug('run async request, running ' + HTTPBindingClient.runningRequests.length + ' queued ' + HTTPBindingClient.queuedRequests.length);
        var runAsyncRequest = function() {
            if (req.canceled) {
                HTTPBindingClient.forgetRequest(req);
                //debug('canceled timed async request, running ' + HTTPBindingClient.runningRequests.length + ' queued ' + HTTPBindingClient.queuedRequests.length);
                return false;
            }
            try {
                req.http = new XMLHttpRequest();
                return req.f(req.http, function asyncRequestDone() {
                    // Execute any requests left in the queue
                    HTTPBindingClient.forgetRequest(req);
                    //debug('done async request, running ' + HTTPBindingClient.runningRequests.length + ' queued ' + HTTPBindingClient.queuedRequests.length);
                    HTTPBindingClient.runAsyncRequests(false);
                    return true;
                });
            } catch(e) {
                // Execute any requests left in the queue
                HTTPBindingClient.forgetRequest(req);
                //debug('err async request, running ' + HTTPBindingClient.runningRequests.length + ' queued ' + HTTPBindingClient.queuedRequests.length);
                HTTPBindingClient.runAsyncRequests(false);
            }
            return false;
        };
        if (false)
            setTimeout(runAsyncRequest, 0);
        else
            runAsyncRequest();
    } else {
        //debug('canceled queued async request, running ' + HTTPBindingClient.runningRequests.length + ' queued ' + HTTPBindingClient.queuedRequests.length);
    }

    // Execute any requests left in the queue
    HTTPBindingClient.runAsyncRequests(fromui);
};

/**
 * Get a cache item from local storage.
 */
HTTPBindingClient.getCacheItem = function(k) {
    var ls = lstorage || localStorage;
    return ls.getItem('cache.d.' + k);
};

/**
 * Set a cache item in local storage.
 */
HTTPBindingClient.setCacheItem = function(k, v) {
    HTTPBindingClient.collectCacheItems();
    var ls = lstorage || localStorage;
    try {
        var s = ls.getItem('cache.size');
        var size = parseInt(s);
        var ov = ls.getItem('cache.d.' + k);
        var nsize = size - (ov != null? ov.length : 0) + (v != null? v.length : 0);
        if (nsize != size)
            ls.setItem('cache.size', nsize.toString());
    } catch(e) {}
    return ls.setItem('cache.d.' + k, v);
};

/**
* Remove a cache item from local storage.
*/
HTTPBindingClient.removeCacheItem = function(k) {
    var ls = lstorage || localStorage;
    try {
        var s = ls.getItem('cache.size');
        var size = parseInt(s);
        var ov = ls.getItem('cache.d' + k);
        if (ov != null) {
            var nsize = size - ov.length;
            ls.setItem('cache.size', nsize.toString());
        }
    } catch(e) {}
    return ls.removeItem('cache.d.' + k);
};

/**
 * Keep local storage cache entries under 2MB.
 */
HTTPBindingClient.maxCacheSize = /* 20000; */ 2097152;
HTTPBindingClient.collectCacheSize = /* 10000; */ 1048576;
HTTPBindingClient.collectCacheItems = function() {
    var ls = window.lstorage || localStorage;
    var nkeys = window.lstorage? function() { return ls.length(); } : function() { return ls.length; };
    try {
        // Get the current cache size
        var size = 0;
        var s = ls.getItem('cache.size');
        if(s == null) {
            // Calculate and store initial cache size
            //debug('calculating cache size');
            var n = nkeys();
            for(var i = 0; i < n; i++) {
                var k = ls.key(i);
                if(k == null || k.substr(0, 8) != 'cache.d.')
                    continue;
                var v = ls.getItem(k);
                if(v == null)
                    continue;
                size += v.length;
            }
            ls.setItem('cache.size', size.toString());
        } else
            size = parseInt(s);

        // Nothing to do if it's below the max size
        //debug('cache.size', size);
        if (size <= HTTPBindingClient.maxCacheSize)
            return false;

        // Collect random cache entries until we reach our min size
        //debug('collecting cache items');
        var keys = new Array();
        var n = nkeys();
        for(var i = 0; i < n; i++) {
            var k = ls.key(i);
            if(k == null || k.substr(0, 8) != 'cache.d.')
                continue;
            keys.push(k);
        }
        while (keys.length != 0 && size >= HTTPBindingClient.collectCacheSize) {
            var r = Math.floor(keys.length * Math.random());
            if (r == keys.length)
                continue;
            var k = keys[r];
            var v = ls.getItem(k);
            //debug('collect cache item', k);
            ls.removeItem(k);
            keys.splice(r, 1);
            if (v != null)
                size = size - v.length;
        }

        // Store the new cache size
        //debug('updated cache.size', size);
        ls.setItem('cache.size', size.toString());
        return true;
    } catch(e) {}
    return false;
};

/**
 * Apply a function remotely using JSON-RPC.
 */
HTTPBindingClient.prototype.jsonApply = function(req) {
    var hascb = req.cb? true : false;

    // Call asynchronously with a callback
    if(hascb) {
        var u = this.uri;
        return HTTPBindingClient.scheduleAsyncRequest(function jsonApplyRequest(http, done) {
            http.open("POST", u, true);
            http.setRequestHeader("Accept", "*/*");
            http.setRequestHeader("Content-Type", "application/json-rpc");
            http.onreadystatechange = function() {
                if(http.readyState == 4) {
                    // Pass the result or exception
                    if(http.status == 200) {
                        var res = null;
                        try {
                            res = HTTPBindingClient.jsonResult(http);
                            try {
                                req.cb(res);
                            } catch(cbe) {}
                        } catch(e) {
                            try {
                                req.cb(null, e);
                            } catch(cbe) {}
                        }
                    } else {
                        try {
                            req.cb(null, HTTPBindingClient.Exception(http.status, http.statusText));
                        } catch(cbe) {}
                    }
                    return done();
                }
            };

            // Send the request
            http.send(req.data);
            return req.id;
        }, false);
    }

    // Call synchronously and return the result or exception
    var http = new XMLHttpRequest();
    http.open("POST", this.uri, false);
    http.setRequestHeader("Accept", "*/*");
    http.setRequestHeader("Content-Type", "application/json-rpc");
    http.send(req.data);
    if(http.status == 200)
        return HTTPBindingClient.jsonResult(http);
    throw new HTTPBindingClient.Exception(http.status, http.statusText);
};


/**
 * REST GET method.
 */
HTTPBindingClient.prototype.get = function(id, cb, mode) {
    var u = id? (this.uri? this.uri + '/' + id : id) : this.uri;
    var hascb = cb? true : false;

    // Get from local storage first
    var item = null;
    if(mode != 'remote') {
        item = HTTPBindingClient.getCacheItem(u);
        if(item != null && item != '') {
            if(!hascb)
                return item;

            // Pass local result to callback
            setTimeout(function() {
                cb(item);
            }, 0);
        }
    }

    // Call asynchronously with a callback
    if(hascb) {
        return HTTPBindingClient.scheduleAsyncRequest(function getRequest(http, done) {
            http.open("GET", u, true);
            http.setRequestHeader("Accept", "*/*");
            http.onreadystatechange = function() {
                //debug('readystate', http.readyState, 'status', http.status, 'headers', http.getAllResponseHeaders());
                if(http.readyState == 4) {
                    // Pass result if different from local result
                    if(http.status == 200) {
                        var xl = http.getResponseHeader("X-Login");
                        var ct = http.getResponseHeader("Content-Type");
                        if(xl != null && xl != '' && ct != null && ct.indexOf('text/html') == 0) {
                            // Detect redirect to a login page
                            try {
                                var le = new HTTPBindingClient.Exception(403, 'X-Login');
                                if(window.onloginredirect)
                                    window.onloginredirect(le);
                                cb(null, le);
                                return done();
                            } catch(cbe) {}

                        }
                        if(http.responseText == '' || ct == null || ct == '') {
                            // Report empty response
                            try {
                                cb(null, new HTTPBindingClient.Exception(403, 'No-Content'));
                                return done();
                            } catch(cbe) {}

                        } else {
                            if(item == null || http.responseText != item) {
                                // Store retrieved entry in local storage
                                if(http.responseText != null)
                                    HTTPBindingClient.setCacheItem(u, http.responseText);
                                try {
                                    cb(http.responseText);
                                    return done();
                                } catch(cbe) {}
                            }
                        }
                    }
                    else {
                        // Pass exception if we didn't have a local result
                        if(item == null) {
                            try {
                                cb(null, new HTTPBindingClient.Exception(http.status, http.statusText));
                                return done();
                            } catch(cbe) {}
                        }
                    }
                    return done();
                }
            };

            // Send the request
            http.send(null);
            return true;
        }, true);
    }

    // Call synchronously and return the result or exception
    var http = new XMLHttpRequest();
    http.open("GET", u, false);
    http.setRequestHeader("Accept", "*/*");
    http.send(null);
    if(http.status == 200) {
        var xl = http.getResponseHeader("X-Login");
        var ct = http.getResponseHeader("Content-Type");
        if(xl != null && xl != '' && ct != null && ct.indexOf('text/html') == 0) {
            // Detect redirect to a login page
            var le = new HTTPBindingClient.Exception(403, 'X-Login');
            if(window.onloginredirect)
                window.onloginredirect(le);
            throw le;

        }
        var ct = http.getResponseHeader("Content-Type");
        if(http.responseText == '' || ct == null || ct == '') {
            // Report empty response
            throw new HTTPBindingClient.Exception(403, 'No-Content');
        }
        return http.responseText;
    }
    throw new HTTPBindingClient.Exception(http.status, http.statusText);
};

/**
 * REST POST method.
 */
HTTPBindingClient.prototype.post = function (entry, cb) {
    var hascb = cb? true : false;

    // Call asynchronously with a callback
    if(hascb) {
        var u = this.uri;
        return HTTPBindingClient.scheduleAsyncRequest(function postRequest(http, done) {
            http.open("POST", u, true);
            http.setRequestHeader("Accept", "*/*");
            http.setRequestHeader("Content-Type", "application/atom+xml");
            http.onreadystatechange = function() {
                if(http.readyState == 4) {
                    if(http.status == 201) {
                        // Successful result
                        try {
                            cb(http.responseText);
                        } catch(cbe) {}
                    }
                    else {
                        // Report status code as an exception
                        try {
                            cb(null, new HTTPBindingClient.Exception(http.status, http.statusText));
                        } catch(cbe) {}
                    }
                    return done();
                }
            };
            // Send the request
            http.send(entry);
            return true;
        }, false);
    }

    // Call synchronously
    var http = new XMLHttpRequest();
    var hascb = cb? true : false;
    http.open("POST", this.uri, false);
    http.setRequestHeader("Accept", "*/*");
    http.setRequestHeader("Content-Type", "application/atom+xml");
    http.send(entry);
    if(http.status == 201)
        return http.responseText;

    // Return status code as an exception
    throw new HTTPBindingClient.Exception(http.status, http.statusText);
};

/**
 * REST PUT method.
 */
HTTPBindingClient.prototype.put = function(id, entry, cb, mode) {
    var u = id? (this.uri? this.uri + '/' + id : id) : this.uri;
    var hascb = cb? true : false;

    // Update local storage
    var oentry = null;
    if(mode != 'remote') {
        oentry = HTTPBindingClient.getCacheItem(u);
        HTTPBindingClient.setCacheItem(u, entry);
    }

    // Call asynchronously with a callback
    if(hascb) {
        return HTTPBindingClient.scheduleAsyncRequest(function putRequest(http, done) {
            http.open("PUT", u, true);
            http.setRequestHeader("Accept", "*/*");
            http.setRequestHeader("Content-Type", "application/atom+xml");
            http.onreadystatechange = function() {
                if(http.readyState == 4) {
                    if(http.status == 200) {
                        // Successful result
                        try {
                            cb();
                        } catch(cbe) {}
                    } else {
                        if(http.status == 404) {
                            // Undo local storage update
                            if(mode != 'remote') {
                                try {
                                    if(oentry != null)
                                        HTTPBindingClient.setCacheItem(u, oentry);
                                    else
                                        HTTPBindingClient.removeCacheItem(u);
                                } catch(e) {}
                            }
                        }

                        // Report status code as an exception
                        try {
                            cb(new HTTPBindingClient.Exception(http.status, http.statusText));
                        } catch(cbe) {}
                    }
                    return done();
                }
            };

            // Send the request
            http.send(entry);
            return true;
        }, false);
    }

    // Call synchronously
    var http = new XMLHttpRequest();
    http.open("PUT", u, false);
    http.setRequestHeader("Accept", "*/*");
    http.setRequestHeader("Content-Type", "application/atom+xml");
    http.send(entry);
    if(http.status == 200)
        return true;
    if(http.status == 404) {
        // Undo local storage update
        if(mode != 'remote') {
            try {
                if(oentry != null)
                    HTTPBindingClient.setCacheItem(u, oentry);
                else
                    HTTPBindingClient.removeCacheItem(u);
            } catch(e) {}
        }
    }

    // Return status code as an exception
    throw new HTTPBindingClient.Exception(http.status, http.statusText);
};

/**
 * REST DELETE method.
 */
HTTPBindingClient.prototype.del = function(id, cb, mode) {
    var u = id? (this.uri? this.uri + '/' + id : id) : this.uri;
    var hascb = cb? true : false;

    // Update local storage
    var ls = window.lstorage || localStorage;
    if(mode != 'remote')
        HTTPBindingClient.removeCacheItem(u);

    // Call asynchronously with a callback
    if(hascb) {
        return HTTPBindingClient.scheduleAsyncRequest(function delRequest(http, done) {
            http.open("DELETE", u, true);        
            http.setRequestHeader("Accept", "*/*");
            http.onreadystatechange = function() {
                if(http.readyState == 4) {
                    if(http.status == 200) {
                        // Successful result
                        try {
                            cb();
                        } catch(cbe) {}
                    }
                    else {
                        // Report status code as an exception
                        try {
                            cb(new HTTPBindingClient.Exception(http.status, http.statusText));
                        } catch(cbe) {}
                    }
                    return done();
                }
            };

            // Send the request
            http.send(null);
            return true;
        }, false);
    }

    // Call synchronously
    var http = new XMLHttpRequest();
    http.open("DELETE", u, false);        
    http.setRequestHeader("Accept", "*/*");
    http.send(null);
    if(http.status == 200)
        return true;

    // Report status code as an exception
    throw new HTTPBindingClient.Exception(http.status, http.statusText);
};

/**
 * HTTPBindingClient exceptions.
 */
HTTPBindingClient.Exception = function(code, message) {
    this.name = "HTTPBindingClientException";
    this.code = code;
    this.message = message;
};

HTTPBindingClient.Exception.prototype = new Error();

HTTPBindingClient.Exception.prototype.toString = function() {
    return this.name + ": " + this.message;
};

/**
 * Public API.
 */

var sca = {};

/**
 * Return an HTTP client proxy.
 */
sca.httpclient = function(name, uri, domain) {
    return new HTTPBindingClient(name, uri, domain);
};

/**
 * Return a component proxy.
 */
sca.component = function(name, domain) {
    if(!domain)
        return new HTTPBindingClient(name, '/c/' + name, domain);
    return new HTTPBindingClient(name, '/' + domain + '/c/' + name, domain);
};

/**
 * Return a reference proxy.
 */
sca.reference = function(comp, rname) {
    if(!comp.domain)
        return new HTTPBindingClient(comp.name + '/' + rname, '/r/' + comp.name + '/' + rname, comp.domain);
    return new HTTPBindingClient(comp.name + '/' + rname, '/' + comp.domain + '/r/' + comp.name + '/' + rname, comp.domain);
};

/**
 * Add proxy functions to a reference proxy.
 */
sca.defun = function(ref) {
    function defapply(name) {
        return function() {
            var args = new Array();
            args[0] = name;
            for(i = 0, n = arguments.length; i < n; i++)
                args[i + 1] = arguments[i];
            return this.apply.apply(this, args);
        };
    }

    for(f = 1; f < arguments.length; f++) {
        var fn = arguments[f];
        ref[fn]= defapply(fn);
    }
    return ref;
};

