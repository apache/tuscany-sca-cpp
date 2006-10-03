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
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

import org.apache.tuscany.sca.cpp.tools.common.CParsingTool;
import org.apache.tuscany.sca.cpp.tools.common.Options;
import org.apache.tuscany.sca.cpp.tools.common.Utils;

/**
 * This is the main top level class. Its purpose is to create a
 * Composite/FragmentFile handler visitor and pass it to a DirectoryScanner for
 * processing.
 */
public class Scagen extends CParsingTool {

    public static Set COMPOSITE_EXTENSIONS = new HashSet(Arrays
            .asList(new Object[] { "composite", "fragment" }));

    /**
     * @throws Exception
     *  
     */
    public Scagen(String[] args) throws Exception {
        super(args);
    }

    /**
     * Take a directory scanning class and create a vistor that knows how to
     * handle any .composite or .fragment that the scanner comes across.
     * 
     * @param args
     *            standard main args. THe values we expect in this class are
     *            scagen -dir input_dir -output output_dir
     *  
     */
    public static void main(String[] args) {
        boolean failed = false;
        try {
            Scagen env = new Scagen(args);
            CompositeOrFragmentFileHandler composite_handler = new CompositeOrFragmentFileHandler();

            // Check and access the input SCA composite directory
            String name = (String) Options.getOption("-dir");
            if (null == name) {
                Utils
                        .screenMessage("Please provide a SCA composite directory name as a \"-dir\" option.");
                env.printUsage();
                System.exit(-1);
            }
            File source = new File(name);
            if (!source.isFile() && !source.isDirectory()) {
                Utils
                        .screenMessage("The SCA composite directory provided as the \"-dir\" option cannot be accessed,");
                Utils.screenMessage("the option given was: " + source);
                env.printUsage();
                System.exit(-1);
            }

            String deployDir = null;
            try {
                deployDir = (String) Options.getOption("-deploy");
                if (null != deployDir || Options.deploy()) {
                    Utils.setReportArtefacts(true);
                }
            } catch (Exception e) {
                // let it default to null
            }

            Utils.postEvent(Utils.DEPLOYMENT_INPUT_DIRECTORY, source
                    .getAbsolutePath());

            // We check the -output option here as we wish to
            // reuse the env.maybeCreateDirectory method
            // unchanged from the original that went into axis and it will do a
            // System.exit if there is no matching Option
            String outputDirName = (String) Options.getOption("-output");
            if (null == outputDirName) {
                Utils
                        .screenMessage("Please provide an output directory name for the generated files as a \"-output\" option.");
                env.printUsage();
                System.exit(-1);
            }

            File outputDir = new File(outputDirName);

            // Check we can create the output directory
            if (outputDir == null || !outputDir.exists() && !outputDir.mkdir()) {
                Utils.screenMessage("Failed to create output directory: "
                        + outputDirName);
                env.printUsage();
                System.exit(-1);
            }

            Utils.postEvent(Utils.DEPLOYMENT_OUTPUT_DIRECTORY, outputDir
                    .getAbsolutePath());

            DirectoryScanner scanner = new DirectoryScanner(composite_handler,
                    COMPOSITE_EXTENSIONS);
            scanner.walkTree(source, outputDir, 1);

            if (0 == composite_handler.getFilesActedOn()) {
                Utils
                        .screenMessage("No SCA composite or fragment files were found in: "
                                + source);
            }

            failed = composite_handler.failed;

        } catch (Exception exception) {
            Utils
                    .screenMessage("Unexpected error occurred while runnning the Scagen tool. The Java exception is below.");
            exception.printStackTrace();
            failed = true;
        }

        if (failed) {
            Utils
                    .outputDebugString("Finished! (but encountered problems parsing composites)");
            System.exit(-2);
        }

        Utils.outputDebugString("Finished!");
    }

    /**
     * Provide a hint to the user on how to call this class
     */
    protected void printUsage() {
        System.out
                .println("usage: Java Scagen -dir <input_directory> -output <output_directory> [-verbose] [-deploy <deploy_dir>] [-nogenerate] [-outputCommand] [-command <copy_cmd>]");
        System.out
                .println("       -dir <input_directory>: the SCA composite root directory");
        System.out
                .println("       -output <output_directory>: a directory to put the generated output into");
        System.out.println("       [-verbose]: report on what scagen is doing");
        System.out
                .println("       [-deploy <deploy_dir>]: output text to help in deploying the composite's artefacts");
        System.out
                .println("       [-command <copy_cmd>]: a string that is injected into the deploy text");
        System.out
                .println("       [-list]: change the deploy output text to a simple list of artefacts");
        System.out
                .println("       [-outputCommand]: change the deploy output text to command text format");
        System.out
                .println("                       output is of form \"copy_cmd <dir>file1 <deploy_dir>file1\"");
        System.out
                .println("       [-nogenerate]: do not generate proxies and wrappers");

    }

}