#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include "banner.h"

int
addr_parse (const char *str, address *norm)
{
  struct addrinfo hints = {
    .ai_family = PF_UNSPEC,
    .ai_socktype = 0,
    .ai_protocol = 0,
    .ai_flags = AI_CANONNAME | AI_NUMERICHOST
  };

  struct addrinfo *res;

  int r = getaddrinfo (str, 0, &hints, &res);
  if (r != 0)
    {
      *norm = 0;
      return -1;
    }

  if (res->ai_canonname)
    *norm = strdup (res->ai_canonname);
  else
    *norm = strdup (str);

  freeaddrinfo (res);
  return 0;
}


void
addr_free (address addr)
{
  free ((char *) addr);
}


