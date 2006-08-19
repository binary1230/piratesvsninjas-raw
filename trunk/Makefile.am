SUBDIRS = src
EXTRA_DIST = docs 

dist_common:
# make -C libs/alogg clean

doc: src/Doxyfile
	cd src && doxygen

test: $(DISTFILES)
	src/ninjeditor
