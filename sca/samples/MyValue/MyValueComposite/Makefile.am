SUBDIRS = CustomerInfo MyValue

BUILT_SOURCES = scagen

scagen:
	cd $(top_builddir)/bin && \
	./scagen.sh -dir $(prefix)/samples/MyValue/MyValueComposite -output $(prefix)/samples/MyValue/MyValueComposite
	mv CustomerInfoImpl_* CustomerInfo
	mv MyValueImpl_* MyValue
	
