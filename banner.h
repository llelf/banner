#pragma once

typedef const char *address;
int addr_parse (const char *str, address *addr);


typedef struct matcher_patterns matcher_patterns;
int matcher_init (const matcher_patterns **mstate);
int match (const matcher_patterns *matcher, const char *line);

void watcher (const matcher_patterns *mstate);

