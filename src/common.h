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
#ifdef HAVE_LIBMAGIC
  magic_t magic_handle;
#endif
};


#endif
