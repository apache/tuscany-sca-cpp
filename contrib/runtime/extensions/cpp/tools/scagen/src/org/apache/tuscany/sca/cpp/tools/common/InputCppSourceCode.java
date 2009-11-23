/**
 *
 *  Licensed to the Apache Software Foundation (ASF) under one
 *  or more contributor license agreements.  See the NOTICE file
 *  distributed with this work for additional information
 *  regarding copyright ownership.  The ASF licenses this file
 *  to you under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in compliance
 *  with the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing,
 *  software distributed under the License is distributed on an
 *  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, either express or implied.  See the License for the
 *  specific language governing permissions and limitations
 *  under the License.
 */

/* @version $Rev$ $Date$ */

/*
 * Branched from the original class that was also contributed to the
 * org.apache.axis.tools.common package.
 */
package org.apache.tuscany.sca.cpp.tools.common;

import java.io.BufferedReader;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.StringTokenizer;

public class InputCppSourceCode {

    private ArrayList parts = new ArrayList();

    private String name;

    public InputCppSourceCode(BufferedReader br, String name) throws Exception {
        this.name = name;

        String s = null;
        StringBuffer buff = new StringBuffer();
        for (int i = 1;; i++) {
            try {
                s = br.readLine();
            } catch (Exception e) {
                System.err.println("Ignoring exception thrown parsing file "
                        + name + " line number " + i);
                e.printStackTrace();
                break;
            }
            if (s == null)
                break;
            buff.append(s + "\n");
        }
        String str = buff.toString();

        // TODO: When checking for rest.startsWith("struct") should
        // check the next letter after struct is not alphanumeric otherwise
        // we'll get false matches on a function called structify() for
        // instance. Also applies to enum, union, public, typedef, etc

        String rest, text = "";
        int scopedepth = 0;
        String scope = "public";
        String currentClass = null;
        String currentNamespace = null;
        for (int idx = 0; idx < str.length(); /* No idx++ */
        ) {
            rest = str.substring(idx);
            if (Character.isWhitespace(rest.charAt(0))) {
                int ridx = 0;
                while (ridx < rest.length()
                        && Character.isWhitespace(rest.charAt(ridx)))
                    ridx++;
                text = rest.substring(0, ridx);
                FilePart fp = new FilePart(text, FilePart.WHITESPACE);
                parts.add(fp);
                idx += ridx;

            } else if (rest.startsWith("/*")) {
                int ridx = rest.indexOf("*/"); // Don't use Utils here
                text = str.substring(idx, idx + ridx + 2);
                FilePart fp = new FilePart(text, FilePart.COMMENT);
                parts.add(fp);
                idx += text.length();

            } else if (rest.startsWith("//")) {
                text = str.substring(idx, idx + rest.indexOf("\n"));
                FilePart fp = new FilePart(text, FilePart.COMMENT);
                parts.add(fp);
                idx += text.length();

            } else if (rest.startsWith("#")) {
                int ridx = rest.indexOf("\n");
                char c = rest.charAt(ridx - 1);
                while (-1 != ridx && '\\' == c) {
                    String rest2 = rest.substring(ridx + 1);
                    ridx += rest2.indexOf("\n") + 1;
                    c = rest.charAt(ridx - 1);
                }
                text = str.substring(idx, idx + ridx);
                FilePart fp = new FilePart(text, FilePart.DIRECTIVE);
                parts.add(fp);
                idx += text.length();

            } else if (rest.startsWith("}")) {
                if (scopedepth <= 0) //Tuscany need to increase scopedepth for
                    // namespaces?
                    Utils.rude("Braces do not match", name, lineNo(str, idx),
                            rest.substring(0, rest.indexOf("\n")));
                else
                    scopedepth--;
                // TODO: better checking that this brace really ends the class
                if (0 == scopedepth)
                    currentClass = null;
                scope = "public";
                parts.add(new FilePart("}", FilePart.ENDSCOPE));
                idx++;

            } else if (rest.startsWith(";")) {
                parts.add(new FilePart(";", FilePart.FIELD));
                idx++;

            } else if (!Character.isLetter(rest.charAt(0))
                    && '~' != rest.charAt(0) && '_' != rest.charAt(0)) {
                Utils.rude("Lines must start with a letter ", name, lineNo(str,
                        idx), rest.substring(0, rest.indexOf("\n")));

            } else if (MacroPart.isAMacro(rest)) {
                MacroPart mp = MacroPart.create(rest);
                parts.add(mp);
                idx += mp.length();

            } else if (beginsScope(rest)) {

                //Tuscany a namespace comes in here
                scopedepth++;
                text = rest.substring(0, Utils.indexOf(rest, "{") + 1);
                FilePart fp = new FilePart(text, FilePart.BEGINSCOPE);
                parts.add(fp);
                idx += text.length();
                if (Utils.startsWith(text, "class")) {
                    // TODO: cope with comments here
                    // TODO: split out classes into a ClassPart
                    StringTokenizer st = new StringTokenizer(text,
                            Utils.whitespace + ":{");
                    st.nextToken(); // step over "class"
                    while (st.hasMoreTokens()) {
                        String word = st.nextToken();
                        if (Configuration.isAttribute(word))
                            continue;
                        currentClass = word;
                        break;
                    }
                }

                //Tuscany
                if (Utils.startsWith(text, "namespace")) {
                    // TODO: cope with comments here
                    StringTokenizer st = new StringTokenizer(text,
                            Utils.whitespace + "{");
                    st.nextToken(); // step over "namespace"
                    String word = "";
                    while (st.hasMoreTokens()) {
                        word = st.nextToken();
                        if (word.equals("{")) {
                            break;
                        }

                    }
                    
                    if(currentNamespace == null)
                    {
                        currentNamespace = word;
                    }
                    else
                    {
                        currentNamespace += "::" + word;
                    }
                    //We have not got to the class yet
                    //so will need ot deal with the namespace
                    //when we do
                }
                // Tuscany end

            } else if (isEnumOrUnion(rest)) {
                int ridx = Utils.findMatching(rest, '{', '}') + 1;
                String rest2 = rest.substring(ridx);
                ridx = idx + ridx + Utils.indexOf(rest2, ';') + 1;
                text = str.substring(idx, ridx);
                FilePart fp = new FilePart(text, FilePart.ENUM);
                parts.add(fp);
                idx += text.length();

            } else if (scopedepth > 0
                    && (rest.startsWith("public")
                            || rest.startsWith("protected") || rest
                            .startsWith("private"))) {
                int colon = rest.indexOf(":");
                if (-1 == colon)
                    Utils.rude("No colon found after public or private ", name,
                            lineNo(str, idx), rest.substring(0, rest
                                    .indexOf("\n")));
                scope = str.substring(idx, idx + colon);
                text = str.substring(idx, idx + colon + 1);
                FilePart fp = new FilePart(text, FilePart.CLASSATTRIBUTE);
                parts.add(fp);
                idx += text.length();

            } else if (Utils.startsWith(rest, "typedef")) {
                int semicolon = Utils.indexOf(rest, ';');
                int brace = Utils.indexOf(rest, '{');

                if (-1 == semicolon)
                    Utils.rude("No semicolon found after typedef", name,
                            lineNo(str, idx), rest.substring(0, rest
                                    .indexOf("\n")));

                if (-1 == brace || semicolon < brace) {
                    // Simple typedef
                    text = str.substring(idx, idx + semicolon + 1);
                } else {
                    // Typedef of a struct, etc
                    int endbrace = Utils.findMatching(rest, '{', '}');
                    String rest2 = rest.substring(endbrace);
                    semicolon = Utils.indexOf(rest2, ';');
                    text = str.substring(idx, idx + endbrace + semicolon + 1);
                }
                FilePart fp = new FilePart(text, FilePart.TYPEDEF);
                parts.add(fp);
                idx += text.length();

            } else {
                if (isMethod(rest)) {

                    int brace = Utils.indexOf(rest, '{');
                    Signature signature = new Signature(str.substring(idx, idx
                            + brace));
                    if (signature.failed())
                        Utils.rude("Signature parsing failed", name, lineNo(
                                str, idx), signature.getOriginal());
                    if (null != currentClass
                            && null == signature.getClassName())
                        signature.setClassName(currentClass);
                    signature.setScope(scope);
                    signature.setNamespace(currentNamespace);

                    String body = rest.substring(brace);
                    int endBrace = Utils.findMatching(body, '{', '}');
                    body = body.substring(0, endBrace + 1);
                    int endIdx = idx + signature.originalLength()
                            + body.length();
                    text = str.substring(idx, endIdx);
                    MethodPart mp = new MethodPart(text, signature, body);
                    parts.add(mp);
                    idx += text.length();

                } else if (isField(rest)) {
                    int semicolon = Utils.indexOf(rest, ';');
                    text = str.substring(idx, idx + semicolon + 1);
                    FilePart fp = new FilePart(text, FilePart.FIELD);
                    parts.add(fp);
                    idx += text.length();

                } else if (isPrototype(rest)) {
                    int semicolon = Utils.indexOf(rest, ';');
                    text = str.substring(idx, idx + semicolon + 1);
                    PrototypePart pp = new PrototypePart(text, currentClass, currentNamespace);
                    pp.setScope(scope);
                    parts.add(pp);
                    idx += text.length();

                } else {
                    //TODO other file parts here - not sure if there are any
                    // others?
                    Utils.rude("Unrecognised file part", name,
                            lineNo(str, idx), rest.substring(0, rest
                                    .indexOf("\n")));
                } // end if
            } // end if
        } // end for
    }

    public Iterator getPartIterator() {
        return parts.iterator();
    }

    private int lineNo(String s, int idx) {
        int n = 0;
        for (int i = 0; i < idx && i < s.length(); i++)
            if ('\n' == s.charAt(i))
                n++;
        return n;
    }

    /**
     * Find out whether we are defining a class, struct or extern "C" which may
     * contain function implementations. These will have braces which begin a
     * new scope. Ignore function prototypes that return a struct. struct mystr {
     * int f1; }; struct mystr func(); struct mystr func() { struct mystr a;
     * return a; }
     */
    private static boolean beginsScope(String s) throws ParsingException {
        if (isMethod(s))
            return false;

        int brace = Utils.indexOf(s, '{');
        int semicolon = Utils.indexOf(s, ';');

        // Return false for class prototypes, but true for class definitions.
        if (Utils.startsWith(s, "class")) {
            if (-1 == brace)
                return false;
            if (-1 == semicolon)
                return true;
            return brace < semicolon;
        }

        if (Utils.startsWith(s, "struct")) {
            if (-1 == brace || -1 == semicolon)
                return false;
            return brace < semicolon;
        }

        //Tuscany handle namespace for prototypes
        //in a similar way to "class"
        if (Utils.startsWith(s, "namespace")) {
            if (-1 == brace || -1 == semicolon)
                return false;
            return brace < semicolon;
        }

        return startsWithExternScope(s);
    }

    /**
     * There are 4 types of extern ... extern int field; extern int func();
     * extern "C" int func() { return 2; } extern "C" { int func() { return 2; } }
     * This method should return true only for the last of these three examples
     * since only the last one creates a new scope using braces.
     */
    private static boolean startsWithExternScope(String s)
            throws ParsingException {
        if (!s.startsWith("extern"))
            return false;

        int brace = Utils.indexOf(s, '{');
        int semicolon = Utils.indexOf(s, ';');
        int bracket = Utils.indexOf(s, '(');

        if (-1 == brace)
            return false;
        return (-1 == semicolon || brace < semicolon)
                && (-1 == bracket || brace < bracket);
    }

    /**
     * Find out whether we are defining an enum or union which will contain
     * braces. Ignore function prototypes that return an enum or union. enum
     * colour { red, blue }; enum colour func(); enum colour func() { return
     * colour.red; }
     */
    private static boolean isEnumOrUnion(String s) throws ParsingException {
        if ((!Utils.startsWith(s, "enum") && !Utils.startsWith(s, "union"))
                || isMethod(s))
            return false;

        int brace = Utils.indexOf(s, '{');
        int semicolon = Utils.indexOf(s, ';');
        return -1 != brace && (-1 == semicolon || brace < semicolon);
    }

    /**
     * Rules to recognise fields and methods...
     * 
     * Fields must contain a semicolon Methods may or may not contain a
     * semicolon Prototypes must contain a semicolon Fields may or may not
     * contain a brace (array initialisers do) Methods must contain a brace
     * Prototypes must not contain a brace Fields may or may not contain a
     * bracket (casts do) Methods must contain a bracket Prototypes must contain
     * a bracket
     * 
     * It's a method if it contains a bracket and then a brace before the first
     * semicolon (if there is a semicolon). It's a prototype if it's not a
     * method and it contains brackets before a semicolon. It's a field if it's
     * not a method or a prototype and it contains a semicolon. If it's not a
     * field, a method or a prototype and we haven't recognised it previously
     * then it's an error.
     */
    private static boolean isMethod(String s) throws ParsingException {
        int semicolon = Utils.indexOf(s, ';');
        int brace = Utils.indexOf(s, '{');
        int bracket = Utils.indexOf(s, '(');

        return (-1 != bracket && -1 != brace && bracket < brace && (-1 == semicolon || brace < semicolon));
    }

    private static boolean isPrototype(String s) throws ParsingException {
        int semicolon = Utils.indexOf(s, ';');
        int bracket = Utils.indexOf(s, '(');
        return !isMethod(s) && -1 != semicolon && -1 != bracket
                && bracket < semicolon;
    }

    private static boolean isField(String s) throws ParsingException {
        int semicolon = Utils.indexOf(s, ';');
        return !isMethod(s) && !isPrototype(s) && -1 != semicolon;
    }

    public String getName() {
        return name;
    }

    public String toString() {
        StringBuffer text = new StringBuffer();
        for (int i = 0; i < parts.size(); i++) {
            text.append(((FilePart) (parts.get(i))).toString());
        }
        return text.toString();
    }

}
