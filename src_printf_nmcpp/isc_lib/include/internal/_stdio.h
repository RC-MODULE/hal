//==============================================================================
//  Component:
//      STD (C++ Standard Library)
//
//  Library:
//      $KW; $FN; $EKW;
//      $KW; $Own; $Ver; $ChkD; $EKW;
//
//  Purpose:
//
//  Copyright (C) 1998-2001, Interstron, Ltd.
//  All rights reserved.
//==###=========================================================================

#ifdef __MULTICORE__
#include "Elvis\Elcore\_stdio.h"
#endif
#ifdef __UNC80__
#include "Senvico\_stdio.h"
#endif


struct _FILE{
    int _fd;        /* file descriptor */
    int _unc;       /* charater that is pushed back by ungetc */
    char *_eback;   /* "end back" stream pointer */
    char *_gptr;    /* "get pointer" stream pointer */
    char *_egptr;   /* "get end pointer" stream pointer */
    unsigned long _bfmode;
};

#ifdef _INTERNAL_IN_STD
#ifndef _CSIZE_T
#define _CSIZE_T
typedef unsigned int size_t;
#endif
#ifndef _CFILE__
#define _CFILE__
typedef struct _FILE FILE;
#endif
#ifndef _CARG__
#define _CARG__
typedef void * va_list;
#endif
#ifndef _CFPOS_T
#define _CFPOS_T
typedef long long fpos_t;
#endif
#endif
#ifdef _INTERNAL_NOT_IN_STD
#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif
#ifndef _FILE__
#define _FILE__
typedef struct _FILE FILE;
#endif
#ifndef _ARG__
#define _ARG__
typedef void * va_list;
#endif
#ifndef _FPOS_T
#define _FPOS_T
typedef long long fpos_t;
#endif
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*) 0)
#endif
#endif

#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

#define BUFSIZ 256

#define EOF (-1)

#define FOPEN_MAX 0

#define FILENAME_MAX 128

#define L_tmpnam 128

#define SEEK_CUR 0
#define SEEK_END 1
#define SEEK_SET 2

#define TMP_MAX 25  /* at least 25 */

extern FILE _stdio[3];

#define stderr (_stdio + 0)
#define stdin (_stdio + 1)
#define stdout (_stdio + 2)

int ungetc(int c, FILE *stream);

int setvbuf(FILE *stream, char *buf, int mode, size_t size);

void setbuf(FILE *stream, char *buf);

int fputs(const char *s, FILE *stream);
int putc( int c, FILE *stream );
int puts(const char *s);

int vfprintf(FILE *stream, const char *format, va_list arg);

int vprintf(const char *format, va_list arg);

int fprintf(FILE *stream, const char *format, ...);

int printf(const char *format, ...);

int vsprintf(char *s, const char *format, va_list arg);

int sprintf(char *s, const char *format, ...);

int sscanf(const char* s, const char* format, ...);
int scanf(const char* format, ...);
int fscanf(FILE* stream, const char* format, ...);

// # не стандартная функция, описанна в документации.
FILE *_fopen_userbuf(const char *filename, const char *mode, const void *buf, size_t bufsize);
FILE *fopen(const char *filename, const char *mode);
size_t fwrite(const void*, size_t, size_t, FILE* );
size_t fread(void*, size_t, size_t, FILE* );
FILE *freopen(const char *filename, const char *mode, FILE *stream);
int fclose(FILE *stream);
int fflush(FILE *stream);
int fgetc(FILE *stream);
int fputc(int c, FILE *stream);
int getchar(void);
int putchar(int c);
int fsetpos(FILE*, const fpos_t* );
int fgetpos(FILE*, fpos_t* );
int fseek(FILE*, long int, int );
int rename(const char* oldName, const char* newName);
int remove(const char *filename);
void rewind(FILE* );
char* tmpnam(char* );
FILE *tmpfile(void);
long int ftell(FILE* );
int feof(FILE* );
int ferror(FILE* );
void clearerr(FILE* );
char* fgets(char*, int, FILE* );
char* gets(char*);
int getc( FILE *stream );
void perror(const char* );
//==###=========================================================================
//  End of module:
//      $KW; $FN; $EKW;
//==============================================================================
