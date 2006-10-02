lib_LTLIBRARIES = libtuscany_sdo.la
AM_CPPFLAGS = $(CPPFLAGS)

libtuscany_sdo_la_SOURCES = \
	ChangedDataObjectListImpl.cpp \
	ChangeSummaryBuilder.cpp \
	ChangeSummary.cpp \
	ChangeSummaryImpl.cpp \
	CopyHelper.cpp \
	DASProperty.cpp \
	DASType.cpp \
	DASValue.cpp \
	DASValues.cpp \
	DataFactory.cpp \
	DataFactoryImpl.cpp \
	DataGraph.cpp \
	DataGraphImpl.cpp \
	DataObject.cpp \
	DataObjectImpl.cpp \
	DataObjectInstance.cpp \
	DataObjectList.cpp \
	DataObjectListImpl.cpp \
	DefaultLogWriter.cpp \
	EqualityHelper.cpp \
	GroupDefinition.cpp \
	GroupEvent.cpp \
	HelperProvider.cpp \
	Logger.cpp \
	LogWriter.cpp \
	ParserErrorSetter.cpp \
	Property.cpp \
	PropertyDefinition.cpp \
	PropertyDefinitionImpl.cpp \
	PropertyImpl.cpp \
	PropertyList.cpp \
	PropertySetting.cpp \
	RefCountingObject.cpp \
	RefCountingPointer.cpp \
	SAX2Attribute.cpp \
	SAX2Attributes.cpp \
	SAX2Namespaces.cpp \
	SAX2Parser.cpp \
	SchemaInfo.cpp \
	SdoCheck.cpp \
	SDODate.cpp \
	SdoRuntime.cpp \
	SDORuntimeException.cpp \
	SDOSAX2Parser.cpp \
	SDOSchemaSAX2Parser.cpp \
	SDOUtils.cpp \
	SDOXMLBufferWriter.cpp \
	SDOXMLFileWriter.cpp \
	SDOXMLStreamWriter.cpp \
	SDOXMLString.cpp \
	SDOXMLWriter.cpp \
	SDOXSDBufferWriter.cpp \
	SDOXSDFileWriter.cpp \
	SDOXSDStreamWriter.cpp \
	SDOXSDWriter.cpp \
	Sequence.cpp \
	SequenceImpl.cpp \
	Setting.cpp \
	SettingList.cpp \
	Type.cpp \
	TypeDefinition.cpp \
	TypeDefinitions.cpp \
	TypeDefinitionImpl.cpp \
	TypeDefinitionsImpl.cpp \
	TypeImpl.cpp \
	TypeList.cpp \
	XMLDocument.cpp \
	XMLDocumentImpl.cpp \
	XMLHelper.cpp \
	XMLHelperImpl.cpp \
	XMLQName.cpp \
	XpathHelper.cpp \
	XSDHelper.cpp \
	XSDHelperImpl.cpp \
	XSDPropertyInfo.cpp \
	XSDTypeInfo.cpp

libtuscany_sdo_la_LIBADD = -L${LIBXML2_LIB} -lxml2

INCLUDES = -I$(top_builddir)/runtime/core/src \
           -I${LIBXML2_INCLUDE}
                   
