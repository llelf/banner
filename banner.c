#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/queue.h>
#include "banner.h"

static const char banner_version[] = "0.0";


static struct option banner_long_opts[] = {
  { "version", no_argument, 0, 'v' },
  { "help",    no_argument, 0, 'h' }
};

struct logfile
{
  const char *log;
  STAILQ_ENTRY(logfile) logfiles;
};

STAILQ_HEAD (logfiles, logfile);


int
main_banner (struct logfiles *logfiles)
{
  struct logfile *log;
  STAILQ_FOREACH (log, logfiles, logfiles)
    {
      printf ("log file added `%s'\n", log->log);
    }


  matcher_state *matcher;
  matcher_init (&matcher);

  watcher (matcher);

  return 1;
}


int
main (int argc, char *argv[])
{
  enum { BANNER, HELP, VERSION } what = BANNER;

  struct logfiles logfiles_list = STAILQ_HEAD_INITIALIZER (logfiles_list);
  STAILQ_INIT (&logfiles_list);

  int c;
  while ((c = getopt_long (argc, argv, "l:hv", banner_long_opts, 0)) != -1)
    {
      switch (c)
	{
	case 'h':
	  what = HELP;
	  break;

	case 'v':
	  what = VERSION;
	  break;

	case 'l':
	  {
	    struct logfile *log = (struct logfile *) malloc (sizeof log);
	    log->log = optarg;
	    STAILQ_INSERT_TAIL (&logfiles_list, log, logfiles);
	  }
	}
    }

  int status = 0;
  switch (what)
    {
    case HELP:
      printf ("banner v%s\n", banner_version);
      break;

    case VERSION:
      printf ("banner [-l LOGFILE]... [--help|--version]\n");
      break;

    case BANNER:
      status = main_banner (&logfiles_list);
    }

  return status;
}


