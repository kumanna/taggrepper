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
static int
match_string(pcre *re, const char *subject)
{
  /* ovector's length is three since we are just interested in the
     first match. */
  int ovector[3];
  int rc;

  /* If the regular expression was not initialized, succeed. */
  if (!re) {
    return 1;
  }
  /* but if the string is blank, fail */
  if (!subject) {
    return 0;
  }

  rc = pcre_exec(re, NULL, subject, strlen(subject), 0, 0, ovector, 3);
  return (rc >= 0 && rc != PCRE_ERROR_NOMATCH);
}

int
match_tag_regexps(struct media_file_tags *media_file_tags, struct tag_regexes *tag_regexes) {
  return match_string(tag_regexes->title_regex, media_file_tags->title) && \
    match_string(tag_regexes->artist_regex, media_file_tags->artist) && \
    match_string(tag_regexes->album_regex, media_file_tags->album) &&	\
    match_string(tag_regexes->year_regex, media_file_tags->year) &&	\
    match_string(tag_regexes->genre_regex, media_file_tags->genre) &&	\
    match_string(tag_regexes->comment_regex, media_file_tags->comment) && \
    match_string(tag_regexes->track_regex, media_file_tags->track) &&	\
    match_string(tag_regexes->composer_regex, media_file_tags->composer) && \
    match_string(tag_regexes->orig_artist_regex, media_file_tags->orig_artist) &&	\
    match_string(tag_regexes->copyright_regex, media_file_tags->copyright) && \
    match_string(tag_regexes->url_regex, media_file_tags->url) &&	\
    match_string(tag_regexes->encoded_by_regex, media_file_tags->encoded_by);
}
