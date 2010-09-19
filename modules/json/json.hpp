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

#define XP_UNIX
#include <jsapi.h>
#include "string.hpp"
#include "list.hpp"
#include "value.hpp"
#include "element.hpp"
#include "monad.hpp"

namespace tuscany {
namespace json {

/**
 * Report JSON errors.
 */
void reportError(unused JSContext *cx, const char *message, JSErrorReport *report) {
#ifdef WANT_MAINTAINER_MODE
    cdebug << (const char*)(report->filename? report->filename : "<no filename>") << ":"
            << (int)report->lineno << ":" << message << endl;
#endif
}

/**
 * Encapsulates a JavaScript runtime. Shared by multiple threads in
 * a process.
 */
class JSONRuntime {
public:
    JSONRuntime() {
        // Create JS runtime
        rt = JS_NewRuntime(8L * 1024L * 1024L);
        if(rt == NULL)
            cleanup();
    }

    operator JSRuntime*() const {
        return rt;
    }
private:
    bool cleanup() {
        if(rt != NULL) {
            JS_DestroyRuntime(rt);
            rt = NULL;
        }
        JS_ShutDown();
        return true;
    }

    JSRuntime* rt;
} jsRuntime;

JSClass jsGlobalClass = { "global", JSCLASS_GLOBAL_FLAGS, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
        JS_PropertyStub, JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub, JSCLASS_NO_OPTIONAL_MEMBERS};

/**
 * Represents a JavaScript context. Create one per thread.
 */
class JSONContext {
public:
    JSONContext() {
        // Create JS context
        cx = JS_NewContext(jsRuntime, 8192);
        if(cx == NULL)
            return;
        JS_SetOptions(cx, JSOPTION_VAROBJFIX);
        JS_SetVersion(cx, JSVERSION_DEFAULT);
        JS_SetErrorReporter(cx, reportError);

        // Create global JS object
        global = JS_NewObject(cx, &jsGlobalClass, NULL, NULL);
        if(global == NULL) {
            cleanup();
            return;
        }

        // Populate global object with the standard globals, like Object and Array
        if(!JS_InitStandardClasses(cx, global)) {
            cleanup();
            return;
        }
    }

    ~JSONContext() {
        cleanup();
    }

    operator JSContext*() const {
        return cx;
    }

    JSObject* getGlobal() const {
        return global;
    }

private:
    bool cleanup() {
        if(cx != NULL) {
            JS_DestroyContext(cx);
            cx = NULL;
        }
        return true;
    }

    JSContext* cx;
    JSObject* global;
};

/**
 * Returns true if a list represents a JS array.
 */
const bool isJSArray(const list<value>& l) {
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
 * Converts JS properties to values.
 */
const list<value> jsPropertiesToValues(const list<value>& propertiesSoFar, JSObject* o, JSObject* i, const JSONContext& cx) {

    const value jsValToValue(const jsval& jsv, const JSONContext& cx);

    jsid id;
    if(!JS_NextProperty(cx, i, &id) || id == JSVAL_VOID)
        return propertiesSoFar;
    jsval jsv;
    if(!JS_GetPropertyById(cx, o, id, &jsv))
        return propertiesSoFar;
    const value val = jsValToValue(jsv, cx);

    jsval idv;
    JS_IdToValue(cx, id, &idv);
    if(JSVAL_IS_STRING(idv)) {
        if (isNil(val) && !isList(val))
            return jsPropertiesToValues(propertiesSoFar, o, i, cx);
        const string name = JS_GetStringBytes(JSVAL_TO_STRING(idv));
        if (substr(name, 0, 1) == atsign)
            return jsPropertiesToValues(cons<value>(mklist<value>(attribute, c_str(substr(name, 1)), val), propertiesSoFar), o, i, cx);
        if (isList(val) && !isJSArray(val))
            return jsPropertiesToValues(cons<value>(cons<value>(element, cons<value>(c_str(name), list<value>(val))), propertiesSoFar), o, i, cx);
        return jsPropertiesToValues(cons<value> (mklist<value> (element, c_str(name), val), propertiesSoFar), o, i, cx);
    }
    return jsPropertiesToValues(cons(val, propertiesSoFar), o, i, cx);
}

/**
 * Converts a JS val to a value.
 */
const value jsValToValue(const jsval& jsv, const JSONContext& cx) {
    switch(JS_TypeOfValue(cx, jsv)) {
    case JSTYPE_STRING: {
        return value(string(JS_GetStringBytes(JSVAL_TO_STRING(jsv))));
    }
    case JSTYPE_BOOLEAN: {
        return value((bool)JSVAL_TO_BOOLEAN(jsv));
    }
    case JSTYPE_NUMBER: {
        jsdouble jsd;
        JS_ValueToNumber(cx, jsv, &jsd);
        return value((double)jsd);
    }
    case JSTYPE_OBJECT: {
        JSObject* o = JSVAL_TO_OBJECT(jsv);
        if (o == NULL)
            return value();
        JSObject* i = JS_NewPropertyIterator(cx, o);
        if(i == NULL)
            return value(list<value> ());
        const value pv = jsPropertiesToValues(list<value> (), o, i, cx);
        return pv;
    }
    default: {
        return value();
    }
    }
}

/**
 * Consumes JSON strings and populates a JS object.
 */
failable<bool> consume(JSONParser* parser, const list<string>& ilist, const JSONContext& cx) {
    if (isNil(ilist))
        return true;
    JSString* jstr = JS_NewStringCopyZ(cx, c_str(car(ilist)));
    if(!JS_ConsumeJSONText(cx, parser, JS_GetStringChars(jstr), JS_GetStringLength(jstr)))
        return mkfailure<bool>("JS_ConsumeJSONText failed");
    return consume(parser, cdr(ilist), cx);
}

/**
 * Convert a list of strings representing a JSON document to a list of values.
 */
const failable<list<value> > readJSON(const list<string>& ilist, const JSONContext& cx) {
    jsval val;
    JSONParser* parser = JS_BeginJSONParse(cx, &val);
    if(parser == NULL)
        return mkfailure<list<value> >("JS_BeginJSONParse failed");

    const failable<bool> consumed = consume(parser, ilist, cx);

    if(!JS_FinishJSONParse(cx, parser, JSVAL_NULL))
        return mkfailure<list<value> >("JS_FinishJSONParse failed");
    if(!hasContent(consumed))
        return mkfailure<list<value> >(reason(consumed));

    return list<value>(jsValToValue(val, cx));
}

/**
 * Converts a list of values to JS array elements.
 */
JSObject* valuesToJSElements(JSObject* a, const list<value>& l, int i, const JSONContext& cx) {
    const jsval valueToJSVal(const value& val, const JSONContext& cx);
    if (isNil(l))
        return a;
    jsval pv = valueToJSVal(car(l), cx);
    JS_SetElement(cx, a, i, &pv);
    return valuesToJSElements(a, cdr(l), ++i, cx);
}

/**
 * Converts a value to a JS val.
 */
const jsval valueToJSVal(const value& val, const JSONContext& cx) {
    JSObject* valuesToJSProperties(JSObject* o, const list<value>& l, const JSONContext& cx);

    switch(type(val)) {
    case value::String:
    case value::Symbol: {
        return STRING_TO_JSVAL(JS_NewStringCopyZ(cx, c_str((string)val)));
    }
    case value::Bool: {
        return BOOLEAN_TO_JSVAL((bool)val);
    }
    case value::Number: {
        return DOUBLE_TO_JSVAL(JS_NewDouble(cx, (double)val));
    }
    case value::List: {
        if (isJSArray(val))
            return OBJECT_TO_JSVAL(valuesToJSElements(JS_NewArrayObject(cx, 0, NULL), val, 0, cx));
        return OBJECT_TO_JSVAL(valuesToJSProperties(JS_NewObject(cx, NULL, NULL, NULL), val, cx));
    }
    default: {
        return JSVAL_VOID;
    }
    }
}

/**
 * Converts a list of values to JS properties.
 */
JSObject* valuesToJSProperties(JSObject* o, const list<value>& l, const JSONContext& cx) {
    if (isNil(l))
        return o;

    // Write an attribute
    const value token(car(l));

    if (isTaggedList(token, attribute)) {
        jsval pv = valueToJSVal(attributeValue(token), cx);
        JS_SetProperty(cx, o, c_str(atsign + string(attributeName(token))), &pv);

    } else if (isTaggedList(token, element)) {

        // Write the value of an element
        if (elementHasValue(token)) {
            jsval pv = valueToJSVal(elementValue(token), cx);
            JS_SetProperty(cx, o, c_str(string(elementName(token))), &pv);

        } else {

            // Write a parent element
            JSObject* child = JS_NewObject(cx, NULL, NULL, NULL);
            jsval pv = OBJECT_TO_JSVAL(child);
            JS_SetProperty(cx, o, c_str(string(elementName(token))), &pv);

            // Write its children
            valuesToJSProperties(child, elementChildren(token), cx);
        }
    }

    // Go on
    return valuesToJSProperties(o, cdr(l), cx);
}

/**
 * Context passed to the JSON write callback function.
 */
template<typename R> class WriteContext {
public:
    WriteContext(const lambda<R(const string&, const R)>& reduce, const R& accum, const JSONContext& cx) : cx(cx), reduce(reduce), accum(accum) {
    }
    const JSONContext& cx;
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
template<typename R> const failable<R> writeJSON(const lambda<R(const string&, const R)>& reduce, const R& initial, const list<value>& l, const JSONContext& cx) {
    jsval val = OBJECT_TO_JSVAL(valuesToJSProperties(JS_NewObject(cx, NULL, NULL, NULL), l, cx));

    WriteContext<R> wcx(reduce, initial, cx);
    if (!JS_Stringify(cx, &val, NULL, JSVAL_NULL, writeCallback<R>, &wcx))
        return mkfailure<R>("JS_Stringify failed");
    return wcx.accum;
}

/**
 * Convert a list of values to a list of strings representing a JSON document.
 */
const failable<list<string> > writeJSON(const list<value>& l, const JSONContext& cx) {
    const failable<list<string> > ls = writeJSON<list<string>>(rcons<string>, list<string>(), l, cx);
    if (!hasContent(ls))
        return ls;
    return reverse(list<string>(content(ls)));
}

/**
 * Convert a list of function + params to a JSON-RPC request.
 */
const failable<list<string> > jsonRequest(const value& id, const value& func, const value& params, json::JSONContext& cx) {
    const list<value> r = mklist<value>(mklist<value>("id", id), mklist<value>("method", string(func)), mklist<value>("params", params));
    return writeJSON(valuesToElements(r), cx);
}

/**
 * Convert a value to a JSON-RPC result.
 */
const failable<list<string> > jsonResult(const value& id, const value& val, JSONContext& cx) {
    return writeJSON(valuesToElements(mklist<value>(mklist<value>("id", id), mklist<value>("result", val))), cx);
}

/**
 * Convert a JSON-RPC result to a value.
 */
const failable<value> jsonResultValue(const list<string>& s, JSONContext& cx) {
    const failable<list<value> > jsres = json::readJSON(s, cx);
    if (!hasContent(jsres))
        return mkfailure<value>(reason(jsres));
    const list<value> rval(cadr<value>(elementsToValues(content(jsres))));
    const value val = cadr(rval);
    if (isList(val) && !isJSArray(val))
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
 * Strip the "system." and "Service." prefixes added by some JSON-RPC clients.
 */
const string funcName(const string& f) {
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
