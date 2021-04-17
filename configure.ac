dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(cathy, 4.6.4)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AC_PROG_CXX




dnl ***************************************************************************
dnl Internationalization
dnl ***************************************************************************
IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=cathy
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
AM_GLIB_GNU_GETTEXT



LT_INIT



PKG_CHECK_MODULES(FMT,fmt)

PKG_CHECK_MODULES(XMMS2_CLIENT_CPP,xmms2-client-cpp-glib
xmms2-client-cpp)

PKG_CHECK_MODULES(GLIB,xmms2-client-cpp-glib
xmms2-client-cpp
glib-2.0)

PKG_CHECK_MODULES(CATHY, [gtkmm-3.0 ]
xmms2-client-cpp-glib
xmms2-client-cpp
glib-2.0)




AC_OUTPUT([
Makefile
src/Makefile
po/Makefile.in
])
