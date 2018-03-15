#include "mc5103load.h"
#include "stdio.h"
#include "nm_io_host.h"
#include "sleep.h"
static PL_Board *board=0;
static PL_Access *access=0;
static PL_Access* access_io=0;
static NM_IO_Service *nmservice=0;

#define TRACE(str) printf("%s", str)

extern "C"{

int halSync(int val){
	int ret;
	PL_Sync(access,val,&ret);
	return ret;
}

int halOpen(char* absfile=0,...){

	va_list args;
	va_start(args, absfile);
	char* abs[3]={0,0,0};
	abs[0] = absfile;
	abs[1] = va_arg(args, char*);
	va_end(args);
	if (absfile==0)
		absfile=abs[1];


	unsigned int boardCount;
	PL_Word Length = 0;
	
	PL_Word *Buffer = 0;
	PL_Word AddrLoad = 0;

	TRACE ("Connection to board...");
	if (PL_GetBoardCount(&boardCount))	{
		TRACE( "ERROR: Can't open driver.\n");
		return  (1);
	}

	if (boardCount < 1)	{
		TRACE( "ERROR: Can't find board.\n");
		return  (1);
	}

	if (PL_GetBoardDesc(0, &board))	{
		TRACE( "ERROR: Can't open board 0 \n");
		return  (1);
	}

	if (absfile){
		if (PL_ResetBoard(board)) {
			TRACE( "ERROR: Can't reset board  0 .\n");
			return  (1);
		}
		PL_ResetBoard(board);
	}

	if (PL_GetAccess(board, 0, &access)){
		TRACE( "ERROR: Can't access processor  0 on board  0  \n");
		return  (1);
	}
	
	
	
	if (absfile){
		if (PL_LoadProgramFile(access, absfile)){
			access=0;
			TRACE( ": : ERROR: Can't load program into board.\n");
			//return  (1);
		}
	}

	if (PL_GetAccess(board, 0, &access_io)){
		TRACE( "ERROR: Can't access processor  0 on board  0  \n");
		return  (1);
	}

	if (absfile){
		nmservice=new NM_IO_Service(absfile,access_io);		
		if (nmservice==0)
			return 1;

	}

		


	TRACE ("OK!\n");
	return 0;
}
	

int halReadMemBlock (unsigned long* dstHostAddr, unsigned srcBoardAddr, unsigned size32, unsigned processor=0){
	return PL_ReadMemBlock(access, (PL_Word*)dstHostAddr, srcBoardAddr, size32);
}

int halWriteMemBlock(unsigned long* srcHostAddr, unsigned dstBoardAddr, unsigned size32, unsigned processor=0){
	return PL_WriteMemBlock(access, (PL_Word*)srcHostAddr, dstBoardAddr, size32);
}

/*
void boardSleep()	//virtual int memcpy(unsigned* dst_addr, unsigned* src_addr, int size32){return 0;};
	void Sleep(int msecs){
		::Sleep(msecs);
	}
*/	
int halClose(){
	PL_CloseAccess(access);
	delete nmservice;
	return PL_CloseBoardDesc(board);
}

int halGetResult(unsigned* result,  unsigned processor=0){
	PL_Word status=0;
	while ((PROGRAM_FINISHED&status)==0){
		PL_GetStatus(access,&status);
		halSleep(500);
	}
	return PL_GetResult(access,(PL_Word*)result);
}

};