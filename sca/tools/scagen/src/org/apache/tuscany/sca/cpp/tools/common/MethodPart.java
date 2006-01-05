/*
 *
 *  Copyright 2005 The Apache Software Foundation or its licensors, as applicable.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 *  Branched from the original class that was also contributed to the 
 *  org.apache.axis.tools.common package.
 */
package org.apache.tuscany.sca.cpp.tools.common;

import java.util.ArrayList;

/**
 * A C or C++ method from a piece of source code. The method has a signature and
 * a body (the bit between the braces).
 */
public class MethodPart extends FilePart {
    private Signature signature;

    private String body;

    MethodPart(String s, Signature signature, String body) {
        super(s, METHOD);
        this.signature = signature;
        this.body = body;
    }

    public Signature getSignature() {
        return signature;
    }

    public String getOriginalSignature() {
        return signature.getOriginal();
    }

    /**
     * Returns the method body as code snippets, each ending with a place where
     * a trace statement belongs. The end of the first code snippet is where the
     * entry trace should go. The end of every other snippet is a return from
     * the method.
     */
    public BodyPart[] getBodyParts() throws ParsingException {
        String b = body; // Don't alter field member
        if (b.startsWith("{"))
            b = b.substring(1);

        // Add in trace exit at all the return statements in the method.
        ArrayList al = new ArrayList();
        int idxR = Utils.indexOf(b, "return");
        int idxC = Utils.indexOf(b, "catch");
        while (-1 != idxR || -1 != idxC) {
            if (-1 == idxC || (-1 != idxR && idxR < idxC)) {
                String frag = b.substring(0, idxR);
                String rest = b.substring(idxR + "return".length());

                int semicolon = Utils.indexOf(rest, ';');
                if (-1 == semicolon)
                    Utils.rude("Missing semicolon in " + signature);
                String retVal = rest.substring(0, semicolon);
                BodyPart bp = new BodyPart(frag, retVal);
                al.add(bp);
                b = b.substring(idxR + "return".length() + retVal.length() + 1);
            } else {
                String frag = b.substring(0, idxC);
                String rest = b.substring(idxC);

                int brace = Utils.indexOf(rest, "{");
                if (-1 == brace)
                    Utils.rude("Missing open brace in " + signature);
                Signature signature = new Signature(rest.substring(0, brace));
                frag = frag + rest.substring(0, brace + 1);
                BodyPart bp = new BodyPart(frag, signature.getParameters()[0]);
                al.add(bp);
                b = rest.substring(brace + 1);
            }
            idxR = Utils.indexOf(b, "return");
            idxC = Utils.indexOf(b, "catch");
        }

        // Add in trace exit before the last } if there are no returns in
        // the method or there is code after the last return and the method
        // returns void.
        // int f1(){try{return f2();}catch(Exception& e){throw;}}
        // has code after the last return but having a traceexit before the
        // last brace wouldn't compile since the method returns an int. We
        // cope with this by only adding in a traceexit before the last brace
        // if the method returns void. That may mean we add in an unreachable
        // traceexit which may give a compiler warning, but that should be
        // benign.
        //
        // TODO: Not quite good enough for
        // void f(int a){if(a){printf("a");}else{printf("!a");return;}}
        // as a trace exit is needed before the last } in case a>0 but
        // void f(int a){if(a){printf("a");return;}else{printf("!a");return;}}
        // would give compiler warnings about unreachable code if a trace
        // exit is added before the last brace. This could be tricky to fix.
        if ((0 == al.size() || -1 != Utils.indexOf(b, ';'))
                && null == signature.getReturnType().getType()) {

            int last = b.lastIndexOf('}');
            if (-1 == last)
                Utils.rude("Missing end brace in " + signature);
            String b2 = b.substring(0, last);
            al.add(new BodyPart(b2));
            b = b.substring(last);
        }

        // The final body part is the last }
        al.add(new BodyPart(b));

        BodyPart[] bps = new BodyPart[al.size()];
        System.arraycopy(al.toArray(), 0, bps, 0, al.size());
        return bps;
    }
}