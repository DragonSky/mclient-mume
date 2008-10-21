AC_INIT([shop_test],[1.0])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([aux])
AM_INIT_AUTOMAKE([shop_test], 1.0)

AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

AC_LANG([C])

AT_WITH_QT([xml network gui],[qt warn_off debug],
            [MOC_DIR = ./moc
            OBJECTS_DIR = ./obj
            UI_DIR = ./ui])

AX_BOOST_BASE([1.33.1])
AX_BOOST_PYTHON

AC_PREFIX_DEFAULT(`pwd`)

AC_CONFIG_FILES([Makefile
                 Makedef
                 ui/Makefile
                 moc/Makefile
                 src/Makefile])

AC_OUTPUT
