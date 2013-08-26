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
function cell(car, cdr) {
    this.car = car;
    this.cdr = cdr;
}

cell.prototype.toString = function() {
    return writeValue(this);
};

function cons(car, cdr) {
    return new cell(car, cdr);
}

var nil = new cell(undefined, null);

function mklist() {
    if(arguments.length == 0)
        return nil;
    var l = nil;
    for(var i = arguments.length - 1; i >= 0; i--)
        l = cons(arguments[i], l);
    return l;
}

function mkalist(a) {
    if(a.length == 0)
        return nil;
    var l = nil;
    for(var i = a.length - 1; i >= 0; i--)
        l = cons(a[i], l);
    return l;
}

function car(l) {
    if(l.cdr == null)
        throw new Error('car out of bounds');
        //error('car out of bounds');
    return l.car;
}

function cdr(l) {
    if(l.cdr == null)
        throw new Error('cdr out of bounds');
        //error('cdr out of bounds');
    return l.cdr;
}

function cadr(l) {
    return car(cdr(l));
}

function cddr(l) {
    return cdr(cdr(l));
}

function caddr(l) {
    return car(cdr(cdr(l)));
}

function cdddr(l) {
    return cdr(cdr(cdr(l)));
}

function cadddr(l) {
    return car(cdr(cdr(cdr(l))));
}

function last(l) {
    if(l == nil)
        throw new Error('last out of bounds');
        //error('last out of bounds');
    if(cdr(l) == nil)
        return car(l);
    return last(cdr(l));
}

function append(a, b) {
    if(a == nil)
        return b;
    return cons(car(a), append(cdr(a), b));
}

function reverse(l) {
    function reverseIter(acc, l) {
        if(l == nil)
            return acc;
        return reverseIter(cons(car(l), acc), cdr(l));
    }
    return reverseIter(nil, l);
}

function seq(start, end) {
    if(start == end)
        return mklist(start);
    if(start < end)
        return cons(start, seq(start + 1, end));
    return cons(start, seq(start - 1, end));
}

function isNull(v) {
    return v == nil || v == null || typeof v == 'undefined';
}

function isSymbol(v) {
    return typeof v == 'string' && v[0] == "'";
}

function isString(v) {
    return typeof v == 'string' && v[0] != "'";
}

function isList(v) {
    return v != null && typeof v != 'undefined' && typeof v.cdr != 'undefined';
}

function isTaggedList(v, t) {
    return isList(v) && v != nil && car(v) == t;
}

function mkarray(l) {
    return reduce(function(a, v) {
        a[a.length] = v;
        return a;
    }, [], l);
}

function length(l) {
    function lengthRef(c, l) {
        if(l == nil)
            return c;
        return lengthRef(c + 1, cdr(l));
    }
    return lengthRef(0, l);
}

/**
 * Scheme-like associations.
 */
function assoc(k, l) {
    if(l == nil)
        return nil;
    var c = car(l);
    if(isList(c) && c != nil  && k == car(c))
        return c;
    return assoc(k, cdr(l));
}

/**
 * Map, filter and reduce functions.
 */
function map(f, l) {
    if(l == nil)
        return nil;
    return cons(f(car(l)), map(f, cdr(l)));
}

function filter(f, l) {
    if(l == nil)
        return nil;
    if (f(car(l)))
        return cons(car(l), filter(f, cdr(l)));
    return filter(f, cdr(l));
}

function reduce(f, i, l) {
    function reduceAccumulate(acc, l) {
        if(l == nil)
            return acc;
        return reduceAccumulate(f(acc, car(l)), cdr(l));
    };
    return reduceAccumulate(i, l);
}

/**
 * Sort.
 */
function sort(f, l) {
    return mkalist(mkarray(l).sort(f));
}

/**
 * String split, join and tokenize functions.
 */
function split(s, d) {
    return mkalist(s.split(d));
}

function join(l, d) {
    return mkarray(l).join(d);
}

function tokens(path) {
    return filter(function(s) { return s.length != 0; }, split(path, '/'));
}

/**
 * Log values to debug log.
 */
if(window.debugging == undefined)
    window.debugging = false;
var remoteLog;
var bufferedLog;
 
function debug() {
    if (!window.debugging)
        return false;
    var s = '';
    for(var i = 0; i < arguments.length; i++) {
        s += writeValue(arguments[i]);
        if(i < arguments.length)
            s += ' ';
    }
    if(remoteLog)
        remoteLog.log(s);
    if (bufferedLog)
        bufferedLog[bufferedLog.length] = s;
    return console.log(s);
}

function error() {
    var s = '';
    for(var i = 0; i < arguments.length; i++) {
        s += writeValue(arguments[i]);
        var a = arguments[i];
        if(a != null && typeof a == 'object' && typeof a.stack != 'undefined') {
            s += ' ';
            s += writeValue(a.stack);
        }
        if(i < arguments.length)
            s = s + ' ';
    }

    if(remoteLog)
        remoteLog.error(s);
    if (bufferedLog) {
        try { throw new Error(); } catch(t) { bufferedLog[bufferedLog.length] = writeValue(t.stack); }
        bufferedLog[bufferedLog.length] = s;
    }
    return console.error(s);
}

/**
 * Log uncaught errors.
 */
if (typeof window != 'undefined') {
    window.onerror = function(msg, url, line) {
        error('window.onerror', msg, url, line);
        return false;
    };
}

/**
 * Buffer log entries in memory.
 */
function bufferLog() {
    bufferedLog = [];
    return true;
}

function printLog() {
    if (!bufferedLog)
        return false;
    for(var i in bufferedLog)
        console.log(bufferedLog[i]);
    return true;
}

function clearLog() {
    bufferedLog = [];
    return true;
}

/**
 * Dump an object to the log.
 */
function dump(o) {
    if (!window.debugging)
        return false;
    for(var f in o)
        debug('dump', f, '=', o[f]);
    return true;
}

/**
 * Return true if the current browser is Internet Explorer.
 */
function isMSIE() {
    if(typeof isMSIE.detected != 'undefined')
        return isMSIE.detected;
    isMSIE.detected = navigator.userAgent.match(/MSIE/i);
    return isMSIE.detected;
}

/**
 * External build configuration.
 */
var config;
if(isNull(config))
    config = {};

/**
 * Assertion.
 */
function assert(exp) {
    if(!exp)
        throw new Error('assertion failed');
    return true;
}

/**
 * Write a list of strings.
 */
function writeStrings(l) {
    return reduce(function(a, s) { return a + s; }, '', l);
}

/**
 * Write a value using a Scheme-like syntax.
 */
function writeValue(v) {
    function writePrimitive(p) {
        if(isSymbol(p))
            return '' + p.substring(1);
        if(isString(p))
            return '"' + p + '"';
        return '' + p;
    }

    function writeList(l) {
        if(l == nil)
            return '';
        return ' ' + writeValue(car(l)) + writeList(cdr(l));
    }

    if(!isList(v))
        return writePrimitive(v);
    if(v == nil)
        return '()';
    return '(' + writeValue(car(v)) + writeList(cdr(v)) + ')';
}

/**
 * Apply a function and memoize its result.
 */
function memo(obj, key, f) {
    if(typeof obj.memo == 'undefined') {
        obj.memo = {};
        return obj.memo[key] = f();
    }
    if(key in obj.memo)
        return obj.memo[key];
    return obj.memo[key] = f();
}

/**
 * Un-memoize stored results.
 */
function unmemo(obj, prefix) {
    if(!prefix) {
        obj.memo = {};
        return true;
    }
    if(typeof obj.memo == 'undefined') {
        obj.memo = {};
        return true;
    }
    for(var key in obj.memo) {
        if(key.substring(0, prefix.length) == prefix)
            delete obj.memo[key];
    }
    return true;
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
    if(!lstorage.enabled)
        return undefined;
    return localStorage.key(i);
};

/**
 * Return the number of keys.
 */
lstorage.length = function() {
    if(!lstorage.enabled)
        return 0;
    return localStorage.length;
};

/**
 * Get an item.
 */
lstorage.getItem = function(k) {
    if(!lstorage.enabled)
        return undefined;
    return localStorage.getItem(k);
};

/**
 * Set an item.
 */
lstorage.setItem = function(k, v) {
    if(!lstorage.enabled)
        return v;
    if (localStorage.getItem(k) != v)
        return localStorage.setItem(k, v);
    else
        return v;
};

/**
 * Remove an item.
 */
lstorage.removeItem = function(k) {
    if(!lstorage.enabled)
        return undefined;
    return localStorage.removeItem(k);
};

/**
 * Returns a list of the properties of an object.
 */
function properties(o) {
    var l = nil;
    for(var p in o)
        l = cons(p, l);
    return reverse(l);
}

/**
 * Convert a DOM node list to a regular list.
 */
function nodeList(n) {
    if (n == null || n.length == 0)
        return nil;
    var l = nil;
    for (var i = n.length - 1; i >= 0; i--)
        l = cons(n[i], l);
    return l;
}

/**
 * Convert a host name to a domain name.
 */
function domainName(host) {
    var ds = host.indexOf('//');
    if(ds != -1)
        return domainName(host.substring(ds + 2));
    var s = host.indexOf('/');
    if(s != -1)
        return domainName(host.substring(0, s));
    var h = reverse(split(host, '.'));
    var d = (cddr(h) != nil && caddr(h) == 'www')? mklist(car(h), cadr(h), caddr(h)) : mklist(car(h), cadr(h));
    return join(reverse(d), '.');
}

/**
 * Convert a host name to a top domain name.
 */
function topDomainName(host) {
    var d = reverse(split(domainName(host),'.'));
    return join(mklist(car(d), cadr(d)), '.');
}

/**
 * Return true if a host name is a subdomain.
 */
function isSubDomain(host) {
    return length(split(host, '.')) > 2;
}

/**
 * Clear auth information from the document cookie.
 */
function clearAuthCookie() {
    var d = new Date(1970,01,01).toGMTString();
    var dn = domainName(window.location.hostname);
    document.cookie = 'TuscanyOpenAuth=; expires=' + d + '; domain=.' + dn + '; path=/; secure; httponly';
    document.cookie = 'TuscanyOAuth1=; expires=' + d + '; domain=.' + dn + '; path=/; secure; httponly';
    document.cookie = 'TuscanyOAuth2=; expires=' + d + '; domain=.' + dn + '; path=/; secure; httponly';
    document.cookie = 'TuscanyOpenIDAuth=; expires=' + d + '; domain=.' + dn + '; path=/; secure; httponly';
    return true;
}

/**
 * Format a string like Python format.
 */
function format() {
    var i = 0;
    var s = '';
    for(var a = 0; a < arguments.length; a++) {
        s = i == 0? arguments[a] : s.replace('{' + a + '}', arguments[a]);
        i++;
    }
    return s;
}

/**
 * Parse an XML dateTime.
 */
function xmlDateTime(xml) {
    var re = /^([0-9]{4,})-([0-9]{2})-([0-9]{2})T([0-9]{2}):([0-9]{2}):([0-9]{2})(\.[0-9]+)?(Z|([+-])([0-9]{2}):([0-9]{2}))?$/;
    var match = xml.match(re);
    if(!match)
        return new Date();
    return new Date(Date.UTC(match[1], parseInt(match[2]) - 1, match[3],
                match[9]? parseInt(match[4]) + parseInt(match[10]) * (match[9] == '+'? 1 : -1) : match[4],
                match[9]? parseInt(match[5]) + parseInt(match[11]) * (match[9] == '+'? 1 : -1) : match[5],
                match[6], 0));
}

/**
 * Encode a string to a url-safe base64 format.
 */
function safeB64Encode(s) {
    return btoa(s).replace(/\+/g, '-').replace(/\//g, '_').replace(/\=+$/, '');
}

/**
 * Decode a url-safe base64 encoded string.
 */
function safeB64Decode(s) {
    return atob((s.replace(/\-/g, '+').replace(/\_/g, '/') + '===').substring(0, s.length + (s.length % 4)));
}

/**
 * Return a uuid4.
 */
function uuid4() {
    if(window.crypto && window.crypto.getRandomValues) {
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
function hex2Ascii(x) {
    var a = '';
    for(var i = 0; i < x.length; i += 2)
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
    l.car = v;
    return l;
}

/**
 * Set the cadr of a list.
 */
function setcadr(l, v) {
    l.cdr.car = v;
    return l;
}

/**
 * Set the caddr of a list.
 */
function setcaddr(l, v) {
    l.cdr.cdr.car = v;
    return l;
}

/**
 * Set the cdr of a list.
 */
function setcdr(a, b) {
    a.cdr = b;
    return a;
}

/**
 * Set the contents of a list.
 */
function setList(a, b) {
    if(b == a)
        return a;
    a.car = b.car;
    a.cdr = b.cdr;
    return a;
}

/**
 * Append the elements of a list to a list.
 */
function setAppend(a, b) {
    if(b.cdr == null)
        return a;
    return setList(a, append(a, b));
}

/**
 * Uncomment to run the tests.
 */
/*
(function testUtil() {
    console.log('Testing...');

    assert(car(cons(1, nil)) == 1);
    assert(car(mklist(1)) == 1);
    assert(cadr(mklist(1, 2)) == 2);

    assert(0 == length(nil));
    assert(1 == length(mklist(1)));
    assert(2 == length(cons(1, mklist(2))));

    assert(car(append(mklist(1), mklist(2))) == 1);
    assert(car(cdr(append(mklist(1), mklist(2)))) == 2);
    assert(car(cdr(cdr(append(mklist(1), mklist(2, 3))))) == 3);
    assert(isNull(cdr(cdr(cdr(append(mklist(1), mklist(2, 3)))))));
    assert(last(mklist(1, 2, 3)) == 3);
    assert('' + mklist(1, 2, 3) == '(1 2 3)');

    function square(v) { return v * v; }
    assert(isNull(map(square, nil)));
    var m = map(square, mklist(2, 3));
    assert(car(m) == 4);
    assert(car(cdr(m)) == 9);

    function add(x, y) { return x + y; }
    assert(reduce(add, 0, mklist(1, 2, 3)) == 6);

    function isPositive(x) { return x >= 0; }
    assert(car(filter(isPositive, mklist(1, -1, 2, -2))) == 1);
    assert(cadr(filter(isPositive, mklist(1, -1, 2, -2))) == 2);

    assert(isNull(reverse(nil)));
    assert(car(reverse(mklist(1, 2, 3))) == 3);
    assert(cadr(reverse(mklist(1, 2, 3))) == 2);

    var l = mklist(mklist('x', 'X'), mklist('a', 'A'), mklist('y', 'Y'), mklist('a', 'AA'));
    assert(car(assoc('a', l)) == 'a');
    assert(isNull(assoc('z', l)));

    var s = seq(0.0, 1000.0);
    assert(1001 == length(s));
    function seqreduce(acc, v) { return acc + 1.0; }
    assert(1001 == reduce(seqreduce, 0.0, s));

    function compare(a, b) { return a < b? -1 : a == b? 0 : 1; }
    var l2 = sort(compare, mklist(4, 3, 1, 2));
    assert(car(l2) == 1);
    assert(last(l2) == 4);

    var t = new Date();
    var p = nil;
    for(var i = 0; i < 100000; i++) {
        p = cons(i, p);
    }
    for(var i = 0; i < 100000; i++) {
        var x = car(p);
        p = cdr(p);
    }
    console.log('list perf', new Date() - t, 'ms');

    console.log('OK');
    return true;
})();
*/

