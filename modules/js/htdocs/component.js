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

/**
 * Escape a character.
 */
var JSONClient = new Object();

JSONClient.escapeJSONChar = function(c) {
    if(c == "\"" || c == "\\") return "\\" + c;
    else if (c == "\b") return "\\b";
    else if (c == "\f") return "\\f";
    else if (c == "\n") return "\\n";
    else if (c == "\r") return "\\r";
    else if (c == "\t") return "\\t";
    var hex = c.charCodeAt(0).toString(16);
    if(hex.length == 1) return "\\u000" + hex;
    else if(hex.length == 2) return "\\u00" + hex;
    else if(hex.length == 3) return "\\u0" + hex;
    else return "\\u" + hex;
};

/**
 * Encode a string into JSON format.
 */
JSONClient.escapeJSONString = function(s) {
    /* The following should suffice but Safari's regex is broken
       (doesn't support callback substitutions)
       return "\"" + s.replace(/([^\u0020-\u007f]|[\\\"])/g,
       JSONClient.escapeJSONChar) + "\"";
    */

    /* Rather inefficient way to do it */
    var parts = s.split("");
    for(var i = 0; i < parts.length; i++) {
    var c = parts[i];
    if(c == '"' ||
       c == '\\' ||
       c.charCodeAt(0) < 32 ||
       c.charCodeAt(0) >= 128)
        parts[i] = JSONClient.escapeJSONChar(parts[i]);
    }
    return "\"" + parts.join("") + "\"";
};

/**
 * Marshall objects to JSON format.
 */
JSONClient.toJSON = function(o) {
    if(o == null) {
        return "null";
    } else if(o.constructor == String) {
        return JSONClient.escapeJSONString(o);
    } else if(o.constructor == Number) {
        return o.toString();
    } else if(o.constructor == Boolean) {
        return o.toString();
    } else if(o.constructor == Date) {
        return '{javaClass: "java.util.Date", time: ' + o.valueOf() +'}';
    } else if(o.constructor == Array) {
        var v = [];
        for(var i = 0; i < o.length; i++) v.push(JSONClient.toJSON(o[i]));
        return "[" + v.join(", ") + "]";
    } else {
        var v = [];
        for(attr in o) {
            if(o[attr] == null) v.push("\"" + attr + "\": null");
            else if(typeof o[attr] == "function"); /* skip */
            else v.push(JSONClient.escapeJSONString(attr) + ": " + JSONClient.toJSON(o[attr]));
        }
        return "{" + v.join(", ") + "}";
    }
};

/**
 * Construct an HTTPBindingClient.
 */
function HTTPBindingClient(cname, uri, objectID) {
    this.uri = "/references/" + cname + "/" + uri;
    this.objectID = objectID;
    this.apply = this._createApplyMethod();

    if (typeof DOMParser == "undefined") {
       DOMParser = function () {}
    
       DOMParser.prototype.parseFromString = function (str, contentType) {
          if (typeof ActiveXObject != "undefined") {
             var d = new ActiveXObject("MSXML.DomDocument");
             d.loadXML(str);
             return d;
          } else if (typeof XMLHttpRequest != "undefined") {
             var req = new XMLHttpRequest;
             req.open("GET", "data:" + (contentType || "application/xml") +
                             ";charset=utf-8," + encodeURIComponent(str), false);
             if (req.overrideMimeType) {
                req.overrideMimeType(contentType);
             }
             req.send(null);
             return req.responseXML;
          }
       }
    }
}

/**
 * HTTPBindingClient.Exception.
 */
HTTPBindingClient.Exception = function(code, message, javaStack) {
    this.code = code;
    var name;
    if(javaStack) {
        this.javaStack = javaStack;
        var m = javaStack.match(/^([^:]*)/);
        if(m) name = m[0];
    }
    if(name) this.name = name;
    else this.name = "HTTPBindingClientException";
    this.message = message;
};

HTTPBindingClient.Exception.CODE_REMOTE_EXCEPTION = 490;
HTTPBindingClient.Exception.CODE_ERR_CLIENT = 550;
HTTPBindingClient.Exception.CODE_ERR_PARSE = 590;
HTTPBindingClient.Exception.CODE_ERR_NOMETHOD = 591;
HTTPBindingClient.Exception.CODE_ERR_UNMARSHALL = 592;
HTTPBindingClient.Exception.CODE_ERR_MARSHALL = 593;

HTTPBindingClient.Exception.prototype = new Error();
HTTPBindingClient.Exception.prototype.toString = function(code, msg) {
    return this.name + ": " + this.message;
};

/**
 * Default top level exception handler.
 */
HTTPBindingClient.default_ex_handler = function(e) {
    alert(e);
};

/**
 * Client settable variables
 */
HTTPBindingClient.toplevel_ex_handler = HTTPBindingClient.default_ex_handler;
HTTPBindingClient.profile_async = false;
HTTPBindingClient.max_req_active = 1;
HTTPBindingClient.requestId = 1;

/**
 * HTTPBindingClient implementation
 */
HTTPBindingClient.prototype._createApplyMethod = function() {
    var fn = function() {
        var args = [];
        var callback = null;
        var methodName = arguments[0];
        for(var i = 1; i < arguments.length; i++) args.push(arguments[i]);

        if(typeof args[args.length - 1] == "function") callback = args.pop();

        var req = fn.client._makeRequest.call(fn.client, methodName, args, callback);
        if(callback == null) {
            return fn.client._sendRequest.call(fn.client, req);
        } else {
            HTTPBindingClient.async_requests.push(req);
            HTTPBindingClient.kick_async();
            return req.requestId;
        }
    };
    fn.client = this;
    return fn;
};

HTTPBindingClient._getCharsetFromHeaders = function(http) {
    try {
        var contentType = http.getResponseHeader("Content-type");
        var parts = contentType.split(/\s*;\s*/);
        for(var i = 0; i < parts.length; i++) {
            if(parts[i].substring(0, 8) == "charset=")
                return parts[i].substring(8, parts[i].length);
        }
    } catch (e) {}
    return "UTF-8";
};

/**
 * Async queue globals
 */
HTTPBindingClient.async_requests = [];
HTTPBindingClient.async_inflight = {};
HTTPBindingClient.async_responses = [];
HTTPBindingClient.async_timeout = null;
HTTPBindingClient.num_req_active = 0;

HTTPBindingClient._async_handler = function() {
    HTTPBindingClient.async_timeout = null;

    while(HTTPBindingClient.async_responses.length > 0) {
        var res = HTTPBindingClient.async_responses.shift();
        if(res.canceled) continue;
        if(res.profile) res.profile.dispatch = new Date();
        try {
            res.cb(res.result, res.ex, res.profile);
        } catch(e) {
            HTTPBindingClient.toplevel_ex_handler(e);
        }
    }

    while(HTTPBindingClient.async_requests.length > 0 && HTTPBindingClient.num_req_active < HTTPBindingClient.max_req_active) {
        var req = HTTPBindingClient.async_requests.shift();
        if(req.canceled) continue;
        req.client._sendRequest.call(req.client, req);
    }
};

HTTPBindingClient.kick_async = function() {
    if(HTTPBindingClient.async_timeout == null)
        HTTPBindingClient.async_timeout = setTimeout(HTTPBindingClient._async_handler, 0);
};

HTTPBindingClient.cancelRequest = function(requestId) {
    /* If it is in flight then mark it as canceled in the inflight map
       and the XMLHttpRequest callback will discard the reply. */
    if(HTTPBindingClient.async_inflight[requestId]) {
        HTTPBindingClient.async_inflight[requestId].canceled = true;
        return true;
    }

    /* If its not in flight yet then we can just mark it as canceled in
       the the request queue and it will get discarded before being sent. */
    for(var i in HTTPBindingClient.async_requests) {
        if(HTTPBindingClient.async_requests[i].requestId == requestId) {
            HTTPBindingClient.async_requests[i].canceled = true;
            return true;
        }
    }

    /* It may have returned from the network and be waiting for its callback
       to be dispatched, so mark it as canceled in the response queue
       and the response will get discarded before calling the callback. */
    for(var i in HTTPBindingClient.async_responses) {
        if(HTTPBindingClient.async_responses[i].requestId == requestId) {
            HTTPBindingClient.async_responses[i].canceled = true;
            return true;
        }
    }

    return false;
};

HTTPBindingClient.prototype._makeRequest = function(methodName, args, cb) {
    var req = {};
    req.client = this;
    req.requestId = HTTPBindingClient.requestId++;

    var obj = {};
    obj.id = req.requestId;
    if (this.objectID)
        obj.method = ".obj#" + this.objectID + "." + methodName;
    else
        obj.method = methodName;
    obj.params = args;

    if (cb) req.cb = cb;
    if (HTTPBindingClient.profile_async)
        req.profile = { "submit": new Date() };
    req.data = JSONClient.toJSON(obj);

    return req;
};

HTTPBindingClient.prototype._sendRequest = function(req) {
    if(req.profile) req.profile.start = new Date();

    /* Get free http object from the pool */
    var http = HTTPBindingClient.poolGetHTTPRequest();
    HTTPBindingClient.num_req_active++;

    /* Send the request */
    http.open("POST", this.uri, (req.cb != null));
    http.setRequestHeader("Content-type", "application/json-rpc");

    /* Construct call back if we have one */
    if(req.cb) {
        var self = this;
        http.onreadystatechange = function() {
            if(http.readyState == 4) {
                http.onreadystatechange = function () {};
                var res = { "cb": req.cb, "result": null, "ex": null};
                if (req.profile) {
                    res.profile = req.profile;
                    res.profile.end = new Date();
                }
                try { res.result = self._handleResponse(http); }
                catch(e) { res.ex = e; }
                if(!HTTPBindingClient.async_inflight[req.requestId].canceled)
                    HTTPBindingClient.async_responses.push(res);
                delete HTTPBindingClient.async_inflight[req.requestId];
                HTTPBindingClient.kick_async();
            }
        };
    } else {
        http.onreadystatechange = function() {};
    }

    HTTPBindingClient.async_inflight[req.requestId] = req;
    
    try {
        http.send(req.data);
    } catch(e) {
        HTTPBindingClient.poolReturnHTTPRequest(http);
        HTTPBindingClient.num_req_active--;
        throw new HTTPBindingClient.Exception(HTTPBindingClient.Exception.CODE_ERR_CLIENT, "Connection failed");
    }

    if(!req.cb) return this._handleResponse(http);
};

HTTPBindingClient.prototype._handleResponse = function(http) {
    /* Get the charset */
    if(!this.charset) {
        this.charset = HTTPBindingClient._getCharsetFromHeaders(http);
    }

    /* Get request results */
    var status, statusText, data;
    try {
        status = http.status;
        statusText = http.statusText;
        data = http.responseText;
    } catch(e) {
        HTTPBindingClient.poolReturnHTTPRequest(http);
        HTTPBindingClient.num_req_active--;
        HTTPBindingClient.kick_async();
        throw new HTTPBindingClient.Exception(HTTPBindingClient.Exception.CODE_ERR_CLIENT, "Connection failed");
    }

    /* Return http object to the pool; */
    HTTPBindingClient.poolReturnHTTPRequest(http);
    HTTPBindingClient.num_req_active--;

    /* Unmarshall the response */
    if(status != 200) {
        throw new HTTPBindingClient.Exception(status, statusText);
    }
    var obj;
    try {
        eval("obj = " + data);
    } catch(e) {
        throw new HTTPBindingClient.Exception(550, "error parsing result");
    }
    if(obj.error)
        throw new HTTPBindingClient.Exception(obj.error.code, obj.error.msg, obj.error.trace);
    var res = obj.result;

    /* Handle CallableProxy */
    if(res && res.objectID && res.JSONRPCType == "CallableReference")
        return new HTTPBindingClient(this.uri, res.objectID);

    return res;
};


/**
 * XMLHttpRequest wrapper code
 */
HTTPBindingClient.http_spare = [];
HTTPBindingClient.http_max_spare = 8;

HTTPBindingClient.poolGetHTTPRequest = function() {
    if(HTTPBindingClient.http_spare.length > 0) {
        return HTTPBindingClient.http_spare.pop();
    }
    return HTTPBindingClient.getHTTPRequest();
};

HTTPBindingClient.poolReturnHTTPRequest = function(http) {
    if(HTTPBindingClient.http_spare.length >= HTTPBindingClient.http_max_spare)
        delete http;
    else
        HTTPBindingClient.http_spare.push(http);
};

HTTPBindingClient.msxmlNames = [ "MSXML2.XMLHTTP.5.0",
                 "MSXML2.XMLHTTP.4.0",
                 "MSXML2.XMLHTTP.3.0",
                 "MSXML2.XMLHTTP",
                 "Microsoft.XMLHTTP" ];

HTTPBindingClient.getHTTPRequest = function() {
    /* Mozilla XMLHttpRequest */
    try {
        HTTPBindingClient.httpObjectName = "XMLHttpRequest";
        return new XMLHttpRequest();
    } catch(e) {}

    /* Microsoft MSXML ActiveX */
    for (var i=0; i < HTTPBindingClient.msxmlNames.length; i++) {
        try {
            HTTPBindingClient.httpObjectName = HTTPBindingClient.msxmlNames[i];
            return new ActiveXObject(HTTPBindingClient.msxmlNames[i]);
        } catch (e) {}
    }

    /* None found */
    HTTPBindingClient.httpObjectName = null;
    throw new HTTPBindingClient.Exception(0, "Can't create XMLHttpRequest object");
};


HTTPBindingClient.prototype.get = function(id, responseFunction) {
    var xhr = HTTPBindingClient.getHTTPRequest();
    xhr.onreadystatechange = function() {
        if (xhr.readyState == 4) {
            if (xhr.status == 200) {
                var strDocument = xhr.responseText;
                var xmlDocument = xhr.responseXML;
                if(!xmlDocument || xmlDocument.childNodes.length==0){ 
                    xmlDocument = (new DOMParser()).parseFromString(strDocument, "text/xml");
                } 
                if (responseFunction != null) responseFunction(xmlDocument);
            } else {
                alert("get - Error getting data from the server");
            }
        }
    }
    xhr.open("GET", this.uri + '/' + id, true);
    xhr.send(null);
};

HTTPBindingClient.prototype.post = function (entry, responseFunction) {
    var xhr = HTTPBindingClient.getHTTPRequest();
    xhr.onreadystatechange = function() {
        if (xhr.readyState == 4) {
            if (xhr.status == 201) {
                var strDocument = xhr.responseText;
                var xmlDocument = xhr.responseXML;
                if(!xmlDocument || xmlDocument.childNodes.length==0){ 
                    xmlDocument = (new DOMParser()).parseFromString(strDocument, "text/xml");
                } 
                if (responseFunction != null) responseFunction(xmlDocument);
            } else {
                alert("post - Error getting data from the server");
            }
        }
    }
    xhr.open("POST", this.uri, true);
    xhr.setRequestHeader("Content-Type", "application/atom+xml;type=entry");
    xhr.send(entry);
};

HTTPBindingClient.prototype.put = function (id, entry, responseFunction) {
    var xhr = HTTPBindingClient.getHTTPRequest();
    xhr.onreadystatechange = function() {
        if (xhr.readyState == 4) {
            if (xhr.status == 200) {
                var strDocument = xhr.responseText;
                var xmlDocument = xhr.responseXML;
                if(!xmlDocument || xmlDocument.childNodes.length==0){ 
                    xmlDocument = (new DOMParser()).parseFromString(strDocument, "text/xml");
                } 
                if (responseFunction != null) responseFunction(xmlDocument);
            } else {
                alert("put - Error getting data from the server");
            }
        }
    }
    xhr.open("PUT", this.uri + '/' + id, true);
    xhr.setRequestHeader("Content-Type", "application/atom+xml;type=entry");
    xhr.send(entry);
};

HTTPBindingClient.prototype.del = function (id, responseFunction) {       
    var xhr = HTTPBindingClient.getHTTPRequest();
    xhr.onreadystatechange = function() {
        if (xhr.readyState == 4) {
            if (xhr.status == 200) {
                if (responseFunction != null) responseFunction();
            } else {
                alert("delete - Error getting data from the server");
            }
        }
    }
    xhr.open("DELETE", this.uri + '/' + id, true);        
    xhr.send(null);
};

/**
 * Public API.
 */

var sca = new Object();

/**
 * Return a component.
 */
sca.component = function(name) {
    function ClientComponent(name) {
        this.name = name;
    }

    return new ClientComponent(name);
};

/**
 * Return a reference proxy.
 */
sca.reference = function(comp, name) {
    return new HTTPBindingClient(comp.name, name);
};

/**
 * Add proxy functions to a reference proxy.
 */
sca.defun = function(ref) {
    function defapply(name) {
        return function() {
            var args = new Array();
            args[0] = name;
            for (i = 0, n = arguments.length; i < n; i++)
                args[i + 1] = arguments[i];
            return this.apply.apply(this, args);
        };
    }

    for (f = 1; f < arguments.length; f++) {
        var fn = arguments[f];
        ref[fn]= defapply(fn);
    }
    return ref;
};

