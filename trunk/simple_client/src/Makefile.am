include $(top_srcdir)/aux/autotroll.mk
include @top_builddir@/Makedef

#AM_CPPFLAGS = ${ROOTFLAGS}
#AM_LDFLAGS =-L$(QTGUI_LIB)

# For a program:
bin_PROGRAMS = simple
#noinst_HEADERS =
INCLUDES = $(ROOTINC) -I@top_builddir@/ui -I$(QTGUI_INC)
LDADD = @top_builddir@/moc/libmoc.la $(LIBS)

simple_SOURCES = main.cpp ClientWidget.cpp ClientLineEdit.cpp \
				 ClientTextEdit.cpp PowwowWrapper.cpp ClientSocket.cpp

simple_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS)
simple_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
simple_LDFLAGS  = $(QT_LDFLAGS) $(AM_LDFLAGS) $(ROOTLDFLAGS)
simple_LDADD    = $(LDADD)
