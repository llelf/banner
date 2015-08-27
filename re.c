#include <pcre.h>
#include <stdio.h>



struct matcher_state
{
  pcre *pcre;
};


static const char *patterns[] = {
  "xxx (\\d+)"
};


static int test ()
{
  const char *errstr = malloc(1024);
  int erroffset;
  pcre *re = pcre_compile ("(?<mem>abc)", 0, &errstr, &erroffset, 0);
  printf ("re=%p %s\n", re, errstr + erroffset);

  int rs[10];
  pcre_extra ext = { .flags = 0 }; //PCRE_EXTRA_MARK | PCRE_EXTRA_TABLES };
  const char *str = "abcd";
  int r = pcre_exec (re, &ext, str, 4, 0, 0, rs, 10);

  printf ("%d\n", r);
  for (int i = 0; i < r; i++)
    {
      const char *m = 0;
      pcre_get_named_substring (re, str, rs, r, "mem", &m);
      printf ("%d `%s'\n", i, m);
    }

}

