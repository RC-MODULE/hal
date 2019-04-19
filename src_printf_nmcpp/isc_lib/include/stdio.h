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
#ifndef _STDIO_H
#define _STDIO_H

#include <stdarg.h>

#ifdef  __cplusplus
    #include "cstdio"
    
    using std::size_t;
    using std::FILE;
    using std::va_list;
    using std::fpos_t;
    
    using std::_stdio;
    using std::ungetc;
    using std::setvbuf;
    using std::setbuf;
    using std::fputs;
    using std::putc;
    using std::puts;
    using std::vfprintf;
    using std::vprintf;
    using std::fprintf;
    using std::printf;
    using std::vsprintf;
    using std::sprintf;            
    using std::sscanf;
    using std::scanf;
    using std::fscanf;                         
    using std::fopen;
    using std::_fopen_userbuf;
    using std::fwrite;
    using std::fread;
    using std::freopen;
    using std::fclose;
    using std::fflush;
    using std::fgetc;
    using std::fputc;
    using std::getchar;
    using std::putchar;
    using std::fsetpos;
    using std::fgetpos;
    using std::fseek;
    using std::rename;
    using std::remove;
    using std::rewind;
    using std::tmpnam;
    using std::tmpfile;
    using std::ftell;
    using std::feof;
    using std::ferror;
    using std::clearerr;
    using std::fgets;
    using std::gets;
    using std::getc;
    using std::perror;
#else
    #define _INTERNAL_NOT_IN_STD
        #include "internal\_stdio.h"
    #undef _INTERNAL_NOT_IN_STD
#endif
    
#endif
//==###=========================================================================
//  End of module:
//      $KW; $FN; $EKW;
//==============================================================================
    
    
    
    
    
