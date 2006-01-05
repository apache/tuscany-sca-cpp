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
*  
*/
package org.apache.tuscany.sca.cpp.tools.common;

import java.io.File;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

//Apache Common Logging
//import org.apache.commons.logging.Log; 
//import org.apache.commons.logging.LogFactory; 

/**
* Static utility methods. Some of these methods are similar to the methods on
* java.lang.String except they are aware of C/C++ comments and string literals.
* 
* TODO: Many of these methods would perform better using StringBuffer not
* String
*/
public final class Utils {
   // All the C primitive data types
   public final static Set cPrimitives = new HashSet(Arrays
           .asList(new Object[] { "void", "byte", "char", "unsigned",
                   "signed", "int", "short", "long", "double", "float",
                   "struct", "class", "enum", "union" }));

   // All the qualifiers that can affect C types
   public final static Set cTypeQualifiers = new HashSet(
           Arrays.asList(new Object[] { "(", ")", "*", ",", "&", "]", "[",
                   "const" }));

   public final static String whitespace = " \t\r\n";

   //private static Log log = LogFactory.getLog(CLASS.class);

   /**
    * Never instantiate this class
    */
   private Utils() {
   }

   /**
    * Is this string all whitespace?
    */
   static boolean isSpace(String s) {
       for (int i = 0; i < s.length(); i++)
           if (!Character.isWhitespace(s.charAt(i)))
               return false;
       return true;
   }

   // TODO look for other trailing chars like { (because of class{)
   static boolean startsWith(String source, String target) {
       if (source == null || target == null)
           return false;
       if (!source.startsWith(target))
           return false;
       if (source.length() == target.length())
           return true;
       if (Character.isWhitespace(source.charAt(target.length())))
           return true;
       return false;
   }

   /**
    * Performs a C-aware version of String.indexOf(char) in that it skips
    * characters in string literals and comments.
    */
   static int indexOf(String s, char c) throws ParsingException {
       if ('"' == c)
           rude("Utils.indexOf cannot be passed quotes");

       for (int i = 0; i < s.length(); i++) {
           if (s.charAt(i) == c)
               return i;

           i = skip(s.substring(i), i);
           if (-1 == i)
               return -1;
       }
       return -1;
   }

   /**
    * Performs a C-aware version of String.indexOf(String) in that it skips
    * characters in string literals and comments and makes sure that the target
    * string is not embedded in a longer word.
    */
   static int indexOf(String s, String t) {
       char t0 = t.charAt(0);
       for (int i = 0; i < s.length(); i++) {
           if (s.charAt(i) == t0
                   && s.substring(i).startsWith(t)
                   && (0 == i || !Character.isLetterOrDigit(s.charAt(i - 1)))
                   && (s.length() == (i + t.length()) || !Character
                           .isLetterOrDigit(s.charAt(i + t.length()))))
               return i;

           i = skip(s.substring(i), i);
           if (-1 == i)
               return -1;
       }
       return -1;
   }

   /**
    * Matches braces or quotes and is C-aware. It skips characters in string
    * literals and comments.
    */
   static int findMatching(String s, char c1, char c2) {
       int depth = 0;
       for (int i = 0; i < s.length(); i++) {
           if (s.charAt(i) == c1)
               depth++;
           else if (s.charAt(i) == c2) {
               depth--;
               if (depth == 0)
                   return i;
           } else {
               i = skip(s.substring(i), i);
               if (-1 == i)
                   return -1;
           }
       }
       return -1;
   }

   /**
    * Failed to parse the source code for some reason. This method prints out a
    * suitably rude message, and then what? I haven't quite decided yet.
    * 
    * TODO: Do something sensible here like throw an Exception which will give
    * up on this file completely and tidy up the output file. It may be just
    * too dangerous to try to carry on. But we need to fail in such a way that
    * the build system knows that we've failed for this file and can build this
    * file without trace.
    */
   public static void rude(String reason, String filename, int lineno,
           String codefragment) throws ParsingException {

       String text = "Bad C++ code!! ";
       if (reason != null)
           text += reason;
       if (filename != null)
           text += " " + filename + " lineno=" + lineno;
       if (codefragment != null)
           text += " <" + codefragment + ">";
       System.err.println(text);
       throw new ParsingException();
   }

   /**
    * This method reports an error level problem
    * 
    * @param reason
    *            why we have an error level problem
    */
   public static void rude(String reason) throws ParsingException {
       // Apache commons logging
       // log.error(Object line, null);
       // or for now....
       rude(reason, null, 0, null);
   }

   /**
    * This method reports an error level problem
    * 
    * @param reason
    *            why we have an error level problem
    */
   public static void screenMessage(String msg) {
       // Apache commons logging
       // log.error(Object line, null);
       // or for now....
       System.out.println(msg);
   }

   /**
    * Escapes special characters like " so that they can be output in a C
    * string literal. Also removes newlines, since C string literals can't be
    * split over lines.
    */
   String pretty(String s) {
       StringBuffer sb = new StringBuffer(s);
       for (int i = 0; i < sb.length(); i++)
           switch (sb.charAt(i)) {
           case '"':
               sb = sb.insert(i, '\\');
               i++;
               break;
           case '\n':
               sb = sb.deleteCharAt(i);
               i--;
               break;
           }
       return sb.toString();
   }

   private static boolean startsWithComment(String s) {
       if (null == s || s.length() < 2)
           return false;
       if (s.startsWith("//"))
           return true;
       if (s.startsWith("/*"))
           return true;
       return false;
   }

   private static int endOfComment(String s) {
       int idx;
       if (s.startsWith("//"))
           idx = s.indexOf("\n");
       else {
           idx = s.indexOf("*/");
           if (-1 != idx)
               idx++; // Step over */
       }
       return idx;
   }

   private static boolean startsWithStringLiteral(String s) {
       if (null == s || s.length() < 1)
           return false;
       if (s.startsWith("\"") || s.startsWith("'"))
           return true;
       return false;
   }

   private static int endOfStringLiteral(String s) {
       boolean escape = false;
       char c0 = s.charAt(0);
       for (int i = 1; i < s.length(); i++) {
           if (!escape && s.charAt(i) == c0)
               return i;

           // \" or \' does not end the literal
           if ('\\' == s.charAt(i))
               // Escaping a \ should switch escape off so \\' does end
               // the literal
               escape = !escape;
           else
               escape = false;
       }
       return -1;
   }

   /**
    * If the String s starts with a string literal or a comment, return i plus
    * the index of the end of the literal or comment. String literals are
    * enclosed in " or ' and comments start with /* or //.
    */
   private static int skip(String s, int i) {
       int j = 0;
       if (startsWithStringLiteral(s)) {
           j = endOfStringLiteral(s);
           if (-1 == j)
               return -1;
       } else if (startsWithComment(s)) {
           j = endOfComment(s);
           if (-1 == j)
               return -1;
       }
       return i + j;
   }

   /**
    * A better method than .equals() because it doesn't NullPointerException
    * when one of the parameters is null.
    */
   public static boolean safeEquals(Object o1, Object o2) {
       if (null == o1 && null == o2)
           return true;
       if (null == o1 && null != o2)
           return false;
       if (null != o1 && null == o2)
           return false;
       return o1.equals(o2);
   }

   public static void outputDebugString(String line) {
       if (!Options.quiet()) {
           if (Options.debug())
               // Apache commons logging
               // log.debug(Object line, null);
               // or for now
               System.out.println(line);
       }
   }

   /**
    * This static method allows different parts of the code to inform about
    * significant events. Code interested in specific types of event can
    * register a listener against that type (not written yet)
    * 
    * @param eventType
    *            An int type enum indicating the type of event.
    * @param message
    *            A message that can be output to the user.
    */
   public static final int EVENT_TYPE_XML_ITEM_PARSED = 1;

   public static final int EVENT_TYPE_FILE_PARSED = 2;

   public static final int EVENT_TYPE_FILE_CREATE = 3;

   public static final int VERBOSE_LIMIT = 1024;

   public static final int DEPLOYMENT_ARTEFACT = VERBOSE_LIMIT;

   public static final int DEPLOYMENT_ARTEFACT_ENCOUNTERED = DEPLOYMENT_ARTEFACT + 1;

   public static final int DEPLOYMENT_ARTEFACT_GENERATED = DEPLOYMENT_ARTEFACT + 2;

   public static final int DEPLOYMENT_INPUT_DIRECTORY = DEPLOYMENT_ARTEFACT + 3;

   public static final int DEPLOYMENT_OUTPUT_DIRECTORY = DEPLOYMENT_ARTEFACT + 4;

   private static boolean reportArtefacts = false;

   /**
    * An easily callable method to allow tracking/reposting of events in scagen
    * and other tools.
    * 
    * @param eventType
    *            used for classifying event
    * @param message
    *            a user readable message
    */
   public static void postEvent(int eventType, String message) {
       if (Options.verbose() && eventType < VERBOSE_LIMIT) {
           screenMessage(message);
       }

       if ((eventType & DEPLOYMENT_ARTEFACT) > 0) {
           reportArtefact(message, eventType);
       }

   }

   /**
    * @param message
    *            The user message
    * @param eventType
    *            The type of event (input or output). This is used to determine
    *            if the path name of the file starts with the MODULE_ROOT
    *            directory or the given output directory as the one of these
    *            prefixes is removed from the path name in order to give the
    *            new (destination) path relative to the new module root
    * 
    *  
    */

   static String scagenInputDir = "MODULE_ROOT";

   static String scagenOutputDir = "SCAGEN_OUTPUT";

   static String newModuleRoot = "NEW_MODULE_ROOT";

   static String generatedDirName = "$sourceDir1";

   private static void reportArtefact(String message, int eventType) {

       if (Utils.isReportArtefacts()) {

           // Changing the value of the variable below will alter the output of
           // the
           // deploy assist strings:
           //    true will result in a "copy source NEW_MODULE_ROOT\dest" output
           // and
           //    false will result in a "inputDir c:\fred"
           //                           "outputDir c:\bob"
           //                           "input c:\fred\sca.module"
           //                           "output c:\bob\proxy.h" type output
           boolean outputCommand = Options.outputCommand();
           String command = null;

           try {
               newModuleRoot = (String) Options.getOption("-deploy");
               command = (String) Options.getOption("-command");
           } catch (Exception e) {
               // let it default
           }

           if (null == newModuleRoot) {
               newModuleRoot = "DEPLOY_MODULE_ROOT";
           }

           if (null == command) {
               command = "copy";
           }

           if (outputCommand) {

               String tail = message;
               switch (eventType) {
               case DEPLOYMENT_ARTEFACT_ENCOUNTERED:
                   if (message.startsWith(scagenInputDir)) {
                       tail = message.substring(scagenInputDir.length());
                   }

                   String dest = joinPathElements(newModuleRoot, tail);

                   System.out.println(command + " " + platformSlashes(message) + " " + platformSlashes(dest));
                   break;

               case DEPLOYMENT_ARTEFACT_GENERATED:
                   if (message.startsWith(scagenOutputDir)) {
                       tail = message.substring(scagenOutputDir.length());
                   }

                   dest = joinPathElements(newModuleRoot, tail);
                   System.out.println(command + " " + platformSlashes(message) + " " + platformSlashes(dest));
                   break;
               case DEPLOYMENT_INPUT_DIRECTORY:
                   scagenInputDir = message;
                   //System.out.println("inputDir " + message);
                   break;
               case DEPLOYMENT_OUTPUT_DIRECTORY:
                   scagenOutputDir = message;
                   //System.out.println("outputDir " + message);
                   break;
               default:
                   break;
               }

           } else {
               switch (eventType) {
               case DEPLOYMENT_ARTEFACT_ENCOUNTERED:
                   //TODO make efficient
                   System.out.println("$sourceDir1"
                           + message.substring(scagenInputDir.length()));
                   break;
               case DEPLOYMENT_ARTEFACT_GENERATED:
                   //TODO make efficient
                   System.out.println(generatedDirName
                           + message.substring(scagenOutputDir.length()));
                   break;
               case DEPLOYMENT_INPUT_DIRECTORY:
                   scagenInputDir = message;
                   System.out.println("sourceDir1=" + message);
                   break;
               case DEPLOYMENT_OUTPUT_DIRECTORY:
                   scagenOutputDir = message;
                   if (!scagenInputDir.equals(scagenOutputDir)) {
                       generatedDirName = "$sourceDir2";
                       System.out.println("sourceDir2=" + message);
                   } else {
                       //generatedDirName = "sourceDir1";
                   }
                   break;
               default:
                   break;
               }
           }
       }
   }

   /**
    * @param tail
    * @param tail
    * @return
    */
   public static String joinPathElements(String root, String tail) {
       String separator;
       // Stick in a "/" (File.separator) if required.
       if ((tail.substring(0, 1).equals("/") || newModuleRoot.substring(
               root.length() - 1, root.length()).equals("/"))
               || (tail.substring(0, 1).equals("\\") || root.substring(
                       root.length() - 1, root.length()).equals("\\"))

       ) {
           separator = "";
       } else {
           separator = File.separator;
       }
       String dest = newModuleRoot + separator + tail;
       return dest;
   }

   /**
    * @param reportArtefacts
    *            The reportArtefacts to set.
    */
   public static void setReportArtefacts(boolean reportArtefacts) {
       Utils.reportArtefacts = reportArtefacts;
   }

   /**
    * @return Returns the reportArtefacts.
    */
   private static boolean isReportArtefacts() {
       return reportArtefacts;
   }

   private static String platformSlashes(String path) {
       if (null == path) {
           return path;
       }
       // We need a double level of \\ escapes if the slashes are
       // this way round.
       String separatorForRegex = File.separator
               .replaceAll("\\\\", "\\\\\\\\");
       return path.replaceAll("[/\\\\]+", separatorForRegex);

   }

}