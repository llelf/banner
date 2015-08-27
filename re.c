#include <pcre.h>
#include <stdio.h>
#include <err.h>
#include <string.h>


struct matcher_state
{
  int n;
  pcre **pcres;
};
typedef struct matcher_state matcher_state;

static const char *patterns[] = {
  "xxx (\\d+)",
  "yyy (\\d+)"
};


int
matcher_init (matcher_state *mstate)
{
  const int n = sizeof patterns / sizeof patterns[0];
  *mstate = (matcher_state) {
    .n = n,
    .pcres = calloc (n, sizeof *mstate->pcres)
  };

  for (int i = 0; i < n; i++)
    {
      const char *errstr;
      int erroffset;
      pcre *re = pcre_compile (patterns[i], 0, &errstr, &erroffset, 0);
      if (re == 0)
	{
	  warnx ("invalid pattern `%s'\n", patterns[i]);
	  return 1;
	}

      mstate->pcres[i] = re;
    }

  return 0;
}


int
match (matcher_state *matcher, const char *line)
{
  for (int i = 0; i < matcher->n; i++)
    {
      int rs[2];
      int rsn = sizeof rs / sizeof rs[0];

      int r = pcre_exec (matcher->pcres[i], 0, line, strlen (line),
			 0, 0, rs, rsn);

      if (r > 0)
	return i;
    }

  return -1;
}




static void test ()
{
  const char *errstr;
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

