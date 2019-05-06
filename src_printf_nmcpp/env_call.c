//----------------------------------------------------------------------//
//                                                                      //
//         NeuroMatrix SDK                                              //
//         Библиотека С: работа с файлами                               //
//         Реализация системных вызовов при работе с файлами            //
//         как вызова удаленных процедур на хосте                       //
//         ответная часть на хосте - services.cpp                       //
//                                                                      //
//         Автор: Алексей Бирюков                                       //
//                                                                      //
//                      (c) RC Module Inc., Moscow, Russia              //
//                                                                      //
//----------------------------------------------------------------------//
#pragma code_section ".text_hal_libc"
#include <cassert>
#include "string.h"
#include "host_duties.h"
#include "section-printf.h"

#ifndef __GNUC__
typedef int _off_t;                /* file offset value */
#endif

typedef int _HANDLE;
typedef int fpos_t;

//  Тип возвращаемый функцией _fstat, должен определяться в stat.h

typedef unsigned short _ino_t;      /* i-node number (not used on DOS) */
typedef unsigned int _dev_t;        /* device code */
typedef long long __time64_t;     /* 64-bit time value */

struct _stat64i32 {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned short st_mode;
        short      st_nlink;
        short      st_uid;
        short      st_gid;
        _dev_t     st_rdev;
        _off_t     st_size;
        __time64_t st_atime;
        __time64_t st_mtime;
        __time64_t st_ctime;
};

#define _stat  _stat64i32
#define _fstat  _stat64i32


int* beginMessage();
void completeMessage( NM_IO_ServiceID id, int size );

//  errno ???

#ifdef __cplusplus
    extern "C" {
#endif

extern int exit(int);

extern volatile int backBuffer[ backBufferSize ];
extern volatile int backBufferReady;
extern int errno;

INSECTION(".text-printf.h") int _putchar(int ch){
    char* buf= (char*)beginMessage();
	*buf++ =ch;
	completeMessage( INDEX_PUTCHAR, 1 );
    return 0;
}

INSECTION(".text-printf.h") int fff( volatile int i)//  for volatile bug
{
    return i;
}

INSECTION(".text-printf.h") void wait_for_host()
{
	while ( fff( backBufferReady )==0 )
		;
}

INSECTION(".text-printf.h") int _getchar(void){
    beginMessage();
	completeMessage( INDEX_GETCHAR, 0 );
    wait_for_host();
    assert(backBufferReady==1);
	backBufferReady=0;
    return backBuffer[0];
}

INSECTION(".text-printf.h") _HANDLE _open(const char* fileName, unsigned int oflags){
    int* buf= beginMessage();
    int* end= buf;
    do
        *end++ = *fileName;
    while (*fileName++);
	*end++ =oflags;
	completeMessage( INDEX_OPEN, end-buf );
    wait_for_host();
    assert(backBufferReady==2);
	backBufferReady=0;
    errno= backBuffer[1];
    return backBuffer[0];
}

INSECTION(".text-printf.h") int _close(_HANDLE file){
    char* buf= (char*)beginMessage();
	*buf++ =file;
	completeMessage( INDEX_CLOSE, 1 );
    wait_for_host();
    assert(backBufferReady==2);
	backBufferReady=0;
    errno= backBuffer[1];
    return backBuffer[0];
}

INSECTION(".text-printf.h") int _read(_HANDLE file, void* dst, int size){
    char* buf= (char*)beginMessage();
    *buf++= file;
    *buf++= reinterpret_cast<int>( dst);
    *buf++= size;
	completeMessage( INDEX_READ, 3 );
    wait_for_host();
    assert( backBufferReady== backBuffer[0]+2 );
    int i;
    int* iDst= (int*)dst;
    if ( backBuffer[0] >0 ){
        for (i=0; i<backBuffer[0]; i++ )
        {
            *iDst++ = backBuffer[i+2];
        }
    }
    backBufferReady= 0;
    errno= backBuffer[1];
    return backBuffer[0];
}

INSECTION(".text-printf.h") int _write(_HANDLE file, const void* content, int size){
//    exit (115);
    int* buf= (int*)beginMessage();
    *buf++= file;
    *buf++= size;
    int i;
    for (i=0; i<size; i++ )
    {
        *buf++= ((int*)content)[i];
    }
	completeMessage( INDEX_WRITE, size+2 );
    wait_for_host();
    assert(backBufferReady==2);
	backBufferReady=0;
    errno= backBuffer[1];
    return backBuffer[0];
}

INSECTION(".text-printf.h") int _flush( _HANDLE ){
    //  параметр игнорируем, поскольку делать будем _flushall:
    //  _flush(_HANDLE) на хосте нету
    beginMessage();
	completeMessage( INDEX_FLUSH, 0 );
    return 0;
}

INSECTION(".text-printf.h") int _getpos(_HANDLE file, fpos_t* pos){
    int* buf= (int*)beginMessage();
    *buf++= file;
	completeMessage( INDEX_GETPOS, 1 );
    wait_for_host();
    assert(backBufferReady== 2 );
	backBufferReady=0;
    errno= backBuffer[1];
    if ( backBuffer[0] != -1 ){
        *pos= backBuffer[0];
        return -1;
    }
    return 0;
}

INSECTION(".text-printf.h") int _setpos(_HANDLE file, const fpos_t* pos){
    int* buf= (int*)beginMessage();
    *buf++= file;
    *buf++= *pos;
	completeMessage( INDEX_SETPOS, 2 );
    wait_for_host();
    assert(backBufferReady==2);
	backBufferReady=0;
    errno= backBuffer[1];
    return backBuffer[0]!=-1 ? -1:0;
}

INSECTION(".text-printf.h") int _rename(const char* oldName, const char* newName){
    char* buf= (char*)beginMessage();
    int len1= strlen( oldName );
    int len2= strlen( newName );
    strcpy( buf, oldName );
    strcpy( buf+len1+1, newName );
	completeMessage( INDEX_RENAME, len1+len2+2 );
    wait_for_host();
    assert(backBufferReady==2);
	backBufferReady=0;
    errno= backBuffer[1];
    return backBuffer[0];
}

INSECTION(".text-printf.h") int _remove(const char* path){
    char* buf= (char*)beginMessage();
    int len= strlen( path );
    strcpy( buf, path );
	completeMessage( INDEX_REMOVE, len+1 );
    wait_for_host();
    assert(backBufferReady==2);
	backBufferReady=0;
    errno= backBuffer[1];
    return backBuffer[0];
}

INSECTION(".text-printf.h") size_t _getenv(const char *name, char *p_buf, size_t bufsize){
    char* buf= (char*)beginMessage();
    int len= strlen( name );
    strcpy( buf, name );
	completeMessage( INDEX_GETENV, len+1 );
    wait_for_host();
    int i;
    for ( i=0; i<bufsize; i++ ){
        p_buf[i]= backBuffer[i+1];
        if ( p_buf[i]==0 )
            break;
    }
	backBufferReady=0;
    if ( i==bufsize ){
        errno= 34;//ERANGE;
    }
    else
        errno= backBuffer[0];
    return i;
}

INSECTION(".text-printf.h") int system(const char *command){
    char* buf= (char*)beginMessage();
    int len= strlen( command );
    strcpy( buf, command );
	completeMessage( INDEX_SYSTEM, len+1 );
    wait_for_host();
    assert(backBufferReady==2);
	backBufferReady=0;
    errno= backBuffer[1];
    return backBuffer[0];
}
//int printf(const char *format, ...);

INSECTION(".text-printf.h") int _fstat(_HANDLE handle, struct _stat *buffer){

#ifdef _USE_32BIT_TIME_T
#error _USE_32BIT_TIME_T is unsupported
#endif

    char* buf= (char*)beginMessage();
	*buf++ =handle;
	completeMessage( INDEX_FSTAT, 1 );
    wait_for_host();
    assert(backBufferReady== sizeof(struct _stat)+2 );
	backBufferReady=0;
    errno= backBuffer[1];
    if ( backBuffer[0]==0 ){
        memcpy( buffer, (int const*)backBuffer+2, sizeof(struct _stat) );
    }
    return backBuffer[0];
}

//extern "C" int putchar( int c );
//
//int _fstatX(_HANDLE handle, void *buffer){
//
//#ifdef _USE_32BIT_TIME_T
//#error _USE_32BIT_TIME_T is unsupported
//#endif
//
////    printf( "=x%d=", handle );
//    putchar( '=' );
//    putchar( '>' );
//    putchar( '0'+(handle/100000)%10 );
//    putchar( '0'+(handle/10000)%10 );
//    putchar( '0'+(handle/1000)%10 );
//    putchar( '0'+(handle/100)%10 );
//    putchar( '0'+(handle/10)%10 );
//    putchar( '0'+(handle/1)%10 );
//    putchar( '=' );
//    char* buf= (char*)beginMessage();
//	*buf++ =handle;
//	completeMessage( INDEX_FSTAT, 1 );
//    wait_for_host();
//    assert(backBufferReady== sizeof(struct _stat)+2 );
//	backBufferReady=0;
//    errno= backBuffer[1];
//    if ( backBuffer[0]==0 ){
//        memcpy( buffer, (int const*)backBuffer+2, sizeof(struct _stat) );
//    }
//    return backBuffer[0];
//}

/* Function to get system information*/
INSECTION(".text-printf.h") size_t _sys_info (int /*operation*/, void* buf, size_t bufsize)
{
    int i;
    for ( i=0; i<bufsize; i++ ){
        ((int*)buf)[i]=42;
    }
    return i;
//    return _env_call_func(INDEX_SYS_INFO, operation, buf, bufsize);
}


INSECTION(".text-printf.h") fpos_t _getf_size(_HANDLE file){
    struct _stat buffer;
    _fstat( file, &buffer );
    return buffer.st_size;
}


#ifdef __cplusplus
    }
#endif
