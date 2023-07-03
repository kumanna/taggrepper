/*
 * Copyright (c) 2009, 2012, Kumar Appaiah
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of the author nor the names of the contributors may
 *   be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <string.h>
#include "tagregexps.h"

/* Initialize and return a regular expression */
pcre2_code *
initialize_regexp(const char *regexp_str)
{
  pcre2_code *re;
  const char *error;
  int errorcode;
  PCRE2_SIZE offset;
  uint32_t options = PCRE2_UTF | PCRE2_UCP;

  re = pcre2_compile((PCRE2_SPTR)regexp_str, PCRE2_ZERO_TERMINATED, options, &errorcode, &offset, NULL);
  if (re) {
    return re;
  }

  PCRE2_UCHAR buffer[256];
  pcre2_get_error_message(errorcode, buffer, sizeof(buffer));
  fprintf(stderr, "ERROR: compiling regular expression: %s\n", buffer);
  return NULL;
}

/* Match a string against a regular expression */
static int
match_string(pcre2_code *re, const char *subject)
{
  int rc;
  pcre2_match_data *match_data;

  match_data = pcre2_match_data_create_from_pattern(re, NULL);

  /* If the regular expression was not initialized, succeed. */
  if (!re) {
    return 1;
  }
  /* but if the string is blank, fail */
  if (!subject) {
    return 0;
  }

  rc = pcre2_match(re, (PCRE2_SPTR) subject, strlen(subject), 0, 0, match_data, NULL);
  pcre2_match_data_free(match_data);
  return (rc >= 0 && rc != PCRE2_ERROR_NOMATCH);
}

int
match_tag_regexps_any(struct media_file_tags *media_file_tags, pcre2_code *any_tag_regex){
  return match_string(any_tag_regex, media_file_tags->title) || \
    match_string(any_tag_regex, media_file_tags->artist) || \
    match_string(any_tag_regex, media_file_tags->album) ||	\
    match_string(any_tag_regex, media_file_tags->year) ||	\
    match_string(any_tag_regex, media_file_tags->genre) ||	\
    match_string(any_tag_regex, media_file_tags->comment) || \
    match_string(any_tag_regex, media_file_tags->track) ||	\
    match_string(any_tag_regex, media_file_tags->composer) || \
    match_string(any_tag_regex, media_file_tags->orig_artist) ||	\
    match_string(any_tag_regex, media_file_tags->copyright) || \
    match_string(any_tag_regex, media_file_tags->url) ||	\
    match_string(any_tag_regex, media_file_tags->encoded_by);
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
