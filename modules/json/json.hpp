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
#include <string>
#include "list.hpp"
#include "value.hpp"
#include "element.hpp"
#include "monad.hpp"

namespace tuscany {
namespace json {

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
 * Converts JS properties to values.
 */
const list<value> jsPropertiesToValues(const JSONContext& cx, const list<value>& propertiesSoFar, JSObject* o, JSObject* i) {

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
    if(JSVAL_IS_STRING(idv)) {
        const value type = isList(val)? element : element;
        return jsPropertiesToValues(cx, cons<value> (mklist<value> (type, JS_GetStringBytes(JSVAL_TO_STRING(idv)), val), propertiesSoFar), o, i);
    }
    return jsPropertiesToValues(cx, cons(val, propertiesSoFar), o, i);
}

/**
 * Converts a JS val to a value.
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
        jsdouble jsd;
        JS_ValueToNumber(cx, jsv, &jsd);
        return value((double)jsd);
    }
    case JSTYPE_OBJECT: {
        JSObject* o = JSVAL_TO_OBJECT(jsv);
        JSObject* i = JS_NewPropertyIterator(cx, o);
        if(i == NULL)
            return value(list<value> ());
        const value pv = jsPropertiesToValues(cx, list<value> (), o, i);
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
failable<bool, std::string> consume(const JSONContext& cx, JSONParser* parser, const list<std::string>& ilist) {
    if (isNil(ilist))
        return true;
    JSString* jstr = JS_NewStringCopyZ(cx, car(ilist).c_str());
    if(!JS_ConsumeJSONText(cx, parser, JS_GetStringChars(jstr), JS_GetStringLength(jstr)))
        return "JS_ConsumeJSONText failed";
    return consume(cx, parser, cdr(ilist));
}

/**
 * Convert a list of strings representing a JSON document to a list of values.
 */
const failable<list<value>, std::string> read(const JSONContext& cx, const list<std::string>& ilist) {
    jsval val;
    JSONParser* parser = JS_BeginJSONParse(cx, &val);
    if(parser == NULL)
        return std::string("JS_BeginJSONParse failed");

    const failable<bool, std::string> consumed = consume(cx, parser, ilist);

    if(!JS_FinishJSONParse(cx, parser, JSVAL_NULL))
        return std::string("JS_FinishJSONParse failed");
    if(!hasValue(consumed))
        return std::string(consumed);

    return list<value>(jsValToValue(cx, val));
}

/**
 * Converts a list of values to JS array elements.
 */
JSObject* valuesToJSElements(const JSONContext& cx, JSObject* a, const list<value>& l, int i) {
    const jsval valueToJSVal(const JSONContext& cx, const value& val);
    if (isNil(l))
        return a;
    jsval pv = valueToJSVal(cx, car(l));
    JS_SetElement(cx, a, i, &pv);
    return valuesToJSElements(cx, a, cdr(l), ++i);
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
JSObject* valuesToJSProperties(const JSONContext& cx, JSObject* o, const list<value>& l) {
    const jsval valueToJSVal(const JSONContext& cx, const value& val);
    if(isNil(l))
        return o;
    const list<value> p = car(l);
    jsval pv = valueToJSVal(cx, caddr(p));
    JS_SetProperty(cx, o, ((std::string)cadr(p)).c_str(), &pv);
    return valuesToJSProperties(cx, o, cdr(l));
}

/**
 * Converts a value to a JS val.
 */
const jsval valueToJSVal(const JSONContext& cx, const value& val) {
    switch(type(val)) {
    case value::String:
    case value::Symbol: {
        return STRING_TO_JSVAL(JS_NewStringCopyZ(cx, ((std::string)val).c_str()));
    }
    case value::Boolean: {
        return BOOLEAN_TO_JSVAL((bool)val);
    }
    case value::Number: {
        return DOUBLE_TO_JSVAL(JS_NewDouble(cx, (double)val));
    }
    case value::List: {
        if (isJSArray(val))
            return OBJECT_TO_JSVAL(valuesToJSElements(cx, JS_NewArrayObject(cx, 0, NULL), val, 0));
        return OBJECT_TO_JSVAL(valuesToJSProperties(cx, JS_NewObject(cx, NULL, NULL, NULL), val));
    }
    default: {
        return JSVAL_VOID;
    }
    }
}

const failable<bool, std::string> writeList(const JSONContext& cx, const list<value>& l, JSObject* o) {
    if (isNil(l))
        return true;

    // Write an attribute
    const value token(car(l));

    if (isTaggedList(token, attribute)) {
        jsval pv = valueToJSVal(cx, attributeValue(token));
        JS_SetProperty(cx, o, std::string(attributeName(token)).c_str(), &pv);

    } else if (isTaggedList(token, element)) {

        // Write the value of an element
        if (elementHasValue(token)) {
            jsval pv = valueToJSVal(cx, elementValue(token));
            JS_SetProperty(cx, o, std::string(elementName(token)).c_str(), &pv);

        } else {

            // Write a parent element
            JSObject* child = JS_NewObject(cx, NULL, NULL, NULL);
            jsval pv = OBJECT_TO_JSVAL(child);
            JS_SetProperty(cx, o, std::string(elementName(token)).c_str(), &pv);

            // Write its children
            const failable<bool, std::string> w = writeList(cx, elementChildren(token), child);
            if (!hasValue(w))
                return w;
        }
    }

    // Go on
    return writeList(cx, cdr(l), o);
}

/**
 * Context passed to the JSON write callback function.
 */
template<typename R> class WriteContext {
public:
    WriteContext(const JSONContext& cx, const lambda<R(R, std::string)>& reduce, const R& accum) : cx(cx), reduce(reduce), accum(accum) {
    }
    const JSONContext& cx;
    const lambda<R(R, std::string)> reduce;
    R accum;
};

/**
 * Called by JS_Stringify to write JSON out.
 */
template<typename R> JSBool writeCallback(const jschar *buf, uint32 len, void *data) {
    WriteContext<R>& wcx = *(static_cast<WriteContext<R>*> (data));
    JSString* jstr = JS_NewUCStringCopyN(wcx.cx, buf, len);
    wcx.accum = wcx.reduce(wcx.accum, std::string(JS_GetStringBytes(jstr), JS_GetStringLength(jstr)));
    return JS_TRUE;
}

/**
 * Convert a list of values to a JSON document.
 */
template<typename R> const failable<R, std::string> write(const JSONContext& cx, const lambda<R(R, std::string)>& reduce, const R& initial, const list<value>& l) {
    JSObject* o = JS_NewObject(cx, NULL, NULL, NULL);
    jsval val = OBJECT_TO_JSVAL(o);
    const failable<bool, std::string> w = writeList(cx, l, o);
    if (!hasValue(w))
        return std::string(w);

    WriteContext<R> wcx(cx, reduce, initial);
    if (!JS_Stringify(cx, &val, NULL, JSVAL_NULL, writeCallback<R>, &wcx))
        return std::string("JS_Stringify failed");
    return wcx.accum;
}

/**
 * Convert a list of values to a list of strings representing a JSON document.
 */
const list<std::string> writeStrings(const list<std::string>& listSoFar, const std::string& s) {
    return cons(s, listSoFar);
}

const failable<list<std::string>, std::string> write(const JSONContext& cx, const list<value>& l) {
    const failable<list<std::string>, std::string> ls = write<list<std::string> >(cx, writeStrings, list<std::string>(), l);
    if (!hasValue(ls))
        return ls;
    return reverse(list<std::string>(ls));
}

}
}

#endif /* tuscany_json_hpp */
