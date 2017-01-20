
dnl Process this file with autoconf to produce a configure script.
AC_INIT(ez-ipupdate.c)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(ez-ipupdate,3.0.10)


dnl Checks for programs.

AC_ARG_PROGRAM
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL

AC_TYPE_SIGNAL

dnl Checks for libraries.

AC_CHECK_FUNC(gethostbyname)
if test $ac_cv_func_gethostbyname = no; then
    AC_CHECK_LIB(nsl, gethostbyname)
fi
AC_CHECK_FUNC(connect)
if test $ac_cv_func_connect = no; then
    AC_CHECK_LIB(socket, connect)
fi

AC_CHECK_FUNCS( socket \
		strdup \
		getopt_long \
                gethostbyaddr \
	        getservbyname \
	        inet_addr \
		inet_ntoa \
		snprintf \
		vfprintf \
                stat \
		vsprintf \
		vsnprintf \
		strerror \
		strftime \
		wait \
		waitpid \
		getpid \
		fork \
		vfork \
		inet_aton \
		herror )

dnl Checks for header files.
AC_CHECK_HEADERS( arpa/inet.h \
		  sys/types.h \
		  sys/time.h \
		  sys/stat.h \
		  fcntl.h \
		  signal.h \
		  syslog.h \
		  pwd.h \
		  stdarg.h \
		  grp.h \
		  errno.h \
		  sys/sockio.h \
		  sys/wait.h \
		  getopt.h )
AC_CHECK_HEADERS( unistd.h \
		  netinet/in.h \
		  netdb.h \
		  sys/socket.h
                  ,,
                  AC_MSG_ERROR(could not locate neccessary system header files) )

AC_CHECK_LIB(c, sys_errlist, AC_DEFINE(HAVE_SYS_ERRLIST))

dnl you need at least to have getopt, but getopt_long will be used if it
dnl is present
AC_CHECK_FUNCS(getopt)
if test "$ac_cv_func_getopt" != yes; then
  AC_MSG_ERROR(getopt is needed for this program to work)
fi

AC_CHECK_FUNC(getpass, AC_DEFINE(HAVE_GETPASS),
    [EXTRASRC="$EXTRASRC \$(srcdir)/getpass.c"]
    [EXTRAOBJ="$EXTRAOBJ \$(srcdir)/getpass.o"] )

dnl Get system canonical name
AC_CANONICAL_HOST
AC_DEFINE_UNQUOTED(OS, "${host}")

dnl allow selection of the default service
dnl the default is to use ez-ip
AC_MSG_CHECKING(for user supplied default service)
AC_ARG_ENABLE(default-service,
              [  --enable-default-service=SERVICE
                          the default service type to use
                          possibilities are: ezip, pgpow, justlinux, dhs,
                          dyndns, dyndns-static, ods, tzo, gnudip, easydns, dyns, hn, zoneedit],
	      [ use_SERVICE=$enableval;
		AC_MSG_RESULT(yes) ],
	      [ AC_MSG_RESULT(no) 
                use_SERVICE=null
                AC_MSG_WARN(not setting default service) ]   )
case "$use_SERVICE" in
  ezip ) AC_DEFINE(DEF_SERVICE, EZIP_service);;
  ez-ip ) AC_DEFINE(DEF_SERVICE, EZIP_service);;
  pgpow ) AC_DEFINE(DEF_SERVICE, PGPOW_service);;
  penguinpowered ) AC_DEFINE(DEF_SERVICE, PGPOW_service);;
  justlinux ) AC_DEFINE(DEF_SERVICE, JUSTL_service);;
  dhs ) AC_DEFINE(DEF_SERVICE, DHS_service);;
  dyndns ) AC_DEFINE(DEF_SERVICE, DYNDNS_service);;
  dyndns-stat ) AC_DEFINE(DEF_SERVICE, DYNDNS_STAT_service);;
  dyndns-static ) AC_DEFINE(DEF_SERVICE, DYNDNS_STAT_service);;
  ods ) AC_DEFINE(DEF_SERVICE, ODS_service);;
  tzo ) AC_DEFINE(DEF_SERVICE, TZO_service);;
  gnudip ) AC_DEFINE(DEF_SERVICE, GNUDIP_service);;
  easydns ) AC_DEFINE(DEF_SERVICE, EASYDNS_service);;
  dyns ) AC_DEFINE(DEF_SERVICE, DYNS_service);;
  hn ) AC_DEFINE(DEF_SERVICE, HN_service);;
  zoneedit ) AC_DEFINE(DEF_SERVICE, ZONEEDIT_service);;
  null ) AC_DEFINE(DEF_SERVICE, NULL_service);;
  "" ) ;;
  * ) AC_MSG_ERROR(unknown default service type);;
esac

dnl check weather we want debugging support
dnl the default is to NOT use debugging support
AC_MSG_CHECKING(whether user wants debugging support)
AC_ARG_ENABLE(debug,
	      [  --enable-debug          include support for debugging],
	      [ AC_DEFINE(DEBUG)
		AC_MSG_RESULT(yes) ],
	      [ AC_MSG_RESULT(no) ]   )

dnl check weather we want to disable MD5 support
AC_MSG_CHECKING(whether user wants to dissable MD5 support)
AC_ARG_ENABLE(md5,
	      [  --disable-md5           disable MD5 support],
	      [ AC_MSG_RESULT(yes) ],
	      [ AC_DEFINE(USE_MD5)
                AC_MSG_RESULT(no) ]   )

AC_SUBST(EXTRASRC)
AC_SUBST(EXTRAOBJ)

AC_OUTPUT(Makefile)

