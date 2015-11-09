#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/event.h>
#include "banner.h"

static const char *log_files[] = {
  "log",
  "log1"
};

struct watch_state
{
  char buf[512];
  char *bufptr;
  char *bufend;
  bool ignore_this_line;
};



void
handle_log_line (const matcher_state *matcher, const char *str)
{
  int r = match (matcher, str);
  printf ("LOG `%s' match=%d\n", str, r);
}


void
watcher (const matcher_state *matcher)
{
  int kq = kqueue ();

  int n = sizeof log_files / sizeof log_files[0];

  struct kevent logs[n];

  struct watch_state states[n];

  for (int i = 0; i < n; i++)
    {
      int fd = open (log_files[i], O_RDONLY);
      if (fd == -1)
	err (0, "could not open %s", log_files[i]);

      EV_SET (&logs[i], fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &states[i]);
    }

  struct kevent evs[n];


  for (int i = 0; i < n; i++)
    {
      struct watch_state *st = &states[i];
      st->bufptr = st->buf;
      st->bufend = st->buf + sizeof st->buf;
      st->ignore_this_line = false;
    }

  for (;;)
    {
      int nev = kevent (kq, logs, n, evs, n, 0);

      for (int i = 0; i < nev; i++)
	{
	  struct watch_state *st = (struct watch_state *) evs[i].udata;

	  int rn = read (evs[i].ident, st->bufptr, st->bufend - st->bufptr);

	  //printf ("== %d/%d, read %d\n", i, nev, rn);

	  char *lf;
	  while ((lf = memchr (st->buf, '\n', st->bufptr - st->buf + rn)) != 0)
	    {
	      *lf++ = '\0';
	      
	      if (! st->ignore_this_line)
		handle_log_line (matcher, st->buf);
	      else
		st->ignore_this_line = false;
	      
	      memmove (st->buf, lf, st->bufend - lf);
	      st->bufptr -= lf - st->buf;
	    }

	  st->bufptr += rn;
	  if (st->bufptr >= st->bufend)
	    {
	      warnx ("line too big, it is a log for elephants?");
	      st->bufptr = st->buf;
	      st->ignore_this_line = true;
	    }
	}
    }
}

