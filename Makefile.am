## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src po

cathydocdir = ${prefix}/doc/cathy
cathydoc_DATA = \
	README\
	README.md\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = $(cathydoc_DATA) \
			 cathy.desktop \
			 cathy.png \
	$(INTLTOOL_FILES)

pixmapdir = $(datadir)/pixmaps

pixmap_DATA = cathy.png


Applicationsdir = $(datadir)/applications
Applications_DATA = cathy.desktop



AM_CXXFLAGS = -std=c++20 

DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(cathydocdir)
