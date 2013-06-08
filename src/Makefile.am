## Process this file with automake to produce Makefile.in

## Created by Anjuta


uidir = $(pkgdatadir)/ui
ui_DATA = cathy.ui \
	grizzlybear.0.png \
	gtk-new.svg \
	cathy.png


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir). \
	$(XMMS2_CLIENT_CPP_CFLAGS). \
	$(gtkmm-3.0_CFLAGS)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(CATHY_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = cathy

cathy_SOURCES = \
	main.cc \
	mainwin.cc \
	mainwin.h \
	list-view-format-text.cc \
	list-view-format-text.h \
	matrix-box.cc \
	matrix-box.h \
	basemesage.cc \
	basemesage.h \
	mesage.cc \
	mesage.h \
	dialog-new-playlist.cc \
	dialog-new-playlist.h \
	lvt.cc \
	lvt.h \
	dnc.cc \
	dnc.h \
	dd.cc \
	dd.h \
	dr.cc \
	dr.h

cathy_LDFLAGS = 

cathy_LDADD = $(CATHY_LIBS) \
	$(gtkmm-3.0_LIBS) \
	$(XMMS2_CLIENT_CPP_LIBS)


EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(pkgdatadir)

