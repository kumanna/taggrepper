#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <id3tag.h>
#include <vorbis/vorbisfile.h>
#include <ftw.h>

#include "files.h"
#include "tagregexps.h"

enum TG_FILETYPE {TG_UNKNOWN = 0, TG_MP3, TG_OGGVORBIS};

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
initialize_mp3(const struct id3_file *id3_file, struct media_file_tags *media_file_tags)
{
  struct id3_tag *id3_tag;
  struct id3_frame *frame;

  id3_tag = id3_file_tag(id3_file);

  /* Store the tag, if it exists. Else store NULL, so that we can
     happily free these later. */
  frame = id3_tag_findframe(id3_tag, ID3_FRAME_TITLE, 0);   
  media_file_tags->title = (frame) ? (char *)mp3tag_from_frame(frame) : NULL;

  frame = id3_tag_findframe(id3_tag, ID3_FRAME_ARTIST, 0);   
  media_file_tags->artist = frame ? (char *)mp3tag_from_frame(frame) : NULL;

  frame = id3_tag_findframe(id3_tag, ID3_FRAME_ALBUM, 0);   
  media_file_tags->album = frame ? (char *)mp3tag_from_frame(frame) : NULL;

  frame = id3_tag_findframe(id3_tag, ID3_FRAME_TRACK, 0);   
  media_file_tags->track = frame ? (char *)mp3tag_from_frame(frame) : NULL;

  frame = id3_tag_findframe(id3_tag, ID3_FRAME_YEAR, 0);   
  media_file_tags->year = frame ? (char *)mp3tag_from_frame(frame) : NULL;

  frame = id3_tag_findframe(id3_tag, ID3_FRAME_GENRE, 0);   
  media_file_tags->genre = frame ? (char *)mp3tag_from_frame(frame) : NULL;

  frame = id3_tag_findframe(id3_tag, ID3_FRAME_COMMENT, 0);   
  media_file_tags->comment = frame ? (char *)mp3tag_from_frame(frame) : NULL;

  frame = id3_tag_findframe(id3_tag, ID3_FRAME_ENCODED_BY, 0);   
  media_file_tags->encoded_by = frame ? (char *)mp3tag_from_frame(frame) : NULL;

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
initialize_oggvorbis(const OggVorbis_File *oggv_file, struct media_file_tags *media_file_tags)
{
  int i;
  
  for (i = 0; i < oggv_file->vc->comments; ++i) {
    if (!strncmp(oggv_file->vc->user_comments[i], "TITLE=", 6)) {
      media_file_tags->title = oggv_file->vc->user_comments[i] + 6;
    }
    else if (!strncmp(oggv_file->vc->user_comments[i], "ARTIST=", 7)) {
      media_file_tags->artist = oggv_file->vc->user_comments[i] + 7;
    }
    else if (!strncmp(oggv_file->vc->user_comments[i], "ALBUM=", 6)) {
      media_file_tags->album = oggv_file->vc->user_comments[i] + 6;
    }
    else if (!strncmp(oggv_file->vc->user_comments[i], "TRACK=", 6)) {
      media_file_tags->track = oggv_file->vc->user_comments[i] + 6;
    }
    else if (!strncmp(oggv_file->vc->user_comments[i], "YEAR=", 5)) {
      media_file_tags->year = oggv_file->vc->user_comments[i] + 5;
    }
    else if (!strncmp(oggv_file->vc->user_comments[i], "GENRE=", 6)) {
      media_file_tags->genre = oggv_file->vc->user_comments[i] + 6;
    }
    else if (!strncmp(oggv_file->vc->user_comments[i], "COMMENT=", 8)) {
      media_file_tags->comment = oggv_file->vc->user_comments[i] + 8;
    }
    else if (!strncmp(oggv_file->vc->user_comments[i], "ENCODED-BY=", 11)) {
      media_file_tags->encoded_by = oggv_file->vc->user_comments[i] + 11;
    }
  }
  return 1;
}

static enum TG_FILETYPE
detect_filetype_extension(const char *filename)
{
  /* Fall back to file extension based detection. */
  const char *extension;
  int len;
  len = strlen(filename);
  extension = filename + len - 4;
  if (!strcasecmp(extension, ".mp3")) {
    return TG_MP3;
  }
  else if (!strcasecmp(extension, ".ogg")) {
    return TG_OGGVORBIS;
  }
  return TG_UNKNOWN;

}

#ifdef HAVE_LIBMAGIC
static enum TG_FILETYPE
detect_filetype(const char *filename, magic_t magic_handle)
{
  /* Check if we are doing smart detection (magic) or file extension
     based detection. */
  const char *err;
  if (magic_handle) {
    const char *desc = magic_file(magic_handle, filename);    
    if ((err = magic_error(magic_handle))) {
      fprintf(stderr, "%s\n", err);
      return TG_UNKNOWN;
    }
    if (strstr(desc, "MPEG ADTS, layer III")) {
      return TG_MP3;
    }
    else if (strstr(desc, "Ogg data, Vorbis audio")) {
      return TG_OGGVORBIS;
    }
    else {
      return TG_UNKNOWN;
    }
  }
  return detect_filetype_extension(filename);
}
#endif

/* Process one media file. */
int
processFile(const char *filename, struct tag_regexes *tag_regexes)
{
  struct media_file_tags media_file_tags;
  enum TG_FILETYPE file_type;

  memset(&media_file_tags, 0, sizeof(media_file_tags));
#ifdef HAVE_LIBMAGIC
  file_type = detect_filetype(filename, tag_regexes->magic_handle);
#else
  file_type = detect_filetype_extension(filename);
#endif

  if (file_type == TG_MP3) {
    struct id3_file *id3_file = id3_file_open(filename, ID3_FILE_MODE_READONLY);
    if (id3_file) {
      initialize_mp3(id3_file, &media_file_tags);
      if (match_tag_regexps(&media_file_tags, tag_regexes)) {
	printf("%s\n", filename);
      }
      free_media_tags(&media_file_tags);
      id3_file_close(id3_file);
    }
    else {
      fprintf(stderr, "Error reading file %s\n", filename);
    }
  }
  else if (file_type == TG_OGGVORBIS) {
    OggVorbis_File oggv_file;
    if (ov_fopen(filename, &oggv_file) >= 0) {
      initialize_oggvorbis(&oggv_file, &media_file_tags);
      if (match_tag_regexps(&media_file_tags, tag_regexes)) {
	printf("%s\n", filename);
      }
      /* We don't free the media tags, since they just point to the
         strings in the OggVorbis_File structure. */
      ov_clear(&oggv_file);
    }
    else {
      fprintf(stderr, "Error reading file %s\n", filename);
    }
  }

  return 1;
}

/* We copy the regular expressions, since FTW's callback function
   cannot receive any custon data. */
static struct tag_regexes *
tag_regexes_copy;

static int
process_file_wrapper(const char *filename, const struct stat *sb, int tflag)
{
  if (tflag == FTW_F || tflag == FTW_SL) {
    processFile(filename, tag_regexes_copy);
  }
  return 0;
}

int
processFile_recursive(const char *filename, struct tag_regexes *tag_regexes)
{
  tag_regexes_copy = tag_regexes;
  if (ftw(filename, process_file_wrapper, 20) == -1) {
    fprintf(stderr, "Error traversing %s\n", filename);
  }
  return 1;
}
