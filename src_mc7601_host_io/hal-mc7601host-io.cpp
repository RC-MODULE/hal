#include "mc7601_ll_host.h"
#include "nm_io_host.h"
#include <stdio.h>
#include <stdlib.h>
static WR_Access *access[2]={0,0};
static WR_Board  *board=0;
static WR_Access* access_io[2]={0,0};
static NM_IO_Service *nmservice[2]={0,0};
static unsigned result[2];
static bool isFinished[2]={false,false};
#define TRACE(str) printf("%s", str)

const int IP_ADDRESS = 0x0201A8C0; // 192.168.1.2

extern "C"{

int halSync(int val, unsigned processor=0){
	WR_Word ret;
	WR_Sync(access[processor],0xFFFFFFFF,val,&ret);
	return ret;
}

int halOpen(char* absfile,...){
	
	system ("taskkill /F /IM LLC_MULTI_TASK_DAEMON.exe >nul");
	//tasklist /FI "IMAGENAME eq LLC1*

	WR_channel_conf conf;
	WR_Word Length = 0;

	WR_Word *Buffer = 0;
	WR_Word AddrLoad = 0;
	
	char* ipStrAddress;
	int   ipAddress=0x0201A8C0;
	ipStrAddress = getenv ("MC7601IP");
	if (ipStrAddress==NULL){
		TRACE("Warning: Environment variable MC7601 is not defined: 192.168.1.2 will be used as default\n");
	}
	else {
		printf("Current MC7601IP is,%s\n",ipStrAddress);
		//....
	}
  

	va_list args;
	va_start(args, absfile);
	char* abs[3]={0,0,0};
	abs[0] = absfile;
	abs[1] = va_arg(args, char*);
	va_end(args);

	WR_SetChannel(conf, ANMETHERNET, ipAddress, 6901);
	if (WR_GetBoardDesc(&conf, &board) != WR_OK)
	{
		TRACE("ERROR: Can't open board!\n");
		return (1);
	}

	for(int processor=0; abs[processor]; processor++){
		if (WR_GetAccess(board, processor, &access[processor]) != WR_OK)
		{
			TRACE( "ERROR: Can't access processor  0 on board  0  \n");
			return  (1);
		}

		if (WR_LoadInitCode(access[processor]) != WR_OK)
		{
			TRACE ("ERROR: Can't load init code!\n");
			return  (1);
		}

		if (WR_LoadProgramFile(access[processor], abs[processor], 0xFFFFFFFF) != WR_OK)
		{
			TRACE ("ERROR: Can't load program!\n");
			return  (1);
		}
	}
	for (int proc=0; abs[proc]!=0 ;proc++){
		if (strlen(abs[proc])){
			// io initialization
			if (WR_GetAccess(board, proc, &access_io[proc])){
				TRACE( "ERROR: Can't access processor  0 on board  0  \n");
				return  (1);
			}

			nmservice[proc]=new NM_IO_Service(abs[proc],access_io[proc]);		
			if (nmservice[proc]==0)
				return (1);
			if (nmservice[proc]->invalid()){
				WR_CloseAccess(access_io[proc]);
				delete nmservice[proc];
				nmservice[proc]=0;
				access_io[proc]=0;
			}
			else 
				break;
		}
	}
	return WR_OK;
}
	



int halReadMemBlock (unsigned long* dstHostAddr, unsigned srcBoardAddr, unsigned size32, unsigned processor=0){
	return WR_ReadMemBlock(access[processor], (WR_Word*)dstHostAddr, srcBoardAddr, size32);
}

int halWriteMemBlock(unsigned long* srcHostAddr, unsigned dstBoardAddr, unsigned size32, unsigned processor=0){
	return WR_WriteMemBlock(access[processor], (WR_Word*)srcHostAddr, dstBoardAddr, size32);
}

/*
void boardSleep()	//virtual int memcpy(unsigned* dst_addr, unsigned* src_addr, int size32){return 0;};
	void Sleep(int msecs){
		::Sleep(msecs);
	}
*/	
int halClose(){
	if (nmservice[0])
		delete nmservice[0];
	if (nmservice[1])
		delete nmservice[1];

		
	if (access[0]){
		if (isFinished[0]==false){
			WR_WaitEndProgram(access[0], &result[0], 0xFFFFFFFF) ;
			isFinished[0]=true;
		}
		WR_CloseAccess(access[0]);
	}
	if (access[1]){
		if (isFinished[1]==false){
			WR_WaitEndProgram(access[1], &result[1], 0xFFFFFFFF) ;
			isFinished[1]=true;
		}
		WR_CloseAccess(access[1]);
	}
	return WR_CloseBoardDesc(board);
}

int halGetResult(unsigned* returnCode, int processor=0){
	if (isFinished[processor]){
		*returnCode=result[processor];
		return WR_OK;
	} 
	else {
		int ret=WR_WaitEndProgram(access[processor], &result[processor], 0xFFFFFFFF);
		isFinished[processor]=true;
		*returnCode=result[processor];
		return ret;
	}
}

};