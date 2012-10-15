#include <sys/stat.h>
#include <sys/types.h>

int
mkdir (const char *pathname, mode_t mode)
{
  // TODO
  (void) pathname;
  (void) mode;
  return 0;
}

int
_mkdir (const char *pathname, mode_t mode)
__attribute__ ((weak, alias ("mkdir")));
