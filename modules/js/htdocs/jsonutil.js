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
 * JSON data conversion functions.
 */
var json = {};

/**
 * JSON exceptions.
 */
json.Exception = function(code, message) {
    this.name = "JSONException";
    this.code = code;
    this.message = message;
};

json.Exception.prototype = new Error();

json.Exception.prototype.toString = function() {
    return this.name + ": " + this.message;
};

/**
 * Return true if a list represents a JS array.
 */
json.isJSArray = function(l) {
    if (isNull(l))
        return true;
    var v = car(l);
    if (isSymbol(v))
        return false;
    if (isList(v))
        if (!isNull(v) && isSymbol(car(v)))
            return false;
    return true;
};

/**
 * Converts JSON properties to values.
 */
json.jsPropertiesToValues = function(propertiesSoFar, o, i) {
    if (isNull(i))
        return propertiesSoFar;
    var p = car(i);
    var jsv = o[p];
    var v = json.jsValToValue(jsv);

    if (typeof p == 'string') {
        if (p[0] == '@')
            return json.jsPropertiesToValues(cons(mklist(attribute, "'" + p.substring(1), v), propertiesSoFar), o, cdr(i));
        if (isList(v) && !json.isJSArray(v))
            return json.jsPropertiesToValues(cons(cons(element, cons("'" + p, v)), propertiesSoFar), o, cdr(i));
        return json.jsPropertiesToValues(cons(mklist(element, "'" + p, v), propertiesSoFar), o, cdr(i));
    }
    return json.jsPropertiesToValues(cons(v, propertiesSoFar), o, cdr(i));
};

/**
 * Converts a JSON val to a value.
 */
json.jsValToValue = function(jsv) {
    if (jsv == null)
        return null;
    if (isList(jsv))
        return json.jsPropertiesToValues(nil, jsv, reverse(seq(0, jsv.length)));
    if (typeof jsv == 'object')
        return json.jsPropertiesToValues(nil, jsv, reverse(properties(jsv)));
    if (typeof jsv == 'string')
        return '' + jsv;
    return jsv;
}

/**
 * Return true if a list of strings contains a JSON document.
 */
json.isJSON = function(l) {
    if (isNull(l))
        return false;
    var s = car(l)[0];
    return s == "[" || s == "{";
};

/**
 * Convert a list of strings representing a JSON document to a list of values.
 */
json.readJSON = function(l) {
    var v = JSON.parse(writeStrings(l));
    return json.jsValToValue(v);
};

/**
 * Convert a list of values to JSON array elements.
 */
json.valuesToJSElements = function(a, l, i) {
    if (isNull(l))
        return a;
    var pv = json.valueToJSVal(car(l));
    a[i] = pv
    return json.valuesToJSElements(a, cdr(l), i + 1);
};
    
/**
 * Convert a value to a JSON value.
 */
json.valueToJSVal = function(v) {
    if (!isList(v))
        return v;
    if (json.isJSArray(v))
        return json.valuesToJSElements(seq(0, v.length), v, 0);
    return json.valuesToJSProperties({}, v);
};

/**
 * Convert a list of values to JSON properties.
 */
json.valuesToJSProperties = function(o, l) {
    if (isNull(l))
        return o;
    var token = car(l);
    if (isTaggedList(token, attribute)) {
        var pv = json.valueToJSVal(attributeValue(token));
        o['@' + attributeName(token).substring(1)] = pv;
    } else if (isTaggedList(token, element)) {
        if (elementHasValue(token)) {
            var pv = json.valueToJSVal(elementValue(token));
            o[elementName(token).substring(1)] = pv;
        } else {
            var child = {};
            o[elementName(token).substring(1)] = child;
            json.valuesToJSProperties(child, elementChildren(token));
        }
    }
    return json.valuesToJSProperties(o, cdr(l));
};

/**
 * Convert a list of values to a list of strings representing a JSON document.
 */
json.writeJSON = function(l) {
    var jsv;
    if (json.isJSArray(l))
        jsv = json.valuesToJSElements(seq(0, l.length), l, 0);
    else
        jsv = json.valuesToJSProperties({}, l);
    return mklist(JSON.stringify(jsv));
}

/**
 * Convert a list + params to a JSON-RPC request.
 */
json.jsonRequest = function(id, func, params) {
    var r = mklist(mklist("'id", id), mklist("'method", func), mklist("'params", params));
    return json.writeJSON(valuesToElements(r));
};

/**
 * Convert a value to a JSON-RPC result.
 */
json.jsonResult = function(id, val) {
    return json.writeJSON(valuesToElements(mklist(mklist("'id", id), mklist("'result", val))));
};

/**
 * Convert a JSON-RPC result to a value.
 */
json.jsonResultValue = function(s) {
    var jsres = json.readJSON(s);
    var res = elementsToValues(jsres);
    var val = cadr(assoc("'result", res));
    if (isList(val) && !json.isJSArray(val))
        return mklist(val);
    return val;
};

