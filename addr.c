#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

int
addr_parse (const char *str, char **norm)
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
amain ()
{
  char *res;
  addr_parse ("::1x", &res);
  //printf ("%s\n", res);
}

