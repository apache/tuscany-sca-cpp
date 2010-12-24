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

#ifndef tuscany_scheme_io_hpp
#define tuscany_scheme_io_hpp

/**
 * Script evaluator IO functions.
 */

#include <ctype.h>
#include "stream.hpp"
#include "string.hpp"

#include "list.hpp"
#include "value.hpp"
#include "primitive.hpp"

namespace tuscany {
namespace scheme {

const value rightParenthesis(mklist<value>(")"));
const value leftParenthesis(mklist<value>("("));
const value comment(mklist<value>(";"));

const double stringToNumber(const string& str) {
    return atof(c_str(str));
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

const char readChar(istream& in) {
    if(in.eof()) {
        return -1;
    }
    char c = (char)get(in);
    return c;
}

const char peekChar(istream& in) {
    if(eof(in))
        return -1;
    char c = (char)peek(in);
    return c;
}

const bool isQuote(const value& token) {
    return token == quoteSymbol;
}

const value skipComment(istream& in);
const value readQuoted(istream& in);
const value readIdentifier(const char chr, istream& in);
const value readString(istream& in);
const value readNumber(const char chr, istream& in);
const value readValue(istream& in);

const value readToken(istream& in) {
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
    logStream() << "Illegal lexical syntax '" << firstChar << "'" << endl;
    return readToken(in);
}

const value skipComment(istream& in) {
    const char nextChar = readChar(in);
    if (nextChar == '\n')
        return readToken(in);
    return skipComment(in);
}

const value readQuoted(istream& in) {
    return mklist(quoteSymbol, readValue(in));
}

const list<value> readList(const list<value>& listSoFar, istream& in) {
    const value token = readToken(in);
    if(isNil(token) || isRightParenthesis(token))
        return reverse(listSoFar);
    if(isLeftParenthesis(token))
        return readList(cons(value(readList(list<value> (), in)), listSoFar), in);
    return readList(cons(token, listSoFar), in);
}

const string listToString(const list<char>& l) {
    if(isNil(l))
        return "";
    const char buf[1] = { car(l) };
    return string(buf, 1) + listToString(cdr(l));
}

const list<char> readIdentifierHelper(const list<char>& listSoFar, istream& in) {
    const char nextChar = peekChar(in);
    if(isIdentifierPart(nextChar))
        return readIdentifierHelper(cons(readChar(in), listSoFar), in);
    return reverse(listSoFar);
}

const value readIdentifier(const char chr, istream& in) {
    const value val = c_str(listToString(readIdentifierHelper(mklist(chr), in)));
    if (val == "false")
        return value((bool)false);
    if (val == "true")
        return value((bool)true);
    return val;
}

const list<char> readStringHelper(const list<char>& listSoFar, istream& in) {
    const char nextChar = readChar(in);
    if(nextChar != -1 && nextChar != '"')
        return readStringHelper(cons(nextChar, listSoFar), in);
    return reverse(listSoFar);
}

const value readString(istream& in) {
    return listToString(readStringHelper(list<char>(), in));
}

const list<char> readNumberHelper(const list<char>& listSoFar, istream& in) {
    const char nextChar = peekChar(in);
    if(isDigit(nextChar))
        return readNumberHelper(cons(readChar(in), listSoFar), in);
    return reverse(listSoFar);
}

const value readNumber(const char chr, istream& in) {
    return stringToNumber(listToString(readNumberHelper(mklist(chr), in)));
}

const value readValue(istream& in) {
    const value nextToken = readToken(in);
    if(isLeftParenthesis(nextToken))
        return readList(list<value> (), in);
    return nextToken;
}

const value readValue(const string s) {
    istringstream in(s);
    const value nextToken = readToken(in);
    if(isLeftParenthesis(nextToken))
        return readList(list<value> (), in);
    return nextToken;
}

const bool writeValue(const value& val, ostream& out) {
    out << val;
    return true;
}

const string writeValue(const value& val) {
    ostringstream out;
    out << val;
    return str(out);
}

const value readScript(istream& in) {
    const value val = readValue(in);
    if (isNil(val))
        return list<value>();
    return cons(val, (list<value>)readScript(in));
}

}
}
#endif /* tuscany_scheme_io_hpp */
