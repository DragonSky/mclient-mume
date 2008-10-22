include $(top_srcdir)/aux/autotroll.mk
include @top_builddir@/Makedef

AM_CPPFLAGS=
AM_LDFLAGS = 
AM_LIBADD = @top_builddir@/moc/libmoc.la $(LIBS)

# For a program:
bin_PROGRAMS = simple
#noinst_HEADERS =
INCLUDES = -I@top_builddir@/ui

simple_SOURCES = main.cpp ClientWidget.cpp ClientLineEdit.cpp \
				 ClientTextEdit.cpp PowwowWrapper.cpp ClientSocket.cpp

simple_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
simple_LDFLAGS  = $(QT_LDFLAGS) $(AM_LDFLAGS)
simple_LDADD    = $(AM_LIBADD)
