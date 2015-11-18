#include <pcre.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include "banner.h"

struct matcher_patterns
{
  int n;
  pcre **pcres;
};

static const char *patterns[] = {
  "xxx {{addr}}",
  "yyy {{addr}}"
};


const char addr_regex[] = "[0-9:.]+";


/* RESULT is written with PAT',
   where PAT' is PAT with PIECE replaced by SUBST.

   Assumes RESULT is properly alloced
   (e.g. has len of length(pat) + length(subst)).

   Returns RESULT.
 */
static char *
replace_piece (const char *pat, const char *piece, const char *subst,
	       char *result)
{
  const char *a = strstr (pat, piece);
  if (a == 0)
    strcpy (result, pat);
  else
    sprintf (result, "%.*s%s%s",
	     (int) (a - pat), pat, subst, a + strlen (piece));

  return result;
}

static char *
replace_addr_re (const char *pat, char *re)
{
  const char addr_Lit[] = "{{addr}}";
  return replace_piece (pat, addr_Lit, addr_regex, re);
}

int
matcher_init (const matcher_patterns **mstate)
{
  const int n = sizeof patterns / sizeof patterns[0];
  matcher_patterns *pats = malloc (sizeof (matcher_patterns));
  *pats = (matcher_patterns) {
    .n = n,
    .pcres = calloc (n, sizeof (*mstate)->pcres)
  };

  *mstate = pats;

  for (int i = 0; i < n; i++)
    {
      char *re_str = alloca (strlen (patterns[i]) + sizeof addr_regex);
      char *r = replace_addr_re (patterns[i], re_str);
      if (r == 0)
	{
	  warnx ("no address info in `%s'\n", re_str);
	  return 1;
	}

      printf ("RE `%s'\n", re_str);

      const char *errstr;
      int erroffset;
      pcre *re = pcre_compile (re_str, 0, &errstr, &erroffset, 0);
      if (re == 0)
	{
	  warnx ("invalid pattern `%s'\n", re_str);
	  return 1;
	}

      (*mstate)->pcres[i] = re;
    }

  return 0;
}


int
match (const matcher_patterns *pats, const char *line)
{
  for (int i = 0; i < pats->n; i++)
    {
      int rs[2];
      int rsn = sizeof rs / sizeof rs[0];

      int r = pcre_exec (pats->pcres[i], 0, line, strlen (line),
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

