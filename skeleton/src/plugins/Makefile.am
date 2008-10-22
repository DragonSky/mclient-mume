include $(top_srcdir)/aux/autotroll.mk
include @top_builddir@/Makedef

AM_CPPFLAGS=-DQT_PLUGIN
AM_LDFLAGS = -version-info 0:0:0 

lib_LTLIBRARIES = liboutputplugin.la
#noinst_HEADERS =
INCLUDES = -I@top_builddir@/ui
AM_LIBADD = @top_builddir@/moc/libmoc.la $(LIBS)

liboutputplugin_la_SOURCES = main.cpp ClientW

liboutputplugin_la_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
liboutputplugin_la_LDFLAGS  = $(QT_LDFLAGS) $(AM_LDFLAGS)
liboutputplugin_la_LIBADD    = $(AM_LIBADD)
