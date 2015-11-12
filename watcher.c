#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/event.h>
#include "banner.h"


struct watch_state
{
  char buf[512];
  char *bufptr;
  char *bufend;
  bool ignore_this_line;
};



void
handle_log_line (const matcher_patterns *matcher, const char *str)
{
  int r = match (matcher, str);
  printf ("LOG `%s' match=%d\n", str, r);
}


void
watcher (const struct logfiles *files, const matcher_patterns *pats)
{
  int n = 0;
  const struct logfile *log;
  STAILQ_FOREACH (log, files, logfiles)
    {
      n++;
    }

  int kq = kqueue ();

  struct kevent logs[n];

  struct watch_state states[n];

  const struct logfile *cur = STAILQ_FIRST (files);
  for (int i = 0; i < n; i++)
    {
      const char *log = cur->log;

      int fd = open (log, O_RDONLY);
      if (fd == -1)
	err (0, "could not open %s", log);

      EV_SET (&logs[i], fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &states[i]);

      cur = STAILQ_NEXT (cur, logfiles);
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
		handle_log_line (pats, st->buf);
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

