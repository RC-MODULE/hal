#include "mc12101load.h"
#include <stdio.h>
#include "nm_io_host.h"
static PL_Board *board=0;
static PL_Access *access[2]={0,0};
static PL_Access* access_io[2]={0,0};
static NM_IO_Service *nmservice[2]={0,0};
#define TRACE(str) printf("%s", str)

static unsigned result[2];
static bool isFinished[2]={false,false};


extern "C"{

int ubcSync(int val,int processor=0){
	int ret;
#ifdef DEFAULT_PROCESSOR 
	PL_Sync(access[DEFAULT_PROCESSOR],val,&ret);
#else 
	PL_Sync(access[processor],val,&ret);
#endif
	return ret;
}

static unsigned sharedBuffer;
static unsigned sharedSize32;

int ubcOpen(char* absfile=0,...){

	va_list args;
	va_start(args, absfile);
	char* abs[3]={0,0,0};
	abs[0] = absfile;
	abs[1] = va_arg(args, char*);
	va_end(args);



	unsigned int boardCount,ok;
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


#ifdef DEFAULT_PROCESSOR 
	if (PL_GetAccess(board, DEFAULT_PROCESSOR, &access[DEFAULT_PROCESSOR])){
		TRACE( "ERROR: Can't access processor  on board    \n");
		return  (1);
	}
	if (PL_LoadProgramFile(access[DEFAULT_PROCESSOR], abs[0])){
		TRACE( ": : ERROR: Can't load program into board.\n");
		return  (1);
	}
	sharedBuffer=ubcSync(0x8086);
	sharedSize32=ubcSync(0x8086);
#else
	bool first_enter=true;
	for (int proc=0; abs[proc]!=0 ;proc++){
		if (strlen(abs[proc])){
			if (PL_GetAccess(board, proc, &access[proc])){
				TRACE( "ERROR: Can't access processor  0 on board  0  \n");
				return  (1);
			}
			if (PL_LoadProgramFile(access[proc], abs[proc])){
				TRACE( ": : ERROR: Can't load program into board.\n");
				return  (1);
			}
	

			
			if (first_enter){
				sharedBuffer=ubcSync(0x8086,proc);
				sharedSize32=ubcSync(0x8086,proc);
				first_enter=false;
			}
			else {
				ok=ubcSync(sharedBuffer,proc);
				ok=ubcSync(sharedSize32,proc);
			}
		}
	}
	for (int proc=0; abs[proc]!=0 ;proc++){
		if (strlen(abs[proc])){
			// io initialization
			if (PL_GetAccess(board, proc, &access_io[proc])){
				TRACE( "ERROR: Can't access processor  0 on board  0  \n");
				return  (1);
			}

			nmservice[proc]=new NM_IO_Service(abs[proc],access_io[proc]);		
			if (nmservice[proc]==0)
				return (1);
			if (nmservice[proc]->invalid()){
				PL_CloseAccess(access_io[proc]);
				delete nmservice[proc];
				nmservice[proc]=0;
				access_io[proc]=0;
			}
			else 
				break;
		}
	}
#endif
	
	
	TRACE ("OK!\n");
	return 0;
}
	

int ubcReadMemBlock (unsigned long* dstHostAddr, unsigned srcBoardAddr, unsigned size32, unsigned processor=0){
#ifdef DEFAULT_PROCESSOR 
	return PL_ReadMemBlock(access[DEFAULT_PROCESSOR], (PL_Word*)dstHostAddr, srcBoardAddr, size32);
#else
	return PL_ReadMemBlock(access[processor], (PL_Word*)dstHostAddr, srcBoardAddr, size32);
#endif
}

int ubcWriteMemBlock(unsigned long* srcHostAddr, unsigned dstBoardAddr, unsigned size32, unsigned processor=0){
#ifdef DEFAULT_PROCESSOR 
	return PL_WriteMemBlock(access[DEFAULT_PROCESSOR ], (PL_Word*)srcHostAddr, dstBoardAddr, size32);
#else
	return PL_WriteMemBlock(access[processor], (PL_Word*)srcHostAddr, dstBoardAddr, size32);
#endif
}


int ubcClose(){
	if (nmservice[0])
		delete nmservice[0];
	if (nmservice[1])
		delete nmservice[1];
	if (access[0])
		PL_CloseAccess(access[0]);
	if (access[1])
		PL_CloseAccess(access[1]);
	
	return PL_CloseBoardDesc(board);
}


int ubcGetResult(unsigned long* returnCode, int processor=0){
#ifdef DEFAULT_PROCESSOR 
	return PL_GetResult(access[DEFAULT_PROCESSOR], returnCode);
#else
	return PL_GetResult(access[processor], returnCode);
#endif
	
}

};