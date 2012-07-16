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
 * Test script evaluator.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "driver.hpp"

namespace tuscany {
namespace scheme {

bool testEnv() {
    gc_scoped_pool pool;
    Env globalEnv = list<value>();
    Env env = extendEnvironment(mklist<value>("a"), mklist<value>(1), globalEnv);
    defineVariable("x", env, env);
    assert(lookupVariableValue(value("x"), env) == env);
    assert(lookupVariableValue("a", env) == value(1));
    return true;
}

bool testEnvGC() {
    resetLambdaCounters();
    resetListCounters();
    resetValueCounters();
    testEnv();
    assert(checkValueCounters());
    assert(checkLambdaCounters());
    assert(checkListCounters());
    return true;
}

bool testRead() {
    istringstream is("abcd");
    assert(readValue(is) == "abcd");

    istringstream is2("123");
    assert(readValue(is2) == value(123));

    istringstream is3("(abcd)");
    assert(readValue(is3) == mklist(value("abcd")));

    istringstream is4("(abcd xyz)");
    assert(readValue(is4) == mklist<value>("abcd", "xyz"));

    istringstream is5("(abcd (xyz tuv))");
    assert(readValue(is5) == mklist<value>("abcd", mklist<value>("xyz", "tuv")));

    return true;
}

bool testWrite() {
    {
        const list<value> i = list<value>()
                + (list<value>() + "item" + "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"
                    + (list<value>() + "item"
                        + (list<value>() + "name" + "Apple")
                        + (list<value>() + "price" + "$2.99")))
                + (list<value>() + "item" + "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c"
                    + (list<value>() + "item"
                        + (list<value>() + "name" + "Orange")
                        + (list<value>() + "price" + "$3.55")));
        const list<value> a = cons<value>("Feed", cons<value>("feed-1234", i));
        ostringstream os;
        writeValue(a, os);
        istringstream is(str(os));
        assert(readValue(is) == a);
    }
    {
        const list<value> i = mklist<value>("x", value());
        const list<value> a = mklist<value>(i);
        ostringstream os;
        writeValue(a, os);
        istringstream is(str(os));
        assert(readValue(is) == a);
    }
    return true;
}

const string testSchemeNumber(
  "(define (testNumber) (if (= 1 1) (display \"testNumber ok\") (error \"testNumber\"))) "
  "(testNumber)");

const string testSchemeString(
  "(define (testString) (if (= \"abc\" \"abc\") (display \"testString ok\") (error \"testString\"))) "
  "(testString)");

const string testSchemeDefinition(
  "(define a \"abc\") (define (testDefinition) (if (= a \"abc\") (display \"testDefinition ok\") (error \"testDefinition\"))) "
  "(testDefinition)");

const string testSchemeIf(
  "(define (testIf) (if (= \"abc\" \"abc\") (if (= \"xyz\" \"xyz\") (display \"testIf ok\") (error \"testNestedIf\")) (error \"testIf\"))) "
  "(testIf)");

const string testSchemeCond(
  "(define (testCond) (cond ((= \"abc\" \"abc\") (display \"testCond ok\")) (else (error \"testIf\"))))"
  "(testCond)");

const string testSchemeBegin(
  "(define (testBegin) "
    "(begin "
        "(define a \"abc\") "
        "(if (= a \"abc\") (display \"testBegin1 ok\") (error \"testBegin\")) "
        "(define x \"xyz\") "
        "(if (= x \"xyz\") (display \"testBegin2 ok\") (error \"testBegin\")) "
      ") "
  ") "
  "(testBegin)");

const string testSchemeLambda(
  "(define sqrt (lambda (x) (* x x))) "
  "(define (testLambda) (if (= 4 (sqrt 2)) (display \"testLambda ok\") (error \"testLambda\"))) "
  "(testLambda)");

const string testSchemeForward(
  "(define (testLambda) (if (= 4 (sqrt 2)) (display \"testForward ok\") (error \"testForward\"))) "
  "(define sqrt (lambda (x) (* x x))) "
  "(testLambda)");

const string evalOutput(const string& scm) {
    istringstream is(scm);
    ostringstream os;
    evalDriverRun(is, os);
    return str(os);
}

bool testEval() {
    gc_scoped_pool pool;
    assert(contains(evalOutput(testSchemeNumber), "testNumber ok"));
    assert(contains(evalOutput(testSchemeString), "testString ok"));
    assert(contains(evalOutput(testSchemeDefinition), "testDefinition ok"));
    assert(contains(evalOutput(testSchemeIf), "testIf ok"));
    assert(contains(evalOutput(testSchemeCond), "testCond ok"));
    assert(contains(evalOutput(testSchemeBegin), "testBegin1 ok"));
    assert(contains(evalOutput(testSchemeBegin), "testBegin2 ok"));
    assert(contains(evalOutput(testSchemeLambda), "testLambda ok"));
    assert(contains(evalOutput(testSchemeForward), "testForward ok"));
    return true;
}

bool testEvalExpr() {
    gc_scoped_pool pool;
    const value exp = mklist<value>("+", 2, 3);
    Env env = setupEnvironment();
    const value r = evalExpr(exp, env);
    assert(r == value(5));
    return true;
}

bool testEvalRun() {
    gc_scoped_pool pool;
    evalDriverRun(cin, cout);
    return true;
}

const value mult(const list<value>& args) {
    const double x = car(args);
    const double y = cadr(args);
    return x * y;
}

const string testReturnLambda(
  "(define (testReturnLambda) * )");

const string testCallLambda(
  "(define (testCallLambda l x y) (l x y))");

bool testEvalLambda() {
    gc_scoped_pool pool;
    Env env = setupEnvironment();

    const value trl = mklist<value>("testReturnLambda");
    istringstream trlis(testReturnLambda);
    const value trlv = evalScript(trl, trlis, env);

    istringstream tclis(testCallLambda);
    const value tcl = cons<value>("testCallLambda", quotedParameters(mklist<value>(trlv, 2, 3)));
    const value tclv = evalScript(tcl, tclis, env);
    assert(tclv == value(6));

    istringstream tcelis(testCallLambda);
    const value tcel = cons<value>("testCallLambda", quotedParameters(mklist<value>(primitiveProcedure(mult), 3, 4)));
    const value tcelv = evalScript(tcel, tcelis, env);
    assert(tcelv == value(12));
    return true;
}

bool testEvalGC() {
    resetLambdaCounters();
    resetListCounters();
    resetValueCounters();
    testEval();
    testEvalExpr();
    testEvalLambda();
    assert(checkValueCounters());
    assert(checkLambdaCounters());
    assert(checkListCounters());
    return true;
}

}
}

int main() {
    tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::scheme::testEnv();
    tuscany::scheme::testEnvGC();
    tuscany::scheme::testRead();
    tuscany::scheme::testWrite();
    tuscany::scheme::testEval();
    tuscany::scheme::testEvalExpr();
    tuscany::scheme::testEvalLambda();
    tuscany::scheme::testEvalGC();

    tuscany::cout << "OK" << tuscany::endl;
    return 0;
}
