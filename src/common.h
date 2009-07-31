#ifndef COMMON_H
#define COMMON_H

#include "config.h"
#ifdef HAVE_LIBMAGIC
#include <magic.h>
#endif
#include <pcre.h>

struct media_file_tags {
  char *title;
  char *artist;
  char *album;
  char *year;
  char *genre;
  char *comment;
  char *track;
  char *composer;
  char *orig_artist;
  char *copyright;
  char *url;
  char *encoded_by;
};

struct tag_regexes {
  pcre *title_regex;
  pcre *artist_regex;
  pcre *album_regex;
  pcre *year_regex;
  pcre *genre_regex;
  pcre *comment_regex;
  pcre *track_regex;
  pcre *composer_regex;
  pcre *orig_artist_regex;
  pcre *copyright_regex;
  pcre *url_regex;
  pcre *encoded_by_regex;
  pcre *any_tag_regex;
};

struct aux_params {
  /* delimiter char used to end file names */
  char delimiter;
#ifdef HAVE_LIBMAGIC
  /* Use the magic library, if available */
  magic_t magic_handle;
#endif
  int display_title;
  int display_artist;
  int display_album;
  int display_year;
  int display_genre;
  int display_comment;
  int display_track;
  int display_composer;
  int display_orig_artist;
  int display_copyright;
  int display_url;
  int display_encoded_by;
  int any_tag; /* do we check all tags? */
};

#define VERSION_STRING ("%s %s\nCopyright (C) Kumar Appaiah\nLicense: BSD License (see COPYING file in the distribution tarball for details)\n")
#endif
