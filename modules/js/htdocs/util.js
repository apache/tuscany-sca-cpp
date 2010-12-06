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
 * Utility functions.
 */

/**
 * Simple scheme-like lists.
 */
function cons(car, cdr) {
    return new Array(car).concat(cdr);
}

function car(l) {
    return l[0];
}

function cdr(l) {
    return l.slice(1);
}

function mklist() {
    var a = new Array();
    for (i = 0; i < arguments.length; i++)
        a[i] = arguments[i];
    return a;
}

function isList(v) {
    return toString.call(v) === '[object Array]';
}

function isNil(v) {
    return v == 'undefined' || v == null;
}

function isEmpty(l) {
    return l.length == 0;
}

/**
 * convert an array or object to a non-sparse array.
 */ 
function array(obj) {
    if (isNil(obj.length)) {
        var a = new Array();
        a[0] = obj;
        return a;
    } else {
        var a = new Array();
        var n = 0;
        for (var i in obj)
            a[n++] = obj[i];
        return a;
    }
}

/**
 * Dump an object to the console.
 */
function dump(o) {
    for (f in o) {
        try {
            console.log(f + '=' + o[f]);
        } catch (e) {}
    }
}

