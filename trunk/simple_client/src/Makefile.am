include $(top_srcdir)/aux/autotroll.mk
include @top_builddir@/Makedef

#AM_CPPFLAGS = ${ROOTFLAGS}
AM_CPPFLAGS=$(BOOST_CPPFLAGS)
#AM_LDFLAGS = -l$(PYTHON_LIB)

# For a program:
bin_PROGRAMS = simple
#noinst_HEADERS =
INCLUDES = -I@top_builddir@/ui -I$(PYTHON_INCLUDE_DIR)
LDADD = @top_builddir@/moc/libmoc.la $(LIBS) -l$(PYTHON_LIB)

simple_SOURCES = main.cpp ClientWidget.cpp ClientLineEdit.cpp \
				 ClientTextEdit.cpp PowwowWrapper.cpp ClientSocket.cpp

simple_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS)
simple_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
simple_LDFLAGS  = $(QT_LDFLAGS) $(AM_LDFLAGS)
simple_LDADD    = $(LDADD)
