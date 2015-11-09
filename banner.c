#include <stdio.h>
#include "banner.h"


int
init ()
{
  return 0;
}


int main ()
{
  matcher_state *matcher;
  matcher_init (&matcher);

  watcher (matcher);

  printf ("wtf");
}


