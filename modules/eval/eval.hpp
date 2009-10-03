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

#ifndef tuscany_eval_eval_hpp
#define tuscany_eval_eval_hpp

/**
 * Core script evaluation logic.
 */

#include <string.h>
#include "list.hpp"
#include "value.hpp"
#include "primitive.hpp"
#include "read.hpp"
#include "environment.hpp"

namespace tuscany
{

const value eval(const value& exp, Env& env);

const value compoundProcedureSymbol("compound-procedure");
const value procedureSymbol("procedure");
const value applySymbol("apply");
const value beginSymbol("begin");
const value condSymbol("cond");
const value elseSymbol("else");
const value ifSymbol("if");

const bool isBegin(const value& exp) {
    return isTaggedList(exp, beginSymbol);
}

const list<value> beginActions(const value& exp) {
    return cdr((list<value> )exp);
}

const bool isLambda(const value& exp) {
    return isTaggedList(exp, lambdaSymbol);
}

const list<value> lambdaParameters(const value& exp) {
    return car(cdr((list<value> )exp));
}

static list<value> lambdaBody(const value& exp) {
    return cdr(cdr((list<value> )exp));
}

const value makeProcedure(const list<value>& parameters, const value& body, const Env& env) {
    return value(makeList(procedureSymbol, value(parameters), body, value(env)));
}

const bool isApply(const value& exp) {
    return isTaggedList(exp, applySymbol);
}

const bool isApplication(const value& exp) {
    return isList(exp);
}

const value operat(const value& exp) {
    return car((list<value> )exp);
}

const list<value> operands(const value& exp) {
    return cdr((list<value> )exp);
}

const list<value> listOfValues(const list<value> exps, Env& env) {
    if(exps == list<value> ())
        return list<value> ();
    return cons(eval(car(exps), env), listOfValues(cdr(exps), env));
}

const value applyOperat(const value& exp) {
    return cadr((list<value> )exp);
}

const value applyOperand(const value& exp) {
    return caddr((list<value> )exp);
}

const bool isCompoundProcedure(const value& procedure) {
    return isTaggedList(procedure, procedureSymbol);
}

const list<value> procedureParameters(const value& exp) {
    return car(cdr((list<value> )exp));
}

const value procedureBody(const value& exp) {
    return car(cdr(cdr((list<value> )exp)));
}

const Env procedureEnvironment(const value& exp) {
    return (Env)car(cdr(cdr(cdr((list<value> )exp))));
}

const bool isLastExp(const list<value>& seq) {
    return cdr(seq) == list<value> ();
}

const value firstExp(const list<value>& seq) {
    return car(seq);
}

const list<value> restExp(const list<value>& seq) {
    return cdr(seq);
}

const value makeBegin(const list<value> seq) {
    return value(cons(beginSymbol, seq));
}

const value evalSequence(const list<value>& exps, Env& env) {
    if(isLastExp(exps))
        return eval(firstExp(exps), env);
    eval(firstExp(exps), env);
    return evalSequence(restExp(exps), env);
}

const value applyProcedure(const value& procedure, list<value>& arguments) {
    if(isPrimitiveProcedure(procedure))
        return applyPrimitiveProcedure(procedure, arguments);
    if(isCompoundProcedure(procedure)) {
        Env env = extendEnvironment(procedureParameters(procedure), arguments, procedureEnvironment(procedure));
        return evalSequence(procedureBody(procedure), env);
    }
    std::cout << "Unknown procedure type " << procedure << "\n";
    return value();
}

const value sequenceToExp(const list<value> exps) {
    if(exps == list<value> ())
        return value(list<value>());
    if(isLastExp(exps))
        return firstExp(exps);
    return makeBegin(exps);
}

const list<value> condClauses(const value& exp) {
    return cdr((list<value> )exp);
}

const value condPredicate(const value& clause) {
    return car((list<value> )clause);
}

const list<value> condActions(const value& clause) {
    return cdr((list<value> )clause);
}

const value ifPredicate(const value& exp) {
    return car(cdr((list<value> )exp));
}

const value ifConsequent(const value& exp) {
    return car(cdr(cdr((list<value> )exp)));
}

const value ifAlternative(const value& exp) {
    if(cdr(cdr(cdr((list<value> )exp))) != list<value> ())
        return car(cdr(cdr(cdr((list<value> )exp))));
    return value(false);
}

const bool isCond(const value& exp) {
    return isTaggedList(exp, condSymbol);
}

const bool isCondElseClause(const value& clause) {
    return condPredicate(clause) == elseSymbol;
}

const bool isIf(const value& exp) {
    return isTaggedList(exp, ifSymbol);
}

const value makeIf(value predicate, value consequent, value alternative) {
    return value(makeList(ifSymbol, predicate, consequent, alternative));
}

const value expandClauses(const list<value>& clauses) {
    if(clauses == list<value> ())
        return value(false);
    const value first = car(clauses);
    const list<value> rest = cdr(clauses);
    if(isCondElseClause(first)) {
        if(rest == list<value> ())
            return sequenceToExp(condActions(first));
        std::cout << "else clause isn't last " << clauses << "\n";
        return value();
    }
    return makeIf(condPredicate(first), sequenceToExp(condActions(first)), expandClauses(rest));
}

value condToIf(const value& exp) {
    return expandClauses(condClauses(exp));
}

value evalIf(const value& exp, Env& env) {
    if(isTrue(eval(ifPredicate(exp), env)))
        return eval(ifConsequent(exp), env);
    return eval(ifAlternative(exp), env);
}

const value evalDefinition(const value& exp, Env& env) {
    env = defineVariable(definitionVariable(exp), eval(definitionValue(exp), env), env);
    return definitionVariable(exp);
}

const value eval(const value& exp, Env& env) {
    if(isSelfEvaluating(exp))
        return exp;
    if(isQuoted(exp))
        return textOfQuotation(exp);
    if(isDefinition(exp))
        return evalDefinition(exp, env);
    if(isIf(exp))
        return evalIf(exp, env);
    if(isBegin(exp))
        return evalSequence(beginActions(exp), env);
    if(isCond(exp))
        return eval(condToIf(exp), env);
    if(isLambda(exp))
        return makeProcedure(lambdaParameters(exp), value(lambdaBody(exp)), env);
    if(isVariable(exp))
        return lookupVariableValue(exp, env);
    if(isApply(exp)) {
        list<value> applyOperandValues = eval(applyOperand(exp), env);
        return applyProcedure(eval(applyOperat(exp), env), applyOperandValues);
    }
    if(isApplication(exp)) {
        list<value> operandValues = listOfValues(operands(exp), env);
        return applyProcedure(eval(operat(exp), env), operandValues);
    }
    std::cout << "Unknown expression type " << exp << "\n";
    return value();
}

}
#endif /* tuscany_eval_eval_hpp */
