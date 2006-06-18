SUBDIRS = 

BUILT_SOURCES = scagen_build
EXTRA_DIST = build.xml scagen.sh scagen.cmd src
bin_SCRIPTS = scagen.sh scagen.cmd

scagen_build:
	ant 
	touch scagen_build
	
clean:
	rm -f scagen_build
	                    
install-exec-hook:
	cp bld/scagen.jar $(bindir)
	
	 
    

