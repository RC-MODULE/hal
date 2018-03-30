//////////////////////////////////////////////////////////////////////////
//                                                                      //
//               NeuroMatrix(r) C runtime library                       //
//                                                                      //
//		���������� nm_io                                                //
//                                                                      //
//	����������� ���������������� ��������� �������� �����    			//
//                                                                      //
//  Copyright (c) 2004-2012 RC Module                                   //
//                                                                      //
//  �����: ������� �.�.                                                 //
//////////////////////////////////////////////////////////////////////////

#include <iostream>

//	������������ �� ������������� ����� ���������� �������� � ������
typedef unsigned long PL_Word;
typedef unsigned long PL_Addr;


#include "mc7601_ll_host.h"
//	END #include "mc5101load.h"

class NM_IO_Service
{
    unsigned long datSecAddr;
    unsigned long bssSecAddr;
    unsigned long datSecSize;
    unsigned long bssSecSize;
    static bool work;
    WR_Access* a;
    int* bsStatus;
    int* bsBuf;
    int* readStatus();
    void writeBackStatus();
    volatile bool letsShutDown;
    unsigned int dispatch();
	bool valid;
	std::ostream* nm_cout;
//    NM_IO_Service( const char* fName, PL_Access* acc );
	WR_Word processPrintf( int* bufferFromBoard, int received  );
public:
    static int freeze;  //  1 - stop; 0 - allowed; 2 - working
	bool invalid() const {	return !valid; }
    NM_IO_Service( const char* fName, WR_Access* access, std::ostream* _cout= &std::cout, 
							std::ostream* _cerr= &std::cerr, std::istream* _cin= &std::cin );
    ~NM_IO_Service();
	friend unsigned int __stdcall nmServiceDispatcher( void* v_nms );
};