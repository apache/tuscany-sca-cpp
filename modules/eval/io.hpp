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

#ifndef tuscany_eval_io_hpp
#define tuscany_eval_io_hpp

/**
 * Script evaluator IO functions.
 */

#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>

#include "list.hpp"
#include "value.hpp"
#include "primitive.hpp"

namespace tuscany {
namespace eval {

const value rightParenthesis(mklist<value>(")"));
const value leftParenthesis(mklist<value>("("));
const value comment(mklist<value>(";"));

const double stringToNumber(const std::string& str) {
    double d;
    std::istringstream is(str);
    is >> d;
    return d;
}

const bool isWhitespace(const char ch) {
    return ch != -1 && isspace(ch);
}

const bool isIdentifierStart(const char ch) {
    return ch != -1 && !isspace(ch) && !isdigit(ch);
}

const bool isIdentifierPart(const char ch) {
    return ch != -1 && !isspace(ch) && ch != '(' && ch != ')';
}

const bool isDigit(const char ch) {
    return isdigit(ch) || ch == '.';
}

const bool isLeftParenthesis(const value& token) {
    return leftParenthesis == token;
}

const bool isRightParenthesis(const value& token) {
    return rightParenthesis == token;
}

const char readChar(std::istream& in) {
    if(in.eof()) {
        return -1;
    }
    char c = (char)in.get();
    return c;
}

const char peekChar(std::istream& in) {
    if(in.eof())
        return -1;
    char c = (char)in.peek();
    return c;
}

const bool isQuote(const value& token) {
    return token == quoteSymbol;
}

const value skipComment(std::istream& in);
const value readQuoted(std::istream& in);
const value readIdentifier(const char chr, std::istream& in);
const value readString(std::istream& in);
const value readNumber(const char chr, std::istream& in);
const value readValue(std::istream& in);

const value readToken(std::istream& in) {
    const char firstChar = readChar(in);
    if(isWhitespace(firstChar))
        return readToken(in);
    if(firstChar == ';')
        return skipComment(in);
    if(firstChar == '\'')
        return readQuoted(in);
    if(firstChar == '(')
        return leftParenthesis;
    if(firstChar == ')')
        return rightParenthesis;
    if(firstChar == '"')
        return readString(in);
    if(isIdentifierStart(firstChar))
        return readIdentifier(firstChar, in);
    if(isDigit(firstChar))
        return readNumber(firstChar, in);
    if(firstChar == -1)
        return value();
    logStream() << "Illegal lexical syntax '" << firstChar << "'" << std::endl;
    return readToken(in);
}

const value skipComment(std::istream& in) {
    const char nextChar = readChar(in);
    if (nextChar == '\n')
        return readToken(in);
    return skipComment(in);
}

const value readQuoted(std::istream& in) {
    return mklist(quoteSymbol, readValue(in));
}

const list<value> readList(const list<value>& listSoFar, std::istream& in) {
    const value token = readToken(in);
    if(isNil(token) || isRightParenthesis(token))
        return reverse(listSoFar);
    if(isLeftParenthesis(token))
        return readList(cons(value(readList(list<value> (), in)), listSoFar), in);
    return readList(cons(token, listSoFar), in);
}

const std::string listToString(const list<char>& l) {
    if(isNil(l))
        return "";
    return car(l) + listToString(cdr(l));
}

const list<char> readIdentifierHelper(const list<char>& listSoFar, std::istream& in) {
    const char nextChar = peekChar(in);
    if(isIdentifierPart(nextChar))
        return readIdentifierHelper(cons(readChar(in), listSoFar), in);
    return reverse(listSoFar);
}

const value readIdentifier(const char chr, std::istream& in) {
    return listToString(readIdentifierHelper(mklist(chr), in)).c_str();
}

const list<char> readStringHelper(const list<char>& listSoFar, std::istream& in) {
    const char nextChar = readChar(in);
    if(nextChar != -1 && nextChar != '"')
        return readStringHelper(cons(nextChar, listSoFar), in);
    return reverse(listSoFar);
}

const value readString(std::istream& in) {
    return listToString(readStringHelper(list<char>(), in));
}

const list<char> readNumberHelper(const list<char>& listSoFar, std::istream& in) {
    const char nextChar = peekChar(in);
    if(isDigit(nextChar))
        return readNumberHelper(cons(readChar(in), listSoFar), in);
    return reverse(listSoFar);
}

const value readNumber(const char chr, std::istream& in) {
    return stringToNumber(listToString(readNumberHelper(mklist(chr), in)));
}

const value readValue(std::istream& in) {
    const value nextToken = readToken(in);
    if(isLeftParenthesis(nextToken))
        return readList(list<value> (), in);
    return nextToken;
}

const value readValue(const std::string s) {
    std::istringstream in(s);
    const value nextToken = readToken(in);
    if(isLeftParenthesis(nextToken))
        return readList(list<value> (), in);
    return nextToken;
}

const bool writeValue(const value& val, std::ostream& out) {
    out << val;
    return true;
}

const std::string writeValue(const value& val) {
    std::ostringstream out;
    out << val;
    return out.str();
}

const value readScript(std::istream& in) {
    const value val = readValue(in);
    if (isNil(val))
        return list<value>();
    return cons(val, (list<value>)readScript(in));
}

}
}
#endif /* tuscany_eval_io_hpp */
