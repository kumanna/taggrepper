#ifndef FILES_H
#define FILES_H

#include "common.h"

#define ID3_FRAME_ENCODED_BY "TENC"

int
processFile(const char *, struct tag_regexes *);

#endif
