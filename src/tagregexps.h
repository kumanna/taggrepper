#ifndef TAGREGEXPS_H
#define TAGREGEXPS_H

#include "files.h"

pcre *
initialize_regexp(const char *regexp_str);

int
match_tag_regexps(struct media_file_tags *, struct tag_regexes *);

int
match_tag_regexps_any(struct media_file_tags *, pcre *);
#endif
