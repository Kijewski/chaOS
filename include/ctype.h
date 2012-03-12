#ifndef CTYPE_H__
#define CTYPE_H__

#include <attributes.h>

// TODO

int   isalnum(int);
int   isalpha(int);
int   isascii(int) DEPRECATED;
int   iscntrl(int);
int   isdigit(int);
int   isgraph(int);
int   islower(int);
int   isprint(int);
int   ispunct(int);
int   isspace(int);
int   isupper(int);
int   isxdigit(int);
int   toascii(int) DEPRECATED;
int   tolower(int);
int   toupper(int);

int   _toupper(int) DEPRECATED;
int   _tolower(int) DEPRECATED;

#endif
