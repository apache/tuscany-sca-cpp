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

#ifndef tuscany_js_hpp
#define tuscany_js_hpp

/**
 * Javascript evaluation functions.
 */

#define XP_UNIX
#ifdef WANT_MAINTAINER_MODE
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#endif
#include <jsapi.h>
#ifdef WANT_MAINTAINER_MODE
#pragma GCC diagnostic warning "-Wunused-parameter"
#pragma GCC diagnostic warning "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#endif
#include "string.hpp"
#include "list.hpp"
#include "value.hpp"
#include "element.hpp"
#include "monad.hpp"
#include "parallel.hpp"

namespace tuscany {
namespace js {

/**
 * Report Javascript errors.
 */
void reportError(unused ::JSContext *cx, const char *message, JSErrorReport *report) {
#ifdef WANT_MAINTAINER_MODE
    cdebug << (const char*)(report->filename? report->filename : "<no filename>") << ":"
            << (int)report->lineno << ":" << message << endl;
#else
    cerr << (const char*)(report->filename? report->filename : "<no filename>") << ":"
            << (int)report->lineno << ":" << message << endl;
#endif
}

/**
 * Encapsulates a JavaScript runtime. Shared by multiple threads in
 * a process.
 */
class JSRuntime {
public:
    JSRuntime() {
        // Create JS runtime
        debug("js::jsruntime");
        rt = JS_NewRuntime(32L * 1024L * 1024L);
        if(rt == NULL)
            cleanup();
    }

    operator ::JSRuntime*() const {
        return rt;
    }

    ~JSRuntime() {
        debug("js::~jsruntime");
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

    ::JSRuntime* rt;
} jsRuntime;

JSClass jsGlobalClass = { "global", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub, JS_PropertyStub,
    JS_PropertyStub, JS_StrictPropertyStub,
    JS_EnumerateStub, JS_ResolveStub,
    JS_ConvertStub, JS_FinalizeStub,
    JSCLASS_NO_OPTIONAL_MEMBERS };

/**
 * Represents a JavaScript context. Maintains one context per thread.
 */
#ifdef WANT_THREADS
perthread_ptr<JSContext> jsContext;
#else
::JSContext* jsContext = NULL;
#endif

class JSContext {
public:
    JSContext() {
        // Create JS context if necessary
        debug("js::jscontext");
        if (jsContext != NULL) {
            cx = jsContext;
            return;
        }
        cx = JS_NewContext(jsRuntime, 8192);
        if(cx == NULL)
            return;
        JS_SetOptions(cx, JSOPTION_VAROBJFIX | JSOPTION_JIT | JSOPTION_METHODJIT);
        JS_SetVersion(cx, JSVERSION_LATEST);
        JS_SetErrorReporter(cx, reportError);

        // Create global JS object
        global = JS_NewCompartmentAndGlobalObject(cx, &jsGlobalClass, NULL);
        if(global == NULL) {
            cleanup();
            return;
        }

        // Populate global object with the standard globals, like Object and Array
        if(!JS_InitStandardClasses(cx, global)) {
            cleanup();
            return;
        }
        jsContext = cx;
    }

    ~JSContext() {
        debug("js::~jscontext");
        if (cx != NULL)
            JS_MaybeGC(cx);
        cleanup();
    }

    operator ::JSContext*() const {
        return cx;
    }

    JSObject* getGlobal() const {
        return global;
    }

private:
    bool cleanup() {
        if(cx != NULL) {
            if (cx != jsContext)
                JS_DestroyContext(cx);
            cx = NULL;
        }
        return true;
    }

    ::JSContext* cx;
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
const list<value> jsPropertiesToValues(const list<value>& propertiesSoFar, JSObject* o, JSObject* i, const js::JSContext& cx) {

    const value jsValToValue(const jsval& jsv, const js::JSContext& cx);

    jsid id;
    if(!JS_NextProperty(cx, i, &id))
        return propertiesSoFar;
    jsval idv;
    JS_IdToValue(cx, id, &idv);
    if (idv == JSVAL_VOID)
        return propertiesSoFar;

    jsval jsv;
    if(!JS_GetPropertyById(cx, o, id, &jsv))
        return propertiesSoFar;
    const value val = jsValToValue(jsv, cx);

    if(JSVAL_IS_STRING(idv)) {
        char* cname = JS_EncodeString(cx, JSVAL_TO_STRING(idv));
        const string name = cname;
        JS_free(cx, cname);
        if (isNil(val) && !isList(val))
            return jsPropertiesToValues(cons<value> (mklist<value> (element, c_str(name), val), propertiesSoFar), o, i, cx);
            //return jsPropertiesToValues(propertiesSoFar, o, i, cx);
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
const value jsValToValue(const jsval& jsv, const js::JSContext& cx) {
    switch(JS_TypeOfValue(cx, jsv)) {
    case JSTYPE_STRING: {
        char* cvalue = JS_EncodeString(cx, JSVAL_TO_STRING(jsv));
        const string svalue = string(cvalue);
        JS_free(cx, cvalue);
        return value(svalue);
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
 * Converts a list of values to JS array elements.
 */
JSObject* valuesToJSElements(JSObject* a, const list<value>& l, int i, const js::JSContext& cx) {
    const jsval valueToJSVal(const value& val, const js::JSContext& cx);
    if (isNil(l))
        return a;
    jsval pv = valueToJSVal(car(l), cx);
    JS_SetElement(cx, a, i, &pv);
    return valuesToJSElements(a, cdr(l), ++i, cx);
}

/**
 * Converts a value to a JS val.
 */
const jsval valueToJSVal(const value& val, const js::JSContext& cx) {
    JSObject* valuesToJSProperties(JSObject* o, const list<value>& l, const js::JSContext& cx);

    switch(type(val)) {
    case value::String: {
        return STRING_TO_JSVAL(JS_NewStringCopyZ(cx, c_str((string)val)));
    }
    case value::Symbol: {
        return STRING_TO_JSVAL(JS_NewStringCopyZ(cx, c_str((string)val)));
    }
    case value::Bool: {
        return BOOLEAN_TO_JSVAL((bool)val);
    }
    case value::Number: {
        jsval jsv;
        if (!JS_NewNumberValue(cx, (jsdouble)val, &jsv))
            return DOUBLE_TO_JSVAL(0);
        return jsv;
    }
    case value::List: {
        if (isJSArray(val))
            return OBJECT_TO_JSVAL(valuesToJSElements(JS_NewArrayObject(cx, 0, NULL), val, 0, cx));
        return OBJECT_TO_JSVAL(valuesToJSProperties(JS_NewObject(cx, NULL, NULL, NULL), val, cx));
    }
    case value::Nil: {
        return JSVAL_NULL;
    }
    default: {
        return JSVAL_VOID;
    }
    }
}

/**
 * Converts a list of values to JS properties.
 */
JSObject* valuesToJSProperties(JSObject* o, const list<value>& l, const js::JSContext& cx) {
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
 * Evaluate a script provided as a string.
 */
const failable<value> evalScript(const string& s) {
    js::JSContext cx;
    jsval rval;
    JSBool rc = JS_EvaluateScript(cx, cx.getGlobal(), c_str(s), (uintN)length(s), "eval.js", 1, &rval);
    if (rc != JS_TRUE) {
        return mkfailure<value>("Couldn't evaluate Javascript script.");
    }
    return jsValToValue(rval, cx);
}

}
}

#endif /* tuscany_js_hpp */
