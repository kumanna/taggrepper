#ifndef FILES_H
#define FILES_H

#include "taggrep.h"

#define ID3_FRAME_ENCODED_BY "TENC"

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

int
processFile(const char *, struct tag_regexes *);

#endif
