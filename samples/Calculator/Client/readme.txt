Tuscany SCA for C++ Samples - Calculator Sample
===============================================

This is a very simple sample to show how an SCA module with a single component
can be built and called from an executable.

There are three sub projects in this workspace:
    - CalculatorModule
      This contains the source code and SCA artifacts for the SCA module. This
      is the module that will be deployed into the SCA runtime.
    - CalculatorSubsystem
      This contains the sca.subsystem file which describes a subsystem to the 
      SCA runtime. 
    - Client
      An example client which will call the service exposed by the single component
      in the CalculatorModule.


Build instructions
------------------

Notes:

The projects are set up to locate SCA and SDO libraries using the environment variables
TUSCANY_SCACPP and TUSCANY_SDOCPP. These must be set before building.

scagan is a code generation tool written in java. This will need to be built before you 
can run it. To build scagen, you must go to the tools/scagen directory, and type "ant".
(You must have apache ant installed, and have a valid java virtual machine installed - 
upwards of version 1.4.2).  Once scagen has successfully built, it can be run from the 
sca/bin directory.

1) Build the SCA module.
    - Set CalculatorModule project as the active project.
      Right click->Set as active project

    - Run the SCA generation tool to create the proxy and wrapper classes.
      Either Tools->SCA Gen if you have added SCA Gen as an external tool
   dir Hello   Or run from the command line: 
        scagen -dir <projectDirectory> -output <projectDirectory> 
      where the projectDirectory is the directory with the sca.module file.

    - Rebuild All
      Build->Rebuild All

2) Package and deploy the SCA module
    - There is a script file in the sca/samples/Calculator directory which will
      copy across all the files needed for deployment to the Tuscany runtime.
      On a command line run pack_deploy.cmd
      As a default, the sample will be copied to sca/samples/runtime directory
      Both the SCA module and the subsystem will be copied.

3) Build the client
    - Set the Client project as the active project.
      Right click->Set as active project

    - Rebuild All
      Build->Rebuild All

4) Run the client
    - Use the command file in the sca/samples/Calculator/Client directory to run the
      client. This command file will set the SCA environment variables before running
      the client. For example, 
        runclient add 4 9
