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
 * Simple utility functions.
 */

/**
 * Scheme-like lists.
 */
function cons(car, cdr) {
    var a = new Array();
    a.push(car);
    return a.concat(cdr);
}

function car(l) {
    return l[0];
}

function first(l) {
    return l[0];
}

function cdr(l) {
    return l.slice(1);
}

function rest(l) {
    return l.slice(1);
}

function cadr(l) {
    return l[1];
}

function cddr(l) {
    return l.slice(2);
}

function caddr(l) {
    return l[2];
}

function cdddr(l) {
    return l.slice(3);
}

function cadddr(l) {
    return l[3];
}

function append(a, b) {
    return a.concat(b);
}

function reverse(l) {
    return l.slice(0).reverse();
}

function range(a, b) {
    var l = new Array();
    for (var x = a; x < b; x++)
        l.push(x);
    return l;
}

function isNull(v) {
    return (v == null || typeof v == 'undefined' || (v.constructor == Array && v.length == 0));
}

function isSymbol(v) {
    return (typeof v == 'string' && v.slice(0, 1) == "'");
}

function isString(v) {
    return (typeof v == 'string' && v.slice(0, 1) != "'");
}

function isList(v) {
    return (v != null && typeof v != 'undefined' && v.constructor == Array);
}

function isTaggedList(v, t) {
    return (isList(v) && !isNull(v) && car(v) == t);
}

var emptylist = new Array();

function mklist() {
    if (arguments.length == 0)
        return emptylist;
    var a = new Array();
    for (i = 0; i < arguments.length; i++)
        a[i] = arguments[i];
    return a;
}

function length(l) {
    return l.length;
}

/**
 * Scheme-like associations.
 */
function assoc(k, l) {
    if (isNull(l))
        return emptylist;
    var n = l.length;
    for(var i = 0; i < n; i++) {
        if (k == car(l[i]))
            return l[i];
    }
    return emptylist;
}

/**
 * Map, filter and reduce functions.
 */
function map(f, l) {
    if (isNull(l))
        return l;
    var n = l.length;
    var a = new Array();
    for(var i = 0; i < n; i++) {
        a.push(f(l[i]));
    }
    return a;
}

function filter(f, l) {
    if (isNull(l))
        return l;
    var n = l.length;
    var a = new Array();
    for(var i = 0; i < n; i++) {
        if (f(l[i]))
            a.push(l[i]);
    }
    return a;
}

function reduce(f, i, l) {
    if (isNull(l))
        return i;
    return reduce(f, f(i, car(l)), cdr(l));
}

/**
 * Split a path into a list of segments.
 */
function tokens(path) {
    return filter(function(s) { return length(s) != 0; }, path.split("/"));
}

/**
 * Debug log a value.
 */
var rconsole;

function debug(v) {
    try {
        var s = '';
        for (i = 0; i < arguments.length; i++) {
            s = s + writeValue(arguments[i]);
            if (i < arguments.length)
                s = s + ' ';
        }

        if (rconsole) {
            try {
                rconsole.log(s);
            } catch (e) {}
        }
        try {
            console.log(s);
        } catch (e) {}
    } catch (e) {}
    return true;
}

/**
 * Dump an object to the console.
 */
function dump(o) {
    try {
        for (f in o) {
            try {
                debug('dump ' + f + '=' + o[f]);
            } catch (e) {}
        }
    } catch (e) {}
    return true;
}

/**
 * Return true if the current browser is Internet Explorer.
 */
function isIE() {
    if (typeof isIE.detected != 'undefined')
        return isIE.detected;
    isIE.detected = navigator.appName == 'Microsoft Internet Explorer';
    return isIE.detected;
};

/**
 * External build configuration.
 */
var config;
if (isNull(config))
    config = {};

/**
 * Simple assert function.
 */
function AssertException() {
}

AssertException.prototype.toString = function () {
    return 'AssertException';
};

function assert(exp) {
    if (!exp)
        throw new AssertException();
}

/**
 * Write a list of strings.
 */
function writeStrings(l) {
    if (isNull(l))
        return '';
    var s = '';
    var n = l.length;
    for(var i = 0; i < n; i++) {
        s = s + l[i];
    }
    return s;
}

/**
 * Write a value using a Scheme-like syntax.
 */
function writeValue(v) {
    function writePrimitive(p) {
        if (isSymbol(p))
            return '' + p.substring(1);
        if (isString(p))
            return '"' + p + '"';
        return '' + p;
    }

    function writeList(l) {
        if (isNull(l))
            return '';
        return ' ' + writeValue(car(l)) + writeList(cdr(l));
    }

    if (!isList(v))
        return writePrimitive(v);
    if (isNull(v))
        return '()';
    return '(' + writeValue(car(v)) + writeList(cdr(v)) + ')';
}

/**
 * Apply a function and memoize its result.
 */
function memo(obj, key, f) {
    if (!('memo' in obj)) {
        obj.memo = {};
        return obj.memo[key] = f();
    }
    if (key in obj.memo)
        return obj.memo[key];
    return obj.memo[key] = f();
}

/**
 * Un-memoize stored results.
 */
function unmemo(obj, prefix) {
    if (!prefix) {
        obj.memo = {};
        return true;
    }
    if (!('memo' in obj)) {
        obj.memo = {};
        return true;
    }
    for (key in obj.memo) {
        if (key.substring(0, prefix.length) == prefix)
            delete obj.memo[key];
    }
}

/**
 * Local storage.
 */
var lstorage = {};
lstorage.enabled = true;

/**
 * Get a key.
 */
lstorage.key = function(i) {
    if (!lstorage.enabled)
        return null;
    try {
        return localStorage.key(i);
    } catch(e) {
        return null;
    }
};

/**
 * Return the number of keys.
 */
lstorage.length = function() {
    if (!lstorage.enabled)
        return 0;
    try {
        return localStorage.length;
    } catch(e) {
        return 0;
    }
};

/**
 * Get an item.
 */
lstorage.getItem = function(k) {
    if (!lstorage.enabled)
        return null;
    try {
        return localStorage.getItem(k);
    } catch(e) {
        return null;
    }
};

/**
 * Set an item.
 */
lstorage.setItem = function(k, v) {
    if (!lstorage.enabled)
        return null;
    try {
        return localStorage.setItem(k, v);
    } catch(e) {
        return null;
    }
};

/**
 * Remove an item.
 */
lstorage.removeItem = function(k) {
    if (!lstorage.enabled)
        return null;
    try {
        return localStorage.removeItem(k);
    } catch(e) {
        return null;
    }
};

/**
 * Returns a list of the properties of an object.
 */
function properties(o) {
    var a = new Array();
    for (p in o)
        a.push(p);
    return a;
}

/**
 * Convert a host name to a domain name.
 */
function domainname(host) {
    var ds = host.indexOf('//');
    if (ds != -1)
        return domainname(host.substring(ds + 2));
    var s = host.indexOf('/');
    if (s != -1)
        return domainname(host.substring(0, s));
    var h = reverse(host.split('.'));
    var d = (!isNull(cddr(h)) && caddr(h) == 'www')? mklist(car(h), cadr(h), caddr(h)) : mklist(car(h), cadr(h));
    return reverse(d).join('.');
}

/**
 * Convert a host name to a top domain name.
 */
function topdomainname(host) {
    var d = reverse(domainname(host).split('.'));
    return reverse(mklist(car(d), cadr(d))).join('.');
}

/**
 * Return true if a host name is a subdomain.
 */
function issubdomain(host) {
    return host.split('.').length > 2;
}

/**
 * Clear auth information from the document cookie.
 */
function clearauthcookie() {
    document.cookie = 'TuscanyOpenAuth=; expires=' + new Date(1970,01,01).toGMTString() + '; domain=.' + domainname(window.location.hostname) + '; path=/; secure; httponly';
    document.cookie = 'TuscanyOAuth1=; expires=' + new Date(1970,01,01).toGMTString() + '; domain=.' + domainname(window.location.hostname) + '; path=/; secure; httponly';
    document.cookie = 'TuscanyOAuth2=; expires=' + new Date(1970,01,01).toGMTString() + '; domain=.' + domainname(window.location.hostname) + '; path=/; secure; httponly';
    document.cookie = 'TuscanyOpenIDAuth=; expires=' + new Date(1970,01,01).toGMTString() + '; domain=.' + domainname(window.location.hostname) + '; path=/; secure; httponly';
    return true;
}

/**
 * Format a string like Python format.
 */
function format() {
    var i = 0;
    var s = '';
    for (a in arguments) {
        s = i == 0? arguments[a] : s.replace('{' + a + '}', arguments[a]);
        i++;
    }
    return s;
}

/**
 * Parse an XML dateTime.
 */
function xmldatetime(xml) {
    var re = /^([0-9]{4,})-([0-9]{2})-([0-9]{2})T([0-9]{2}):([0-9]{2}):([0-9]{2})(\.[0-9]+)?(Z|([+-])([0-9]{2}):([0-9]{2}))?$/;
    var match = xml.match(re);
    if (!match)
        return new Date();
    return new Date(Date.UTC(match[1], parseInt(match[2]) - 1, match[3],
                match[9]? parseInt(match[4]) + parseInt(match[10]) * (match[9] == '+'? 1 : -1) : match[4],
                match[9]? parseInt(match[5]) + parseInt(match[11]) * (match[9] == '+'? 1 : -1) : match[5],
                match[6], 0));
}

/**
 * Encode a string to a url-safe base64 format.
 */
function safeb64encode(s) {
    return btoa(s).replace(/\+/g, '-').replace(/\//g, '_').replace(/\=+$/, '');
}

/**
 * Decode a url-safe base64 encoded string.
 */
function safeb64decode(s) {
    return atob((s.replace(/\-/g, '+').replace(/\_/g, '/') + '===').substring(0, s.length + (s.length % 4)));
}

/**
 * Return a uuid4.
 */
function uuid4() {
    if (window.crypto && window.crypto.getRandomValues) {
        var b = new Uint16Array(8);
        window.crypto.getRandomValues(b);
        function s4(n) {
            var s = '000' + n.toString(16);
            return s.substr(s.length - 4);
        }
        return s4(b[0]) + s4(b[1]) + '-' + s4(b[2]) + '-' + s4(b[3]) + '-' + s4(b[4]) + '-' + s4(b[5]) + s4(b[6]) + s4(b[7]);
    } else {
        return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(c) {
            var r = Math.random() * 16 | 0
            return (c == 'x'? r : (r & 0x3 | 0x8)).toString(16);
        });
    }
}

/**
 * Convert an hexadecimal string to ascii.
 */
function hex2ascii(x) {
    var a = '';
    for (var i = 0; i < x.length; i += 2)
        a += String.fromCharCode(parseInt(x.substr(i, 2), 16));
    return a;
}

/**
 * Functions with side effects. Use with moderation.
 */

/**
 * Set the car of a list.
 */
function setcar(l, v) {
    l[0] = v;
    return l;
}

/**
 * Set the cadr of a list.
 */
function setcadr(l, v) {
    l[1] = v;
    return l;
}

/**
 * Set the caddr of a list.
 */
function setcaddr(l, v) {
    l[2] = v;
    return l;
}

/**
 * Append the elements of a list to a list.
 */
function setappend(a, b) {
    if (isNull(b))
        return a;
    a.push(car(b));
    return setappend(a, cdr(b));
}

/**
 * Set the cdr of a list.
 */
function setcdr(a, b) {
    a.length = 1;
    return setappend(a, b);
}

/**
 * Set the contents of a list.
 */
function setlist(a, b) {
    if (b == a)
        return b;
    a.length = 0;
    return setappend(a, b);
}

