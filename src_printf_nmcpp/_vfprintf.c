#pragma code_section ".text_hal_libc"
#include "isc_lib\include\stdio.h"

#include <cassert>
#include "string.h"
#include "section-printf.h"
using namespace std;

#ifdef __cplusplus
    extern "C" {
#endif

char vfprintf_buf[ 8096 ];


int _vfprintf(FILE *stream, const char *format, va_list arg, int ){
    int r= vsprintf( vfprintf_buf, format, arg );
    if ( r>0 )
        r= fwrite( vfprintf_buf, 1, r, stream );
    return r;
}


#ifdef __cplusplus
    }
#endif
