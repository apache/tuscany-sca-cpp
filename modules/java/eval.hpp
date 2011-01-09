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

#ifndef tuscany_java_eval_hpp
#define tuscany_java_eval_hpp

/**
 * Java component implementation evaluation logic.
 */
#include <jni.h>

#include "list.hpp"
#include "value.hpp"

namespace tuscany {
namespace java {

/**
 * Handle differences between various JNI APIs.
 */
#ifdef JAVA_HARMONY_VM
#define JNI_VERSION JNI_VERSION_1_4
#else
#define JNI_VERSION JNI_VERSION_1_6
#endif

/**
 * Represent a Java VM runtime.
 */
jobject JNICALL nativeInvoke(JNIEnv *env, jobject self, jobject proxy, jobject method, jobjectArray args);
jobject JNICALL nativeUUID(JNIEnv *env);

class JavaRuntime {
public:
    JavaRuntime() {
        debug("java::javaruntime");

        // Get existing JVM
        jsize nvms = 0;
        JNI_GetCreatedJavaVMs(&jvm, 1, &nvms);
        if (nvms == 0) {

            // Create a new JVM
            JavaVMInitArgs args;
            args.version = JNI_VERSION;
            args.ignoreUnrecognized = JNI_FALSE;
            JavaVMOption options[3];
            args.options = options;
            args.nOptions = 0;

            // Configure classpath
            const char* envcp = getenv("CLASSPATH");
            const string cp = string("-Djava.class.path=") + (envcp == NULL? "." : envcp);
            options[args.nOptions].optionString = const_cast<char*>(c_str(cp));
            options[args.nOptions++].extraInfo = NULL;

#ifdef WANT_MAINTAINER_MODE
            // Enable assertions
            options[args.nOptions++].optionString = const_cast<char*>("-ea");
#endif

            // Configure Java debugging
            const char* jpdaopts = getenv("JPDA_OPTS");
            if (jpdaopts != NULL) {
                options[args.nOptions].optionString = const_cast<char*>(jpdaopts);
                options[args.nOptions++].extraInfo = NULL;
            } else {
                const char* jpdaaddr = getenv("JPDA_ADDRESS");
                if (jpdaaddr != NULL) {
                    const string jpda = string("-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=") + jpdaaddr;
                    options[args.nOptions].optionString = const_cast<char*>(c_str(jpda));
                    options[args.nOptions++].extraInfo = NULL;
                }
            }

            // Create the JVM
#ifdef JAVA_HARMONY_VM
            JNI_CreateJavaVM(&jvm, &env, &args);
#else
            JNI_CreateJavaVM(&jvm, (void**)&env, &args);
#endif

        } else {

            // Just point to existing JVM
            jvm->GetEnv((void**)&env, JNI_VERSION);
        }

        // Lookup System classes and methods
        classClass = env->FindClass("java/lang/Class");
        methodClass = env->FindClass("java/lang/reflect/Method");
        objectClass = env->FindClass("java/lang/Object");
        doubleClass = env->FindClass("java/lang/Double");
        booleanClass = env->FindClass("java/lang/Boolean");
        stringClass = env->FindClass("java/lang/String");
        objectArrayClass = env->FindClass("[Ljava/lang/Object;");
        iterableClass = env->FindClass("java/lang/Iterable");
        classForName = env->GetStaticMethodID(classClass, "forName", "(Ljava/lang/String;ZLjava/lang/ClassLoader;)Ljava/lang/Class;");
        doubleValueOf = env->GetStaticMethodID(doubleClass, "valueOf", "(D)Ljava/lang/Double;");
        doubleValue = env->GetMethodID(doubleClass, "doubleValue", "()D");
        booleanValueOf = env->GetStaticMethodID(booleanClass, "valueOf", "(Z)Ljava/lang/Boolean;");
        booleanValue = env->GetMethodID(booleanClass, "booleanValue", "()Z");
        declaredMethods = env->GetMethodID(classClass, "getDeclaredMethods", "()[Ljava/lang/reflect/Method;");
        methodName = env->GetMethodID(methodClass, "getName", "()Ljava/lang/String;");
        parameterTypes = env->GetMethodID(methodClass, "getParameterTypes", "()[Ljava/lang/Class;");

        // Lookup Tuscany classes and methods
        loaderClass = env->FindClass("org/apache/tuscany/ClassLoader");
        loaderValueOf = env->GetStaticMethodID(loaderClass, "valueOf", "(Ljava/lang/String;)Ljava/lang/ClassLoader;");
        loaderForName = env->GetStaticMethodID(loaderClass, "forName", "(Ljava/lang/String;ZLjava/lang/ClassLoader;)Ljava/lang/Class;");
        invokerClass = env->FindClass("org/apache/tuscany/InvocationHandler");
        invokerValueOf = env->GetStaticMethodID(invokerClass, "valueOf", "(Ljava/lang/Class;J)Ljava/lang/Object;");
        invokerStackTrace = env->GetStaticMethodID(invokerClass, "stackTrace", "(Ljava/lang/Throwable;)Ljava/lang/String;");
        invokerLambda = env->GetFieldID(invokerClass, "lambda", "J");
        iterableUtilClass = env->FindClass("org/apache/tuscany/IterableUtil");
        iterableValueOf = env->GetStaticMethodID(iterableUtilClass, "list", "([Ljava/lang/Object;)Ljava/lang/Iterable;");
        iterableIsNil = env->GetStaticMethodID(iterableUtilClass, "isNil", "(Ljava/lang/Object;)Z");
        iterableCar = env->GetStaticMethodID(iterableUtilClass, "car", "(Ljava/lang/Object;)Ljava/lang/Object;");
        iterableCdr = env->GetStaticMethodID(iterableUtilClass, "cdr", "(Ljava/lang/Object;)Ljava/lang/Iterable;");
        uuidClass = env->FindClass("org/apache/tuscany/UUIDUtil");

        // Register our native invocation handler function
        JNINativeMethod invokenm;
        invokenm.name = const_cast<char*>("invoke");
        invokenm.signature = const_cast<char*>("(Ljava/lang/Object;Ljava/lang/reflect/Method;[Ljava/lang/Object;)Ljava/lang/Object;");
        invokenm.fnPtr = (void*)nativeInvoke;
        env->RegisterNatives(invokerClass, &invokenm, 1);

        // Register our native UUID function
        JNINativeMethod uuidnm;
        uuidnm.name = const_cast<char*>("uuid");
        uuidnm.signature = const_cast<char*>("()Ljava/lang/String;");
        uuidnm.fnPtr = (void*)nativeUUID;
        env->RegisterNatives(uuidClass, &uuidnm, 1);
    }

    ~JavaRuntime() {
        debug("java::~javaruntime");
    }

    JavaVM* jvm;
    JNIEnv* env;

    jclass classClass;
    jclass methodClass;
    jclass objectClass;
    jclass doubleClass;
    jclass booleanClass;
    jclass stringClass;
    jclass objectArrayClass;
    jclass iterableClass;
    jmethodID doubleValueOf;
    jmethodID doubleValue;
    jmethodID booleanValueOf;
    jmethodID booleanValue;
    jmethodID declaredMethods;
    jmethodID methodName;
    jmethodID parameterTypes;
    jmethodID classForName;
    jclass loaderClass;
    jmethodID loaderValueOf;
    jmethodID loaderForName;
    jclass invokerClass;
    jmethodID invokerValueOf;
    jmethodID invokerStackTrace;
    jfieldID invokerLambda;
    jclass iterableUtilClass;
    jmethodID iterableValueOf;
    jmethodID iterableCar;
    jmethodID iterableCdr;
    jmethodID iterableIsNil;
    jclass uuidClass;
};

/**
 * Return the last exception that occurred in a JVM.
 */
string lastException(const JavaRuntime& jr) {
    if (!jr.env->ExceptionCheck())
        return "No Exception";
    const jthrowable ex = jr.env->ExceptionOccurred();
    const jstring trace = (jstring)jr.env->CallStaticObjectMethod(jr.invokerClass, jr.invokerStackTrace, ex);
    const char* c = jr.env->GetStringUTFChars(trace, NULL);
    const string msg(c);
    jr.env->ReleaseStringUTFChars(trace, c);
    jr.env->ExceptionClear();
    return msg;
}

/**
 * Declare conversion functions.
 */
const jobject valueToJobject(const JavaRuntime& jr, const value& jtype, const value& v);
const value jobjectToValue(const JavaRuntime& jr, const jobject o);
const jobjectArray valuesToJarray(const JavaRuntime& jr, const list<value>& v);
const list<value> jarrayToValues(const JavaRuntime& jr, const jobjectArray o);
const list<value> jiterableToValues(const JavaRuntime& jr, const jobject o);

/**
 * Convert a Java class name to a JNI class name.
 */
const bool jniClassNameHelper(char* to, const char* from) {
    if (*from == '\0') {
        *to = '\0';
        return true;
    }
    *to = *from == '.'? '/' : *from;
    return jniClassNameHelper(to + 1, from + 1);
}

const string jniClassName(const string& from) {
    char buf[length(from) + 1];
    jniClassNameHelper(buf, c_str(from));
    return string(buf);
}

/**
 * Create a new Java object representing a lambda expression.
 */
class javaLambda {
public:
    javaLambda(const JavaRuntime& jr, const value& iface, const lambda<value(const list<value>&)>& func) : jr(jr), iface(iface), func(func) {
    }

    const value operator()(const list<value>& expr) const {
        if (isNil(expr))
            return func(expr);
        const value& op(car(expr));
        if (op == "equals")
            return value(cadr(expr) == this);
        if (op == "hashCode")
            return value((double)(long)this);
        if (op == "toString") {
            ostringstream os;
            os << this;
            return value(string("org.apache.tuscany.InvocationHandler@") + (c_str(str(os)) + 2));
        }
        return func(expr);
    }

    const JavaRuntime& jr;
    const value iface;
    const lambda<value(const list<value>&)> func;
};

/**
 * Native implementation of the InvocationHandler.invoke Java method.
 * Dispatches the call to the lambda function wrapped in the invocation handler.
 */
jobject JNICALL nativeInvoke(JNIEnv* env, jobject self, unused jobject proxy, jobject method, jobjectArray args) {

    // Retrieve the lambda function from the invocation handler
    jclass clazz = env->GetObjectClass(self);
    jfieldID f = env->GetFieldID(clazz, "lambda", "J");
    const javaLambda& jl = *(javaLambda*)(long)env->GetLongField(self, f);

    // Retrieve the function name
    const jstring s = (jstring)env->CallObjectMethod(method, jl.jr.methodName);
    const char* c = env->GetStringUTFChars(s, NULL);
    const value func(c);
    env->ReleaseStringUTFChars(s, c);

    // Build the expression to evaluate, either (func, args[0], args[1], args[2]...)
    // or just args[0] for the special eval(...) function
    const list<value> expr = func == "eval"? (list<value>)car<value>(jarrayToValues(jl.jr, args)) : cons<value>(func, jarrayToValues(jl.jr, args));
    debug(expr, "java::nativeInvoke::expr");

    // Invoke the lambda function
    value result = jl(expr);
    debug(result, "java::nativeInvoke::result");

    // Convert result to a jobject
    return valueToJobject(jl.jr, value(), result);
}

/**
 * Native implementation of IterableUtil.uuid. We are providing a native implementation
 * of this function as java.util.UUID seems to behave differently with different JDKs.
 */
jobject JNICALL nativeUUID(JNIEnv* env) {
    const value uuid = mkuuid();
    return env->NewStringUTF(c_str(uuid));
}

/**
 * Convert a lambda function to Java proxy.
 */
const jobject mkJavaLambda(const JavaRuntime& jr, unused const value& iface, const lambda<value(const list<value>&)>& l) {
    const gc_ptr<javaLambda> jl = new (gc_new<javaLambda>()) javaLambda(jr, iface, l);
    jclass jc = (jclass)(long)(double)iface;
    const jobject obj = jr.env->CallStaticObjectMethod(jr.invokerClass, jr.invokerValueOf, jc, (long)(javaLambda*)jl);
    return obj;
}

/**
 * Convert a list of values to a Java jobjectArray.
 */
const jobjectArray valuesToJarrayHelper(const JavaRuntime& jr, jobjectArray a, const list<value>& v, const int i) {
    if (isNil(v))
        return a;
    jr.env->SetObjectArrayElement(a, i, valueToJobject(jr, value(), car(v)));
    return valuesToJarrayHelper(jr, a, cdr(v), i + 1);
}

const jobjectArray valuesToJarray(const JavaRuntime& jr, const list<value>& v) {
    jobjectArray a = jr.env->NewObjectArray((jsize)length(v), jr.objectClass, NULL);
    return valuesToJarrayHelper(jr, a, v, 0);
}

/**
 * Convert a Java jobjectArray to a Java iterable.
 */
const jobject jarrayToJiterable(const JavaRuntime& jr, jobjectArray a) {
    return jr.env->CallStaticObjectMethod(jr.iterableClass, jr.iterableValueOf, a);
}

/**
 * Convert a value to a Java jobject.
 */
const jobject valueToJobject(const JavaRuntime& jr, const value& jtype, const value& v) {
    switch (type(v)) {
    case value::List:
        return jarrayToJiterable(jr, valuesToJarray(jr, v));
    case value::Lambda:
        return mkJavaLambda(jr, jtype, v);
    case value::Symbol:
        return jr.env->NewStringUTF(c_str(string("'") + v));
    case value::String:
        return jr.env->NewStringUTF(c_str(v));
    case value::Number:
        return jr.env->CallStaticObjectMethod(jr.doubleClass, jr.doubleValueOf, (double)v);
    case value::Bool:
        return jr.env->CallStaticObjectMethod(jr.booleanClass, jr.booleanValueOf, (bool)v);
    default:
        return NULL;
    }
}

/**
 * Convert a list of values to an array of jvalues.
 */
const jvalue* valuesToJvaluesHelper(const JavaRuntime& jr, jvalue* a, const list<value>& types, const list<value>& v) {
    if (isNil(v))
        return a;
    a->l = valueToJobject(jr, car(types), car(v));
    return valuesToJvaluesHelper(jr, a + 1, cdr(types), cdr(v));
}

const jvalue* valuesToJvalues(const JavaRuntime& jr, const list<value>& types, const list<value>& v) {
    const size_t n = length(v);
    jvalue* a = new (gc_anew<jvalue>(n)) jvalue[n];
    valuesToJvaluesHelper(jr, a, types, v);
    return a;
}

/**
 * Convert a Java jobjectArray to a list of values.
 */
const list<value> jarrayToValuesHelper(const JavaRuntime& jr, jobjectArray a, const int i, const int size) {
    if (i == size)
        return list<value>();
    return cons(jobjectToValue(jr, jr.env->GetObjectArrayElement(a, i)), jarrayToValuesHelper(jr, a, i + 1, size));
}

const list<value> jarrayToValues(const JavaRuntime& jr, jobjectArray o) {
    if (o == NULL)
        return list<value>();
    return jarrayToValuesHelper(jr, o, 0, jr.env->GetArrayLength(o));
}

/**
 * Convert a Java Iterable to a list of values.
 */
const list<value> jiterableToValuesHelper(const JavaRuntime& jr, jobject o) {
    if ((bool)jr.env->CallStaticBooleanMethod(jr.iterableUtilClass, jr.iterableIsNil, o))
        return list<value>();
    jobject car = jr.env->CallStaticObjectMethod(jr.iterableUtilClass, jr.iterableCar, o);
    jobject cdr = jr.env->CallStaticObjectMethod(jr.iterableUtilClass, jr.iterableCdr, o);
    return cons(jobjectToValue(jr, car), jiterableToValuesHelper(jr, cdr));
}

const list<value> jiterableToValues(const JavaRuntime& jr, jobject o) {
    if (o == NULL)
        return list<value>();
    return jiterableToValuesHelper(jr, o);
}

/**
 * Lambda function used to represent a Java callable object.
 */
struct javaCallable {
    const JavaRuntime& jr;
    const jobject obj;

    javaCallable(const JavaRuntime& jr, const jobject obj) : jr(jr), obj(obj) {
    }

    const value operator()(const list<value>& args) const {
        jobjectArray jargs = valuesToJarray(jr, args);
        jobject result = jargs; //CallObject(func, jargs);
        return jobjectToValue(jr, result);
    }
};

/**
 * Convert a Java jobject to a value.
 */
const value jobjectToValue(const JavaRuntime& jr, const jobject o) {
    if (o == NULL)
        return value();
    const jclass clazz = jr.env->GetObjectClass(o);
    if ((jr.env->IsSameObject(clazz, jr.stringClass))) {
        const char* s = jr.env->GetStringUTFChars((jstring)o, NULL);
        if (*s == '\'') {
            const value v(s + 1);
            jr.env->ReleaseStringUTFChars((jstring)o, s);
            return v;
        }
        const value v = string(s);
        jr.env->ReleaseStringUTFChars((jstring)o, s);
        return v;
    }
    if (jr.env->IsSameObject(clazz, jr.booleanClass))
        return value((bool)jr.env->CallBooleanMethod(o, jr.booleanValue));
    if (jr.env->IsSameObject(clazz, jr.doubleClass))
        return value((double)jr.env->CallDoubleMethod(o, jr.doubleValue));
    if (jr.env->IsAssignableFrom(clazz, jr.iterableClass))
        return jiterableToValues(jr, o);
    if (jr.env->IsAssignableFrom(clazz, jr.objectArrayClass))
        return jarrayToValues(jr, (jobjectArray)o);
    return lambda<value(const list<value>&)>(javaCallable(jr, o));
}

/**
 * Returns a balanced tree of the methods of a class.
 */
const value parameterTypeToValue(const jobject t) {
    return value((double)(long)t);
}

const list<value> parameterTypesToValues(const JavaRuntime& jr, const jobjectArray t, const int i) {
    if (i == 0)
        return list<value>();
    return cons<value>(parameterTypeToValue(jr.env->GetObjectArrayElement(t, i - 1)), parameterTypesToValues(jr, t, i - 1));
}

const value methodToValue(const JavaRuntime& jr, const jobject m) {
    const jobject s = jr.env->CallObjectMethod(m, jr.methodName);
    const char* c = jr.env->GetStringUTFChars((jstring)s, NULL);
    const string& name = string(c);
    jr.env->ReleaseStringUTFChars((jstring)s, c);

    const jmethodID mid = jr.env->FromReflectedMethod(m);

    const jobjectArray t = (jobjectArray)jr.env->CallObjectMethod(m, jr.parameterTypes);
    const list<value> types = reverse(parameterTypesToValues(jr, t, jr.env->GetArrayLength(t)));

    return cons<value>(c_str(name), cons<value>((double)(long)mid, types));
}

const list<value> methodsToValues(const JavaRuntime& jr, const jobjectArray m, const int i) {
    if (i == 0)
        return list<value>();
    return cons<value>(methodToValue(jr, jr.env->GetObjectArrayElement(m, i - 1)), methodsToValues(jr, m, i - 1));
}

const list<value> methodsToValues(const JavaRuntime& jr, const jclass clazz) {
    const jobjectArray m = (jobjectArray)jr.env->CallObjectMethod(clazz, jr.declaredMethods);
    return methodsToValues(jr, m, jr.env->GetArrayLength(m));
}

/**
 * Represents a Java Class.
 */
class JavaClass {
public:
    JavaClass() : loader(NULL), clazz(NULL), obj(NULL) {
    }
    JavaClass(const jobject loader, const jclass clazz, const jobject obj, const list<value> m) : loader(loader), clazz(clazz), obj(obj), m(m) {
    }

    const jobject loader;
    const jclass clazz;
    const jobject obj;
    const list<value> m;
};

/**
 * Read a class.
 */
const failable<JavaClass> readClass(const JavaRuntime& jr, const string& path, const string& name) {

    // Create a class loader from the given path
    const jobject jpath = jr.env->NewStringUTF(c_str(path));
    jobject loader = jr.env->CallStaticObjectMethod(jr.loaderClass, jr.loaderValueOf, jpath);

    // Load the class
    const jobject jname = jr.env->NewStringUTF(c_str(name));
    const jclass clazz = (jclass)jr.env->CallStaticObjectMethod(jr.loaderClass, jr.loaderForName, jname, JNI_TRUE, loader);
    if (clazz == NULL)
        return mkfailure<JavaClass>(string("Couldn't load class: ") + name + " : " + lastException(jr));

    // Create an instance
    const jmethodID constr = jr.env->GetMethodID(clazz, "<init>", "()V");
    if (constr == NULL)
        return mkfailure<JavaClass>(string("Couldn't find constructor: ") + name + " : " + lastException(jr));
    const jobject obj = jr.env->NewObject(clazz, constr);
    if (obj == NULL)
        return mkfailure<JavaClass>(string("Couldn't construct object: ") + name + " : " + lastException(jr));

    return JavaClass(loader, clazz, obj, methodsToValues(jr, clazz));
}

/**
 * Evaluate an expression against a Java class.
 */
const failable<value> evalClass(const JavaRuntime& jr, const value& expr, const JavaClass jc) {
    debug(expr, "java::evalClass::expr");

    // Lookup the Java function named as the expression operand
    const list<value> func = assoc<value>(car<value>(expr), jc.m);
    if (isNil(func)) {

        // The start, stop, and restart functions are optional
        const value fn = car<value>(expr);
        if (fn == "start" || fn == "stop")
            return value(lambda<value(const list<value>&)>());

        return mkfailure<value>(string("Couldn't find function: ") + car<value>(expr) + " : " + lastException(jr));
    }
    const jmethodID fid = (jmethodID)(long)(double)cadr(func);

    // Convert args to Java jvalues
    const jvalue* args = valuesToJvalues(jr, cddr(func), cdr<value>(expr));

    // Call the Java function
    const jobject result = jr.env->CallObjectMethodA(jc.obj, fid, const_cast<jvalue*>(args));
    if (result == NULL)
        return mkfailure<value>(string("Function call failed: ") + car<value>(expr) + " : " + lastException(jr));

    // Convert Java result to a value
    const value v = jobjectToValue(jr, result);
    debug(v, "java::evalClass::result");
    return v;
}

}
}
#endif /* tuscany_java_eval_hpp */
