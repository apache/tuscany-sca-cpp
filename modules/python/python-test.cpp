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
 * Test Python script evaluator.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "driver.hpp"
#include "parallel.hpp"
#include "perf.hpp"

namespace tuscany {
namespace python {

const string testPythonAdd =
        "def add(x, y):\n"
        "    return x + y\n";

const bool testEvalExpr() {
    const gc_scoped_pool pool;
    PythonRuntime py;

    istringstream is(testPythonAdd);
    const failable<PyObject*> script = readScript("script1", "script1.py", is, py);
    assert(hasContent(script));

    const value exp = mklist<value>("add", 2, 3);
    const failable<value> r = evalScript(exp, content(script), py);
    assert(hasContent(r));
    assert(content(r) == value(5));

    releaseScript(content(script), py);
    return true;
}

const string testPythonMap =
        "def addmap(x, y):\n"
        "    return tuple(map(lambda i: i + y, x))\n";

const bool testEvalList() {
    const gc_scoped_pool pool;
    PythonRuntime py;

    istringstream is(testPythonMap);
    const failable<PyObject*> script = readScript("script2", "script2.py", is, py);
    assert(hasContent(script));

    const value exp = mklist<value>("addmap", mklist<value>(1, 2, 3), 1);
    const failable<value> r = evalScript(exp, content(script), py);
    assert(hasContent(r));
    assert(car<value>(content(r)) == value(2));
    assert(cadr<value>(content(r)) == value(3));
    assert(caddr<value>(content(r)) == value(4));

    releaseScript(content(script), py);
    return true;
}

const value mult(const list<value>& args) {
    const double x = car(args);
    const double y = cadr(args);
    return x * y;
}

const string testReturnLambda(
  "def mul(x, y):\n"
  "    return x * y\n"
  "\n"
  "def testReturnLambda():\n"
  "    return mul\n");

const string testCallLambda(
  "def testCallLambda(l, x, y):\n"
  "    return l(x, y)\n");

const bool testEvalLambda() {
    const gc_scoped_pool pool;
    PythonRuntime py;

    const value trl = mklist<value>("testReturnLambda");
    istringstream trlis(testReturnLambda);
    const failable<value> trlv = evalScript(trl, trlis, py);

    assert(hasContent(trlv));
    assert(isLambda(content(trlv)));
    const lvvlambda trll(content(trlv));
    assert(trll(mklist<value>(2, 3)) == value(6));

    istringstream tclis(testCallLambda);
    const value tcl = mklist<value>("testCallLambda", content(trlv), 2, 3);
    const failable<value> tclv = evalScript(tcl, tclis, py);
    assert(hasContent(tclv));
    assert(content(tclv) == value(6));

    istringstream tcelis(testCallLambda);
    const value tcel = mklist<value>("testCallLambda", lvvlambda(mult), 3, 4);
    const failable<value> tcelv = evalScript(tcel, tcelis, py);
    assert(hasContent(tcelv));
    assert(content(tcelv) == value(12));
    return true;
}

const bool testEvalPerf() {
    const gc_scoped_pool pool;
    PythonRuntime py;

    const blambda erl = [&py]() -> const bool {
        istringstream is(testPythonAdd);
        const failable<PyObject*> script = readScript("script3", "script3.py", is, py);
        assert(hasContent(script));

        const value exp = mklist<value>("add", 2, 3);
        const failable<value> r = evalScript(exp, content(script), py);
        assert(hasContent(r));
        assert(content(r) == value(5));

        releaseScript(content(script), py);
        return true;
    };
    cout << "Python read + eval test " << time(erl, 5, 10000) << " ms" << endl;

    istringstream is(testPythonAdd);
    const failable<PyObject*> fscript = readScript("script4", "script4.py", is, py);
    assert(hasContent(fscript));

    PyObject* const script = content(fscript);
    const blambda el = [script, &py]() -> const bool {
        const value exp = mklist<value>("add", 2, 3);
        const failable<value> r = evalScript(exp, script, py);
        assert(hasContent(r));
        assert(content(r) == value(5));
        return true;
    };
    cout << "Python eval test " << time(el, 5, 10000) << " ms" << endl;

    releaseScript(script, py);
    return true;
}

#ifdef WANT_THREADS

const list<future<bool> > submitReadEvals(worker& w, const int max, const int i, PythonRuntime& py) {
    if (i == max)
        return list<future<bool> >();
    const blambda func = [&py]() -> const bool {
        for (int i = 0; i < 100; i++) {
            istringstream is(testPythonAdd);
            const failable<PyObject*> script = readScript("script6", "script6.py", is, py);
            assert(hasContent(script));

            const value exp = mklist<value>("add", 2, 3);
            const failable<value> r = evalScript(exp, content(script), py);
            assert(hasContent(r));
            assert(content(r) == value(5));

            releaseScript(content(script), py);
        }
        return true;
    };
    return cons(submit(w, func), submitReadEvals(w, max, i + 1, py));
}

const list<future<bool> > submitEvals(worker& w, const int max, const int i, PyObject* const script, PythonRuntime& py) {
    if (i == max)
        return list<future<bool> >();
    const blambda func = [script, &py]() -> const bool {
        for (int i = 0; i < 100; i++) {
            const value exp = mklist<value>("add", 2, 3);
            const failable<value> r = evalScript(exp, script, py);
            assert(hasContent(r));
            assert(content(r) == value(5));
        }
        return true;
    };
    return cons(submit(w, func), submitEvals(w, max, i + 1, script, py));
}

const bool checkEvalResults(const list<future<bool> > r) {
    if (isNil(r))
        return true;
    assert(car(r) == true);
    return checkEvalResults(cdr(r));
}

const bool testThreads() {
    const gc_scoped_pool pool;
    PythonRuntime py;

    const int max = 100;
    worker w(max);

    const blambda elr = [&w, max, &py]() -> const bool {
        const list<future<bool> > r(submitReadEvals(w, max, 0, py));
        checkEvalResults(r);
        return true;
    };
    cout << "Python eval + read thread test " << time(elr, 1, 1) / 10000.0 << " ms" << endl;

    istringstream is(testPythonAdd);
    const failable<PyObject*> fscript = readScript("script7", "script7.py", is, py);
    assert(hasContent(fscript));

    PyObject* const script = content(fscript);
    const blambda el = [&w, max, script, &py]() -> const bool {
        const list<future<bool> > r(submitEvals(w, max, 0, script, py));
        checkEvalResults(r);
        return true;
    };
    cout << "Python eval thread test " << time(el, 1, 1) / 10000.0 << " ms" << endl;

    releaseScript(script, py);
    return true;
}

#endif

}
}

int main() {
    const tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::python::testEvalExpr();
    tuscany::python::testEvalList();
    tuscany::python::testEvalLambda();
    tuscany::python::testEvalLambda();
    tuscany::python::testEvalLambda();
    tuscany::python::testEvalPerf();
#ifdef WANT_THREADS
    tuscany::python::testThreads();
#endif

    tuscany::cout << "OK" << tuscany::endl;
    return 0;
}
