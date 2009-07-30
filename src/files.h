#ifndef FILES_H
#define FILES_H

#include "common.h"

#define ID3_FRAME_ENCODED_BY "TENC"

int
process_file(const char *, struct tag_regexes *, struct aux_params *);

int
process_file_recursive(const char *filename, struct tag_regexes *tag_regexes, struct aux_params *);
#endif
