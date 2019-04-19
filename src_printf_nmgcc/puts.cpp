
#include "host_duties.h"

int* beginMessage_();
void completeMessage_( NM_IO_ServiceID id, int size );

extern "C" int puts_( const char* str );

extern "C" int puts( const char* str )
{
    char* buf= (char*)beginMessage_();
	int i;
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

