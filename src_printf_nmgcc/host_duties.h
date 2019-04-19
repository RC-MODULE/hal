
//////////////////////////////////////////////////////////////////////////
//                                                                      //
//               NeuroMatrix(r) C runtime library                       //
//                                                                      //
//		���������� nm_io                                                //
//                                                                      //
//	����������� ���������� ��������� ����������,						//
//	������������ ��� ������ ��������� � ����� � ����					//
//                                                                      //
//  Copyright (c) 2004-2012 RC Module                                   //
//                                                                      //
//  �����: ������� �.�.                                                 //
//////////////////////////////////////////////////////////////////////////

enum NM_IO_ServiceID
{
	INDEX_TERMINATE= 0,
	INDEX_PUTCHAR=   1,
	INDEX_GETCHAR=   2,
	INDEX_OPEN   =   3,
	INDEX_CLOSE  =   4,
	INDEX_READ   =   5,
	INDEX_WRITE  =   6,
	INDEX_FLUSH  =   7,
	INDEX_GETPOS =   8,
	INDEX_SETPOS =   9,
	INDEX_RENAME =   10,
	INDEX_REMOVE =   11,
	INDEX_CLOCK  =   12,
	INDEX_TIME   =   13,
	INDEX_GETENV =   14,
	INDEX_SYSTEM =   15,
	INDEX_GMTIME =   16,
	INDEX_FILESIZE=  17,
	INDEX_FSTAT   =  18,
	INDEX_SYS_INFO=  19,
	NMSYNC_SERVICE_TRACE= 0xff000001,
    NMSYNC_SERVICE_PRINTF
};

const int NM_IO_ServiceVer= 1;

const int b2hBufSize = 0x20000;
//  ������ ������ �� ������ ��������� b2hBufSize-b2hBufLimit
const int packetLimit = 0x8000;
const int b2hBufLimit = b2hBufSize-packetLimit;

/*
���������� �������� ������� .nm_services
0: ������� ������� ��������� �����
1: ������� ��������� � ������ �� �����
2: ����� ������� ���������� ����� � ������
3: (���������������)��� ������� - �������� �� ����� �����
4: ������� ������� ������ ����
5: ����� � ������ �� ����� �� �������� ������ ����
6: ���������� ���� ��������� � �����
7: (���������������)
*/
const int bobSize= 8;
const int bobSizeIn= 4;
const int backBufferSize= 0x8000;
