#ifndef TAGGREP_H
#define TAGGREP_H

#include <magic.h>

struct tag_regexes {
  char *title_regex;
  char *artist_regex;
  char *album_regex;
  char *year_regex;
  char *genre_regex;
  char *comment_regex;
  char *track_regex;
  char *composer_regex;
  char *orig_artist_regex;
  char *copyright_regex;
  char *url_regex;
  char *encoded_by_regex;
  magic_t magic_handle;
};


#endif
