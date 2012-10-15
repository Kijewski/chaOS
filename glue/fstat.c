#include <sys/stat.h>

int
fstat (int fd, struct stat *buf)
{
  // TODO
  (void) fd;
  (void) buf;
  return 0;
}

int _fstat (int fd, struct stat *buf) __attribute__ ((weak, alias ("fstat")));
