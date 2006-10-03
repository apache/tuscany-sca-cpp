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
 *  Branched from the original class that was also contributed to the 
 *  org.apache.axis.tools.common package.
 *  
 */
package org.apache.tuscany.sca.cpp.tools.common;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

/**
 * A C or C++ method signature with the ability to parse it. TODO: properly
 * support variable length argument lists using "..." TODO: passing or returning
 * function pointers (hopefully not needed) TODO: Cope with ~ <space>Classname()
 */
public class Signature {
    private String originalText;

    private String attributes;

    private String className = null;
    
    private String namespace = null;

    private String methodName = null;

    private Parameter returnType = null;

    private Parameter[] params = null;

    private String trailingAttributes;

    private String scope = "public";

    private boolean failed = false;

    private boolean traceable = true;

    private final static Set knownAttrs = new HashSet(Arrays
            .asList(new Object[] { "public", "private", "extern", "\"C\"",
                    "virtual", "static", "inline" }));

    private final static Set specialOperators = new HashSet(
            Arrays.asList(new Object[] { "(", ")", "*", ",", "&", "]", "[",
                    "=", "~" }));

    /**
     * Takes an unparsed signature string and parses it.
     * 
     * TODO: Should optionally pass in the className here in case it's an inline
     * method implementation inside the class{}. Just so the className comes out
     * in the trace.
     */
    Signature(String s) {
        originalText = s;

        try {
            List tokens = tokenise(s);

            ArrayList alAttrs = new ArrayList();
            ArrayList alName = new ArrayList();
            ArrayList alParms = new ArrayList();
            ArrayList alTrailAttrs = new ArrayList();
            ArrayList alInits = new ArrayList();
            if (!splitUp(tokens, alAttrs, alName, alParms, alTrailAttrs,
                    alInits)) {
                failed = true;
                return;
            }

            parseAttributes(alAttrs);
            parseNameAndRetType(alName);
            parseParameters(alParms);
            parseTrailingAttributes(alTrailAttrs);

            // Ignore any tokens after the ) since these are (hopefully)
            // constructor initialisers

            traceable = !Configuration.methodExcluded(className, methodName);
        } catch (NullPointerException npe) {
            failed = true;
            traceable = false;
        }
    }

    /**
     * Parse the signature into tokens. This removes whitespace and comments and
     * separates out "*", ",", "(", ")", "&", "[" and "]".
     */
    private static List tokenise(String s) {
        ArrayList tokens = new ArrayList();
        String tok = null;
        boolean space = true;
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            if (Character.isWhitespace(c)) {
                space = true;
                continue;
            }
            if (space) {
                if (tok != null)
                    tokens.add(tok);
                tok = "" + c;
            } else
                tok += c;
            space = false;

            if (tok.endsWith("/*")) {
                String sub = s.substring(i);
                int endcomm = sub.indexOf("*/");
                if (endcomm == -1)
                    break;
                i += endcomm + 1;
                if (tok.equals("/*"))
                    tok = "";
                else
                    tok = tok.substring(0, tok.length() - 2);
                continue;
            }

            if (tok.endsWith("//")) {
                String sub = s.substring(i);
                int endcomm = sub.indexOf("\n");
                if (endcomm == -1)
                    break;
                i += endcomm;
                if (tok.equals("//"))
                    tok = "";
                else
                    tok = tok.substring(0, tok.length() - 1);
                continue;
            }

            if (tok.endsWith("::"))
                space = true;

            String sc = "" + c;
            if (specialOperators.contains(sc)) {
                if (!tok.equals(sc)) {
                    tokens.add(tok.substring(0, tok.length() - 1));
                    tok = sc;
                }
                space = true;
            }
        }
        tokens.add(tok);
        return tokens;
    }

    /**
     * Split up a tokenised method signature into a list of attributes, a list
     * of name and return type tokens, a list of parameter tokens and a list of
     * initialiser tokens.
     */
    private static boolean splitUp(List tokens, List attrs, List nameAndRet,
            List parms, List trailAttrs, List inits) {

        // nameStart points to the start of the return type if there is one
        // else the start of the method name
        int nameStart;
        for (nameStart = 0; nameStart < tokens.size(); nameStart++) {
            String tok = (String) (tokens.get(nameStart));
            if (!knownAttrs.contains(tok) && !Configuration.isAttribute(tok))
                break;
        }
        if (nameStart == tokens.size())
            return false;

        // initStart points to the initialisers, or thrown exceptions after
        // the parameter list. throw is a keyword so we can safely search for
        // it.
        int initStart = tokens.size();
        for (int i = nameStart; i < tokens.size(); i++) {
            String tok = (String) tokens.get(i);
            if ((tok.startsWith(":") && !tok.startsWith("::"))
                    || "throw".equals(tok))
                initStart = i;
        }

        int parmEnd;
        for (parmEnd = initStart - 1; parmEnd > nameStart; parmEnd--)
            if (")".equals(tokens.get(parmEnd)))
                break;
        if (parmEnd == nameStart)
            return false;

        int parmStart = parmEnd;
        for (parmStart = parmEnd; parmStart > nameStart; parmStart--)
            if ("(".equals(tokens.get(parmStart)))
                break;

        for (int i = 0; i < tokens.size(); i++) {
            Object tok = tokens.get(i);
            if (i < nameStart || Configuration.isAttribute((String) tok))
                attrs.add(tok);
            else if (i < parmStart)
                nameAndRet.add(tok);
            else if (i <= parmEnd)
                parms.add(tok);
            else if (i < initStart)
                trailAttrs.add(tok);
            else
                inits.add(tok);
        }
        return true;
    }

    private void parseAttributes(List list) {
        attributes = new String();
        Iterator it = list.iterator();
        while (it.hasNext()) {
            if (attributes.length() > 0)
                attributes += " ";
            String next = (String) it.next();
            
            //Tuscancy 
            //the scope is not present in the attributes
            //but is set later in the InputCppSource contructor
            if ("public".equals(next) || "protected".equals(next)
                    || "private".equals(next))
                scope = next;
            attributes += next;
        }
    }

    private void parseNameAndRetType(List list) {
        int size = list.size();
        int idx;
        // "operator" is a key word so if it's present we know we're
        // dealing with operator overloading. The operator that's been
        // overloaded might have been split up into multiple tokens.
        for (idx = 0; idx < size; idx++)
            if ("operator".equals(list.get(idx)))
                break;

        if (idx < size) {
            methodName = "";
            for (int i = idx; i < size; i++)
                methodName += (String) list.get(i);
        } else { // No operator overloading
            methodName = "" + list.get(size - 1);
            idx = size - 1;
        }

        // If it's a destructor, the "~" will be split out into a separate
        // token, so add it onto the methodName here.
        if (idx > 0 && "~".equals(list.get(idx - 1))) {
            methodName = "~" + methodName;
            idx--;
        }

        // The class name comes before the method name
        while (idx > 0 && ((String) list.get(idx - 1)).endsWith("::")) {
            if (null == className)
                className = (String) list.get(idx - 1);
            else
                className = (String) list.get(idx - 1) + className;
            idx--;
        }

        // Whatever's left before the classname/methodname must be the
        // return type
        ArrayList retParm = new ArrayList();
        for (int i = 0; i < idx; i++)
            retParm.add(list.get(i));

        returnType = new Parameter(retParm, true);
    }

    /**
     * Constructs the parameter list
     */
    private void parseParameters(List list) {
        ArrayList alParams = new ArrayList();
        Iterator it = list.iterator();
        String token = (String) it.next(); // step over the (
        while (it.hasNext() && !")".equals(token)) {
            token = (String) it.next();

            int template = 0; // Depth of template scope
            boolean foundEquals = false;
            // Ignore default value for an optional parameter
            ArrayList parm = new ArrayList();
            while (!token.equals(")") && (!token.equals(",") || template > 0)) {
                if (token.equals("="))
                    foundEquals = true;
                if (!foundEquals)
                    parm.add(token);
                if (contains(token, "<"))
                    template++;
                if (contains(token, ">"))
                    template--;
                token = (String) it.next();
            }

            // No parameters so break out
            if (token.equals(")") && 0 == parm.size())
                break;

            Parameter p = new Parameter(parm);
            if (p.failed()) {
                failed = true;
                return;
            }

            // Copes with void func(void)
            if (!p.isVoid())
                alParams.add(p);
        }

        int size = alParams.size();
        if (size > 0) {
            params = new Parameter[size];
            System.arraycopy(alParams.toArray(), 0, params, 0, size);
        }
    }

    private void parseTrailingAttributes(List list) {
        trailingAttributes = new String();
        Iterator it = list.iterator();
        while (it.hasNext()) {
            if (trailingAttributes.length() > 0)
                trailingAttributes += " ";
            trailingAttributes += (String) it.next();
        }
    }

    public String getOriginal() {
        return originalText;
    }

    public int originalLength() {
        return originalText.length();
    }

    public boolean failed() {
        return failed;
    }

    public String getAttributes() {
        return attributes;
    }

    public String getClassName() {
        return className;
    }

    /**
     * @param namespace The namespace to set.
     */
    public void setNamespace(String namespace) {
        this.namespace = namespace;
    }

    /**
     * @return Returns the namespace.
     */
    public String getNamespace() {
        return namespace;
    }

    public String getTrimClassName() {
        return trimClassName(className);
    }

    public String getMethodName() {
        return methodName;
    }

    public Parameter getReturnType() {
        return returnType;
    }

    public Parameter[] getParameters() {
        return params;
    }

    public boolean isConstructor() {
        return className != null && methodName != null
                && trimClassName(className).equals(methodName);
    }

    public boolean isDestructor() {
        return className != null && methodName != null
                && methodName.startsWith("~")
                && methodName.endsWith(trimClassName(className));
    }

    private static String trimClassName(String name) {
        if (name.endsWith("::"))
            return name.substring(0, name.length() - 2);
        return name;
    }

    void setClassName(String className) {
        if (null == className)
            return;
        if (!className.endsWith("::"))
            className += "::";
        this.className = className;
    }

    public String getScope() {
        return scope;
    }

    /**
     * Sets the scope, but only if the scope is not set by an explicit attribute
     * in the signature.
     */
    public void setScope(String scope) {
        if (-1 == attributes.indexOf(this.scope))
            this.scope = scope;
    }

    /**
     * Should this method be traced?
     */
    public boolean traceable() {
        return traceable;
    }

    private static boolean contains(String src, String tgt) {
        if (src == null || tgt == null)
            return false;
        if (-1 == src.indexOf(tgt))
            return false;
        return true;
    }

    public boolean equals(Object obj) {
        if (null == obj || !(obj instanceof Signature))
            return false;
        Signature that = (Signature) obj;
        if (!Utils.safeEquals(className, that.className))
            return false;
        if (!Utils.safeEquals(methodName, that.methodName))
            return false;
        if (!Utils.safeEquals(returnType, that.returnType))
            return false;
        if (null == params && null == that.params)
            return true;
        if (null != params && null == that.params)
            return false;
        if (null == params && null != that.params)
            return false;
        if (params.length != that.params.length)
            return false;
        for (int i = 0; i < params.length; i++)
            if (!Utils.safeEquals(params[i], that.params[i]))
                return false;
        return true;
    }

    public String toStringWithoutAttrs() {
        String s = new String();
        if (returnType != null)
            s += returnType + " ";
        if (className != null)
            s += className;
        s += methodName + "(";
        for (int i = 0; params != null && i < params.length; i++) {
            if (i > 0)
                s += ", ";
            s += params[i].toString();
        }
        s += ")";
        return s;
    }

    public String toString() {
        String s = attributes;
        if (attributes.length() > 0)
            s += " ";
        s += toStringWithoutAttrs();
        if (trailingAttributes.length() > 0)
            s += " " + trailingAttributes;
        return s;
    }
}