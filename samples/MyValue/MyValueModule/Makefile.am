SUBDIRS = CustomerInfo MyValue

BUILT_SOURCES = scagen

scagen:
	cd $(top_builddir)/bin && \
	./scagen.sh -dir $(prefix)/samples/MyValue/MyValueModule -output $(prefix)/samples/MyValue/MyValueModule
	mv CustomerInfoImpl_* CustomerInfo
	mv MyValueImpl_* MyValue
	
