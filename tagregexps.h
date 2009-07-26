#ifndef TAGREGEXPS_H
#define TAGREGEXPS_H

#include <pcre.h>

pcre *
initialize_regexp(const char *regexp_str);

int
match_string(pcre *re, const char *subject);

#endif
