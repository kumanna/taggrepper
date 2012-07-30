/*
 * Copyright (c) 2009, Kumar Appaiah
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
#include <stdlib.h>
#include <id3tag.h>
#include <ftw.h>

#include "files.h"
#include "tagregexps.h"

#ifdef HAVE_LIBVORBISFILE
#include <vorbis/vorbisfile.h>
#endif
#ifdef HAVE_LIBFLAC
#include <metadata.h>
#endif

enum TG_FILETYPE {TG_UNKNOWN = 0, TG_MP3, TG_OGGVORBIS, TG_FLAC};

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

/* Comments are extracted separately. */
static id3_utf8_t
*mp3comment_from_frame(struct id3_frame *frame, struct id3_tag *tag)
{
  union id3_field const *field;
  id3_ucs4_t const *ucs4;
  id3_utf8_t *utf8;
  unsigned int nstrings, j;
  int i = 0;

  while ((frame = id3_tag_findframe(tag, ID3_FRAME_COMMENT, i++))) {
    ucs4 = id3_field_getstring(id3_frame_field(frame, 2));

    if (*ucs4)
      continue;

    ucs4 = id3_field_getfullstring(id3_frame_field(frame, 3));

    utf8 = id3_ucs4_utf8duplicate(ucs4);
    if (!utf8)
      return NULL;

    return utf8;
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
  media_file_tags->comment = frame ? (char *)mp3comment_from_frame(frame, id3_tag) : NULL;

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


#ifdef HAVE_LIBVORBISFILE
/* Initialize Ogg Vorbis file */
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
#endif /* HAVE_LIBVORBISFILE */

#ifdef HAVE_LIBFLAC
/* Initialize FLAC file */
static int
initialize_flac(const FLAC__StreamMetadata *flac_tags, struct media_file_tags *media_file_tags)
{
  int i;

  for (i = 0; i < flac_tags->data.vorbis_comment.num_comments; ++i) {
    if (!strncmp(flac_tags->data.vorbis_comment.comments[i].entry, "TITLE=", 6)) {
      media_file_tags->title = flac_tags->data.vorbis_comment.comments[i].entry + 6;
    }
    else if (!strncmp(flac_tags->data.vorbis_comment.comments[i].entry, "ARTIST=", 7)) {
      media_file_tags->artist = flac_tags->data.vorbis_comment.comments[i].entry + 7;
    }
    else if (!strncmp(flac_tags->data.vorbis_comment.comments[i].entry, "ALBUM=", 6)) {
      media_file_tags->album = flac_tags->data.vorbis_comment.comments[i].entry + 6;
    }
    else if (!strncmp(flac_tags->data.vorbis_comment.comments[i].entry, "TRACK=", 6)) {
      media_file_tags->track = flac_tags->data.vorbis_comment.comments[i].entry + 6;
    }
    else if (!strncmp(flac_tags->data.vorbis_comment.comments[i].entry, "YEAR=", 5)) {
      media_file_tags->year = flac_tags->data.vorbis_comment.comments[i].entry + 5;
    }
    else if (!strncmp(flac_tags->data.vorbis_comment.comments[i].entry, "GENRE=", 6)) {
      media_file_tags->genre = flac_tags->data.vorbis_comment.comments[i].entry + 6;
    }
    else if (!strncmp(flac_tags->data.vorbis_comment.comments[i].entry, "COMMENT=", 8)) {
      media_file_tags->comment = flac_tags->data.vorbis_comment.comments[i].entry + 8;
    }
    else if (!strncmp(flac_tags->data.vorbis_comment.comments[i].entry, "ENCODED-BY=", 11)) {
      media_file_tags->encoded_by = flac_tags->data.vorbis_comment.comments[i].entry + 11;
    }
  }
  return 1;
}
#endif /* HAVE_LIBFLAC */



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
  else if (len > 4 && !strcasecmp(extension - 1, ".flac")) {
    return TG_FLAC;
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
#ifdef HAVE_LIBVORBISFILE
    else if (strstr(desc, "Ogg data, Vorbis audio")) {
      return TG_OGGVORBIS;
    }
#endif
#ifdef HAVE_LIBFLAC
    else if (strstr(desc, "FLAC audio bitstream data")) {
      return TG_FLAC;
    }
#endif
    else {
      return TG_UNKNOWN;
    }
  }
  return detect_filetype_extension(filename);
}
#endif

/* Display tags requested by the user */
static void
display_tags(struct aux_params *aux_params, struct media_file_tags *media_file_tags)
{
  if (aux_params->display_title) {
    printf("\tTitle: %s\n", media_file_tags->title ? media_file_tags->title : "");
  }
  if (aux_params->display_artist) {
    printf("\tArtist: %s\n", media_file_tags->artist ? media_file_tags->artist : "");
  }
  if (aux_params->display_album) {
    printf("\tAlbum: %s\n", media_file_tags->album ? media_file_tags->album : "");
  }
  if (aux_params->display_year) {
    printf("\tYear: %s\n", media_file_tags->year ? media_file_tags->year : "");
  }
  if (aux_params->display_genre) {
    printf("\tGenre: %s\n", media_file_tags->genre ? media_file_tags->genre : "");
  }
  if (aux_params->display_comment) {
    printf("\tComment: %s\n", media_file_tags->comment ? media_file_tags->comment : "");
  }
  if (aux_params->display_track) {
    printf("\tTrack: %s\n", media_file_tags->track ? media_file_tags->track : "");
  }
  if (aux_params->display_composer) {
    printf("\tComposer: %s\n", media_file_tags->composer ? media_file_tags->composer : "");
  }
  if (aux_params->display_orig_artist) {
    printf("\tOrig_artist: %s\n", media_file_tags->orig_artist ? media_file_tags->orig_artist : "");
  }
  if (aux_params->display_copyright) {
    printf("\tCopyright: %s\n", media_file_tags->copyright ? media_file_tags->copyright : "");
  }
  if (aux_params->display_url) {
    printf("\tURL: %s\n", media_file_tags->url ? media_file_tags->url : "");
  }
  if (aux_params->display_encoded_by) {
    printf("\tEncoded-by: %s\n", media_file_tags->encoded_by ? media_file_tags->encoded_by : "");
  }
}
/* Process one media file. */
int
process_file(const char *filename, struct tag_regexes *tag_regexes, struct aux_params *aux_params)
{
  struct media_file_tags media_file_tags;
  enum TG_FILETYPE file_type;

  memset(&media_file_tags, 0, sizeof(media_file_tags));
#ifdef HAVE_LIBMAGIC
  file_type = detect_filetype(filename, aux_params->magic_handle);
#else
  file_type = detect_filetype_extension(filename);
#endif

  if (file_type == TG_MP3) {
    struct id3_file *id3_file = id3_file_open(filename, ID3_FILE_MODE_READONLY);
    if (id3_file) {
      initialize_mp3(id3_file, &media_file_tags);
      if ((aux_params->any_tag ?
	   match_tag_regexps_any(&media_file_tags, tag_regexes->any_tag_regex) :
	   match_tag_regexps(&media_file_tags, tag_regexes))) {
	printf("%s%c", filename, aux_params->delimiter);
	display_tags(aux_params, &media_file_tags);
      }
      free_media_tags(&media_file_tags);
      id3_file_close(id3_file);
    }
    else {
      fprintf(stderr, "Error reading file %s!\n", filename);
    }
  }
#ifdef HAVE_LIBVORBISFILE
  else if (file_type == TG_OGGVORBIS) {
    OggVorbis_File oggv_file;
    if (ov_fopen(filename, &oggv_file) >= 0) {
      initialize_oggvorbis(&oggv_file, &media_file_tags);
      if ((aux_params->any_tag ?
	   match_tag_regexps_any(&media_file_tags, tag_regexes->any_tag_regex) :
	   match_tag_regexps(&media_file_tags, tag_regexes))) {
	printf("%s%c", filename, aux_params->delimiter);
	display_tags(aux_params, &media_file_tags);
      }
      /* We don't free the media tags, since they just point to the
         strings in the OggVorbis_File structure. */
      ov_clear(&oggv_file);
    }
    else {
      fprintf(stderr, "Error reading file %s!\n", filename);
    }
  }
#endif
#ifdef HAVE_LIBFLAC
  else if (file_type == TG_FLAC) {
    FLAC__StreamMetadata *flac_tags;
    FLAC__metadata_get_tags(filename, &flac_tags);
    if (flac_tags) {
      initialize_flac(flac_tags, &media_file_tags);
      if ((aux_params->any_tag ?
	   match_tag_regexps_any(&media_file_tags, tag_regexes->any_tag_regex) :
	   match_tag_regexps(&media_file_tags, tag_regexes))) {
	printf("%s%c", filename, aux_params->delimiter);
	display_tags(aux_params, &media_file_tags);
      }
      /* We don't free the media tags, since they just point to the
         strings in the flac_tags structure. */
      FLAC__metadata_object_delete(flac_tags);
    }
    else {
      fprintf(stderr, "Error reading file %s!\n", filename);
    }
  }
#endif
  return 1;
}

/* We copy the regular expressions and auxiliary parameters, since
   FTW's callback function cannot receive any custom data. */
static struct tag_regexes *tag_regexes_copy;
static struct aux_params *aux_params_copy;

static int
process_file_wrapper(const char *filename, const struct stat *sb, int tflag)
{
  if (tflag == FTW_F || tflag == FTW_SL) {
    process_file(filename, tag_regexes_copy, aux_params_copy);
  }
  return 0;
}

int
process_file_recursive(const char *filename, struct tag_regexes *tag_regexes, struct aux_params *aux_params)
{
  tag_regexes_copy = tag_regexes;
  aux_params_copy = aux_params;
  if (ftw(filename, process_file_wrapper, 20) == -1) {
    fprintf(stderr, "Error reading %s! (Did you make a mistake with the command line options?)\n", filename);
  }
  return 1;
}
