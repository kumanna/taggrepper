#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <mcheck.h>

#include "taggrep.h"

static struct tag_regexes tag_regexes;

static void
free_tag_regexes(void)
{
  free(tag_regexes.title_regex);
  free(tag_regexes.artist_regex);
  free(tag_regexes.album_regex);
  free(tag_regexes.year_regex);
  free(tag_regexes.genre_regex);
  free(tag_regexes.comment_regex);
  free(tag_regexes.composer_regex);
  free(tag_regexes.orig_artist_regex);
  free(tag_regexes.copyright_regex);
  free(tag_regexes.url_regex);
  free(tag_regexes.encoded_by_regex);
  magic_close(tag_regexes.magic_handle);
}

static int
parse_command_line(int argc, char *argv[])
{
  int c;
  int digit_optind = 0;
  magic_t magic_handle;

  memset(&tag_regexes, 0, sizeof(struct tag_regexes));
  tag_regexes.magic_handle = magic_open(MAGIC_NONE);
  magic_load(tag_regexes.magic_handle, NULL);

  while (1) {
    int this_option_optind = optind ? optind : 1;
    int option_index = 0;
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
      {0, 0, 0, 0}
    };

    c = getopt_long(argc, argv, "t:a:l:y:g:c:",
		    long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
    case 0:
      //      printf("option %s", long_options[option_index].name);
      if (!strcmp(long_options[option_index].name, "composer")) {
	tag_regexes.composer_regex = strdup(optarg);
      }
      else if (!strcmp(long_options[option_index].name, "orig-artist")) {
	tag_regexes.orig_artist_regex = strdup(optarg);
      }
      else if (!strcmp(long_options[option_index].name, "track")) {
	tag_regexes.track_regex = strdup(optarg);
      }
      else if (!strcmp(long_options[option_index].name, "copyright")) {
	tag_regexes.copyright_regex = strdup(optarg);
      }
      else if (!strcmp(long_options[option_index].name, "url")) {
	tag_regexes.url_regex = strdup(optarg);
      }
      else if (!strcmp(long_options[option_index].name, "encoded-by")) {
	tag_regexes.encoded_by_regex = strdup(optarg);
      }

      if (optarg) {
	printf(" with arg %s", optarg);
      }
      printf("\n");
      break;

    case 't':
      tag_regexes.title_regex = strdup(optarg);
      break;

    case 'a':
      tag_regexes.artist_regex = strdup(optarg);
      break;

    case 'l':
      tag_regexes.album_regex = strdup(optarg);
      break;

    case 'y':
      tag_regexes.year_regex = strdup(optarg);
      break;

    case 'g':
      tag_regexes.genre_regex = strdup(optarg);
      break;

    case 'c':
      tag_regexes.comment_regex = strdup(optarg);
      break;

    case '?':
      break;

    default:
      printf("?? getopt returned character code 0%o ??\n", c);
    }
  }
  return EXIT_SUCCESS;
}

int
main(int argc, char *argv[])
{
  int ret;
  
  mtrace();
  ret = parse_command_line(argc, argv);
  if (optind < argc) {
    while (optind < argc) {
      //printf("%s ", argv[optind++]);
      processFile(argv[optind++], &tag_regexes);
    }
  }
  else {
    printf("No files!\n");
    ret = EXIT_FAILURE;
  }
  free_tag_regexes();
  return ret;
}
