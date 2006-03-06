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

import java.io.File;
import java.util.Set;
import java.util.StringTokenizer;

public class DirectoryTree {
    private FileActor actor;

    private Set extensions;

    public DirectoryTree(FileActor actor, Set extensions) {
        this.actor = actor;
        this.extensions = extensions;
    }

    /**
     * Starts adding trace into the given file. If the given file is a directory
     * then this the starting directory and all code beneath and in this
     * directory will be given trace.
     * 
     * @param source -
     *            either the starting directory or one file to add trace to.
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
                if (-1 != dot)
                    ext = name.substring(dot + 1);

                if (file.isDirectory()) {
                    File newTarget = null;
                    if (!noTarget) {
                        StringTokenizer st = new StringTokenizer(
                                file.getPath(), "\\/");
                        String newdir = null;
                        while (st.hasMoreTokens())
                            newdir = st.nextToken();
                        String targetName = maybeAppendSeparator(target
                                .toString());
                        newTarget = new File(targetName + newdir);
                        if (!newTarget.mkdir())
                            Utils.rude("Failed to create target directory "
                                    + newTarget);
                    }

                    // recurse
                    walkTree(file, newTarget, depth);
                } else if (file.isFile()
                        && (extensions == null || (!file.isHidden() && extensions
                                .contains(ext)))) {
                    // this is a file and we need to add trace into it !
                    actor.actOnFile(file, target, depth);
                }
            }
        } else {
            actor.actOnFile(source, target, depth);
        }
    }

    public static String maybeAppendSeparator(String name) {
        if (!name.endsWith("/") && !name.endsWith("\\"))
            name += "/";
        return name;
    }
}