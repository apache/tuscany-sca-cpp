Tuscany SCA C++
===============

Tuscany SCA C++ provides a runtime implementation for the Service Component
Architecture 0.9 specification, written in C++ and will currently support C++
component implementation types. This is not yet a complete implementation and
known restrictions are described below.

Supported SCA Assembly Model features
  *  All features are supported unless listed under the known restrictions
     below. See SCA Assembly Model specification.

Supported language bindings
  * Component implementations written in C++. See SCA Client and
    Implementation Model specification.
  * Component interfaces described by C++ classes. See SCA Client and
    Implementation Model specification.

Supported external service and entry point bindings
  * The web service binding is supported. This implementation will support
    web services which using document literal SOAP bindings conforming to the
    WS-I basic profile (rpc/encoded is not yet supported).

Known restrictions
  * Subsystem wiring, entry points and external services are not supported.
  * Remotable services that will be bound to the web service binding must have
    an interface with a single input parameter which is an SDO DataObjectPtr,
	and a single return type which is an SDO DataObjectPtr.
  * Local service interfaces cannot use overloaded operations (the SCA
    specification limits remote service interfaces to not using overloaded
	operations).
  * Only component properties with simple types are supported.
  * Each WSDL definition for a web service binding must be in a single WSDL
    document.
  * No load time validation of the deployed SCA application (run time validation
    only).
  * No metadata API.



Web Services EntryPoint
=======================

This document describes the use of the Web Service EntryPoint (WS EntryPoint) 
support in the Apache Tuscany SCA C++ runtime.

The WS EntryPoint code is based on Apache Axis2C version 0.92 
(http://ws.apache.org/axis2/c) and allows SCA components to be invoked via Web 
Service calls.

WS EntryPoint currently supports Document Literal style Web Services only. 
There are also restrictions about the parameter and return types of the 
operations in SCA components that can be exposed as Web Services, see below 
for more details.


Defining and deploying an SCA Module with a WS EntryPoint
=========================================================

In this section we will use the Calculator sample as a worked example. 
The Calculator code and files can be found at sca/samples/Calculator.

Pre-requisites:

 o A working component within a module and subsystem, with SCAGEN generated 
   Proxy and Wrapper classes and a DLL or SO library compiled from these and 
   the component class files. The sca.module, *.componentType, *.fragment and 
   sca.subsystem files must also be available and working

1. Create the WSDL that defines the interface of your SCA component. See the 
table "XML Schema Type to C++ Type Mapping" and "WS EntryPoint Notes" below for 
mapping the parameters and return types of the component operations to XML 
schema types in the WSDL. This file needs to be accessible from the component, 
so place it in the same directory as the component or in a subdirectory.

See the sca/samples/Calculator/CalculatorModule/Calculator.wsdl file as an 
example.


2. Add an EntryPoint definition to the sca.module file, setting the 
interface.wsdl interface attribute to the WSDL file defined above, the 
reference element to the name of the component to be invoked and the binding.ws 
port attribute to the namespace, service and port name specified in the 
WSDL, in the form: "<namespace>#wsdl.endpoint(<service-name>/<port-name>)". 
Also give the EntryPoint a name and set the multiplicity.

E.g. for the Calculator component, based on the Calculator.wsdl file:

<!-- Expose as a web service -->
<entryPoint name="WSCalculatorEntrypoint" multiplicity="1..1">
    <interface.wsdl interface="Calculator.wsdl" />
    <binding.ws port="http://ws.apache.org/axis2/c/samplesmath#wsdl.endpoint(CalculatorService/CalculatorServicePort)"/>
    <reference>CalculatorServiceComponent</reference>
</entryPoint>


3. Create a file named Tuscany-model.config in the same directory as your 
component. This file defines which WSDL and XML Schema files to include in the 
Tuscany runtime environment for this module. Set the contents of the file 
using the example below and use the path and name of the WSDL file created 
above.

E.g. for the Calculator component

<?xml version="1.0" encoding="utf-8" ?>
<tuscany-model xmlns="http://org.apache.tuscany/xmlns/cpp/1.0">
    <wsdl>
        <file name="Calculator.wsdl"/>
    </wsdl>
</tuscany-model>



These changes are all that are required in the Tuscany runtime environment to 
expose a component as a Web Service; the following steps detail how to add the 
Web Service to the list of Axis2C services. 


4. Create a new directory within the Axis2C services directory. The name of 
this directory defines the URL of your service. For example, if you create a 
directory named "CalcWebService", the URL of the service will be 
http://localhost:9090/axis2/services/CalcWebService


5. Create a file named services.xml within the new directory. This file defines 
the library to use that contains the service code and the operations that the 
service exposes. For the Tuscany WS EntryPoint, you also need to add some 
Tuscany specific parameters to identify the Tuscany System Root directory and 
the EntryPoint that will be invoked. 

The TuscanySystemRoot parameter requires the full path to your system root 
directory - this is the directory that contains the "modules" and "subsystem" 
directories; which in turn contain your modules and subsystems.

The TuscanyEntryPoint parameter is of the form 
"<subsystem-name>/<service-name>/<entrypoint-name>", with the subsystem and 
service names defined in your sca.subsystem file and the EntryPoint name 
defined in the EntryPoint definition you added to your sca.module file above.

Set the ServiceClass parameter to "tuscany_sca_ws_service" as this library 
(tuscany_sca_ws_service.dll on Windows and libtuscany_sca_ws_service.so on 
Linux) contains the service code that will invoke your component. 

Finally, add all of the operations that your component defines that you want 
to expose as Web Service operations.

E.g. for the Calculator WS EntryPoint, 
axis2c/services/CalcWebService/services.xml contains:

<service name="CalcWebService">

    <parameter name="ServiceClass" locked="xsd:false">tuscany_sca_ws_service</parameter>
    <parameter name="TuscanySystemRoot" locked="xsd:false">C:/tuscany/cpp/sca/samples/runtime</parameter>
    <parameter name="TuscanyEntryPoint" locked="xsd:false">CalculatorSubsystem/CalculatorService/WSCalculatorEntrypoint</parameter>

    <operation name="add"/>
    <operation name="sub"/>
    <operation name="mul"/>
    <operation name="div"/>

</service>


6. Copy the tuscany_sca_ws_service.dll file (or the 
libtuscany_sca_ws_service.so file, if on Linux) to the new service directory 
you created above. This library contains the code that Axis2C requires to call 
a service implementation and links to the tuscany_sca.dll library (or 
libtuscany_sca.so on Linux) which manages the invocation of your component.


7. Ensure the TUSCANY_SCACPP, TUSCANY_SDOCPP, AXIS2C_HOME and PATH environment 
variables are correctly set

E.g. on Windows run the following commands with the correct directories:
set TUSCANY_SCACPP=C:/path_to_tuscany/sca/deploy
set TUSCANY_SDOCPP=C:/path_to_tuscany/sdo/deploy
set AXIS2C_HOME=C:/path_to_axis2c0.92
set PATH=%PATH%;C:/path_to_tuscany/sca/deploy/bin;C:/path_to_tuscany/sdo/deploy/bin;C:/path_to_axis2c0.92/lib


8. Start the Simple Axis server, or your Apache HTTP server if you have 
deployed Axis2C to Apache.

E.g. on Windows run the following commands:
cd C:/path_to_axis2c0.92/bin
./axis2_http_server.exe


Your component should now be exposed as an Axis2C Web Service, via the WS 
EntryPoint you created. See the Axis2C documentation for writing clients to 
invoke the Web Service, or you can use any other Web Service client platform 
(e.g. Axis2 for Java: http://ws.apache.org/axis2), or you can invoke your 
component from another Tuscany runtime by using Tuscany's WS External Service 
support.



XML Schema Type to C++ Type Mapping
===================================

To help define the WSDL that describes the interface of your component, the 
table below lists how incoming XML data in Web Service messages is mapped to 
C++ types used in the parameters and return types of your component operations.


 XML Schema Type  | C++ Type          
-------------------------------------------------------------
                  |                  
 string           | char*             
                  |                  
 int              | long              
                  |                  
 integer          | long              
                  |                  
 short            | short             
                  |                  
 float            | float             
                  |
 double           | long double
                  |
 boolean          | bool
                  |       
 hexBinary        | char*
                  |
 base64Binary     | char*
                  |
 byte             | char
                  |                             
 complexType      | SDO DataObjectPtr 
                  |
 any              | SDO DataObjectPtr with OpenDataObjectType



WS EntryPoint Notes
===================

 o Currently only Document Literal style Web Services are supported by WS 
   EntryPoint, support for RPC style Web Services is planned for future 
   releases.

 o Operation parameter and return messages that are defined in the WSDL must be 
   XML Schema elements containing a complexType - there is currently no 
   support for simpleTypes or single-level elements. 

   For example, a component operation defined in C++ as:
   
      long myOperation(char* arg1, short arg2, DataObjectPtr arg3);
   
   will need to be described in WSDL with messages like:

      <wsdl:message name="myOperationRequestMsg">
        <wsdl:part element="tns:myOperation" name="myOperationRequestPart"/>
      </wsdl:message>
      <wsdl:message name="myOperationResponseMsg">
        <wsdl:part element="tns:myOperationResponse" name="myOperationResponsePart"/> 
      </wsdl:message>

   and will need an XML schema to define the types like:

      <xsd:element name="myOperation">
        <xsd:complexType>
	  <xsd:sequence>
    	    <xsd:element name="arg1" type="xsd:string" minOccurs="1"/>
    	    <xsd:element name="arg2" type="xsd:short" minOccurs="1"/>
            <xsd:element name="arg3" minOccurs="1">
              <xsd:complexType>
	        <xsd:sequence>
		  <xsd:element name="dataObjectFloatData" type="xsd:float"/>
		  <xsd:element name="dataObjectStringData" type="xsd:string"/>
                  <xsd:element name="dataObjectIntData" type="xsd:int"/>
 	        </xsd:sequence>
	      </xsd:complexType>
            </xsd:element>
	  </xsd:sequence>
	</xsd:complexType>
      </xsd:element>

      <xsd:element name="myOperationResponse">
        <xsd:complexType>
	  <xsd:sequence>
    	    <xsd:element name="result" type="xsd:int" minOccurs="1"/>
	  </xsd:sequence>
        </xsd:complexType>
      </xsd:element>
  