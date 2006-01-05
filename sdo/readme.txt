Tuscany SDO for C++
===================


Tuscany SDO is an implmentation of the Service Data Objects 2.0 specification for C++ developers.

The implmentation adheres closely to the specifcation, but there are a number of features which 
did not make it into this release. These are documented below.


Generic get/set
===============

The C++ implementation defines a set of access methods directly appropriate to each Type. For example
getString(), setString(), getBoolean(), setBoolean(). There is no generic get() method as there is no
common base object to return - unlike the java implementation which can return an Object.  

Typesafe interface
==================

There is no code generation in this version, so there is no interface such that a property called
"Name" can be accessed via object->getName().  The name must be accessed via object->getString("Name");

Change Summary Serialization.
=============================

Change summaries contain creations, deletions and modifications to data graphs. The specification 
indicates that these will be serialized to XML such that creations and deletions will become attributes
of the change summary element, whilst changes become elements within the change summary element.
This implementation saves all creations, deletions and changes as elements within the chang summary
element.

Change Summary Undo
===================

The undo method of a change summary is not implemented.

Read-Only
=========

Read-only data elements are not supported by this implementation. Properties may be set read-only, but
the property may still be modified without an error.

Opposites
=========

Opposites are not supported by this implementation. 

Date and Time
=============
Date support is limited to using a time_t. There are no calendar or duration style objects, and no
helpers for conversions.


BigDecimal and BigInteger
=========================

The equivalent of the java BigDecimal and BigInteger objects are not implmented.

NLS support
===========

Data objects may contain multi-byte data in the form of an SDO string, but locale information is not
stored with the data.

The creation API only allows properties and types to have ascii names (taking a char* as the name).

Inheritance
===========

Type inheritance is single only.

