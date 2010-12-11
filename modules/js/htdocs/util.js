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
    return new Array(car).concat(cdr);
}

function car(l) {
    return l[0];
}

function first(l) {
    return car(l);
}

function cdr(l) {
    return l.slice(1);
}

function rest(l) {
    return cdr(l);
}

function cadr(l) {
    return car(cdr(l));
}

function cddr(l) {
    return cdr(cdr(l));
}

function caddr(l) {
    return car(cddr(l));
}

function append(a, b) {
    return a.concat(b);
}

function reverse(l) {
    return l.slice(0).reverse();
}

function isNil(v) {
    if (v == null)
        return true;
    if ('' + v == 'undefined')
        return true;
    try {
        if (isList(v) && v.length == 0)
            return true;
    } catch (e) {}
    return false;
}

function isSymbol(v) {
    if (typeof v == 'string' && v.slice(0, 1) == "'")
        return true;
    return false;
}

function isString(v) {
    if (typeof v == 'string')
        return true;
    return false;
}

function isList(v) {
    try {
        if (v.constructor == Array)
            return true;
    } catch (e) {}
    return false;
}

function isTaggedList(v, t) {
    if (isList(v) && !isNil(v) && car(v) == t)
        return true;
    return false;
}

function mklist() {
    var a = new Array();
    for (i = 0; i < arguments.length; i++)
        a[i] = arguments[i];
    return a;
}

/**
 * Scheme-like associations.
 */
function assoc(k, l) {
    if (isNil(l))
        return mklist();
    if (k == car(car(l)))
        return car(l);
    return assoc(k, cdr(l));
}

/**
 * Map and filter functions.
 */
function map(f, l) {
    if (isNil(l))
        return l;
    return cons(f(car(l)), map(f, cdr(l)));
}

function filter(f, l) {
    if (isNil(l))
        return l;
    if (f(car(l)))
        return cons(car(l), filter(f, cdr(l)));
    return filter(f, cdr(l));
}

/**
 * Dump an object to the debug console.
 */
function debug(o) {
    for (f in o) {
        try {
            console.log(f + '=' + o[f]);
        } catch (e) {}
    }
}

/**
 * Write a list of strings.
 */
function writeStrings(l) {
    if (isNil(l))
        return '';
    return car(l) + writeStrings(cdr(l));
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
        if (isNil(l))
            return '';
        return ' ' + writeValue(car(l)) + writeList(cdr(l));
    }

    if (!isList(v))
        return writePrimitive(v);
    if (isNil(v))
        return '()';
    return '(' + writeValue(car(v)) + writeList(cdr(v)) + ')';
}

