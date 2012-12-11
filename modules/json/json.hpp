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

/* $Rev$ $Date$ */

#ifndef tuscany_json_hpp
#define tuscany_json_hpp

/**
 * JSON data conversion functions.
 */

#include "string.hpp"
#include "list.hpp"
#include "value.hpp"
#include "element.hpp"
#include "monad.hpp"
#include "jansson.h"

namespace tuscany {
namespace json {

/**
 * Customize JSON memory allocation functions.
 */
inline void* jsonAlloc(size_t size) {
    return gc_pool_malloc(size);
}

inline void jsonFree(void *ptr) {
    return gc_pool_free(ptr);
}

class JSONAllocFuncs {
public:
    inline JSONAllocFuncs() {
        json_set_alloc_funcs(jsonAlloc, jsonFree);
    }
} jsonAllocFuncs;

/**
 * Returns true if a list represents a JS array.
 */
inline const bool isJSArray(const list<value>& l) {
    if(isNil(l))
        return true;
    const value v = car(l);
    if (isSymbol(v))
        return false;
    if(isList(v)) {
        if(!isNil((list<value>)v) && isSymbol(car<value>(v)))
            return false;
    }
    return true;
}

/**
 * Converts a JS array to values.
 */
inline const list<value> jsArrayToValues(const list<value>& listSoFar, json_t* const a, const int i) {
    const value jsValToValue(json_t* const jsv);

    if (i < 0)
        return listSoFar;
    json_t* const jsv = json_array_get(a, i);
    const value val = jsValToValue(jsv);
    return jsArrayToValues(cons(val, listSoFar), a, i - 1);
}

/**
 * Converts JS properties to values.
 */
inline const list<value> jsPropertiesToValues(const list<value>& propertiesSoFar, json_t* const o, void* i) {
    const value jsValToValue(json_t* const jsv);

    if (i == NULL)
        return reverse(propertiesSoFar);
    const char* const name = json_object_iter_key(i);
    json_t* const jsv = json_object_iter_value(i);
    const value val = jsValToValue(jsv);
    if (isNil(val) && !isList(val))
        return jsPropertiesToValues(cons<value> (mklist<value> (element, c_str(name), val), propertiesSoFar), o, json_object_iter_next(o, i));
    if (substr(name, 0, 1) == atsign)
        return jsPropertiesToValues(cons<value>(mklist<value>(attribute, c_str(substr(name, 1)), val), propertiesSoFar), o, json_object_iter_next(o, i));
    if (isList(val) && !isJSArray(val))
        return jsPropertiesToValues(cons<value>(cons<value>(element, cons<value>(c_str(name), list<value>(val))), propertiesSoFar), o, json_object_iter_next(o, i));
    return jsPropertiesToValues(cons<value> (mklist<value> (element, c_str(name), val), propertiesSoFar), o, json_object_iter_next(o, i));
}

/**
 * Converts a JS val to a value.
 */
const value jsValToValue(json_t* const jsv) {
    switch(json_typeof(jsv)) {
    case json_type::JSON_STRING: {
        const string svalue = string(json_string_value(jsv));
        return value(svalue);
    }
    case json_type::JSON_TRUE: {
        return value((bool)true);
    }
    case json_type::JSON_FALSE: {
        return value((bool)false);
    }
    case json_type::JSON_REAL: {
        return value((double)json_real_value(jsv));
    }
    case json_type::JSON_INTEGER: {
        return value((double)json_integer_value(jsv));
    }
    case json_type::JSON_NULL: {
        return nilValue;
    }
    case json_type::JSON_ARRAY: {
        const int i = (int)json_array_size(jsv) - 1;
        const value v = jsArrayToValues(list<value> (), jsv, i);
        return v;
    }
    case json_type::JSON_OBJECT: {
        void* const i = json_object_iter(jsv);
        const value v = jsPropertiesToValues(list<value> (), jsv, i);
        return v;
    }
    default: {
        return nilValue;
    }
    }
}

/**
 * Converts a list of values to JS array elements.
 */
inline json_t* const valuesToJSElements(json_t* const a, const list<value>& l) {
    json_t* const valueToJSVal(const value& val);
    if (isNil(l))
        return a;
    json_t* const v = valueToJSVal(car(l));
    json_array_append(a, v);
    return valuesToJSElements(a, cdr(l));
}

/**
 * Converts a value to a JS val.
 */
inline json_t* const valueToJSVal(const value& val) {
    json_t* const valuesToJSProperties(json_t* const o, const list<value>& l);

    switch(type(val)) {
    case value::String: {
        return json_string(c_str((string)val));
    }
    case value::Symbol: {
        return json_string(c_str((string)val));
    }
    case value::Bool: {
        return json_boolean((bool)val);
    }
    case value::Number: {
        const double d = (double)val;
        if (((double)((json_int_t)d)) == d)
            return json_integer((json_int_t)d);
        else
            return json_real(d);
    }
    case value::List: {
        if (isJSArray(val)) {
            json_t* const a = json_array();
            return valuesToJSElements(a, val);
        }
        json_t* const c = json_object();
        return valuesToJSProperties(c, val);
    }
    case value::Nil: {
        return json_null();
    }
    default: {
        return json_null();
    }
    }
}

/**
 * Converts a list of values to JS properties.
 */
inline json_t* const valuesToJSProperties(json_t* const o, const list<value>& l) {
    if (isNil(l))
        return o;

    // Write an attribute
    const value token(car(l));

    if (isTaggedList(token, attribute)) {
        json_t* const v = valueToJSVal(attributeValue(token));
        json_object_set(o, c_str(atsign + string(attributeName(token))), v);

    } else if (isTaggedList(token, element)) {

        // Write the value of an element
        if (elementHasValue(token)) {
            json_t* const v = valueToJSVal(elementValue(token));
            json_object_set(o, c_str(string(elementName(token))), v);

        } else {

            // Write a parent element
            json_t* const c = json_object();
            json_object_set(o, c_str(string(elementName(token))), c);

            // Write its children
            valuesToJSProperties(c, elementChildren(token));
        }
    }

    // Go on
    return valuesToJSProperties(o, cdr(l));
}

/**
 * Return true if a list of strings contains a JSON document.
 */
inline const bool isJSON(const list<string>& ls) {
    if (isNil(ls))
        return false;
    const string s = substr(car(ls), 0, 1);
    return s == "[" || s == "{";
}

 /**
  * Convert a list of strings representing a JSON document to a list of elements.
  */
inline const failable<list<value> > readElements(const list<string>& ilist) {
    ostringstream os;
    write(ilist, os);
    json_error_t e;
    json_t* const val = json_loads(c_str(str(os)), 0, &e);
   if(val == NULL)
        return mkfailure<list<value> >(string("json_loads failed: ") + string(e.text));
    return list<value>(jsValToValue(val));
}
 
/**
 * Convert a list of strings representing a JSON document to a value
 */
inline const failable<value> readValue(const list<string>& ilist) {
    ostringstream os;
    write(ilist, os);
    json_error_t e;
    json_t* const jv = json_loads(c_str(str(os)), JSON_DECODE_ANY, &e);
    if(jv == NULL)
        return mkfailure<value>(string("json_loads failed: ") + string(e.text));
    const value val = jsValToValue(jv);
    if (!isList(val))
        return val;
    return (value)elementsToValues(val);
}

/**
 * Context passed to the JSON write callback function.
 */
template<typename R> class WriteContext {
public:
    inline WriteContext(const lambda<const R(const string&, const R)>& reduce, const R& accum) : reduce(reduce), accum(accum) {
    }

    const lambda<const R(const string&, const R)> reduce;
    gc_mutable_ref<R> accum;
};

/**
 * Called by dump_callback to write JSON out.
 */
template<typename R> inline int writeCallback(const char *buf, size_t len, void *data) {
    WriteContext<R>& wcx = *(WriteContext<R>*)data;
    const string str(buf, len);
    wcx.accum = wcx.reduce(str, wcx.accum);
    return 0;
}

/**
 * Convert a value to a JSON document.
 */
template<typename R> inline const failable<R> writeValue(const lambda<const R(const string&, const R)>& reduce, const R& initial, const list<value>& l) {
    json_t* const val = valueToJSVal(l);
    WriteContext<R> wcx(reduce, initial);
    if (json_dump_callback(val, writeCallback<R>, &wcx, JSON_COMPACT | JSON_ENSURE_ASCII | JSON_PRESERVE_ORDER | JSON_ENCODE_ANY) == -1)
        return mkfailure<R>("json_dump_callback failed");
    return (R)wcx.accum;
}

/**
 * Convert a list of elements to a JSON document.
 */
template<typename R> inline const failable<R> writeElements(const lambda<const R(const string&, const R)>& reduce, const R& initial, const list<value>& l) {
    json_t* const val = isJSArray(l)? valuesToJSElements(::json_array(), l) : valuesToJSProperties(::json_object(), l);

    WriteContext<R> wcx(reduce, initial);
    if (json_dump_callback(val, writeCallback<R>, &wcx, JSON_COMPACT | JSON_ENSURE_ASCII | JSON_PRESERVE_ORDER | JSON_ENCODE_ANY) == -1)
        return mkfailure<R>("json_dump_callback failed");
    return (R)wcx.accum;
}

/**
 * Convert a list of elements to a list of strings representing a JSON document.
 */
inline const failable<list<string> > writeElements(const list<value>& l) {
    const failable<list<string> > ls = writeElements<list<string>>(rcons<string>, list<string>(), valuesToElements(elementsToValues(l)));
     if (!hasContent(ls))
         return ls;
     return reverse(list<string>(content(ls)));
}
 
/**
 * Convert a value to a list of strings representing a JSON document.
 */
inline const failable<list<string> > writeValue(const value& v) {
    if (!isList(v)) {
        const failable<list<string> > ls = writeValue<list<string>>(rcons<string>, list<string>(), v);
        if (!hasContent(ls))
            return ls;
        return reverse(list<string>(content(ls)));
    }

    const failable<list<string> > ls = writeElements<list<string>>(rcons<string>, list<string>(), valuesToElements(elementsToValues(valuesToElements(v))));
    if (!hasContent(ls))
        return ls;
    return reverse(list<string>(content(ls)));
}

/**
 * Convert a list of function + params to a JSON-RPC request.
 */
inline const failable<list<string> > jsonRequest(const value& id, const value& func, const value& params) {
    const list<value> r = mklist<value>(mklist<value>("id", id), mklist<value>("method", string(func)), mklist<value>("params", params));
    return writeElements(valuesToElements(r));
}

/**
 * Convert a value to a JSON-RPC result.
 */
inline const failable<list<string> > jsonResult(const value& id, const value& val) {
    return writeElements(valuesToElements(mklist<value>(mklist<value>("id", id), mklist<value>("result", val))));
}

/**
 * Convert a JSON-RPC result to a value.
 */
inline const failable<value> jsonResultValue(const list<string>& s) {
    const failable<value> jsres = json::readValue(s);
    if (!hasContent(jsres))
        return mkfailure<value>(jsres);
    const list<value> rval(cadr<value>(content(jsres)));
    const value val = cadr(rval);
    if (isList(val) && !isJSArray(val))
        return value(mklist<value>(val));
    return val;
}

/**
 * Return a portable function name from a JSON-RPC function name.
 * Strip the ".", "system." and "Service." prefixes added by some JSON-RPC clients.
 */
inline const string funcName(const string& f) {
    if (length(f) > 1 && find(f, ".", 0) == 0)
        return c_str(f) + 1;
    if (length(f) > 7 && find(f, "system.", 0) == 0)
        return c_str(f) + 7;
    if (length(f) > 8 && find(f, "Service.", 0) == 0)
        return c_str(f) + 8;
    return f;
}

/**
 * Returns a list of param values other than the id and method args from a list
 * of key value pairs.
 */
inline const list<value> queryParams(const list<list<value> >& a) {
    if (isNil(a))
        return nilListValue;
    const list<value> p = car(a);
    if (car(p) == value("id") || car(p) == value("method"))
        return queryParams(cdr(a));
    return cons(cadr(p), queryParams(cdr(a)));
}

}
}

#endif /* tuscany_json_hpp */
