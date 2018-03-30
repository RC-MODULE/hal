#include "mc12101load.h"
#include <stdio.h>

#include "sleep.h"

#ifndef SILENT
#include "nm_io_host.h"
static PL_Access* access_io[2] = { 0,0 };
static NM_IO_Service *nmservice[2] = { 0,0 };
#endif



extern "C"{
#define TRACE(str) printf("%s", str)

static PL_Board *board = 0;
static PL_Access *access[2] = { 0,0 };
static unsigned result[2];
static bool isFinished[2] = { false,false };

static unsigned sharedBuffer;
static unsigned sharedSize32;
static unsigned activeSingleProc1=false;

int halSync(int val,int processor=0){
	int ret;
	if (activeSingleProc1)	processor = 1;
	PL_Sync(access[processor],val,&ret);
	return ret;
};


int halOpen(char* absfile=0,...){

	va_list args;
	va_start(args, absfile);
	char* abs[3]={0,0,0};
	abs[0] = absfile;
	abs[1] = va_arg(args, char*);
	va_end(args);



	unsigned boardCount,ok;
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

	if (strlen(abs[0])==0)
		activeSingleProc1= true;
	else 
		activeSingleProc1= false;

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
		}
	}
	for (int proc=0; abs[proc]!=0 ;proc++){
		if (strlen(abs[proc])){
#ifndef SILENT	
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
#endif				
		}
	}
	return 0;
}
	

int halReadMemBlock (unsigned long* dstHostAddr, unsigned srcBoardAddr, unsigned size32, unsigned processor=0){
	if (activeSingleProc1)	processor = 1;
	return PL_ReadMemBlock(access[processor], (PL_Word*)dstHostAddr, srcBoardAddr, size32);
}

int halWriteMemBlock(unsigned long* srcHostAddr, unsigned dstBoardAddr, unsigned size32, unsigned processor=0){
	if (activeSingleProc1)	processor = 1;
	return PL_WriteMemBlock(access[processor], (PL_Word*)srcHostAddr, dstBoardAddr, size32);
}


int halClose(){
	#ifndef SILENT	
	if (nmservice[0])
		delete nmservice[0];
	if (nmservice[1])
		delete nmservice[1];
	if (access_io[0])
		PL_CloseAccess(access_io[0]);
	if (access_io[1])
		PL_CloseAccess(access_io[1]);

	#endif
	if (access[0])
		PL_CloseAccess(access[0]);
	if (access[1])
		PL_CloseAccess(access[1]);
	return PL_CloseBoardDesc(board);
}


int halGetResult(unsigned long* returnCode, int processor=0){
	PL_Word status=0;
	if (activeSingleProc1)	processor = 1;
	while ((PROGRAM_FINISHED&status)==0){
		
		PL_GetStatus(access[processor],&status);
		halSleep(500);
	}
	return PL_GetResult(access[processor], returnCode);
}


};