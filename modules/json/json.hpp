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
    cerr << (const char*)(report->filename? report->filename : "<no filename>") << ":"
            << (int)report->lineno << ":" << message << endl;
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
        const value type = isList(val)? element : element;
        return jsPropertiesToValues(cons<value> (mklist<value> (type, JS_GetStringBytes(JSVAL_TO_STRING(idv)), val), propertiesSoFar), o, i, cx);
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
 * Returns true if a list represents a JS array.
 */
const bool isJSArray(const list<value>& l) {
    if(isNil(l))
        return false;
    const value v = car(l);
    if(isList(v)) {
        const list<value> p = v;
        if(isSymbol(car(p)))
            return false;
    }
    return true;
}



/**
 * Converts a list of values to JS properties.
 */
JSObject* valuesToJSProperties(JSObject* o, const list<value>& l, const JSONContext& cx) {
    const jsval valueToJSVal(const value& val, const JSONContext& cx);
    if(isNil(l))
        return o;
    const list<value> p = car(l);
    jsval pv = valueToJSVal(caddr(p), cx);
    JS_SetProperty(cx, o, c_str((string)cadr(p)), &pv);
    return valuesToJSProperties(o, cdr(l), cx);
}

/**
 * Converts a value to a JS val.
 */
const jsval valueToJSVal(const value& val, const JSONContext& cx) {
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

const failable<bool> writeList(const list<value>& l, JSObject* o, const JSONContext& cx) {
    if (isNil(l))
        return true;

    // Write an attribute
    const value token(car(l));

    if (isTaggedList(token, attribute)) {
        jsval pv = valueToJSVal(attributeValue(token), cx);
        JS_SetProperty(cx, o, c_str(string(attributeName(token))), &pv);

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
            const failable<bool> w = writeList(elementChildren(token), child, cx);
            if (!hasContent(w))
                return w;
        }
    }

    // Go on
    return writeList(cdr(l), o, cx);
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
    JSObject* o = JS_NewObject(cx, NULL, NULL, NULL);
    jsval val = OBJECT_TO_JSVAL(o);
    const failable<bool> w = writeList(l, o, cx);
    if (!hasContent(w))
        return mkfailure<R>(reason(w));

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
 * Convert a function + params to a JSON request.
 */
const failable<list<string> > jsonRequest(const value& id, const value& func, const value& params, json::JSONContext& cx) {
    const list<value> r = mklist<value>(mklist<value>("id", id), mklist<value>("method", func), mklist<value>("params", params));
    return writeJSON(valuesToElements(r), cx);
}

/**
 * Convert a value to a JSON result.
 */
const failable<list<string> > jsonResult(const value& id, const value& val, JSONContext& cx) {
    return writeJSON(valuesToElements(mklist<value>(mklist<value>("id", id), mklist<value>("result", val))), cx);
}

}
}

#endif /* tuscany_json_hpp */
