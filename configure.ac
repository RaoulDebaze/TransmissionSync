dnl convention: -TR MAJOR MINOR MAINT STATUS - (each a single char)
dnl STATUS: "X" for prerelease beta builds,
dnl         "Z" for unsupported trunk builds,
dnl         "0" for stable, supported releases
dnl these should be the only two lines you need to change
m4_define([user_agent_prefix],[0.00-])
m4_define([peer_id_prefix],[-SB001Z-])

AC_INIT([sibsync],[user_agent_prefix],[https://github.com/RaoulDebaze/TransmissionSync/issues])
AC_SUBST(USERAGENT_PREFIX,[user_agent_prefix])
AC_SUBST(PEERID_PREFIX,[peer_id_prefix])

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_SRCDIR(sibsync/sibsync.c)
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
LT_INIT

AM_GNU_GETTEXT([external])

AC_PROG_CC
AC_PROG_CXX
if test "x$CXX" != "x"; then # CXX is set...
  if test -f "$CXX"; then # maybe it's an absolute path passed in env variables...
    AC_MSG_CHECKING([for $CXX])
    HAVE_CXX="yes"
    AC_MSG_RESULT([$HAVE_CXX])
  else
    AC_CHECK_PROG([HAVE_CXX],[$CXX],[yes],[no])
  fi
fi
AC_C_INLINE

AC_HEADER_STDC
AC_HEADER_TIME


dnl ----------------------------------------------------------------------------
dnl
dnl  platform-specific stuff.

AC_CANONICAL_HOST
have_darwin="no"
have_msw="no"
case $host_os in

   *mingw32*)
     ;;

  *darwin*)
    have_darwin="yes"
    CFLAGS="-DMACOSX $CFLAGS"
    ;;

esac


dnl ----------------------------------------------------------------------------
dnl
dnl  Generate the output

AC_CONFIG_FILES([Makefile po/Makefile.in
                 sibsync/Makefile])

AC_OUTPUT

echo "

Configuration:

   Source code location:                              ${srcdir}
   Compiler:                                          ${CXX}

   Build Mac client:                                  ${build_mac}

"
