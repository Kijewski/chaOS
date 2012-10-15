#include <sys/stat.h>

int
stat (const char *path, struct stat *buf)
{
  // TODO
  (void) path;
  (void) buf;
  return 0;
}

int
_stat (const char *path, struct stat *buf)
__attribute__ ((weak, alias ("stat")));
