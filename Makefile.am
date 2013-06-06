## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src po

cathydocdir = ${prefix}/doc/cathy
cathydoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = $(cathydoc_DATA) \
	$(INTLTOOL_FILES)

AM_CXXFLAGS = -std=c++11 

DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(cathydocdir)
