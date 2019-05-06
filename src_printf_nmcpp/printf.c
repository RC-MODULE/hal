//////////////////////////////////////////////////////////////////////////
//                                                                      //
//               NeuroMatrix(r) C runtime library                       //
//                                                                      //
//  printf.c                                                            //
//  Реализация, резидентная часть.                                      //
//                                                                      //
// Copyright (c) 2004-2008 RC Module                                    //
//                                                                      //
//  Автор: Бирюков А.А.                                                 //
//                                                                      //
// $Revision:: 2    $      $Date:: 25.06.13 19:20    $                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#pragma code_section ".text_hal_libc"
#include "host_duties.h"
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "section-printf.h"

//#include <setjmp.h>
// 
//extern jmp_buf buf;

extern "C"
{
	// Barrier sync with host PC with array exchange
	//int ncl_hostSyncArray(
	//	int value,        // Sync value
	//	void *outAddress, // Sended array address (can be NULL)
	//	size_t outLen,    // Sended array length (can be 0)
	//	void **inAddress, // Received array address pointer (can be NULL)
	//	size_t *inLen);   // Received array size pointer (can be NULL)

    int vsprintf( char *dst, const char* format, va_list args );

#pragma data_section ".rpc_services"
//  Это должно лежать в разделяемой памяти с определенным смещением
//  относительно начала секций
//  модифицируются только с платы
volatile int boardOutServBuf[ b2hBufSize ]; //  .rpc_services.bss +0
//  модифицируются только с хоста
volatile int backBuffer[ backBufferSize ];  //  .rpc_services.bss +b2hBufSize

//  модифицируются только с платы
volatile int boardSend=0;               //  пакеты  //  .rpc_services +0 
volatile int* sended= boardOutServBuf;  //  слова   //  .rpc_services +1 
volatile int* sendMostDistant=boardOutServBuf;      //  .rpc_services +2    //используется при возврате в начало буфера
volatile int nm_io_debug=0;                    // пc//  .rpc_services +3 
//  модифицируются только с хоста
volatile int hostReceive=0;                    // п //  .rpc_services +4 
volatile int* sendConfirmed=boardOutServBuf;   // с //  .rpc_services +5
//  модифицируется отовсюду
volatile int backBufferReady= 0;		       //   //  .rpc_services +6 
volatile int nm_io_debug_back=0;               // пc//  .rpc_services +7 
#pragma default_data_section 
};

static bool fullBuffer()
{ 
    if ( sendConfirmed>sended )
        return sended+ packetLimit >= sendConfirmed;
    else
        return false;
}

static void makeMsgHeader( int* ptr, NM_IO_ServiceID id, int size )
{
    //  Формат пакета: |размер- 1 слово|тип- 1 слово|данные- <размер> слов|
    *ptr++ = (int)size;
    *ptr++ = (int)id;
}

void dummy(){}

bool incomplete= false;
int* beginMessage()
{
    if ( incomplete )
        throw "rpc error: incomplete previous message";
    if ( (sended - boardOutServBuf) >= b2hBufLimit ){
        //longjmp(buf,boardSend);
        while ( sendConfirmed <= boardOutServBuf+packetLimit ) 
            dummy(); //  Здесь ждем при засоре 1
        sendMostDistant= sended;
        sended= boardOutServBuf;
    }
    while( fullBuffer() )
        dummy(); //  Здесь ждем при засоре 2
    incomplete= true;
    return (int*)sended+2;
}

void completeMessage( NM_IO_ServiceID id, int size )
{
    if ( !incomplete )
        throw "rpc error: can't complete- message is not started";
    if ( size+2 >packetLimit )
        throw "rpc error: too big message";
    incomplete= false;
    makeMsgHeader( (int*)sended, id, size );
    sended+= size+2;
    boardSend++;
    return;
}

extern "C" int printf( const char* format,...)
{
    va_list argptr;
    va_start(argptr, format);

    char* buf= (char*)beginMessage();
    nm_io_debug = (int)buf;
    int r= vsprintf( buf, format, argptr);
    nm_io_debug = 0;
    completeMessage( NMSYNC_SERVICE_PRINTF, r );
    va_end( argptr );
    return r;
}



extern "C" int printf_( const char* format,...)
{
    va_list argptr;
    va_start(argptr, format);

    char* buf= (char*)beginMessage();
    nm_io_debug = (int)buf;
    int r= vsprintf( buf, format, argptr);
    nm_io_debug = 0;
    completeMessage( NMSYNC_SERVICE_PRINTF, r );
    va_end( argptr );
    return r;
}

