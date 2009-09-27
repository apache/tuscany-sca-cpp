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
 * Script evaluator core evaluation logic.
 */

#include <string>
#include "list.hpp"
#include "value.hpp"
#include "primitive.hpp"
#include "read.hpp"
#include "environment.hpp"

namespace tuscany
{

const value compoundProcedureSymbol("compound-procedure");
const value procedureSymbol("procedure");
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

const bool isApplication(const value& exp) {
    return isList(exp);
}

const value operat(const value& exp) {
    return car((list<value> )exp);
}

const list<value> operands(const value& exp) {
    return cdr((list<value> )exp);
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

const value makeBegin(const list<value> seq) {
    return value(cons(beginSymbol, seq));
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

const lambda<value(Env&)> analyze(const value exp);

struct evalUndefinedLambda {
    evalUndefinedLambda() {
    }

    const value operator()(Env& env) const {
        return value();
    }
};

struct evalSelfEvaluatingLambda {
    const value exp;
    evalSelfEvaluatingLambda(const value& exp) : exp(exp) {
    }
    const value operator()(Env& env) const {
        return exp;
    }
};

const lambda<value(Env&)> analyzeSelfEvaluating(value exp) {
    return lambda<value(Env&)>(evalSelfEvaluatingLambda(exp));
}

struct evalQuotedLambda {
    const value qval;
    evalQuotedLambda(const value& qval) : qval(qval) {
    }
    const value operator()(Env& env) const {
        return qval;
    }
};

const lambda<value(Env&)> analyzeQuoted(const value& exp) {
    return lambda<value(Env&)>(evalQuotedLambda(textOfQuotation(exp)));
}

struct evalVariableLambda {
    const value var;
    evalVariableLambda(const value& var) : var(var) {
    }
    const value operator()(Env& env) const {
        return lookupVariableValue(var, env);
    }
};

const lambda<value(Env&)> analyzeVariable(const value& exp) {
    return lambda<value(Env&)>(evalVariableLambda(exp));
}

struct evalDefinitionLambda {
    const value var;
    const lambda<value(Env&)> vproc;
    evalDefinitionLambda(const value& var, const lambda<value(Env&)>& vproc) : var(var), vproc(vproc) {
    }
    const value operator()(Env& env) const {
        env = defineVariable(var, vproc(env), env);
        return var;
    }
};

const lambda<value(Env&)> analyzeDefinition(const value& exp) {
    return lambda<value(Env&)>(evalDefinitionLambda(definitionVariable(exp), analyze(definitionValue(exp))));
}

struct evalIfLambda {
    const lambda<value(Env&)> pproc;
    const lambda<value(Env&)> cproc;
    const lambda<value(Env&)> aproc;
    evalIfLambda(const lambda<value(Env&)> pproc, const lambda<value(Env&)>& cproc, const lambda<value(Env&)>& aproc) :
        pproc(pproc), cproc(cproc), aproc(aproc) {
    }
    const value operator()(Env& env) const {
        if(pproc(env))
            return cproc(env);
        return aproc(env);
    }
};

const lambda<value(Env&)> analyzeIf(const value& exp) {
    const lambda<value(Env&)> pproc = analyze(ifPredicate(exp));
    const lambda<value(Env&)> cproc = analyze(ifConsequent(exp));
    const lambda<value(Env&)> aproc = analyze(ifAlternative(exp));
    return lambda<value(Env&)>(evalIfLambda(pproc, cproc, aproc));
}

struct evalSequenceLambda {
    const lambda<value(Env&)> proc1;
    const lambda<value(Env&)> proc2;
    evalSequenceLambda(const lambda<value(Env&)>& proc1, const lambda<value(Env&)>& proc2) :
        proc1(proc1), proc2(proc2) {
    }
    const value operator()(Env& env) const {
        proc1(env);
        return proc2(env);
    }
};

const lambda<value(Env&)> analyzeSequenceSequentially(const lambda<value(Env&)>& proc1, const lambda<value(Env&)>& proc2) {
    return lambda<value(Env&)>(evalSequenceLambda(proc1, proc2));
}

const lambda<value(Env&)> analyzeSequenceLoop(const lambda<value(Env&)>& firstProc, const list<lambda<value(Env&)> >& restProcs) {
    if(restProcs == list<lambda<value(Env&)> >())
        return firstProc;
    return analyzeSequenceLoop(analyzeSequenceSequentially(firstProc, car(restProcs)), cdr(restProcs));
}

const lambda<value(Env&)> analyzeSequence(const list<value>& exps) {
    lambda<lambda<value(Env&)>(value exp)> a(analyze);
    const list<lambda<value(Env&)> > procs = map(a, exps);
    if(procs == list<lambda<value(Env&)> >()) {
        std::cout << "Empty sequence" << "\n";
        return lambda<value(Env&)>(evalUndefinedLambda());
    }
    return analyzeSequenceLoop(car(procs), cdr(procs));
}

struct lambdaLambda {
    const list<value> vars;
    const lambda<value(Env&)> bproc;
    lambdaLambda(const list<value> vars, const lambda<value(Env&)>& bproc)
        : vars(vars), bproc(bproc) {
    }

    const value operator()(Env& env) const {
        return makeProcedure(vars, value(bproc), env);
    }
};

const lambda<value(Env&)> analyzeLambda(const value& exp) {
    const list<value> vars = lambdaParameters(exp);
    const lambda<value(Env&)> bproc = analyzeSequence(lambdaBody(exp));
    return lambda<value(Env&)>(lambdaLambda(vars, bproc));
}

const value executeApplication(const value& proc, const list<value>& args) {
    if(isPrimitiveProcedure(proc)) {
        list<value> ncargs = args;
        return applyPrimitiveProcedure(proc, ncargs);
    }
    if(isCompoundProcedure(proc)) {
        lambda<value(Env&) > bproc(procedureBody(proc));
        Env env = extendEnvironment(procedureParameters(proc), args, procedureEnvironment(proc));
        return bproc(env);
    }
    std::cout << "Unknown procedure type " << proc << "\n";
    return value();
}

struct evalApplicationArgLambda {
    Env& env;
    evalApplicationArgLambda(Env& env) : env(env) {
    }
    const value operator()(const lambda<value(Env&)>& aproc) const {
        return aproc(env);
    }
};

struct evalApplicationLambda {
    const lambda<value(Env&)> fproc;
    const list<lambda<value(Env&)> > aprocs;
    evalApplicationLambda(const lambda<value(Env&)>& fproc, const list<lambda<value(Env&)> >& aprocs) :
        fproc(fproc), aprocs(aprocs) {
    }
    const value operator()(Env& env) const {
        return executeApplication(fproc(env), map(lambda<value(lambda<value(Env&)>)>(evalApplicationArgLambda(env)), aprocs));
    }
};

const lambda<value(Env&)> analyzeApplication(const value& exp) {
    const lambda<value(Env&)> fproc = analyze(operat(exp));
    lambda<lambda<value(Env&)>(value exp)> a(analyze);
    const list<lambda<value(Env&)> > aprocs = map(a, operands(exp));
    return lambda<value(Env&)>(evalApplicationLambda(fproc, aprocs));
}

const lambda<value(Env&)> analyze(const value exp) {
    if(isSelfEvaluating(exp))
        return analyzeSelfEvaluating(exp);
    if(isQuoted(exp))
        return analyzeQuoted(exp);
    if(isDefinition(exp))
        return analyzeDefinition(exp);
    if(isIf(exp))
        return analyzeIf(exp);
    if(isBegin(exp))
        return analyzeSequence(beginActions(exp));
    if(isCond(exp))
        return analyze(condToIf(exp));
    if(isLambda(exp))
        return analyzeLambda(exp);
    if(isVariable(exp))
        return analyzeVariable(exp);
    if(isApplication(exp))
        return analyzeApplication(exp);
    std::cout << "Unknown expression type " << exp << "\n";
    return lambda<value(Env&)>(evalUndefinedLambda());
}

    const value eval(const value& exp, Env& env) {
    return analyze(exp)(env);
}

}
#endif /* tuscany_eval_eval_hpp */
