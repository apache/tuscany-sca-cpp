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

const value rightParenthesis(")");
const value leftParenthesis("(");

inline const double stringToNumber(const string& str) {
    return atof(c_str(str));
}

inline const bool isWhitespace(const char ch) {
    return ch != -1 && isspace(ch);
}

inline const bool isIdentifierStart(const char ch) {
    return ch != -1 && !isspace(ch) && !isdigit(ch);
}

inline const bool isIdentifierPart(const char ch) {
    return ch != -1 && !isspace(ch) && ch != '(' && ch != ')';
}

inline const bool isDigit(const char ch) {
    return isdigit(ch) || ch == '.';
}

inline const bool isLeftParenthesis(const value& token) {
    return leftParenthesis == token;
}

inline const bool isRightParenthesis(const value& token) {
    return rightParenthesis == token;
}

inline const char readChar(istream& in) {
    if(in.eof()) {
        return -1;
    }
    char c = (char)get(in);
    return c;
}

inline const char peekChar(istream& in) {
    if(eof(in))
        return -1;
    char c = (char)peek(in);
    return c;
}

inline const bool isQuote(const value& token) {
    return token == quoteSymbol;
}

const failable<value> skipComment(istream& in);
const value readQuoted(istream& in);
const value readIdentifier(const char chr, istream& in);
const value readString(istream& in);
const value readNumber(const char chr, istream& in);
const failable<value> readValue(istream& in);

inline const failable<value> readToken(istream& in) {
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
        return mkfailure<value>();
    logStream() << "Illegal lexical syntax '" << firstChar << "'" << endl;
    return readToken(in);
}

inline const failable<value> skipComment(istream& in) {
    while(true) {
        const char nextChar = readChar(in);
        if (nextChar == '\n')
            return readToken(in);
    }
}

inline const value readQuoted(istream& in) {
    return mklist(quoteSymbol, content(readValue(in)));
}

inline const list<value> readList(const list<value>& listSoFar, istream& in) {
    const failable<value> ftoken = readToken(in);
    if (!hasContent(ftoken))
        return reverse(listSoFar);
    const value token = content(ftoken);
    if(isRightParenthesis(token))
        return reverse(listSoFar);
    if(isLeftParenthesis(token))
        return readList(cons(value(readList(list<value> (), in)), listSoFar), in);
    return readList(cons(token, listSoFar), in);
}

inline const string readIdentifierHelper(const char chr, istream& in) {
    ostringstream buf;
    buf << chr;
    while(true) {
        const char nextChar = peekChar(in);
        if(!isIdentifierPart(nextChar))
            return str(buf);
        buf << readChar(in);
    }
}

inline const value readIdentifier(const char chr, istream& in) {
    const value val = c_str(readIdentifierHelper(chr, in));
    if (val == "false")
        return value((bool)false);
    if (val == "true")
        return value((bool)true);
    if (val == "nil")
        return nilValue;
    return val;
}

inline const value readString(istream& in) {
    ostringstream buf;
    while(true) {
        const char nextChar = readChar(in);
        if(nextChar == -1 || nextChar == '"')
            return (value)str(buf);
        if (nextChar == '\\') {
            const char escapedChar = readChar(in);
            if (escapedChar == -1)
                return (value)str(buf);
            buf << escapedChar;
        }
        buf << nextChar;
    }
}

inline const value readNumber(const char chr, istream& in) {
    ostringstream buf;
    buf << chr;
    while(true) {
        const char nextChar = peekChar(in);
        if(!isDigit(nextChar))
            return stringToNumber(str(buf));
        buf << readChar(in);
    }
}

inline const failable<value> readValue(istream& in) {
    const failable<value> fnextToken = readToken(in);
    if (!hasContent(fnextToken))
        return nilValue;
    const value nextToken = content(fnextToken);
    if(isLeftParenthesis(nextToken))
        return (value)readList(nilListValue, in);
    return nextToken;
}

inline const failable<value> readValue(const string& s) {
    istringstream in(s);
    const failable<value> fnextToken = readToken(in);
    if (!hasContent(fnextToken))
        return nilValue;
    const value nextToken = content(fnextToken);
    if(isLeftParenthesis(nextToken))
        return (value)readList(nilListValue, in);
    return nextToken;
}

inline const failable<value> readValue(const list<string>& l) {
    ostringstream os;
    write(l, os);
    return readValue(str(os));
}

inline const failable<ostream&> writeValue(const value& val, ostream& out) {
    out << val;
    return out;
}

inline const failable<list<string> > writeValue(const value& val) {
    ostringstream out;
    out << val;
    return mklist<string>(str(out));
}

inline const value readScript(istream& in) {
    const value val = content(readValue(in));
    if (isNil(val))
        return nilListValue;
    return cons(val, (list<value>)readScript(in));
}

}
}
#endif /* tuscany_scheme_io_hpp */
