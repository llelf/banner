#pragma once

typedef const char *address;
int addr_parse (const char *str, address *addr);


typedef struct matcher_state matcher_state;
int matcher_init (matcher_state **mstate);
int match (const matcher_state *matcher, const char *line);

void watcher (const matcher_state *mstate);

