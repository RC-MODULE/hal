
//////////////////////////////////////////////////////////////////////////
//                                                                      //
//               NeuroMatrix(r) C runtime library                       //
//                                                                      //
//  printf.c                                                            //
//  ����������, ����������� �����.                                      //
//                                                                      //
// Copyright (c) 2004-2008 RC Module                                    //
//                                                                      //
//  �����: ������� �.�.                                                 //
//                                                                      //
// $Revision:: 2    $      $Date:: 25.06.13 19:20    $                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "host_duties.h"
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

//#include <setjmp.h>
//
//extern jmp_buf buf;

#ifdef printf
#undef printf
#endif
#define printf printf_h
/*
extern "C"
{
int _exit(int);
	// Barrier sync with host PC with array exchange
	//int ncl_hostSyncArray(
	//	int value,        // Sync value
	//	void *outAddress, // Sended array address (can be NULL)
	//	size_t outLen,    // Sended array length (can be 0)
	//	void **inAddress, // Received array address pointer (can be NULL)
	//	size_t *inLen);   // Received array size pointer (can be NULL)

int vsprintf( char *dst, const char* format, va_list args );

struct RPC_send_region
{
//  ��� ������ ������ � ����������� ������ � ������������ ���������
//  ������������ ������ ������
//  �������������� ������ � �����
volatile int boardOutServBuf[ b2hBufSize ]; //  .rpc_services.bss +0
//  �������������� ������ � �����
volatile int backBuffer[ backBufferSize ];  //  .rpc_services.bss +b2hBufSize
};

__attribute__ ((weak, section(".rpc_services.bss"))) RPC_send_region volatile rpcBuff;


struct RPC_control_region
{
//  �������������� ������ � �����
int boardSend;               //  ������  //  .rpc_services +0
volatile int* sended;  //  �����   //  .rpc_services +1
volatile int* sendMostDistant;      //  .rpc_services +2    //������������ ��� �������� � ������ ������
int nm_io_debug;                    // �c//  .rpc_services +3
//  �������������� ������ � �����
int hostReceive;                    // � //  .rpc_services +4
volatile int* sendConfirmed;   // � //  .rpc_services +5
//  �������������� ��������
int backBufferReady;		       //   //  .rpc_services +6
int nm_io_debug_back;               // �c//  .rpc_services +7

};

__attribute__ ((weak, section(".rpc_services"))) RPC_control_region volatile rpcr=
		{
				0,               //  ������  //  .rpc_services +0
				rpcBuff.boardOutServBuf,  //  �����   //  .rpc_services +1
				rpcBuff.boardOutServBuf,      //  .rpc_services +2    //������������ ��� �������� � ������ ������
				0,                    // �c//  .rpc_services +3
				//  �������������� ������ � �����
				0,                    // � //  .rpc_services +4
				rpcBuff.boardOutServBuf,   // � //  .rpc_services +5
				//  �������������� ��������
				0,		       //   //  .rpc_services +6
				0               // �c//  .rpc_services +7
		};

};	//extern "C"

static inline bool fullBuffer()
{
    if ( rpcr.sendConfirmed>rpcr.sended )
        return rpcr.sended+ packetLimit >= rpcr.sendConfirmed;
    else
        return false;
}

static inline void makeMsgHeader( int* ptr, NM_IO_ServiceID id, int size )
{
    //  ������ ������: |������- 1 �����|���- 1 �����|������- <������> ����|
    *ptr++ = (int)size;
    *ptr++ = (int)id;
}

__attribute__((weak)) void dummy() {}

bool incomplete __attribute__((weak))= false;
__attribute__((weak)) int* beginMessage()
{
//    if ( incomplete )
//        throw "rpc error: incomplete previous message";
    if ( (rpcr.sended - rpcBuff.boardOutServBuf) >= b2hBufLimit ){
        //longjmp(buf,boardSend);
        while ( rpcr.sendConfirmed <= rpcBuff.boardOutServBuf+packetLimit )
            dummy(); //  ����� ���� ��� ������ 1
        rpcr.sendMostDistant= rpcr.sended;
        rpcr.sended= rpcBuff.boardOutServBuf;
    }
    while( fullBuffer() )
        dummy(); //  ����� ���� ��� ������ 2
    incomplete= true;
    return (int*)rpcr.sended+2;
}

__attribute__((weak)) void completeMessage( NM_IO_ServiceID id, int size )
{
//    if ( !incomplete )
//        throw "rpc error: can't complete- message is not started";
//    if ( size+2 >packetLimit )
//        throw "rpc error: too big message";
    incomplete= false;
    makeMsgHeader( (int*)rpcr.sended, id, size );
    rpcr.sended+= size+2;
    rpcr.boardSend++;
    return;
}*/

static inline int printf_h( const char* format,...)
{
    va_list argptr;
    va_start(argptr, format);

//    _exit( (int)rpcr.sended );
    char* buf= (char*)beginMessage();
    rpcr.nm_io_debug = (int)buf;
    int r= vsprintf( buf, format, argptr);
    rpcr.nm_io_debug = 0;
    completeMessage( NMSYNC_SERVICE_PRINTF, r+1 );
    va_end( argptr );
    return r;
}

