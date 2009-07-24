#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <id3tag.h>
#include <vorbis/vorbisfile.h>
#include "files.h"

/* Extract tag from MP3 frame. This code closely follows
   the libid3tag example. */
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
    ucs4 = id3_field_getstrings(field, j);
    utf8 = id3_ucs4_utf8duplicate(ucs4);
   if (j == 0) {
      return utf8;
    }
    else {
      free(utf8);
    }
  }
  return NULL;
}


/* Initalize the MP3 file. Stores tag values in the media_file_tags
   structure. */
static int
initialize_mp3(const char *filename, struct media_file_tags *media_file_tags)
{
  struct id3_file *id3_file;  
  struct id3_tag *id3_tag;

  struct id3_frame *frame;
  id3_file = id3_file_open(filename, ID3_FILE_MODE_READONLY);
  if (!id3_file) {
    return 0;
  }
  id3_tag = id3_file_tag(id3_file);

  /* Store the tag, if it exists. Else store NULL, so that we can
     happily free these later. */
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

  id3_file_close(id3_file);

  return 1;
}

/* Release the string resources we have allocated. */
static int
free_media_tags(struct media_file_tags *media_file_tags)
{
  free(media_file_tags->title);
  free(media_file_tags->artist);
  free(media_file_tags->album);
  free(media_file_tags->track);
  free(media_file_tags->year);
  free(media_file_tags->genre);
  free(media_file_tags->comment);
  free(media_file_tags->encoded_by);
  return 1;
}
 

/* TODO */
static int
initialize_oggvorbis(const char *filename, struct media_file_tags *media_file_tags)
{
  OggVorbis_File oggv_file;
  int i;
  
  if (ov_fopen(filename, &oggv_file) < 0) {
    printf("ERROR opening Ogg-Vorbis file!\n");
    return 0;
  }

  for (i = 0; i < oggv_file.vc->comments; ++i) {
    if (!strncmp(oggv_file.vc->user_comments[i], "TITLE=", 6)) {
      media_file_tags->title = strdup(oggv_file.vc->user_comments[i] + 6);
    }
    else if (!strncmp(oggv_file.vc->user_comments[i], "ARTIST=", 7)) {
      media_file_tags->artist = strdup(oggv_file.vc->user_comments[i] + 7);
    }
    else if (!strncmp(oggv_file.vc->user_comments[i], "ALBUM=", 6)) {
      media_file_tags->album = strdup(oggv_file.vc->user_comments[i] + 6);
    }
    else if (!strncmp(oggv_file.vc->user_comments[i], "TRACK=", 6)) {
      media_file_tags->track = strdup(oggv_file.vc->user_comments[i] + 6);
    }
    else if (!strncmp(oggv_file.vc->user_comments[i], "YEAR=", 5)) {
      media_file_tags->year = strdup(oggv_file.vc->user_comments[i] + 5);
    }
    else if (!strncmp(oggv_file.vc->user_comments[i], "GENRE=", 6)) {
      media_file_tags->genre = strdup(oggv_file.vc->user_comments[i] + 6);
    }
    else if (!strncmp(oggv_file.vc->user_comments[i], "COMMENT=", 8)) {
      media_file_tags->comment = strdup(oggv_file.vc->user_comments[i] + 8);
    }
    else if (!strncmp(oggv_file.vc->user_comments[i], "ENCODED-BY=", 11)) {
      media_file_tags->encoded_by = strdup(oggv_file.vc->user_comments[i] + 11);
    }
  }
  ov_clear(&oggv_file);
  return 1;
}

/* Process one media file. */
int
processFile(const char *filename, struct tag_regexes *tag_regexes)
{
  const char *err;
  struct media_file_tags media_file_tags;
  const char *desc = magic_file(tag_regexes->magic_handle, filename);

  memset(&media_file_tags, 0, sizeof(media_file_tags));
  if (err = magic_error(tag_regexes->magic_handle)) {
    fprintf(stderr, "%s\n", err);
  }
  else if (strstr(magic_file(tag_regexes->magic_handle, filename), "MPEG ADTS, layer III")) {
    if (initialize_mp3(filename, &media_file_tags)) {
      free_media_tags(&media_file_tags);
    }
    else {
      fprintf(stderr, "Error reading file %s\n", filename);
    }
  }
  else if (strstr(magic_file(tag_regexes->magic_handle, filename), "Ogg data, Vorbis audio")) {
    if (initialize_oggvorbis(filename, &media_file_tags)) {
      free_media_tags(&media_file_tags);
    }
    else {
      fprintf(stderr, "Error reading file %s\n", filename);
    }
  }
  return 1;
}
