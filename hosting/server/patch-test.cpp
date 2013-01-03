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
 * Test patch evaluation.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "perf.hpp"
#include "../../modules/scheme/eval.hpp"

namespace tuscany {
namespace server {

const string ratingScript = 
"(define author \"admin\")\n"
"(define updated \"Jan 01, 2012\")\n"
"(define orating 1)\n"
"(define nrating 4)\n"

"(define (patch id e)\n"
    "(define (rating x e)\n"
        "(define a (tree-select-assoc (list x) e))\n"
        "(if (null? a) (list x \"0\") (car a))\n"
    ")\n"

    "(define cratings (list (rating 'rating1 e) (rating 'rating2 e) (rating 'rating3 e) (rating 'rating4 e)))\n"

    "(define (calcrating v i)\n"
        "(define nv (+ v (if (= i orating) (- 1) (if (= i nrating) 1 0))))\n"
        "(if (< nv 0) 0 nv)\n"
    ")\n"

    "(define (calcratings r i)\n"
        "(if (null? r)\n"
            "r\n"
            "(cons (list (car (car r)) (calcrating (cadr (car r)) i)) (calcratings (cdr r) (+ 1 i)))\n"
        ")\n"
    ")\n"

    "(define nratings (calcratings cratings 1))\n"

    "(define neg (+ 1 (+ (* (cadr (assoc 'rating1 nratings)) 2) (cadr (assoc 'rating2 nratings)))))\n"
    "(define pos (+ 1 (+ (* (cadr (assoc 'rating4 nratings)) 2) (cadr (assoc 'rating3 nratings)))))\n"
    "(define arating (* 4 (/ (- (/ (+ pos 1.9208) (+ pos neg)) (/ (* 1.96 (sqrt (+ (/ (* pos neg) (+ pos neg)) 0.9604))) (+ pos neg))) (+ 1 (/ 3.8416 (+ pos neg))))))\n"

    "(list (list 'entry (list 'title (car id)) (list 'id (car id)) (list 'author author) (list 'updated updated) (list 'content (cons 'ratings (cons (list 'rating arating) nratings)))))\n"
")\n";

const bool testRating() {
    const gc_scoped_pool pool;
    scheme::Env env = scheme::setupEnvironment();
    istringstream script(ratingScript);
    const list<value> v = mklist<value>(mklist<value>("entry", mklist<value>("title", string("test")), mklist<value>("id", string("test")), mklist<value>("content", mklist<value>("ratings", mklist<value>("rating", 2.5), mklist<value>("rating1", 10), mklist<value>("rating2", 20), mklist<value>("rating3", 30), mklist<value>("rating4", 40)))));
    const list<value> rv = mklist<value>(mklist<value>("entry", mklist<value>("title", string("test")), mklist<value>("id", string("test")), mklist<value>("author", string("admin")), mklist<value>("updated", string("Jan 01, 2012")), mklist<value>("content", mklist<value>("ratings", mklist<value>("rating", 2.674348916258323), mklist<value>("rating1", 9), mklist<value>("rating2", 20), mklist<value>("rating3", 30), mklist<value>("rating4", 41)))));
    const value pval = scheme::evalScript(cons<value>("patch", scheme::quotedParameters(mklist<value>(mklist<value>(string("test")), v))), script, env);
    assert(pval == rv);
    return true;
}

}
}

int main() {
    const tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::server::testRating();

    tuscany::cout << "OK" << tuscany::endl;
    return 0;
}
