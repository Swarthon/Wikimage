AC_INIT([wikimage], [1.0], [bug-automake@gnu.org])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CHECK_FUNCS([strchr], [strstr])
AC_CHECK_HEADERS([stdlib.h], [string.h], [unistd.h])
PKG_CHECK_MODULES(MAGICKCORE, [MagickCore >= 6.8.6])
AC_SUBST(MAGICKCORE_CFLAGS)
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_PATH_X
AC_PROG_CXX
AC_TYPE_SIZE_T
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
AC_OUTPUT
