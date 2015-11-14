#include "banner.h"
#include <stdio.h>
#include <stdarg.h>

int
banner_log (const char *restrict fmt, ...)
{
  va_list ap;

  va_start (ap, fmt);
  int r = vfprintf (stderr, fmt, ap);
  va_end (ap);

  return r;
}

