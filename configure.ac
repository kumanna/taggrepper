AC_INIT([taggrepper],[0.05],[a.kumar@alumni.iitm.ac.in])
AM_INIT_AUTOMAKE([-Wall])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CHECK_HEADERS([stdlib.h string.h])
AC_CHECK_FUNCS([memset strcasecmp strstr])
AC_CONFIG_FILES([Makefile src/Makefile doc/Makefile])


dnl id3tag
PKG_CHECK_MODULES([id3tag], id3tag, [], AC_MSG_ERROR([could not find id3tag library in \$PATH]))
AC_DEFINE(HAVE_ID3TAG, 1, [Define to 1 if your system supports the id3tag library])
CFLAGS="$CFLAGS $id3tag_CFLAGS"
LIBS="$LIBS $id3tag_LIBS"

dnl pcre
PKG_CHECK_MODULES([pcre], libpcre, [], AC_MSG_ERROR([could not find pcre library in \$PATH]))
CFLAGS="$CFLAGS $pcre_CFLAGS"
LIBS="$LIBS $pcre_LIBS"

dnl vorbisfile
have_libvorbisfile=no
AC_ARG_WITH(libvorbisfile,
AS_HELP_STRING([--with-libvorbisfile],[use libvorbisfile library]),
[use_libvorbisfile=$withval],
[use_libvorbisfile=yes])

if test "$use_libvorbisfile" = "yes"; then
PKG_CHECK_MODULES([vorbisfile], vorbisfile, [], AC_MSG_ERROR([could not find vorbisfile library in \$PATH]))
AC_DEFINE(HAVE_LIBVORBISFILE, 1, [Define to 1 if your system supports libvorbisfile])
CFLAGS="$CFLAGS $vorbisfile_CFLAGS"
LIBS="$LIBS $vorbisfile_LIBS"
fi

dnl flac
have_libflac=no
AC_ARG_WITH(libflac,
AS_HELP_STRING([--with-libflac],[use libflac library]),
[use_libflac=$withval],
[use_libflac=yes])

if test "$use_libflac" = "yes"; then
PKG_CHECK_MODULES([FLAC], flac, [], AC_MSG_ERROR([could not find flac library in \$PATH]))
AC_DEFINE(HAVE_LIBFLAC, 1, [Define to 1 if your system supports libFLAC])
CFLAGS="$CFLAGS $FLAC_CFLAGS"
LIBS="$LIBS $FLAC_LIBS"
fi

dnl magic
have_libmagic=no
AC_ARG_WITH(libmagic,
AS_HELP_STRING([--with-libmagic],[use libmagic library]),
[use_libmagic=$withval],
[use_libmagic=yes])

if test "$use_libmagic" = "yes"; then
AC_CHECK_LIB(magic,magic_open,AC_DEFINE(HAVE_LIBMAGIC, 1, [Define to 1 if your system supports libmagic])
LIBS="$LIBS -lmagic"
have_libmagic=yes)
fi

AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)
AM_PROG_CC_C_O
AC_OUTPUT
