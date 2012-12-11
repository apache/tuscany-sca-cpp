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
#include "perf.hpp"
#include "driver.hpp"
#include "scheme-test.hpp"

namespace tuscany {
namespace scheme {

const bool testEnv() {
    const gc_scoped_pool pool;
    Env globalEnv = nilListValue;
    Env env = extendEnvironment(mklist<value>("a"), mklist<value>(1), globalEnv);
    defineVariable("x", env, env);
    assert(lookupVariableValue(value("x"), env) == env);
    assert(lookupVariableValue("a", env) == value(1));
    return true;
}

const bool testEnvGC() {
    resetLambdaCounters();
    resetListCounters();
    resetValueCounters();
    testEnv();
    assert(checkValueCounters());
    assert(checkLambdaCounters());
    assert(checkListCounters());
    return true;
}

const bool testRead() {
    istringstream is("abcd");
    assert(content(readValue(is)) == "abcd");

    istringstream is2("123");
    assert(content(readValue(is2)) == value(123));

    istringstream is3("(abcd)");
    assert(content(readValue(is3)) == mklist(value("abcd")));

    istringstream is4("(abcd xyz)");
    assert(content(readValue(is4)) == mklist<value>("abcd", "xyz"));

    istringstream is5("(abcd (xyz tuv))");
    assert(content(readValue(is5)) == mklist<value>("abcd", mklist<value>("xyz", "tuv")));

    return true;
}

const bool testWrite() {
    {
        const list<value> i = nilListValue
                + (nilListValue + "item" + "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"
                    + (nilListValue + "item"
                        + (nilListValue + "name" + "Apple")
                        + (nilListValue + "price" + "$2.99")))
                + (nilListValue + "item" + "cart-53d67a61-aa5e-4e5e-8401-39edeba8b83c"
                    + (nilListValue + "item"
                        + (nilListValue + "name" + "Orange")
                        + (nilListValue + "price" + "$3.55")));
        const list<value> a = cons<value>("Feed", cons<value>("feed-1234", i));
        ostringstream os;
        writeValue(a, os);
        istringstream is(str(os));
        assert(content(readValue(is)) == a);
    }
    {
        const list<value> i = mklist<value>("x", nilValue);
        const list<value> a = mklist<value>(i);
        ostringstream os;
        writeValue(a, os);
        istringstream is(str(os));
        assert(content(readValue(is)) == a);
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

const bool testEval() {
    const gc_scoped_pool pool;
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

const bool testEvalExpr() {
    const gc_scoped_pool pool;
    const value exp = mklist<value>("+", 2, 3);
    Env env = setupEnvironment();
    const value r = evalExpr(exp, env);
    assert(r == value(5));
    return true;
}

const bool testEvalRun() {
    const gc_scoped_pool pool;
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

const bool testEvalLambda() {
    const gc_scoped_pool pool;
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

const bool testEvalScript() {
    const gc_scoped_pool pool;
    Env env = setupEnvironment();
    ifstream is("test.scm");
    const value script = readScript(is);
    const value expr = mklist<value>("echo", string("x"));
    const value res = evalScript(expr, script, env);
    assert(res == string("x"));
    return true;
}

const bool testEvalGC() {
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

const string testCustomer = "((customer (@name \"jdoe\") (account (id \"1234\") (@balance 1000)) (address (@city \"san francisco\") (@state \"ca\"))))";

const bool testReadWrite() {
    const gc_scoped_pool pool;

    istringstream is(testCustomer);
    const value r = content(readValue(is));

    ostringstream os;
    writeValue(r, os);
    //assert(str(os) == testCustomer);
    return true;
}

const bool testReadWritePerf() {
    const gc_scoped_pool pool;

    const blambda rwl = blambda(testReadWrite);
    cout << "Scheme read + write test " << time(rwl, 5, 200) << " ms" << endl;

    return true;
}

const bool testReadWriteBigDoc() {
    const gc_scoped_pool pool;

    istringstream is(testBigDoc);
    const value r = content(readValue(is));

    ostringstream os;
    writeValue(r, os);
    //assert(str(os) == testBigDoc);
    return true;
}

const bool testReadWriteBigDocPerf() {
    const gc_scoped_pool pool;

    const blambda rwl = blambda(testReadWriteBigDoc);
    cout << "Scheme big doc read + write test " << time(rwl, 5, 200) << " ms" << endl;

    return true;
}

}
}

int main() {
    const tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::scheme::testEnv();
    tuscany::scheme::testEnvGC();
    tuscany::scheme::testRead();
    tuscany::scheme::testWrite();
    tuscany::scheme::testEval();
    tuscany::scheme::testEvalExpr();
    tuscany::scheme::testEvalLambda();
    tuscany::scheme::testEvalScript();
    tuscany::scheme::testEvalGC();
    tuscany::scheme::testReadWritePerf();
    tuscany::scheme::testReadWriteBigDocPerf();

    tuscany::cout << "OK" << tuscany::endl;
    return 0;
}
