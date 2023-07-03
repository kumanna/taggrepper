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

#ifndef COMMON_H
#define COMMON_H

#include "config.h"
#ifdef HAVE_LIBMAGIC
#include <magic.h>
#endif
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

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
  pcre2_code *title_regex;
  pcre2_code *artist_regex;
  pcre2_code *album_regex;
  pcre2_code *year_regex;
  pcre2_code *genre_regex;
  pcre2_code *comment_regex;
  pcre2_code *track_regex;
  pcre2_code *composer_regex;
  pcre2_code *orig_artist_regex;
  pcre2_code *copyright_regex;
  pcre2_code *url_regex;
  pcre2_code *encoded_by_regex;
  pcre2_code *any_tag_regex;
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
