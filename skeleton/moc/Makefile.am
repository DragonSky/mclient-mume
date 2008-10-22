include $(top_srcdir)/aux/autotroll.mk
include @top_builddir@/Makedef

INCLUDES = -I@top_builddir@/ui
AM_LDFLAGS =
AM_LIBADD = 

noinst_LTLIBRARIES = libmoc.la

libmoc_la_SOURCES = moc_ClientWidget.cpp moc_ClientLineEdit.cpp \
					moc_ClientTextEdit.cpp moc_PowwowWrapper.cpp \
					moc_ClientSocket.cpp

libmoc_la_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
libmoc_la_LDFLAGS  = $(QT_LDFLAGS) $(AM_LDFLAGS)
libmoc_la_LIBADD   = $(QT_LIBS) $(AM_LIBADD)

clean:
	-rm -rf *.cpp
