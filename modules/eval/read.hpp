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

#ifndef tuscany_eval_read_hpp
#define tuscany_eval_read_hpp

/**
 * Script evaluator read functions.
 */

#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>

#include "list.hpp"
#include "value.hpp"
#include "primitive.hpp"

namespace tuscany
{

const value rightParenthesis(makeList<value>(")"));
const value leftParenthesis(makeList<value>("("));
const value comment(makeList<value>(";"));

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
    char c = in.get();
    return c;
}

const char peekChar(std::istream& in) {
    if(in.eof())
        return -1;
    char c = in.peek();
    return c;
}

const bool isQuote(const value& token) {
    return token == quoteSymbol;
}

const value readQuoted(std::istream& in);
const value readIdentifier(const char chr, std::istream& in);
const value readString(const char chr, std::istream& in);
const value readNumber(const char chr, std::istream& in);
const value read(std::istream& in);

const value readToken(std::istream& in) {
    const char firstChar = readChar(in);
    if(isWhitespace(firstChar))
        return readToken(in);
    if(firstChar == '\'')
        return readQuoted(in);
    if(firstChar == '(')
        return leftParenthesis;
    if(firstChar == ')')
        return rightParenthesis;
    if(firstChar == '"')
        return readString(firstChar, in);
    if(isIdentifierStart(firstChar))
        return readIdentifier(firstChar, in);
    if(isDigit(firstChar))
        return readNumber(firstChar, in);
    if(firstChar == -1)
        return value();
    std::cout << "Illegal lexical syntax '" << firstChar << "'" << "\n";
    return readToken(in);
}

const value readQuoted(std::istream& in) {
    return makeList(quoteSymbol, read(in));
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
    return listToString(readIdentifierHelper(makeList(chr), in)).c_str();
}

const list<char> readStringHelper(const list<char>& listSoFar, std::istream& in) {
    const char nextChar = readChar(in);
    if(nextChar != -1 && nextChar != '"')
        return readStringHelper(cons(nextChar, listSoFar), in);
    return reverse(listSoFar);
}

const value readString(const char chr, std::istream& in) {
    return listToString(readStringHelper(list<char>(), in));
}

const list<char> readNumberHelper(const list<char>& listSoFar, std::istream& in) {
    const char nextChar = peekChar(in);
    if(isDigit(nextChar))
        return readNumberHelper(cons(readChar(in), listSoFar), in);
    return reverse(listSoFar);
}

const value readNumber(const char chr, std::istream& in) {
    return stringToNumber(listToString(readNumberHelper(makeList(chr), in)));
}

const value read(std::istream& in) {
    const value nextToken = readToken(in);
    if(isLeftParenthesis(nextToken))
        return readList(list<value> (), in);
    return nextToken;
}

}
#endif /* tuscany_eval_read_hpp */
