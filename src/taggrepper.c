#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <mcheck.h>

#include "common.h"
#include "tagregexps.h"

/* This function releases the PCRE regular expressions. The magic
   handle is also closed. */
static void
free_tag_regexes(struct tag_regexes *tag_regexes)
{
  pcre_free(tag_regexes->title_regex);
  pcre_free(tag_regexes->artist_regex);
  pcre_free(tag_regexes->album_regex);
  pcre_free(tag_regexes->year_regex);
  pcre_free(tag_regexes->genre_regex);
  pcre_free(tag_regexes->comment_regex);
  pcre_free(tag_regexes->composer_regex);
  pcre_free(tag_regexes->orig_artist_regex);
  pcre_free(tag_regexes->copyright_regex);
  pcre_free(tag_regexes->url_regex);
  pcre_free(tag_regexes->encoded_by_regex);
#ifdef HAVE_LIBMAGIC
  if (tag_regexes->magic_handle) {
    magic_close(tag_regexes->magic_handle);
  }
#endif
}

/* Parse the command line options and process each file */
static int
parse_command_line(int argc, char *argv[], struct tag_regexes *tag_regexes, int *recursive)
{
  int c;

  *recursive = 0;
  memset(tag_regexes, 0, sizeof(struct tag_regexes));

  while (1) {
    int option_index = 0;
    pcre *re;

    /* TODO: Is this list of tags good enough? More? Less? */
    static struct option long_options[] = {
      {"title", 1, 0, 't'},
      {"artist", 1, 0, 'a'},
      {"album", 1, 0, 'l'},
      {"year", 1, 0, 'y'},
      {"genre", 1, 0, 'g'},
      {"comment", 1, 0, 'c'},
      {"track", 1, 0, 0},
      {"composer", 1, 0, 0},
      {"orig-artist", 1, 0, 0},
      {"copyright", 1, 0, 0},
      {"url", 1, 0, 0},
      {"encoded-by", 1, 0, 0},
#ifdef HAVE_LIBMAGIC
      {"use-magic", 0, 0, 'm'},
#endif
      {"recursive", 0, 0, 'r'},
      {0, 0, 0, 0}
    };

#ifdef HAVE_LIBMAGIC
    c = getopt_long(argc, argv, "t:a:l:y:g:c:mr",
		    long_options, &option_index);
#else
    c = getopt_long(argc, argv, "t:a:l:y:g:c:r",
		    long_options, &option_index);
#endif
    if (c == -1) {
      break;
    }
#ifdef HAVE_LIBMAGIC
    else if (c == 'm') {
      tag_regexes->magic_handle = magic_open(MAGIC_NONE);
      magic_load(tag_regexes->magic_handle, NULL);
      continue;
    }
#endif
    else if (c == 'r') {
      *recursive = 1;
      continue;
    }

    if (!optarg) {
      return 0;
    }
    re = initialize_regexp(optarg);
    if (!re) {
      return 0;
    }
    switch (c) {
    case 0:
      /* Check if a known long option was specified */
      if (!strcmp(long_options[option_index].name, "composer")) {
	tag_regexes->composer_regex = re;
      }
      else if (!strcmp(long_options[option_index].name, "orig-artist")) {
	tag_regexes->orig_artist_regex = re;
      }
      else if (!strcmp(long_options[option_index].name, "track")) {
	tag_regexes->track_regex = re;
      }
      else if (!strcmp(long_options[option_index].name, "copyright")) {
	tag_regexes->copyright_regex = re;
      }
      else if (!strcmp(long_options[option_index].name, "url")) {
	tag_regexes->url_regex = re;
      }
      else if (!strcmp(long_options[option_index].name, "encoded-by")) {
	tag_regexes->encoded_by_regex = re;
      }
      break;

    /* Handle short options */
    case 't':
      tag_regexes->title_regex = re;
      break;

    case 'a':
      tag_regexes->artist_regex = re;
      break;

    case 'l':
      tag_regexes->album_regex = re;
      break;

    case 'y':
      tag_regexes->year_regex = re;
      break;

    case 'g':
      tag_regexes->genre_regex = re;
      break;

    case 'c':
      tag_regexes->comment_regex = re;
      break;

    case '?':
      return 0;
      break;

    default:
      printf("Error processing command line arguments: getopt returned character code 0%o ??\n", c);
      return 0;
    }
  }
  return 1;
}

int
main(int argc, char *argv[])
{
  int ret;
  /* Structure possessing the regular expressions */
  struct tag_regexes tag_regexes_struct;  
  int recursive;

  mtrace();
  ret = parse_command_line(argc, argv, &tag_regexes_struct, &recursive);

  if (!ret) {
    fprintf(stderr, "Exiting with error!\n");
    return 1;
  }
  /* We interpret non-arguments as file names */
  if (optind < argc) {
    while (optind < argc) {
      if (!recursive) {
	processFile(argv[optind++], &tag_regexes_struct);
      }
      else {
	processFile_recursive(argv[optind++], &tag_regexes_struct);
      }
    }
  }
  else {
    printf("No files!\n");
    ret = 0;
  }
  free_tag_regexes(&tag_regexes_struct);
  return ret == 0;
}
