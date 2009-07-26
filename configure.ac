AC_INIT([taggrepper],[0.01],[a.kumar@alumni.iitm.ac.in])
AM_INIT_AUTOMAKE([-Wall])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CHECK_HEADERS([stdlib.h string.h])
AC_CHECK_FUNCS([memset strcasecmp strstr])
AC_CONFIG_FILES([Makefile src/Makefile])

dnl id3tag
have_libid3tag=no
AC_ARG_WITH(libid3tag,

AS_HELP_STRING([--with-libid3tag],[use libid3tag library]),
[use_libid3tag=$withval],
[use_libid3tag=yes])

if test "$use_libid3tag" = "yes"; then
AC_CHECK_HEADER(id3tag.h,
[AC_CHECK_LIB(id3tag,id3_file_open,[AC_DEFINE(HAVE_LIBID3TAG, 1, [Define to 1 if your system supports libid3tag])
LIBS="$LIBS -lid3tag"
have_libid3tag=yes ])
])
fi

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
have_libvorbisfile=yes ])
])
fi

dnl pcre
have_libpcre=no
AC_ARG_WITH(libpcre,

AS_HELP_STRING([--with-libpcre],[use libpcre library]),
[use_libpcre=$withval],
[use_libpcre=yes])

if test "$use_libpcre" = "yes"; then
AC_CHECK_HEADER(pcre.h,
[AC_CHECK_LIB(pcre,pcre_compile,[AC_DEFINE(HAVE_LIBPCRE, 1, [Define to 1 if your system supports libpcre])
LIBS="$LIBS -lpcre"
have_libpcre=yes ])
])
fi

dnl libmagic
have_libmagic=no
AC_ARG_WITH(libmagic,

AS_HELP_STRING([--with-libmagic],[use libmagic library]),
[use_libmagic=$withval],
[use_libmagic=yes])

if test "$use_libmagic" = "yes"; then
AC_CHECK_HEADER(magic.h,
[AC_CHECK_LIB(magic,magic_open,[AC_DEFINE(HAVE_LIBMAGIC, 1, [Define to 1 if your system supports libmagic])
LIBS="$LIBS -lmagic"
have_libmagic=yes ])
])
fi

AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)
AM_PROG_CC_C_O
AC_OUTPUT