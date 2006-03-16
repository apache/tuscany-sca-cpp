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
    WS-I basic profile (rpc/encoded is not supported).

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
