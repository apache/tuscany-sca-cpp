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

#ifndef tuscany_eval_driver_hpp
#define tuscany_eval_driver_hpp

/**
 * JSON read/write functions.
 */

#include <assert.h>
#define XP_UNIX
#include <jsapi.h>
#include <iostream>
#include <sstream>
#include <string>
#include "list.hpp"
#include "value.hpp"

namespace tuscany {

/**
 * Report JSON errors.
 */
void reportError(JSContext *cx, const char *message, JSErrorReport *report) {
    std::cerr << (const char*)(report->filename? report->filename : "<no filename>") << ":"
            << (unsigned int)report->lineno << ":" << message << std::endl;
}

/**
 * Encapsulates a JavaScript runtime. Can be shared by multiple threads in
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

    ~JSONRuntime() {
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
};

/**
 * Global JavaScript runtime instance.
 */
JSONRuntime jsRuntime;

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
 * Converts JS properties to Tuscany values.
 */
const list<value> jsPropertiesToValues(const JSONContext& cx, const list<value>& propertiesSoFar, JSObject* o,
        JSObject* i) {

    const value jsValToValue(const JSONContext& cx, const jsval& jsv);

    jsid id;
    if(!JS_NextProperty(cx, i, &id) || id == JSVAL_VOID)
        return propertiesSoFar;
    jsval jsv;
    if(!JS_GetPropertyById(cx, o, id, &jsv))
        return propertiesSoFar;
    const value val = jsValToValue(cx, jsv);
    jsval idv;
    JS_IdToValue(cx, id, &idv);
    if(JSVAL_IS_STRING(idv))
        return jsPropertiesToValues(cx, cons<value> (makeList<value> (JS_GetStringBytes(JSVAL_TO_STRING(idv)), val),
                propertiesSoFar), o, i);
    return jsPropertiesToValues(cx, cons(val, propertiesSoFar), o, i);
}

/**
 * Converts a JS value to a Tuscany value.
 */
const value jsValToValue(const JSONContext& cx, const jsval& jsv) {
    switch(JS_TypeOfValue(cx, jsv)) {
    case JSTYPE_STRING: {
        return value(std::string(JS_GetStringBytes(JSVAL_TO_STRING(jsv))));
    }
    case JSTYPE_BOOLEAN: {
        return value((bool)JSVAL_TO_BOOLEAN(jsv));
    }
    case JSTYPE_NUMBER: {
        jsdouble* jsd = JSVAL_TO_DOUBLE(jsv);
        return value((double)*jsd);
    }
    case JSTYPE_OBJECT: {
        JSObject* o = JSVAL_TO_OBJECT(jsv);
        JSObject* i = JS_NewPropertyIterator(cx, o);
        if(i == NULL)
            return value(list<value> ());
        return jsPropertiesToValues(cx, list<value> (), o, i);
    }
    default: {
        return value();
    }
    }
}

/**
 * Reads characters from a JSON input stream.
 */
JSString* readCallback(const JSONContext& cx, std::istream& is) {
    char buffer[1024];
    if(is.eof())
        return NULL;
    is.read(buffer, 1024);
    const int n = is.gcount();
    if(n <= 0)
        return NULL;
    return JS_NewStringCopyN(cx, buffer, n);
}

/**
 * Consumes a JSON document and populates a JS object from it.
 */
bool consumeJSON(const JSONContext& cx, JSONParser* parser, std::istream& is) {
    JSString* jstr = readCallback(cx, is);
    if(jstr == NULL)
        return true;
    if(!JS_ConsumeJSONText(cx, parser, JS_GetStringChars(jstr), JS_GetStringLength(jstr)))
        return false;
    return consumeJSON(cx, parser, is);
}

/**
 * Read a JSON document from an input stream.
 */
const list<value> readJSON(const JSONContext& cx, std::istream& is) {
    jsval val;
    JSONParser* parser = JS_BeginJSONParse(cx, &val);
    if(parser == NULL)
        return list<value> ();

    bool ok = consumeJSON(cx, parser, is);

    if(!JS_FinishJSONParse(cx, parser, JSVAL_NULL))
        return list<value> ();
    if(!ok)
        return list<value> ();

    return jsValToValue(cx, val);
}

/**
 * Returns true if a list represents a JS array.
 */
const bool isJSArray(const list<value>& l) {
    if(l == list<value> ())
        return false;
    value v = car(l);
    if(isList(v)) {
        const list<value> p = v;
        if(isSymbol(car(p))) {
            return false;
        }
    }
    return true;
}

/**
 * Converts a list of values to JS array elements.
 */
JSObject* valuesToJSElements(const JSONContext& cx, JSObject* a, const list<value>& l, int i) {
    const jsval valueToJSVal(const JSONContext& cx, const value& val);

    if (l == list<value>())
        return a;
    jsval pv = valueToJSVal(cx, car(l));
    JS_SetElement(cx, a, i, &pv);
    return valuesToJSElements(cx, a, cdr(l), ++i);
}

/**
 * Converts a list of values to JS properties.
 */
JSObject* valuesToJSProperties(const JSONContext& cx, JSObject* o, const list<value>& l) {
    const jsval valueToJSVal(const JSONContext& cx, const value& val);

    if(l == list<value> ())
        return o;
    const list<value> p = car(l);
    jsval pv = valueToJSVal(cx, cadr(p));
    JS_SetProperty(cx, o, ((std::string)car(p)).c_str(), &pv);
    return valuesToJSProperties(cx, o, cdr(l));
}

/**
 * Converts a Tuscany value to a JS value.
 */
const jsval valueToJSVal(const JSONContext& cx, const value& val) {
    switch(type(val)) {
    case value::String: {
        return STRING_TO_JSVAL(JS_NewStringCopyZ(cx, ((std::string)val).c_str()));
    }
    case value::Boolean: {
        return BOOLEAN_TO_JSVAL((bool)val);
    }
    case value::Number: {
        jsdouble d = (double)val;
        return DOUBLE_TO_JSVAL(&d);
    }
    case value::List: {
        if (isJSArray(val)) {
            return OBJECT_TO_JSVAL(valuesToJSElements(cx, JS_NewArrayObject(cx, 0, NULL), val, 0));
        }
        return OBJECT_TO_JSVAL(valuesToJSProperties(cx, JS_NewObject(cx, NULL, NULL, NULL), val));
    }
    default: {
        return JSVAL_VOID;
    }
    }
}

/**
 * Context passed to JSON write callback function.
 */
class JSONWriteContext {
public:
    JSONWriteContext(const JSONContext& cx, std::ostream& os) :
        os(os), cx(cx) {
    }

private:
    std::ostream& os;
    const JSONContext& cx;

    friend JSBool writeCallback(const jschar *buf, uint32 len, void *data);
};

/**
 * Called by JS_Stringify to write JSON document.
 */
JSBool writeCallback(const jschar *buf, uint32 len, void *data) {
    JSONWriteContext& cx = *(static_cast<JSONWriteContext*> (data));
    JSString* jstr = JS_NewUCStringCopyN(cx.cx, buf, len);
    cx.os.write(JS_GetStringBytes(jstr), JS_GetStringLength(jstr));
    if(cx.os.fail() || cx.os.bad())
        return JS_FALSE;
    return JS_TRUE;
}

/**
 * Write a JSON document to an output stream.
 */
const bool writeJSON(const JSONContext& cx, const list<value>& l, std::ostream& os) {
    jsval val = valueToJSVal(cx, l);
    JSONWriteContext wcx(cx, os);
    if(!JS_Stringify(cx, &val, NULL, JSVAL_NULL, writeCallback, &wcx))
        return false;
    return true;
}

}

#endif
