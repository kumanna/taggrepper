AC_INIT([taggrepper],[0.01],[a.kumar@alumni.iitm.ac.in])
AM_INIT_AUTOMAKE([-Wall])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CHECK_HEADERS([stdlib.h string.h])
AC_CHECK_FUNCS([memset strcasecmp strstr])
AC_CONFIG_FILES([Makefile src/Makefile doc/Makefile])


AC_CHECK_HEADERS([id3tag.h], AC_CHECK_LIB([id3tag], [id3_file_open], , [AC_MSG_ERROR(Can't find libid3tag)])
, [AC_MSG_ERROR(Can't find id3tag.h)])

AC_CHECK_HEADERS([pcre.h], AC_CHECK_LIB([pcre], [id3_file_open], , [AC_MSG_ERROR(Can't find libpcre)])
, [AC_MSG_ERROR(Can't find pcre.h)])


dnl vorbisfile
have_libvorbisfile=no
AC_ARG_WITH(libvorbisfile,
AS_HELP_STRING([--with-libvorbisfile],[use libvorbisfile library]),
[use_libvorbisfile=$withval],
[use_libvorbisfile=yes])

if test "$use_libvorbisfile" = "yes"; then
AC_CHECK_HEADER(vorbis/vorbisfile.h,
[AC_CHECK_LIB(vorbisfile,ov_fopen,[AC_DEFINE(HAVE_LIBVORBISFILE, 1, [Define to 1 if your system supports libvorbisfile])
LIBS="$LIBS -lvorbisfile -lvorbis -lm -logg"
have_libvorbisfile=yes])
])
fi

dnl magic
have_libmagic=no
AC_ARG_WITH(libmagic,
AS_HELP_STRING([--with-libmagic],[use libmagic library]),
[use_libmagic=$withval],
[use_libmagic=yes])

if test "$use_libmagic" = "yes"; then
AC_CHECK_HEADER(magic.h,
[AC_CHECK_LIB(magic,ov_fopen,[AC_DEFINE(HAVE_LIBMAGIC, 1, [Define to 1 if your system supports libmagic])
LIBS="$LIBS -lmagic"
have_libmagic=yes])
])
fi


AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)
AM_PROG_CC_C_O
AC_OUTPUT
