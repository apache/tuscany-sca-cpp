/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 *   
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

package tuscany.antCompilers;

import java.io.File;
import java.util.Vector;

import net.sf.antcontrib.cpptasks.CUtil;
import net.sf.antcontrib.cpptasks.OptimizationEnum;

import net.sf.antcontrib.cpptasks.compiler.CommandLineCompilerConfiguration;
import net.sf.antcontrib.cpptasks.compiler.CompilerConfiguration;
import net.sf.antcontrib.cpptasks.compiler.Linker;
import net.sf.antcontrib.cpptasks.compiler.LinkType;
import net.sf.antcontrib.cpptasks.compiler.PrecompilingCommandLineCCompiler;
import net.sf.antcontrib.cpptasks.compiler.Processor;

import net.sf.antcontrib.cpptasks.devstudio.DevStudioProcessor;
import net.sf.antcontrib.cpptasks.devstudio.DevStudioLinker;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.types.Environment;


/**
 * An abstract base class for compilers that are basically command line
 * compatible with Microsoft(r) C/C++ Optimizing Compiler
 * 
 * This class was taken from cpptasks. Its a combination of the following 2 classes:
 *     net.sf.antcontrib.cpptasks.devstudio.DevStudioCompatibleCCompiler
 *     net.sf.antcontrib.cpptasks.devstudio.DevStudioCCompiler
 *
 * To compile MSVC8.0 in debug mode, the cpptasks msvc compiler doesnt distinguish
 * between msvc 7.1 and 8.0 and seems to actually be 7.1 centric. This implementation
 * For the Apache Tuscany project tries to address those issues. 
 */

public final class TuscanyMSVC8DevStudioCCompiler
        extends PrecompilingCommandLineCCompiler
{
    private static String[] mflags_ = 
    	new String[] {"/ML", "/MLd", null, null, "/MT", "/MTd", "/MD", "/MDd"};
    //   first four are single-threaded
    //      (runtime=static,debug=false), (..,debug=true),
    //      (runtime=dynamic,debug=true), (..,debug=false), (not supported)
    //    next four are multi-threaded, same sequence

	  // Indeces into the mflags_ array 
	private static int MINDEX_DEBUG           =  1;
	private static int MINDEX_STATIC_RUNTIME  =  2;
	private static int MINDEX_MULTI_THREADED  =  4;

	private static final TuscanyMSVC8DevStudioCCompiler instance_ = 
		new TuscanyMSVC8DevStudioCCompiler( "cl", false, null);

    public static TuscanyMSVC8DevStudioCCompiler getInstance()
    {
        return instance_;
    }

    private TuscanyMSVC8DevStudioCCompiler(
    		String command,
    		boolean newEnvironment,
            Environment env)
    {
        super(command, 
              "/bogus",
              new String[]{".c", ".cc", ".cpp", ".cxx", ".c++"},
              new String[]{".h", ".hpp", ".inl"},
              ".obj",
              false,
              null,
              newEnvironment,
              env);
    }

    public Processor changeEnvironment(boolean newEnvironment, Environment env)
    {
        if (newEnvironment || env != null) {
            return new TuscanyMSVC8DevStudioCCompiler(getCommand(), newEnvironment, env);
        }
        return this;
    }

    public Linker getLinker(LinkType type)
    {
        return DevStudioLinker.getInstance().getLinker(type);
    }

    public int getMaximumCommandLength()
    {
        return 32767;
    }

    protected void addImpliedArgs(
    		final Vector args,
            final boolean debug,
            final boolean multithreaded,
            final boolean exceptions,
            final LinkType linkType,
            final Boolean rtti,
            final OptimizationEnum optimization)
    {
        args.addElement("/c");
        args.addElement("/nologo");
        if (exceptions) {
            //   changed to eliminate warning on VC 2005, should support VC 6 and later
            //   use /GX to support VC5 - 2005 (with warning)
            args.addElement("/EHsc");
        }
        int mindex = 0;
        if (multithreaded) {
            mindex += MINDEX_MULTI_THREADED;
        }
        boolean staticRuntime = linkType.isStaticRuntime();
        if (!staticRuntime) {
            mindex += MINDEX_STATIC_RUNTIME;
        }

        if (debug) {
            mindex += MINDEX_DEBUG;
            args.addElement("/Zi");        // Generates complete debugging information
            args.addElement("/Od");        // Disables optimization
            args.addElement("/RTC1");      // Enables run-time error checking as opposed to depracated /GZ
            args.addElement("/Gd");        // Uses the __cdecl calling convention (x86 only)
            args.addElement("/D_DEBUG");   // Debug mode
        } else {
            args.addElement("/DNDEBUG");
            if (optimization != null) {
                if (optimization.isSize()) {
                    args.addElement("/O1");
                }

                if (optimization.isSpeed()) {
                    args.addElement("/O2");
                }
            }
        }

        String mflag = mflags_[mindex];
        if (mflag == null) {
            throw new BuildException(
                    "multithread='false' and runtime='dynamic' not supported");
        }
        args.addElement(mflag);
        if (rtti != null && rtti.booleanValue()) {
                args.addElement("/GR");
        }
    }

    protected void addWarningSwitch(Vector args, int level)
    {
        DevStudioProcessor.addWarningSwitch(args, level);
    }

    protected CompilerConfiguration createPrecompileGeneratingConfig(
            CommandLineCompilerConfiguration baseConfig,
            File prototype,
            String lastInclude)
    {
        String[] additionalArgs = new String[]{
                "/Fp" + CUtil.getBasename(prototype) + ".pch", "/Yc"};
        return new
            CommandLineCompilerConfiguration(
        		baseConfig,
        		additionalArgs,
                null,
                true);
    }    

    protected CompilerConfiguration createPrecompileUsingConfig(
            CommandLineCompilerConfiguration baseConfig,
            File prototype,
            String lastInclude,
            String[] exceptFiles)
    {
        String[] additionalArgs = new String[]{
                "/Fp" + CUtil.getBasename(prototype) + ".pch",
                "/Yu" + lastInclude};
        return new
            CommandLineCompilerConfiguration(
        		baseConfig,
        		additionalArgs,
                exceptFiles,
                false);
    }

    protected void getDefineSwitch(StringBuffer buffer, String define, String value)
    {
        DevStudioProcessor.getDefineSwitch(buffer, define, value);
    }

    protected File[] getEnvironmentIncludePath()
    {
        return CUtil.getPathFromEnvironment("INCLUDE", ";");
    }

    protected String getIncludeDirSwitch(String includeDir)
    {
        return DevStudioProcessor.getIncludeDirSwitch(includeDir);
    }

    protected void getUndefineSwitch(StringBuffer buffer, String define)
    {
        DevStudioProcessor.getUndefineSwitch(buffer, define);
    }
}
