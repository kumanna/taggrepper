#include "tagregexps.h"

pcre *
initialize_regexp(const char *regexp_str)
{
  pcre *re;
  const char *error;
  int erroroffset;

  re = pcre_compile(regexp_str, PCRE_UTF8, &error, &erroroffset, NULL);
  if (re) {
    return re;
  }

  fprintf("ERROR: compiling regular expression at offset %d: %s\n", erroroffset, error);
  return NULL;
}
