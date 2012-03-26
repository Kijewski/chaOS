#ifndef REGEX_H__
#define REGEX_H__

#include <stdlib.h>
#include <sys/types.h>

typedef struct
{
  size_t re_nsub;
} regex_t;

typedef ssize_t regoff_t;

typedef struct
{
  regoff_t re_so;
  regoff_t re_eo;
} regmatch_t;

enum
{
  REG_EXTENDED,
  REG_ICASE,
  REG_NOSUB,
  REG_NEWLINE,
};

enum
{
  REG_NOTBOL,
  REG_NOTEOL,
};

enum
{
  REG_NOMATCH = 1,
  REG_BADPAT,
  REG_ECOLLATE,
  REG_ECTYPE,
  REG_EESCAPE,
  REG_ESUBREG,
  REG_EBRACK,
  REG_EPAREN,
  REG_EBRACE,
  REG_BADBR,
  REG_ERANGE,
  REG_ESPACE,
  REG_BADRPT,
  REG_ENOSYS,
};

int    regcomp(regex_t *restrict, const char *restrict, int);
size_t regerror(int, const regex_t *restrict, char *restrict, size_t);
int    regexec(const regex_t *restrict, const char *restrict, size_t,
           regmatch_t[restrict], int);
void   regfree(regex_t *);


#endif
