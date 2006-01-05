/*
 * 
 * Copyright 2005 The Apache Software Foundation or its licensors, as applicable.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

/*
 * Branched from the original class that was also contributed to the
 * org.apache.axis.tools.common package.
 */
package org.apache.tuscany.sca.cpp.tools.common;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class Headers implements FileActor {
    private ArrayList instanceMethods = new ArrayList();

    private ArrayList staticMethods = new ArrayList();

    private ArrayList allMethods = new ArrayList();

    private ArrayList classNames = new ArrayList();

    private boolean failed = false;

    public void actOnFile(File header, File ignored, int depth)
            throws Exception {
        if (Configuration.fileExcluded(header.getName())) {
            Utils.outputDebugString("excluding " + header + "...");
            return;
        }

        Utils.outputDebugString("pre-parsing " + header + "...");
        FileReader fr = null;
        try {
            fr = new FileReader(header);
        } catch (FileNotFoundException fnfe) {
            throw fnfe;
        }
        BufferedReader inputFile = new BufferedReader(fr);

        try {
            InputCppSourceCode code = new InputCppSourceCode(inputFile, header
                    .getName());
            Iterator it = code.getPartIterator();
            while (it.hasNext()) {
                FilePart fp = (FilePart) (it.next());
                if (fp.getType() != FilePart.PROTOTYPE)
                    continue;
                PrototypePart pp = (PrototypePart) fp;
                String className = pp.className();
                if (null == className)
                    continue;
                String trimClassName = className;
                if (className.endsWith("::"))
                    trimClassName = className.substring(0,
                            className.length() - 2);
                if (!classNames.contains(trimClassName))
                    classNames.add(trimClassName);

                Signature sign = new Signature(fp.toString());
                sign.setClassName(className);
                //Tuscany
                sign.setScope(pp.getSignature().getScope());
                sign.setNamespace(pp.getSignature().getNamespace());

                // "Clean" the signature by stripping off attributes,
                // semicolons, etc
                Signature cleaned = new Signature(sign.toStringWithoutAttrs());
                //Tuscany - problem
                cleaned.setClassName(className);
                cleaned.setScope(pp.getSignature().getScope());
                cleaned.setNamespace(pp.getSignature().getNamespace());
                //Tuscany - end of problem
                
                
                if (-1 == sign.getAttributes().indexOf("static"))
                    instanceMethods.add(cleaned);
                else
                    staticMethods.add(cleaned);
            }
        } catch (ParsingException pe) {
            failed = true;
        }

        inputFile.close();
        allMethods.addAll(staticMethods);
        allMethods.addAll(instanceMethods);
    }

    public boolean failed() {
        return failed;
    }

    public boolean isInstanceMethod(Signature sign) {
        Iterator it = instanceMethods.iterator();
        while (it.hasNext()) {
            Signature s = (Signature) it.next();
            if (s.equals(sign))
                return true;
        }
        return false;
    }

    public boolean isStaticMethod(Signature sign) {
        Iterator it = staticMethods.iterator();
        while (it.hasNext()) {
            Signature s = (Signature) it.next();
            if (s.equals(sign))
                return true;
        }
        return false;
    }

    public List getMethods(String method) {
        ArrayList list = new ArrayList();
        if (null == method)
            return list;

        Iterator it = allMethods.iterator();
        while (it.hasNext()) {
            Signature s = (Signature) it.next();
            if (method.equals(s.getMethodName()))
                list.add(s);
        }
        return list;
    }

    /**
     * Tuscany change - a method to get all the method signatures at once
     */
    public List getAllMethods() {
        ArrayList list = new ArrayList();
        Iterator it = allMethods.iterator();
        while (it.hasNext()) {
            Signature s = (Signature) it.next();
            list.add(s);
        }
        return list;
    }

    public boolean isClassName(String text) {
        return classNames.contains(text);
    }
}