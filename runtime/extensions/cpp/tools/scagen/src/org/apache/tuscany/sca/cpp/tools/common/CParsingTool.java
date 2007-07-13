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
 */
package org.apache.tuscany.sca.cpp.tools.common;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;

/**
 * The superclass of tools that parse C/C++ code. This CParsingTool provides
 * some useful common methods.
 */
public class CParsingTool {
    protected boolean failed = false;

    protected Headers headers = new Headers();

    protected CParsingTool(String[] args) throws Exception {
        String text = new String();
        for (int i = 0; i < args.length; i++)
            text += args[i] + " ";
        Utils.outputDebugString(text);

        Options.set(args);
        String config = (String) Options.getOption("-config");
        if (null != config)
            Configuration.initialise(config);
    }

    /**
     * Read in any include files before the main processing of the tool is done.
     * This constructs the Headers.
     */
    protected Headers preparseHeaders(String option) throws Exception {
        Headers headers = new Headers();
        Object o = Options.getOption(option);
        if (null != o) {
            Utils.outputDebugString("Pre-parsing headers...");
            List includeList;
            if (o instanceof List)
                includeList = (List) o;
            else {
                includeList = new ArrayList();
                includeList.add(o);
            }
            Iterator it = includeList.iterator();
            while (it.hasNext()) {
                File include = new File((String) it.next());
                if (!include.isDirectory())
                    Utils.rude("Bad include directory " + include);

                DirectoryTree tree = new DirectoryTree(headers, new HashSet(
                        Arrays.asList(new Object[] { "hpp", "h" })));
                tree.walkTree(include, null, 0);
            }
            Utils.outputDebugString("Parsing files...");
        }

        return headers;
    }

    /**
     * Checks the source directory looks good.
     */
    protected File checkFile(String option) throws Exception {
        String name = (String) Options.getOption(option);
        if (null == name) {
            printUsage();
            System.exit(-1);
        }

        File file = new File(name);
        if (!file.isFile() && !file.isDirectory())
            Utils.rude("Bad file or directory " + file);
        return file;
    }

    /**
     * Checks the target directory and creates it if it doesn't already exist.
     */
    protected File maybeCreateDirectory(String option) throws Exception {
        String name = (String) Options.getOption(option);
        if (null == name) {
            printUsage();
            System.exit(-1);
        }

        File file = new File(name);
        if (!file.exists() && !file.mkdir())
            Utils.screenMessage("Failed to create directory " + file);
        return file;
    }

    protected void printUsage() {
        System.out.println("usage: ??");
    }
}