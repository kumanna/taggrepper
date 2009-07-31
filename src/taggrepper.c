#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <mcheck.h>

#include "common.h"
#include "tagregexps.h"

/* Detect if some help was displayed, to avoid error messages */
#define HELP_DISPLAYED 2

/* Some custom error codes */
#define ERR_UNRECOGNIZED_COMMAND 3
#define ERR_INCORRECT_REGEXP 4

/* This function releases the PCRE regular expressions. The magic
   handle is also closed. */
static void
free_tag_regexes(struct tag_regexes *tag_regexes, struct aux_params *aux_params)
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
    magic_close(aux_params->magic_handle);
  }
#endif
}

static void
help_message(const char *command)
{
  printf("Usage: %s [--tag-label regexp]... [FILE/DIRECTORY]...\n"
"Perform a regular expression search on the tags of specified media files.\n\n"
"Mandatory arguments to long options are mandatory for short options too.\n"
"  -t, --title=REGEXP\t\tMatch the title tag against REGEXP\n"
"  -a, --artist=REGEXP\t\tMatch the artist tag against REGEXP\n"
"  -l, --album,=REGEXP\t\tMatch the album, tag against REGEXP\n"
"  -y, --year, =REGEXP\t\tMatch the year  tag against REGEXP\n"
"  -g, --genre,=REGEXP\t\tMatch the genre tag against REGEXP\n"
"  -c, --comment=REGEXP\t\tMatch the comment tag against REGEXP\n"
"      --track=REGEXP\t\tMatch the track tag against REGEXP\n"
"  -c, --composer=REGEXP\t\tMatch the composer tag against REGEXP\n"
"  -o, --orig-artist=REGEXP\tMatch the original artist tag against REGEXP\n"
"  -c, --copyright=REGEXP\tMatch the copyright tag against REGEXP\n"
"  -u, --url=REGEXP\t\tMatch the URL tag against REGEXP\n"
"  -e, --encode=REGEXP\t\tMatch the encode tag against REGEXP\n"
"\n"
"      --display-title\t\tdisplay title tag of matching files\n"
"      --display-artist\t\tdisplay artist tag of matching files\n"
"      --display-album\t\tdisplay album tag of matching files\n"
"      --display-year\t\tdisplay year tag of matching files\n"
"      --display-genre\t\tdisplay genre tag of matching files\n"
"      --display-comment\t\tdisplay comment tag of matching files\n"
"      --display-track\t\tdisplay track tag of matching files\n"
"      --display-composer\t\tdisplay composer tag of matching files\n"
"      --display-orig-artist\t\tdisplay orig-artist tag of matching files\n"
"      --display-copyright\t\tdisplay copyright tag of matching files\n"
"      --display-url\t\tdisplay url tag of matching files\n"
"      --display-encoded-by\t\tdisplay encoded-by tag of matching files\n"
"\n"
#ifdef HAVE_LIBMAGIC
"  -u, --use-magic\t\tUse libmagic to find file type, irrespective of extension\n"
#endif
"  -0, --print0,\t\t\tUse null character as delimiter. This option can be used with -0 option of xargs.\n"
"  -r, --recursive\t\tSearch directories recursively\n"
"  -v, --version\t\t\tDisplay version and exit\n"
"  -h, --help\t\t\tDisplay this help message\n"
"\n"
"All regular expressions are PCRE regular expressions. Refer to the\n"
"PCRE documentation for details.\n"
"Report comments and bugs to %s\n"
	 , command, PACKAGE_BUGREPORT);
}

/* Parse the command line options and process each file */
static int
parse_command_line(int argc, char *argv[], struct tag_regexes *tag_regexes, struct aux_params *aux_params, int *recursive)
{
  int c;

  *recursive = 0;
  memset(tag_regexes, 0, sizeof(struct tag_regexes));
  memset(aux_params, 0, sizeof(struct aux_params));
  aux_params->delimiter = '\n';

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
      {"print0", 0, 0, '0'},
      {"recursive", 0, 0, 'r'},
      {"version", 0, 0, 'v'},
      {"help", 0, 0, 'h'},

      {"display-title", 0, 0, 0},
      {"display-artist", 0, 0, 0},
      {"display-album", 0, 0, 0},
      {"display-year", 0, 0, 0},
      {"display-genre", 0, 0, 0},
      {"display-comment", 0, 0, 0},
      {"display-track", 0, 0, 0},
      {"display-composer", 0, 0, 0},
      {"display-orig-artist", 0, 0, 0},
      {"display-copyright", 0, 0, 0},
      {"display-url", 0, 0, 0},
      {"display-encoded-by", 0, 0, 0},
      {0, 0, 0, 0}
    };

#ifdef HAVE_LIBMAGIC
    c = getopt_long(argc, argv, "t:a:l:y:g:c:mrvh0",
		    long_options, &option_index);
#else
    c = getopt_long(argc, argv, "t:a:l:y:g:c:rvh0",
		    long_options, &option_index);
#endif
    if (c == -1) {
      break;
    }
#ifdef HAVE_LIBMAGIC
    else if (c == 'm') {
      aux_params->magic_handle = magic_open(MAGIC_NONE);
      magic_load(aux_params->magic_handle, NULL);
      continue;
    }
#endif
    else if (c == '0'){
      aux_params->delimiter = '\0';
      continue;
    }
    else if (c == 'r') {
      *recursive = 1;
      continue;
    }
    else if (c == 'v') {
      printf(VERSION_STRING, PACKAGE_NAME, PACKAGE_VERSION);
      return HELP_DISPLAYED;
    }
    else if (c == 'h') {
      help_message(argv[0]);
      return HELP_DISPLAYED;
    }

    if (c == 0) {
      if (!strcmp(long_options[option_index].name, "display-title")) {
	aux_params->display_title = 1;
      }
      else if (!strcmp(long_options[option_index].name, "display-artist")) {
	aux_params->display_artist = 1;
      }
      else if (!strcmp(long_options[option_index].name, "display-album")) {
	aux_params->display_album = 1;
      }
      else if (!strcmp(long_options[option_index].name, "display-year")) {
	aux_params->display_year = 1;
      }
      else if (!strcmp(long_options[option_index].name, "display-genre")) {
	aux_params->display_genre = 1;
      }
      else if (!strcmp(long_options[option_index].name, "display-comment")) {
	aux_params->display_comment = 1;
      }
      else if (!strcmp(long_options[option_index].name, "display-track")) {
	aux_params->display_track = 1;
      }
      else if (!strcmp(long_options[option_index].name, "display-composer")) {
	aux_params->display_composer = 1;
      }
      else if (!strcmp(long_options[option_index].name, "display-orig-artist")) {
	aux_params->display_orig_artist = 1;
      }
      else if (!strcmp(long_options[option_index].name, "display-copyright")) {
	aux_params->display_copyright = 1;
      }
      else if (!strcmp(long_options[option_index].name, "display-url")) {
	aux_params->display_url = 1;
      }
      else if (!strcmp(long_options[option_index].name, "display-encoded-by")) {
	aux_params->display_encoded_by = 1;
      }
      continue;
    }

    if (!optarg) {
      fprintf(stderr, "Unrecognized command line option!\n");
      return ERR_UNRECOGNIZED_COMMAND;
    }
    re = initialize_regexp(optarg);
    if (!re) {
      fprintf(stderr, "Couldn't form regular expression!\n");
      return ERR_INCORRECT_REGEXP;
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
      fprintf(stderr, "Couldn't recognize command line parameters!\n");
      return ERR_UNRECOGNIZED_COMMAND;
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
  struct aux_params aux_params_struct;
  int recursive;

  mtrace();
  ret = parse_command_line(argc, argv, &tag_regexes_struct, &aux_params_struct, &recursive);

  if (!ret) {
    fprintf(stderr, "Exiting with error code %d!\n", ret);
    return 1;
  }
  /* We interpret non-arguments as file names */
  if (optind < argc) {
    while (optind < argc) {
      if (!recursive) {
        process_file(argv[optind++], &tag_regexes_struct, &aux_params_struct);
      }
      else {
        process_file_recursive(argv[optind++], &tag_regexes_struct, &aux_params_struct);
      }
    }
  }
  else {
    if (ret != HELP_DISPLAYED) {
      printf("No files specified\n");
      printf("Please use the --help option for usage information.\n");
    }
    ret = 0;
  }
  free_tag_regexes(&tag_regexes_struct, &aux_params_struct);
  return ret == 0;
}
