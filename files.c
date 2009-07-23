#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <id3tag.h>
#include <vorbis/vorbisfile.h>
#include "files.h"

static id3_utf8_t
*mp3tag_from_frame(struct id3_frame *frame)
{
  union id3_field const *field;
  id3_ucs4_t const *ucs4;
  id3_utf8_t *utf8;
  unsigned int nstrings, j;
  field = id3_frame_field(frame, 1);
  nstrings = id3_field_getnstrings(field);
  for (j = 0; j < nstrings; ++j) {
    id3_latin1_t *latin1;
    ucs4 = id3_field_getstrings(field, j);
    utf8 = id3_ucs4_utf8duplicate(ucs4);
    latin1 = id3_ucs4_latin1duplicate(ucs4);
    free(latin1);
   if (j == 0) {
      return utf8;
    }
    else {
      free(utf8);
    }
  }
  return NULL;
}


static struct id3_file *
initialize_mp3(const char *filename, struct media_file_tags *media_file_tags)
{
  struct id3_file *id3_file;  
  struct id3_tag *id3_tag;

  struct id3_frame *frame;
  id3_file = id3_file_open(filename, ID3_FILE_MODE_READONLY);
  if (!id3_file) {
    printf("Failure :-(.\n");
    return 0;
  }
  id3_tag = id3_file_tag(id3_file);
  frame = id3_tag_findframe(id3_tag, ID3_FRAME_TITLE, 0);   
  media_file_tags->title = (frame) ? mp3tag_from_frame(frame) : NULL;

  frame = id3_tag_findframe(id3_tag, ID3_FRAME_ARTIST, 0);   
  media_file_tags->artist = frame ? mp3tag_from_frame(frame) : NULL;

  frame = id3_tag_findframe(id3_tag, ID3_FRAME_ALBUM, 0);   
  media_file_tags->album = frame ? mp3tag_from_frame(frame) : NULL;

  frame = id3_tag_findframe(id3_tag, ID3_FRAME_TRACK, 0);   
  media_file_tags->track = frame ? mp3tag_from_frame(frame) : NULL;

  frame = id3_tag_findframe(id3_tag, ID3_FRAME_YEAR, 0);   
  media_file_tags->year = frame ? mp3tag_from_frame(frame) : NULL;

  frame = id3_tag_findframe(id3_tag, ID3_FRAME_GENRE, 0);   
  media_file_tags->genre = frame ? mp3tag_from_frame(frame) : NULL;

  frame = id3_tag_findframe(id3_tag, ID3_FRAME_COMMENT, 0);   
  media_file_tags->comment = frame ? mp3tag_from_frame(frame) : NULL;

  frame = id3_tag_findframe(id3_tag, ID3_FRAME_ENCODED_BY, 0);   
  media_file_tags->encoded_by = frame ? mp3tag_from_frame(frame) : NULL;

  return id3_file;
}

static int
close_mp3(struct id3_file *id3_file, struct media_file_tags *media_file_tags)
{
  free(media_file_tags->title);
  free(media_file_tags->artist);
  free(media_file_tags->album);
  free(media_file_tags->track);
  free(media_file_tags->year);
  free(media_file_tags->genre);
  free(media_file_tags->comment);
  free(media_file_tags->encoded_by);
  id3_file_close(id3_file);
  return 1;
}
 

int
display_oggvorbis_title(const char *filename)
{
  OggVorbis_File oggv_file;
  int i;
  
  if (ov_fopen(filename, &oggv_file) < 0) {
    printf("ERROR opening Ogg-Vorbis file!\n");
    return 0;
  }

  for (i = 0; i < oggv_file.vc->comments; ++i) {
    printf("%s\n", oggv_file.vc->user_comments[i]);
  }
  ov_clear(&oggv_file);
  return 1;
}

int
processFile(const char *filename, struct tag_regexes *tag_regexes)
{
  const char *err;
  struct media_file_tags media_file_tags;
  const char *desc = magic_file(tag_regexes->magic_handle, filename);

  if (err = magic_error(tag_regexes->magic_handle)) {
    printf("%s\n", err);
  }
  else if (strstr(magic_file(tag_regexes->magic_handle, filename), "MPEG ADTS, layer III")) {
    struct id3_file *id3_file = initialize_mp3(filename, &media_file_tags);
    close_mp3(id3_file, &media_file_tags);
  }
  else if (strstr(magic_file(tag_regexes->magic_handle, filename), "Ogg data, Vorbis audio")) {
    display_oggvorbis_title(filename);
  }
  return 1;
}
