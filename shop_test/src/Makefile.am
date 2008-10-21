include $(top_srcdir)/aux/autotroll.mk
include @top_builddir@/Makedef

#AM_CPPFLAGS = ${ROOTFLAGS}
AM_CPPFLAGS=$(BOOST_CPPFLAGS)
#AM_LDFLAGS = -l$(PYTHON_LIB)

# For a program:
bin_PROGRAMS = shop tokenize
#noinst_HEADERS =
INCLUDES = -I@top_builddir@/ui
LDADD = @top_builddir@/moc/libmoc.la $(LIBS)

shop_SOURCES = shop.cpp 

shop_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
shop_LDFLAGS  = $(QT_LDFLAGS) $(AM_LDFLAGS)
shop_LDADD    = $(LDADD)

tokenize_SOURCES = tokenize.cpp

tokenize_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
tokenize_LDFLAGS  = $(QT_LDFLAGS) $(AM_LDFLAGS)
tokenize_LDADD    = $(LDADD)
