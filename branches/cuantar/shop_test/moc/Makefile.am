include $(top_srcdir)/aux/autotroll.mk
include @top_builddir@/Makedef

INCLUDES = -I@top_builddir@/ui 

noinst_LTLIBRARIES = libmoc.la

libmoc_la_SOURCES = 

libmoc_la_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS)
libmoc_la_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
libmoc_la_LDFLAGS  = $(QT_LDFLAGS) $(AM_LDFLAGS)
libmoc_la_LIBADD   = $(QT_LIBS) $(LDADD)

clean:
	-rm -rf *.cpp
