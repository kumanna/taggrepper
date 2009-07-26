#ifndef TAGGREP_H
#define TAGGREP_H

#include <magic.h>
#include "tagregexps.h"

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
  magic_t magic_handle;
};


#endif
