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

/**
 * Basic JNI test.
 */

#include <assert.h>
#include <jni.h>
#include "stream.hpp"
#include "string.hpp"
#include "fstream.hpp"
#include "eval.hpp"

namespace tuscany {
namespace java {

#ifdef JAVA_HARMONY_VM
#define JNI_VERSION JNI_VERSION_1_4
#else
#define JNI_VERSION JNI_VERSION_1_6
#endif

bool testJNI() {
    gc_scoped_pool pool;
    JavaVM* jvm;
    JNIEnv* env;

    JavaVMInitArgs args;
    args.version = JNI_VERSION;
    args.ignoreUnrecognized = JNI_FALSE;
    JavaVMOption options[3];
    args.options = options;
    args.nOptions = 0;
    const char* envcp = getenv("CLASSPATH");
    const string cp = string("-Djava.class.path=") + (envcp == NULL? "." : envcp);
    options[args.nOptions].optionString = const_cast<char*>(c_str(cp));
    options[args.nOptions++].extraInfo = NULL;
#ifdef JAVA_HARMONY_VM
    JNI_CreateJavaVM(&jvm, &env, &args);
#else
    JNI_CreateJavaVM(&jvm, (void**)&env, &args);
#endif

    jclass classClass = env->FindClass("java/lang/Class");
    assert(classClass != NULL);
    jclass loaderClass = env->FindClass("org/apache/tuscany/ClassLoader");
    assert(loaderClass != NULL);
    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::java::testJNI();

    tuscany::cout << "OK" << tuscany::endl;
    return 0;
}
