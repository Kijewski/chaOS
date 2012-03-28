#ifndef STDIO_H__
#define STDIO_H__

#include <stddef.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>

#include <printf.h>

struct __file;
typedef struct __file FILE;

typedef uint64_t fpos_t;

extern FILE *stderr, *stdin, *stdout;

#define BUFSIZ (0x1000)

#define _IOFBF (0)
#define _IOLBF (1)
#define _IONBF (2)

#define L_tmpnam (0x100)

#define FILENAME_MAX (0x200)
#define FOPEN_MAX (0x1000 / 8)
#define TMP_MAX (10000)


void     clearerr(FILE *);
char    *ctermid(char *);

int      fclose(FILE *);
FILE    *fdopen(int, const char *);

int      feof(FILE *);
int      ferror(FILE *);
int      fflush(FILE *);
int      fgetc(FILE *);
int      fgetpos(FILE *restrict, fpos_t *restrict);
char    *fgets(char *restrict, int, FILE *restrict);
int      fileno(FILE *);

void     flockfile(FILE *);

FILE    *fopen(const char *restrict, const char *restrict);
int      fprintf(FILE *restrict, const char *restrict, ...);
int      fputc(int, FILE *);
int      fputs(const char *restrict, FILE *restrict);
size_t   fread(void *restrict, size_t, size_t, FILE *restrict);
FILE    *freopen(const char *restrict, const char *restrict, FILE *restrict);
int      fscanf(FILE *restrict, const char *restrict, ...);
int      fseek(FILE *, long, int);
int      fseeko(FILE *, off_t, int);

int      fsetpos(FILE *, const fpos_t *);
long     ftell(FILE *);
off_t    ftello(FILE *);

size_t   fwrite(const void *restrict, size_t, size_t, FILE *restrict);
int      getc(FILE *);
int      getchar(void);

char    *gets(char *);
int      pclose(FILE *);

void     perror(const char *);
FILE    *popen(const char *, const char *);

int      printf(const char *restrict, ...);
int      putc(int, FILE *);
int      putchar(int);

int      puts(const char *);
int      remove(const char *);
int      rename(const char *, const char *);
void     rewind(FILE *);
int      scanf(const char *restrict, ...);
void     setbuf(FILE *restrict, char *restrict);
int      setvbuf(FILE *restrict, char *restrict, int, size_t);
int      snprintf(char *restrict, size_t, const char *restrict, ...);
int      sprintf(char *restrict, const char *restrict, ...);
int      sscanf(const char *restrict, const char *restrict, ...);
char    *tempnam(const char *, const char *);

FILE    *tmpfile(void);
char    *tmpnam(char *);
int      ungetc(int, FILE *);
int      vfprintf(FILE *restrict, const char *restrict, va_list);
int      vfscanf(FILE *restrict, const char *restrict, va_list);
int      vprintf(const char *restrict, va_list);
int      vscanf(const char *restrict, va_list);
int      vsnprintf(char *restrict, size_t, const char *restrict, va_list);
int      vsprintf(char *restrict, const char *restrict, va_list);
int      vsscanf(const char *restrict, const char *restrict, va_list arg);

#endif
