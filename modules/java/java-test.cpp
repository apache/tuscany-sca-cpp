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
 * Test Java evaluator.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "driver.hpp"

namespace tuscany {
namespace java {

bool testEvalExpr() {
    const gc_scoped_pool pool;
    const JavaRuntime javaRuntime;
    {
        const failable<JavaClass> obj = readClass(javaRuntime, ".", "test.CalcImpl");
        assert(hasContent(obj));
        const value exp = mklist<value>("mult", 2, 3);
        const failable<value> r = evalClass(javaRuntime, exp, content(obj));
        assert(hasContent(r));
        assert(content(r) == value(6));
    }
    {
        const failable<JavaClass> obj = readClass(javaRuntime, ".", "test.CalcImpl");
        assert(hasContent(obj));
        const value exp = mklist<value>("even", 2);
        const failable<value> r = evalClass(javaRuntime, exp, content(obj));
        assert(hasContent(r));
        assert(content(r) == trueValue);
    }
    {
        const failable<JavaClass> obj = readClass(javaRuntime, ".", "test.AdderImpl");
        assert(hasContent(obj));
        const value exp = mklist<value>("add", 2, 3);
        const failable<value> r = evalClass(javaRuntime, exp, content(obj));
        assert(hasContent(r));
        assert(content(r) == value(5));
    }
    {
        const failable<JavaClass> obj = readClass(javaRuntime, ".", "test.CalcImpl");
        assert(hasContent(obj));
        const value exp = mklist<value>("square", mklist<value>(1, 2, 3));
        const failable<value> r = evalClass(javaRuntime, exp, content(obj));
        assert(hasContent(r));
        assert(content(r) == mklist<value>(1, 4, 9));
    }
    return true;
}

const value add(const list<value>& args) {
    assert(car(args) == "add");
    const double x = cadr(args);
    const double y = caddr(args);
    return x + y;
}

bool testEvalLambda() {
    const gc_scoped_pool pool;
    const JavaRuntime javaRuntime;
    {
        const failable<JavaClass> obj = readClass(javaRuntime, ".", "test.CalcImpl");
        assert(hasContent(obj));
        const value tcel = mklist<value>("add", 3, 4, lvvlambda(add));
        const failable<value> r = evalClass(javaRuntime, tcel, content(obj));
        assert(hasContent(r));
        assert(content(r) == value(7));
    }
    {
        const failable<JavaClass> obj = readClass(javaRuntime, ".", "test.CalcImpl");
        assert(hasContent(obj));
        const value tcel = mklist<value>("addEval", 3, 4, lvvlambda(add));
        const failable<value> r = evalClass(javaRuntime, tcel, content(obj));
        assert(hasContent(r));
        assert(content(r) == value(7));
    }
    return true;
}

bool testClassLoader() {
    const gc_scoped_pool pool;
    const JavaRuntime javaRuntime;
    const failable<JavaClass> obj = readClass(javaRuntime, ".", "org.apache.tuscany.ClassLoader$Test");
    assert(hasContent(obj));
    const value exp = mklist<value>("testClassLoader");
    const failable<value> r = evalClass(javaRuntime, exp, content(obj));
    assert(hasContent(r));
    assert(content(r) == trueValue);
    return true;
}

bool testIterableUtil() {
    const gc_scoped_pool pool;
    const JavaRuntime javaRuntime;
    const failable<JavaClass> obj = readClass(javaRuntime, ".", "org.apache.tuscany.IterableUtil$Test");
    assert(hasContent(obj));
    const value exp = mklist<value>("testList");
    const failable<value> r = evalClass(javaRuntime, exp, content(obj));
    assert(hasContent(r));
    assert(content(r) == trueValue);
    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::java::testEvalExpr();
    tuscany::java::testEvalLambda();
    tuscany::java::testClassLoader();
    tuscany::java::testIterableUtil();

    tuscany::cout << "OK" << tuscany::endl;
    return 0;
}
