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
package org.apache.tuscany.sca.cpp.tools.junit;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;

import junit.framework.TestCase;

import org.apache.tuscany.sca.cpp.tools.common.Options;
import org.apache.tuscany.sca.cpp.tools.services.Scagen;

/**
 * A superclass for testcases that can be used to store common functions.
 */
public class TuscanyTestCase extends TestCase {

    public static String root = "d:\\tuscany\\cpp\\sca\\tools\\scagen\\junit\\";

    public static String junit_modules = root + "testinput\\modules\\";

    public static String junit_output = root + "testoutput\\";

    private String testcase = "TESTCASE NOT SET BY SUBCLASS";

    String input = null;

    String output = null;

    /**
     * Check the resulting files with the contents of the expected_results
     * folder in the testModule method
     */
    public final static boolean check_results = true;

    /**
     * Create test output but do not check it (useful for setting up new
     * expected test output
     */
    public final static boolean create_results = false;

    /**
     *  
     */
    public TuscanyTestCase() {
        super();
    }

    /**
     * @param arg0
     */
    public TuscanyTestCase(String arg0) {
        super(arg0);
    }

    protected void setUp() throws Exception {
        super.setUp();
    }

    protected void tearDown() throws Exception {
        super.tearDown();
    }

    /**
     * Tests to see if two files are the same - this is just a scratch method at
     * the moment that flags up testcase output files not matching expected
     * results rather than needing to be a fully robust implementation and needs
     * a little further work.
     * 
     * @param file1
     *            the first file to compare
     * @param file2
     *            the second file to compare
     *  
     */
    protected boolean areFilesEqual(String file1, String file2) {

        try {
            FileInputStream fis1 = new java.io.FileInputStream(file1);
            FileInputStream fis2 = new java.io.FileInputStream(file2);

            BufferedReader br1 = new BufferedReader(new InputStreamReader(fis1));
            BufferedReader br2 = new BufferedReader(new InputStreamReader(fis2));

            String line1 = br1.readLine();
            String line2 = br2.readLine();
            boolean moretodo = (line1 != null) && (line2 != null);

            for (int line = 1; moretodo; line++) {

                if (line1.equals(line2)) {

                    line1 = br1.readLine();
                    line2 = br2.readLine();

                } else {

                    int l1i, l2i;

                    if ((l1i = line1.lastIndexOf("$Id")) > 0) {
                        //allow CVS strings
                        line1 = br1.readLine();
                    }

                    if ((l2i = line2.lastIndexOf("$Id")) > 0) {
                        //allow CVS strings
                        line2 = br2.readLine();
                    }

                    if (l1i == -1 && l2i == -1) {

                        // they don't match
                        System.out.println("file " + file1 + " and file "
                                + file2);
                        System.out.println(" don't match at line " + line);
                        System.out.println("1 is :" + line1);
                        System.out.println("2 is :" + line2);

                        return false;
                    }
                }

                moretodo = (line1 != null) && (line2 != null);
            }

            System.out.println("MATCH FOR: file " + file1 + " and file "
                    + file2);
            return true;

        } catch (Throwable t) {
            t.printStackTrace();
            return true;
        }

    }

    /**
     * @param outputDir
     */
    protected void clearDirButNotExpectedOutputSubDir(String outputDir) {
        File dir = new File(outputDir);
        if (dir.isDirectory()) {
            String[] files = dir.list();
            for (int i = 0; i < files.length; i++) {
                File child = new File(dir, files[i]);
                if (child.isDirectory()) {
                    if (!(child.getName().equals("expected_output") || child.getName().equals(".svn"))) {
                        clearDirButNotExpectedOutputSubDir(dir
                                .getAbsolutePath()
                                + File.separator + files[i]);
                    }
                }
                else
                {                    
	                if( !child.delete() )
	                {
	                    System.out.println("File "+outputDir+"/"+child.getName()+" could not be deleted");
	                }
                }
            }
        }

    }

    public void testModule(String module, boolean check) {

        Options.reset();
        setTestcase(module);
        clearDirButNotExpectedOutputSubDir(output);

        String[] commandLine = new String[] { "-dir", input, "-output", output };
        try {
            Scagen.main(commandLine);
        } catch (Exception e) {
            fail(e.getMessage());
        }

        if (check) {
            checkDirWithExpected(output);
        }
    }

    public void testModuleDeploy(String module, boolean check,
            String deployDir, String command) {

        Options.reset();
        setTestcase(module);
        clearDirButNotExpectedOutputSubDir(output);

        String[] commandLine;
        if (null == deployDir) {
            commandLine = new String[] { "-dir", input, "-output",
                    output, "-deploy"};
        } else {
            commandLine = new String[] { "-dir", input, "-output",
                    output, "-deploy", deployDir, "-command", command };
        }
        try {
            Scagen.main(commandLine);
        } catch (Exception e) {
            fail(e.getMessage());
        }

        if (check) {
            checkDirWithExpected(output);
        }
    }

    /**
     * @param outputDirName
     */
    private void checkDirWithExpected(String outputDirName) {

        File actualDir = new File(outputDirName);
        if (actualDir == null || !actualDir.isDirectory()) {
            fail("result directory does not exist");
        }

        File expectedDir = new File(outputDirName + File.separator
                + "expected_output");
        if (expectedDir == null || !actualDir.isDirectory()) {
            fail("can't check results as expected directory does not exist");
        }

        //Check every file in the expected output directory is present
        //in the actual directory

        String[] expectedFiles = expectedDir.list();

        if (expectedFiles == null) {
            fail("no expected results for " + expectedDir.getPath());
        }
        for (int i = 0; expectedFiles != null && i < expectedFiles.length; i++) {
            File expectedFile = new File(expectedDir, expectedFiles[i]);
            if (expectedFile.isDirectory()) {
                // we can't check subdirectories yet
                // and it conveniently skips over "CVS"
            } else {
                File actualFile = checkFileExistsFailIfNot(actualDir,
                        expectedFile.getName());
                checkFileHasNoTabsAndEndsInNewline(actualDir, expectedFile
                        .getName());
                if (!areFilesEqual(expectedFile.getAbsolutePath(), actualFile
                        .getAbsolutePath())) {
                    fail("odd output for " + expectedFile.getPath());
                }

            }

        }

    }

    /**
     * @param actualDir
     * @param name
     */
    private File checkFileExistsFailIfNot(File dir, String name) {

        File f = new File(dir, name);
        if (f == null || !f.exists() || !f.isFile()) {
            fail("Expected result file " + f + " does not exist");
        }
        return f;

    }

    /**
     * @param actualDir
     * @param name
     */
    private void checkFileHasNoTabsAndEndsInNewline(File dir, String name) {

        try {
            File f = new File(dir, name);
            FileInputStream fis = new FileInputStream(f);
            InputStreamReader isr = new InputStreamReader(fis);
            Reader br = new BufferedReader(isr);
            int ch, charBeforeMinusOne = -1;
            while ((ch = br.read()) > -1) {
                if (ch == '\t') {
                    fail("found tab in output " + f.getPath());
                }
                charBeforeMinusOne = ch;
            }
            //The last char must be newline;
            if (charBeforeMinusOne != '\n') {
                fail("last char in file is not a newline in " + "(char is:"
                        + ch + "  " + dir.getName() + File.separator + name);
            }

            br.close();
        } catch (IOException e) {
            e.printStackTrace();
            fail("io exception in tab/newline checker for " + dir.getName()
                    + File.separator + name);
            return;
        }

    }

    /**
     * @param testcase
     *            The testcase to set.
     */
    void setTestcase(String testcase) {
        this.testcase = testcase;
        input = TuscanyTestCase.junit_modules + testcase;
        output = TuscanyTestCase.junit_output + testcase;
    }

    /**
     * @return Returns the testcase.
     */
    String getTestcase() {
        return testcase;
    }

}