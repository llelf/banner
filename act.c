#include <string.h>
#include <spawn.h>
#include <err.h>
#include <sys/wait.h>
#include "banner.h"

static const char *const pfctl_path = "/sbin/pfctl";
static const char *const pfctl = "pfctl";
static const char *const table = "banner";


int
go_pfctl (const char *pure_argv[])
{
  char *const *argv = (char **) pure_argv; /* pfctl ain't gonna modify them */

  pid_t pid;
  int r = posix_spawnp (&pid, pfctl_path, 0, 0, argv, 0);
  if (r != 0)
    {
      warnc (r, "cannot spawn %s", pfctl_path);
      return -1;
    }

  int status;
  pid_t rp = waitpid (pid, &status, 0);
  if (rp != 0)
    {
      warnc (rp, "abnormal %s behaviour", pfctl_path);
      return -2;
    }

  if (! (WIFEXITED (status) && WEXITSTATUS (status) == 0))
    {
      warnx ("%s vanished, status=%d", pfctl_path, status);
      return -3;
    }

  return 0;
}


int
add_ban (address addr)
{
  banner_log ("ban %s\n", addr);
  return go_pfctl ((const char * []) { pfctl, "-t", table, "-T", "add", addr, 0 });
}

