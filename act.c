#include "banner.h"

static const char *const pfctl = "pfctl";
static const char *const table = "banner";

int
add_ban ()
{
  const char *argv[] = { pfctl, "-t", table, "-T", "add", 0 };

  return 0;
}

