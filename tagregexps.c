#include <stdio.h>
#include <string.h>
#include "tagregexps.h"

/* Initialize and return a regular expression */
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

  fprintf(stderr, "ERROR: compiling regular expression at offset %d: %s\n", erroroffset, error);
  return NULL;
}

/* Match a string against a regular expression */
int
match_string(pcre *re, const char *subject)
{
  /* ovector's length is three since we are just interested in the
     first match. */
  int ovector[3];
  int rc;
  rc = pcre_exec(re, NULL, subject, strlen(subject), 0, 0, ovector, 3);
  return (rc >= 0 && rc != PCRE_ERROR_NOMATCH);
}
