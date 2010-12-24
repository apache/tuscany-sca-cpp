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
#include "../js/eval.hpp"

namespace tuscany {
namespace json {

/**
 * Return true if a list of strings contains a JSON document.
 */
const bool isJSON(const list<string>& ls) {
    if (isNil(ls))
        return false;
    const string s = substr(car(ls), 0, 1);
    return s == "[" || s == "{";
}

/**
 * Consumes JSON strings and populates a JS object.
 */
failable<bool> consume(JSONParser* parser, const list<string>& ilist, const js::JSContext& cx) {
    if (isNil(ilist))
        return true;
    JSString* jstr = JS_NewStringCopyZ(cx, c_str(car(ilist)));
    if(!JS_ConsumeJSONText(cx, parser, JS_GetStringChars(jstr), (uint32)JS_GetStringLength(jstr)))
        return mkfailure<bool>("JS_ConsumeJSONText failed");
    return consume(parser, cdr(ilist), cx);
}

/**
 * Convert a list of strings representing a JSON document to a list of values.
 */
const failable<list<value> > readJSON(const list<string>& ilist, const js::JSContext& cx) {
    jsval val;
    JSONParser* parser = JS_BeginJSONParse(cx, &val);
    if(parser == NULL)
        return mkfailure<list<value> >("JS_BeginJSONParse failed");

    const failable<bool> consumed = consume(parser, ilist, cx);

    if(!JS_FinishJSONParse(cx, parser, JSVAL_NULL))
        return mkfailure<list<value> >("JS_FinishJSONParse failed");
    if(!hasContent(consumed))
        return mkfailure<list<value> >(reason(consumed));

    return list<value>(js::jsValToValue(val, cx));
}

/**
 * Context passed to the JSON write callback function.
 */
template<typename R> class WriteContext {
public:
    WriteContext(const lambda<R(const string&, const R)>& reduce, const R& accum, const js::JSContext& cx) : cx(cx), reduce(reduce), accum(accum) {
    }
    const js::JSContext& cx;
    const lambda<R(const string&, const R)> reduce;
    R accum;
};

/**
 * Called by JS_Stringify to write JSON out.
 */
template<typename R> JSBool writeCallback(const jschar *buf, uint32 len, void *data) {
    WriteContext<R>& wcx = *(static_cast<WriteContext<R>*> (data));
    JSString* jstr = JS_NewUCStringCopyN(wcx.cx, buf, len);
    wcx.accum = wcx.reduce(string(JS_GetStringBytes(jstr), JS_GetStringLength(jstr)), wcx.accum);
    return JS_TRUE;
}

/**
 * Convert a list of values to a JSON document.
 */
template<typename R> const failable<R> writeJSON(const lambda<R(const string&, const R)>& reduce, const R& initial, const list<value>& l, const js::JSContext& cx) {
    jsval val;
    if (js::isJSArray(l))
        val = OBJECT_TO_JSVAL(valuesToJSElements(JS_NewArrayObject(cx, 0, NULL), l, 0, cx));
    else
        val = OBJECT_TO_JSVAL(valuesToJSProperties(JS_NewObject(cx, NULL, NULL, NULL), l, cx));

    WriteContext<R> wcx(reduce, initial, cx);
    if (!JS_Stringify(cx, &val, NULL, JSVAL_NULL, writeCallback<R>, &wcx))
        return mkfailure<R>("JS_Stringify failed");
    return wcx.accum;
}

/**
 * Convert a list of values to a list of strings representing a JSON document.
 */
const failable<list<string> > writeJSON(const list<value>& l, const js::JSContext& cx) {
    const failable<list<string> > ls = writeJSON<list<string>>(rcons<string>, list<string>(), l, cx);
    if (!hasContent(ls))
        return ls;
    return reverse(list<string>(content(ls)));
}

/**
 * Convert a list of function + params to a JSON-RPC request.
 */
const failable<list<string> > jsonRequest(const value& id, const value& func, const value& params, js::JSContext& cx) {
    const list<value> r = mklist<value>(mklist<value>("id", id), mklist<value>("method", string(func)), mklist<value>("params", params));
    return writeJSON(valuesToElements(r), cx);
}

/**
 * Convert a value to a JSON-RPC result.
 */
const failable<list<string> > jsonResult(const value& id, const value& val, js::JSContext& cx) {
    return writeJSON(valuesToElements(mklist<value>(mklist<value>("id", id), mklist<value>("result", val))), cx);
}

/**
 * Convert a JSON-RPC result to a value.
 */
const failable<value> jsonResultValue(const list<string>& s, js::JSContext& cx) {
    const failable<list<value> > jsres = json::readJSON(s, cx);
    if (!hasContent(jsres))
        return mkfailure<value>(reason(jsres));
    const list<value> rval(cadr<value>(elementsToValues(content(jsres))));
    const value val = cadr(rval);
    if (isList(val) && !js::isJSArray(val))
        return value(mklist<value>(val));
    return val;
}

/**
 * Convert a JSON payload to a list of values.
 */
const list<value> jsonValues(const list<value>& e) {
    return elementsToValues(e);
}

/**
 * Return a portable function name from a JSON-RPC function name.
 * Strip the ".", "system." and "Service." prefixes added by some JSON-RPC clients.
 */
const string funcName(const string& f) {
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
const list<value> queryParams(const list<list<value> >& a) {
    if (isNil(a))
        return list<value>();
    const list<value> p = car(a);
    if (car(p) == value("id") || car(p) == value("method"))
        return queryParams(cdr(a));
    return cons(cadr(p), queryParams(cdr(a)));
}

}
}

#endif /* tuscany_json_hpp */
