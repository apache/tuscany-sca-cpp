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

package org.apache.tuscany.sca.cpp.tools.services;

import java.io.File;
import java.util.Set;

import org.apache.tuscany.sca.cpp.tools.common.FileActor;
import org.apache.tuscany.sca.cpp.tools.common.Utils;

public class DirectoryScanner {
    private FileActor actor;

    private Set actOnTheseFileExtensions;

    public DirectoryScanner(FileActor actor, Set extensions) {
        this.actor = actor;
        this.actOnTheseFileExtensions = extensions;
    }

    /**
     * 
     * @param source
     *            The composite root directory
     * @param target
     *            The directory that will hold the generated output
     * @param depth
     *            The depth from the initial starting point, not significant for
     *            the Scagen tool as we are only interested in the composite root
     *            directory but present due to the FileActor actOnFile interface
     *            method. This code is pulled from the code in the CParsingTool
     *            class and further work is needed to remove the duplication.
     *            Tnterface has been left unchanged as we hope to reconverge the
     *            parser here with the original one once the changes are fed
     *            back into the original code.
     * @throws Exception
     */
    public void walkTree(File source, File target, int depth) throws Exception {
        depth++;
        boolean noTarget = (null == target);

        if (!source.canRead())
            Utils.rude("Cannot read from source directory " + source);
        if (!noTarget && !target.canWrite())
            Utils.rude("Cannot write to target directory " + target);

        if (source.isDirectory()) {
            File[] filesInDirectory = source.listFiles();
            for (int i = 0; i < filesInDirectory.length; i++) {
                File file = filesInDirectory[i];
                String name = file.getName();
                int dot = name.lastIndexOf('.');
                String ext = null;
                if (-1 != dot) {
                    ext = name.substring(dot + 1);
                }

                if (file.isFile()
                        && (actOnTheseFileExtensions == null || (!file
                                .isHidden() && actOnTheseFileExtensions
                                .contains(ext)))) {
                    // this is a file we need to act on!
                    actor.actOnFile(file, target, depth);
                }
            }
        } else {
            return; // Do not act on single files for now we expect a composite
            // root directory
            // and the "main" class checks its parameters to ensure this is so.
        }
    }

}