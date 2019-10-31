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
#include "section-printf.h"
//#include <setjmp.h>
//
//extern jmp_buf buf;
#pragma GCC optimize ("O0")

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

	INSECTION(".rpc_services.bss") RPC_send_region volatile rpcBuff_;


	struct RPC_control_region
	{
		//  �������������� ������ � �����
		int boardSend;               //  ������  //  .rpc_services +0
		volatile int* sended;  //  �����   //  .rpc_services +1
		volatile int* sendMostDistant;      //  .rpc_services +2    //������������ ��� �������� � ������ ������
		volatile int nm_io_debug;                    // �c//  .rpc_services +3
		//  �������������� ������ � �����
		volatile int hostReceive;                    // � //  .rpc_services +4
		volatile int* sendConfirmed;   // � //  .rpc_services +5
		//  �������������� ��������
		volatile int backBufferReady;		       //   //  .rpc_services +6
		volatile int nm_io_debug_back;               // �c//  .rpc_services +7
	};

	INSECTION(".rpc_services") RPC_control_region volatile rpcr_=
	{
			0,               //  ������  //  .rpc_services +0
			rpcBuff_.boardOutServBuf,  //  �����   //  .rpc_services +1
			rpcBuff_.boardOutServBuf,      //  .rpc_services +2    //������������ ��� �������� � ������ ������
			0,                    // �c//  .rpc_services +3
			//  �������������� ������ � �����
			0,                    // � //  .rpc_services +4
			rpcBuff_.boardOutServBuf,   // � //  .rpc_services +5
			//  �������������� ��������
			0,		       //   //  .rpc_services +6
			0               // �c//  .rpc_services +7
	};

};	//extern "C"

INSECTION(".text_printf") static bool __attribute__((optimize("O0"))) fullBuffer()
{
    if ( rpcr_.sendConfirmed>rpcr_.sended )
        return rpcr_.sended+ packetLimit >= rpcr_.sendConfirmed;
    else
        return false;
}

INSECTION(".text_printf") static void __attribute__((optimize("O0"))) makeMsgHeader( int* ptr, NM_IO_ServiceID id, int size )
{
    //  ������ ������: |������- 1 �����|���- 1 �����|������- <������> ����|
    *ptr++ = (int)size;
    *ptr++ = (int)id;
}

INSECTION(".text_printf") void __attribute__((optimize("O0"))) dummy_(){}

INSECTION(".rpc_services") bool volatile incomplete_= false;

INSECTION(".text_printf") int* __attribute__((optimize("O0"))) beginMessage_()
{
//    if ( incomplete )
//        throw "rpc error: incomplete previous message";
    if ( (rpcr_.sended - rpcBuff_.boardOutServBuf) >= b2hBufLimit ){
        //longjmp(buf,boardSend);
        while ( rpcr_.sendConfirmed <= rpcBuff_.boardOutServBuf+packetLimit )
            dummy_(); //  ����� ���� ��� ������ 1
        rpcr_.sendMostDistant= rpcr_.sended;
        rpcr_.sended= rpcBuff_.boardOutServBuf;
    }
    while( fullBuffer() )
        dummy_(); //  ����� ���� ��� ������ 2
    incomplete_= true;
    return (int*)rpcr_.sended+2;
}


	INSECTION(".text_printf") void __attribute__((optimize("O0"))) completeMessage_( NM_IO_ServiceID id, int size )
	{
	//    if ( !incomplete )
	//        throw "rpc error: can't complete- message is not started";
	//    if ( size+2 >packetLimit )
	//        throw "rpc error: too big message";
		incomplete_= false;
		makeMsgHeader( (int*)rpcr_.sended, id, size );
		rpcr_.sended+= size+2;
		rpcr_.boardSend++;
		return;
	}
extern "C" {
	INSECTION(".text_printf") int __attribute__((optimize("O0"))) printf( const char* format,...)
	{
		va_list argptr;
		va_start(argptr, format);

	//    _exit( (int)rpcr_.sended );
		char* buf= (char*)beginMessage_();
		rpcr_.nm_io_debug = (int)buf;
		int r= vsnprintf( buf, packetLimit-4, format, argptr);
		rpcr_.nm_io_debug = 0;
		completeMessage_( NMSYNC_SERVICE_PRINTF, r );
		va_end( argptr );
		return r;
	}


	INSECTION(".text_printf") int __attribute__((optimize("O0"))) puts_( const char* str )
	{
		volatile char* buf= (char*)beginMessage_();
		volatile int i;
		//	��� �������, ������ \0 � ���������� ������
		for( i=0; (*str) && (i<packetLimit-4); i++ )
			*buf++ = *str++;

		if (*str == 0){
			*buf= '\n';
			i++;
		}

		completeMessage_( NMSYNC_SERVICE_PRINTF, i );
		//	��������� ������, ��������� ���� ���� ���������� ������
		if ( *str )
			i += puts_( str );
		return i;
	}
};