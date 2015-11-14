#pragma once
#include <sys/queue.h>

typedef const char *address;
int addr_parse (const char *str, address *addr);

struct logfile
{
  const char *log;
  STAILQ_ENTRY(logfile) logfiles;
};


STAILQ_HEAD (logfiles, logfile);


typedef struct matcher_patterns matcher_patterns;
int matcher_init (const matcher_patterns **mstate);
int match (const matcher_patterns *matcher, const char *line);

void watcher (const struct logfiles *files, const matcher_patterns *mstate);

int add_ban (address addr);

