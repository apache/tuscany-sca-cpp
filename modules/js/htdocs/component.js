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
 */

/**
 * Client component wiring API, supporting JSON and ATOM bindings.
 */

var JSONClient = {};

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
 * Run a function asynchronously.
 */
HTTPBindingClient.delaying = false;
HTTPBindingClient.delay = function(f) {
    if (HTTPBindingClient.delaying)
        return window.setTimeout(f, 0);
    else
        return f();
};

/**
 * Generate client proxy apply method.
 */
HTTPBindingClient.prototype.createApplyMethod = function() {
    var fn = function() {
        var methodName = arguments[0];
        var args = [];
        for(var i = 1; i < arguments.length; i++)
            args[args.length] = arguments[i];

        var cb;
        if(typeof args[args.length - 1] == 'function')
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
    req.data = JSON.stringify(obj);
    return req;
};

/**
 * Return the JSON result from an XMLHttpRequest.
 */
HTTPBindingClient.jsonResult = function(http) {
    var obj = JSON.parse(http.responseText);
    return obj.result;
};

/**
 * Schedule async requests, limiting the number of concurrent running requests.
 */
HTTPBindingClient.queuedRequests = [];
HTTPBindingClient.runningRequests = [];
HTTPBindingClient.concurrentRequests = 4;

HTTPBindingClient.scheduleAsyncRequest = function(f, cancelable) {
    debug('component schedule async request', 'running', HTTPBindingClient.runningRequests.length, 'queued', HTTPBindingClient.queuedRequests.length);

    // Queue the request function
    var req = new Object();
    req.f = f;
    req.cancelable = cancelable;
    req.canceled = false;
    HTTPBindingClient.queuedRequests[HTTPBindingClient.queuedRequests.length] = req;

    // Execute any requests in the queue
    return HTTPBindingClient.runAsyncRequests();
};

HTTPBindingClient.forgetRequest = function(req) {
    req.http = undefined;

    // Remove a request from the list of running requests
    for (var i in HTTPBindingClient.runningRequests) {
        if (HTTPBindingClient.runningRequests[i] == req) {
            HTTPBindingClient.runningRequests.splice(i, 1);
            debug('forget async request', 'running', HTTPBindingClient.runningRequests.length, 'queued', HTTPBindingClient.queuedRequests.length);
            return true;
        }
    }
    return false;
};

HTTPBindingClient.cancelRequests = function() {
    debug('component cancel async requests', 'running', HTTPBindingClient.runningRequests.length, 'queued', HTTPBindingClient.queuedRequests.length);

    // Cancel any cancelable in flight HTTP requests
    for (var i in HTTPBindingClient.queuedRequests) {
        var req = HTTPBindingClient.queuedRequests[i];
        if (req.cancelable)
            req.canceled = true;
    }
    for (var i in HTTPBindingClient.runningRequests) {
        var req = HTTPBindingClient.runningRequests[i];
        if (req.cancelable) {
            req.canceled = true;
            if (req.http) {
                req.http.aborted = true;
                req.http.abort();
                req.http = undefined;
                debug('component abort async request', 'running', HTTPBindingClient.runningRequests.length, 'queued', HTTPBindingClient.queuedRequests.length);
            }
        }
    }

    // Flush the queue
    return HTTPBindingClient.runAsyncRequests();
}

HTTPBindingClient.runAsyncRequests = function() {
    // Stop now if we already have enough requests running or there's no request in the queue
    if(HTTPBindingClient.runningRequests.length >= HTTPBindingClient.concurrentRequests || HTTPBindingClient.queuedRequests.length == 0)
        return true;

    // Run the first request in the queue
    var req = HTTPBindingClient.queuedRequests.shift();
    if (!req.canceled) {
        HTTPBindingClient.runningRequests[HTTPBindingClient.runningRequests.length] = req;
        debug('component run async request', 'running', HTTPBindingClient.runningRequests.length, 'queued', HTTPBindingClient.queuedRequests.length);
        if (req.canceled) {
            HTTPBindingClient.forgetRequest(req);
            debug('component canceled timed async request', 'running', HTTPBindingClient.runningRequests.length, 'queued', HTTPBindingClient.queuedRequests.length);
            return false;
        }
        HTTPBindingClient.delay(function asyncRequest() {
            try {
                req.http = new XMLHttpRequest();
                req.http.aborted = false;
                return req.f(req.http, function asyncRequestDone() {
                    // Execute any requests left in the queue
                    HTTPBindingClient.forgetRequest(req);
                    debug('component done async request', 'running', HTTPBindingClient.runningRequests.length, 'queued', HTTPBindingClient.queuedRequests.length);
                    HTTPBindingClient.runAsyncRequests();
                    return true;
                });
            } catch(e) {
                // Execute any requests left in the queue
                HTTPBindingClient.forgetRequest(req);
                debug('component async request error', 'running', HTTPBindingClient.runningRequests.length, 'queued', HTTPBindingClient.queuedRequests.length, 'error', e);
                HTTPBindingClient.runAsyncRequests();
            }
            return false;
        });
    } else {
        debug('component canceled queued async request', 'running', HTTPBindingClient.runningRequests.length, 'queued', HTTPBindingClient.queuedRequests.length);
    }

    // Execute any requests left in the queue
    HTTPBindingClient.runAsyncRequests();
};

/**
 * Get a cache item from local storage.
 */
HTTPBindingClient.getCacheItem = function(k) {
    var ls = lstorage || localStorage;
    return ls.getItem('dc.d.' + k);
};

/**
 * Set a cache item in local storage.
 */
HTTPBindingClient.setCacheItem = function(k, v) {
    if (v && v.length > 65535)
        return HTTPBindingClient.removeCacheItem(k);
    HTTPBindingClient.collectCacheItems();
    var ls = lstorage || localStorage;
    var s = ls.getItem('dc.size');
    var size = s? parseInt(s) : 0;
    var ov = ls.getItem('dc.d.' + k);
    var nsize = size - (ov? ov.length : 0) + (v? v.length : 0);
    if (nsize != size)
        ls.setItem('dc.size', nsize.toString());
    return ls.setItem('dc.d.' + k, v);
};

/**
* Remove a cache item from local storage.
*/
HTTPBindingClient.removeCacheItem = function(k) {
    var ls = lstorage || localStorage;
    var s = ls.getItem('dc.size');
    var size = s? parseInt(s) : 0;
    var ov = ls.getItem('dc.d.' + k);
    if (ov) {
        var nsize = size - ov.length;
        ls.setItem('dc.size', nsize.toString());
    }
    return ls.removeItem('dc.d.' + k);
};

/**
 * Keep local storage cache entries under 2MB.
 */
HTTPBindingClient.maxCacheSize = /* 20000; */ 2097152;
HTTPBindingClient.collectCacheSize = /* 10000; */ 1048576;
HTTPBindingClient.collectCacheItems = function() {
    var ls = window.lstorage || localStorage;
    var nkeys = window.lstorage? function() { return ls.length(); } : function() { return ls.length; };

    // Get the current cache size
    var size = 0;
    var s = ls.getItem('dc.size');
    if(!s) {
        // Calculate and store initial cache size
        debug('component calculating cache size');
        var n = nkeys();
        for(var i = 0; i < n; i++) {
            var k = ls.key(i);
            if(!k || k.substr(0, 5) != 'dc.d.')
                continue;
            var v = ls.getItem(k);
            if(!v)
                continue;
            size += v.length;
        }
        ls.setItem('dc.size', size.toString());
    } else
        size = parseInt(s);

    // Nothing to do if it's below the max size
    debug('component cache size', size);
    if (size <= HTTPBindingClient.maxCacheSize)
        return false;

    // Collect random cache entries until we reach our min size
    debug('component collecting cache items');
    var keys = [];
    var n = nkeys();
    for(var i = 0; i < n; i++) {
        var k = ls.key(i);
        if(!k || k.substr(0, 5) != 'dc.d.')
            continue;
        keys[keys.length] = k;
    }
    while (keys.length != 0 && size >= HTTPBindingClient.collectCacheSize) {
        var r = Math.floor(keys.length * Math.random());
        if (r == keys.length)
            continue;
        var k = keys[r];
        var v = ls.getItem(k);
        debug('component collect cache item', k);
        ls.removeItem(k);
        keys.splice(r, 1);
        if (v)
            size = size - v.length;
    }

    // Store the new cache size
    debug('component updated cache size', size);
    ls.setItem('dc.size', size.toString());
    return true;
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
            http.open('POST', u, true);
            http.setRequestHeader('Accept', '*/*');
            http.setRequestHeader('Content-Type', 'application/json-rpc');
            http.setRequestHeader('X-Cache-Control', 'no-cache');
            http.onreadystatechange = function() {
                if(http.readyState == 4) {
                    // Pass the result or exception
                    if(http.status == 200) {
                        var res = HTTPBindingClient.jsonResult(http);
                        req.cb(res);
                    } if(!http.aborted) {
                        error('jsonApply error', 'status', http.status, http.statusText);
                        req.cb(undefined, new Error('' + http.status + ' ' + http.statusText));
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
    http.open('POST', this.uri, false);
    http.setRequestHeader('Accept', '*/*');
    http.setRequestHeader('Content-Type', 'application/json-rpc');
    http.setRequestHeader('X-Cache-Control', 'no-cache');
    http.send(req.data);
    if(http.status == 200)
        return HTTPBindingClient.jsonResult(http);
    error('jsonApply error', 'status', http.status, http.statusText);
    throw new Error('' + http.status + ' ' + http.statusText);
};


/**
 * REST GET method.
 */
HTTPBindingClient.prototype.get = function(id, cb, mode) {
    var u = id? (this.uri? this.uri + '/' + id : id) : this.uri;
    var hascb = cb? true : false;

    // Get from local storage first
    var item;
    if(mode != 'remote') {
        item = HTTPBindingClient.getCacheItem(u);
        if(item && item != '') {
            if(!hascb)
                return item;

            // Pass local result to callback
            cb(item);
        }
    }

    // Call asynchronously with a callback
    if(hascb) {
        return HTTPBindingClient.scheduleAsyncRequest(function getRequest(http, done) {
            http.open('GET', u, true);
            http.setRequestHeader('Accept', '*/*');
            http.setRequestHeader('X-Cache-Control', 'no-cache');
            http.onreadystatechange = function() {
                if(http.readyState == 4) {

                    // Pass result if different from local result
                    //debug('readystate', http.readyState, 'status', http.status, 'headers', http.getAllResponseHeaders());
                    if(http.status == 200) {
                        var ct = http.getResponseHeader('Content-Type');
                        if(http.responseText == '' || !ct || ct == '') {

                            // Report empty response
                            error('get received empty response', 'url', u);
                            cb(undefined, new Error('500 No-Content'));
                            return done();

                        } else if(!item || http.responseText != item) {

                            // Store retrieved entry in local storage
                            //debug('received response', 'url', u, 'response', http.responseText);
                            if(http.responseText != null)
                                HTTPBindingClient.setCacheItem(u, http.responseText);
                            cb(http.responseText);
                            return done();
                        }
                    } else if (http.status == 403) {

                        // Redirect to login page
                        error('get received 403 response', 'url', u);
                        var le = new Error('' + http.status + ' ' + http.statusText);
                        if(window.onloginredirect)
                            window.onloginredirect(le);
                        cb(undefined, le);
                        return done();

                    } else if(!http.aborted) {

                        // Pass exception if we didn't have a local result
                        error('get received error', 'url', u, 'status', http.status, http.statusText);
                        if(!item) {
                            cb(undefined, new Error('' + http.status + ' ' + http.statusText));
                            return done();
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
    http.open('GET', u, false);
    http.setRequestHeader('Accept', '*/*');
    http.setRequestHeader('X-Cache-Control', 'no-cache');
    http.send(null);
    if(http.status == 200) {
        var ct = http.getResponseHeader('Content-Type');
        if(http.responseText == '' || !ct || ct == '') {
            // Report empty response
            error('get received empty response', 'url', u);
            throw new Error('500 No Content');
        }
        return http.responseText;
    }
    if(http.status == 403) {
        // Redirect to login page
        error('get received 403 response', 'url', u);
        var le = new Error('' + http.status + ' ' + http.statusText);
        if(window.onloginredirect)
            window.onloginredirect(le);
        throw le;
    }
    error('get received error', 'url', u, 'status', http.status, http.statusText);
    throw new Error('' + http.status + ' ' + http.statusText);
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
            http.open('POST', u, true);
            http.setRequestHeader('Accept', '*/*');
            http.setRequestHeader('Content-Type', 'application/atom+xml');
            http.setRequestHeader('X-Cache-Control', 'no-cache');
            http.onreadystatechange = function() {
                if(http.readyState == 4) {
                    if(http.status == 201) {

                        // Successful result
                        cb(http.responseText);
                    }
                    else {
                        // Report status code as an exception
                        cb(undefined, new Error('' + http.status + ' ' + http.statusText));
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
    http.open('POST', this.uri, false);
    http.setRequestHeader('Accept', '*/*');
    http.setRequestHeader('Content-Type', 'application/atom+xml');
    http.setRequestHeader('X-Cache-Control', 'no-cache');
    http.send(entry);
    if(http.status == 201)
        return http.responseText;

    // Return status code as an exception
    throw new Error('' + http.status + ' ' + http.statusText);
};

/**
 * REST PUT method.
 */
HTTPBindingClient.prototype.put = function(id, entry, cb, mode) {
    var u = id? (this.uri? this.uri + '/' + id : id) : this.uri;
    var hascb = cb? true : false;

    // Update local storage
    var oentry;
    if(mode != 'remote') {
        oentry = HTTPBindingClient.getCacheItem(u);
        HTTPBindingClient.setCacheItem(u, entry);
    }

    // Call asynchronously with a callback
    if(hascb) {
        return HTTPBindingClient.scheduleAsyncRequest(function putRequest(http, done) {
            http.open('PUT', u, true);
            http.setRequestHeader('Accept', '*/*');
            http.setRequestHeader('Content-Type', 'application/atom+xml');
            http.setRequestHeader('X-Cache-Control', 'no-cache');
            http.onreadystatechange = function() {
                if(http.readyState == 4) {
                    if(http.status == 200) {

                        // Successful result
                        cb();

                    } else {
                        if(http.status == 404) {

                            // Undo local storage update
                            if(mode != 'remote') {
                                if(oentry)
                                    HTTPBindingClient.setCacheItem(u, oentry);
                                else
                                    HTTPBindingClient.removeCacheItem(u);
                            }
                        }

                        // Report status code as an exception
                        cb(new Error('' + http.status + ' ' + http.statusText));
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
    http.open('PUT', u, false);
    http.setRequestHeader('Accept', '*/*');
    http.setRequestHeader('Content-Type', 'application/atom+xml');
    http.setRequestHeader('X-Cache-Control', 'no-cache');
    http.send(entry);
    if(http.status == 200)
        return true;
    if(http.status == 404) {
        // Undo local storage update
        if(mode != 'remote') {
            if(oentry)
                HTTPBindingClient.setCacheItem(u, oentry);
            else
                HTTPBindingClient.removeCacheItem(u);
        }
    }

    // Return status code as an exception
    throw new Error('' + http.status + ' ' + http.statusText);
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
            http.open('DELETE', u, true);        
            http.setRequestHeader('Accept', '*/*');
            http.setRequestHeader('X-Cache-Control', 'no-cache');
            http.onreadystatechange = function() {
                if(http.readyState == 4) {
                    if(http.status == 200) {

                        // Successful result
                        cb();
                    }
                    else {
                        // Report status code as an exception
                        cb(new Error('' + http.status + ' ' + http.statusText));
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
    http.open('DELETE', u, false);        
    http.setRequestHeader('Accept', '*/*');
    http.setRequestHeader('X-Cache-Control', 'no-cache');
    http.send(null);
    if(http.status == 200)
        return true;

    // Report status code as an exception
    throw new Error('' + http.status + ' ' + http.statusText);
};

/**
 * Public API.
 */

var sca = {};

/**
 * Return an HTTP client proxy.
 */
sca.httpClient = function(name, uri, domain) {
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
            var args = [];
            args[0] = name;
            for(var i = 0; i < arguments.length; i++)
                args[i + 1] = arguments[i];
            return this.apply.apply(this, args);
        };
    }

    for(var f = 1; f < arguments.length; f++) {
        var fn = arguments[f];
        ref[fn]= defapply(fn);
    }
    return ref;
};

