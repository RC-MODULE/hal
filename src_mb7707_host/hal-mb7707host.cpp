#include "mb7707load.h"
#include <stdio.h>
#include <stdlib.h>
#include "sleep.h"
static PL_Board *board=0;
static PL_Access *access=0;

#ifndef SILENT
#include "nm_io_host.h"
static PL_Access* access_io=0;
static NM_IO_Service *nmservice=0;
#endif
#define TRACE(str) printf("%s", str)


#define PL341_MEMC_STATUS             0x000
#define PL341_MEMC_CMD                0x004
#define PL341_DIRECT_CMD              0x008
#define PL341_MEMORY_CFG              0x00C 
#define PL341_REFRESH_PRD             0x010
#define PL341_CAS_LATENCY             0x014
#define PL341_WRITE_LATENCY           0x018
#define PL341_T_MRD                   0x01C
#define PL341_T_RAS                   0x020
#define PL341_T_RC                    0x024
#define PL341_T_RCD                   0x028
#define PL341_T_RFC                   0x02C
#define PL341_T_RP                    0x030
#define PL341_T_RRD                   0x034
#define PL341_T_WR                    0x038
#define PL341_T_WTR                   0x03C
#define PL341_T_XP                    0x040
#define PL341_T_XSR                   0x044
#define PL341_T_ESR                   0x048
#define PL341_MEMORY_CFG2             0x04C
#define PL341_MEMORY_CFG3             0x050
#define PL341_T_FAW                   0x054
#define PL341_TRD_EN                  0x05C


#define PL341_CHIP_0_CFG              0X200
#define PL341_CHIP_1_CFG              0X204
#define PL341_CHIP_2_CFG              0X208
#define PL341_CHIP_3_CFG              0X20C

#define EM0_Based				      0x40000000
#define EM1_Based				      0xC0000000

#define DMC0_APB_Based                0x20032000
#define DMC1_APB_Based                0x20031000

#define DDR_INITED 2
#define DDR_NOT_INITED 1



inline unsigned ARM2NM (unsigned arm_location){
	//	if (arm_location<0x40000000)
	//		return (((arm_location)>>2)-0x50000);
	//	else 
	return (arm_location>>2);
}

#define READ_MEM(addr,val)   PL_ReadMemBlock(access, &val, ARM2NM(addr),1)
#define WRITE_MEM(addr,val)  {PL_Word x=val; PL_WriteMemBlock(access, &x, ARM2NM(addr),1);}

volatile int DDR_EM0_Inited=DDR_NOT_INITED;		
volatile int DDR_EM1_Inited=DDR_NOT_INITED;


#ifdef __cplusplus
extern "C" {
	
	int static locked=false;
#endif // __cplusplus


	void Init_DDR_EM0();
	void Init_DDR_EM1();
	void INIT_DDR_EMX(unsigned DMCX_APB_Based);


void halLock(){
	while (locked);
	locked=true;
}	
void halUnlock(){
	locked=false;
}
int halSync(int val){
	halLock();
	PL_Word ret;
	PL_Sync(access,val,&ret);
	halUnlock();
	return ret;
}

int halOpen(char* absfile=0,...){
	//int boardCount;
	PL_Word Length = 0;
	
	PL_Word *Buffer = 0;
	PL_Word AddrLoad = 0;

	TRACE ("Connection to board...");

	unsigned long long MAC64;
	unsigned char MAC_ADDRESS[] = { 0x1A,0x2B,0x3C,0x4D,0x5E,0x6F,0,0,0,0 };
	char* strMacAddressEnv = getenv ("MB7707MAC");
	char  strMacAddress[128];
	if (strMacAddressEnv==NULL){
		TRACE("Warning: Environment variable MB7707MAC is not defined: 0x1A2B3C4D5E6F  will be used as default\n");
	} else {
		int i=0;
		while(*strMacAddressEnv!=0){
			char c=*(strMacAddressEnv++);
			if ((c>='0' && c<='9' )  || (c>='a' && c<='f') || (c>='A' && c<='F') )
				strMacAddress[i++]=c;
		}
		strMacAddress[i]=0;
		char* endp;
		MAC64=_strtoi64(strMacAddress,&endp,16);
		char* c=(char*)&MAC64;
		MAC_ADDRESS[0]=c[5];
		MAC_ADDRESS[1]=c[4];
		MAC_ADDRESS[2]=c[3];
		MAC_ADDRESS[3]=c[2];
		MAC_ADDRESS[4]=c[1];
		MAC_ADDRESS[5]=c[0];
	}


	if (PL_GetBoardDesc(MAC_ADDRESS, &board)!=PL_OK){
		TRACE( "ERROR: Can't open board 0 \n");
		return  (1);
	}


	if (PL_GetAccess(board, 0, &access)){
		TRACE( "ERROR: Can't access processor  0 on board  0  \n");
		return  (1);
	}

	Init_DDR_EM0();
	Init_DDR_EM1();

	WRITE_MEM(EM0_Based,0xBABADEDA);
	PL_Word t;
	READ_MEM(EM0_Based,t);
	if (t!=0xBABADEDA){
		TRACE( "ERROR: Init DDR!\n");
		return  (1);
	}

	if (PL_LoadInitCode(access) != PL_OK)
	{
		TRACE( "ERROR: Can't load init code!\n");
		return  (1);
	}

	
	if (PL_LoadProgramFile(access, absfile)){
		access=0;
		TRACE( ": : ERROR: Can't load program into board.\n");
		return  (1);
	}

	//unsigned sharedBuffer=halSync(0x8086);
	//unsigned sharedSize32=halSync(0x8086);
#ifndef SILENT	
	if (absfile){
		nmservice=new NM_IO_Service(absfile,access);		
		if (nmservice==0)
			return 1;

	}
#endif	


	TRACE ("OK!\n");
	return 0;
}
	

int halReadMemBlock (void* dstHostAddr, unsigned srcBoardAddr, unsigned size32, unsigned processor=0){
	halLock();
	int ret=PL_ReadMemBlock(access, (PL_Word*)dstHostAddr, srcBoardAddr, size32);
	halUnlock();
	return ret;
}

int halWriteMemBlock(void* srcHostAddr, unsigned dstBoardAddr, unsigned size32, unsigned processor=0){
	halLock();
	int ret=PL_WriteMemBlock(access, (PL_Word*)srcHostAddr, dstBoardAddr, size32);
	halUnlock();
	return ret;
}

int halGetStatus(PL_Word *status){
	halLock();
	int ret=PL_GetStatus(access, status);
	halUnlock();
	return ret;
}
/*
void boardSleep()	//virtual int memcpy(unsigned* dst_addr, unsigned* src_addr, int size32){return 0;};
	void Sleep(int msecs){
		::Sleep(msecs);
	}
*/	
int halClose(){
	#ifndef SILENT	
	delete nmservice;
	#endif
	PL_CloseAccess(access);
	return PL_CloseBoardDesc(board);
}

int halGetResult(unsigned* result,  unsigned processor=0){
	PL_Word status=0;
	while ((PROGRAM_FINISHED&status)==0){
		//PL_GetStatus(access,&status);
		halGetStatus(&status);
		halSleep(500);
	}

	return PL_GetResult(access,(PL_Word*)result);
}

void Init_DDR_EM0(){

	if (DDR_EM0_Inited==DDR_NOT_INITED){
		PL_Word t;
		WRITE_MEM(0x2003c050,0x1c); //DDR PHY ODT ON
		//	int r=PL_WriteMemBlock(access, &x, ARM2NM(0x2003c050),1);

		//tube_msg("INIT DDR EM0 ...");

		READ_MEM(0x2003c050,t); //DDR PHY ODT ON


		INIT_DDR_EMX(DMC0_APB_Based);
		DDR_EM0_Inited=DDR_INITED;
	}


}



void Init_DDR_EM1(){

	if (DDR_EM1_Inited==DDR_NOT_INITED){
		WRITE_MEM(0x2003c064,0x1c); //DDR PHY ODT ON

		PL_Word t;
		READ_MEM(0x2003c064,t); //DDR PHY ODT ON

		//tube_msg("INIT DDR EM1 ...");
		INIT_DDR_EMX(DMC1_APB_Based);
		DDR_EM1_Inited=DDR_INITED;
	}

}

void INIT_DDR_EMX(unsigned DMCX_APB_Based){
	PL_Word v;
	int i;
	volatile static unsigned PL341_Direct_Cmd[]={
		0x000c0000,    //nop
		0x00000000,    //precharge
		0x00040000,    //autorefresh
		0x00040000,    //autorefresh
		0x00080852,    //modereg 0
		0x00090004,    //modereg 1 - ODT 75
	};

	WRITE_MEM(DMCX_APB_Based+PL341_MEMC_CMD,0x00000003);
	//while ((MEM(DMCX_APB_Based+PL341_MEMC_STATUS)&0x3) != 0x2) {}

	WRITE_MEM(DMCX_APB_Based+PL341_MEMC_CMD,0x00000004);

	do {
		READ_MEM(DMCX_APB_Based+PL341_MEMC_STATUS,v);

	} while ((v&0x3)!= 0x0);


	READ_MEM(DMCX_APB_Based+PL341_MEMC_STATUS,v);
	WRITE_MEM(0x180200,v);

	WRITE_MEM(DMCX_APB_Based+PL341_REFRESH_PRD,0x00000a28); //

	WRITE_MEM(DMCX_APB_Based+PL341_CAS_LATENCY,0x0000000a); //CAS=4

	WRITE_MEM(DMCX_APB_Based+PL341_TRD_EN,0x00000003);

	// tCK=1/324 MHz = 0.003086 * 10^-6 = 3.086 ns= 3 ns
	// tACLK = 1/162= 0.00617 * 10^-6 = 6.17 ns = 6 ns
	WRITE_MEM(DMCX_APB_Based+PL341_T_RAS,0x0000000F); // tRAS = 45 ns / tCK = 15 = 0xF
	WRITE_MEM(DMCX_APB_Based+PL341_T_RC ,0x00000014);  // tRC  = 60 ns / tCK = 20 = 0x14
	WRITE_MEM(DMCX_APB_Based+PL341_T_RCD,0x00000305); // tRCD = 15 ns / tCK = 5  = 0x5
	WRITE_MEM(DMCX_APB_Based+PL341_T_RFC,0x0000132B); // tRFC=127,5 ns/tACLK - 3 = 19 = 0x13; tRFC = 127,5 ns/tACLK= 42,5 = 0x2B;
	WRITE_MEM(DMCX_APB_Based+PL341_T_RP ,0x00000005);  // tRP  = 15 ns / tCK = 5 = 0x5; tRP  = 15 ns / tACLK -3 = 2,5 - 3 =0
	WRITE_MEM(DMCX_APB_Based+PL341_T_RRD,0x00000004); // tRRD = 10 ns / tCK = 3,33=4
	WRITE_MEM(DMCX_APB_Based+PL341_T_WR ,0x00000005);  // tWR  = 15 ns / tCK = 5
	WRITE_MEM(DMCX_APB_Based+PL341_T_WTR,0x00000003); // tWTR = 7,5 ns/ tCK = 2,5=3
	WRITE_MEM(DMCX_APB_Based+PL341_T_XP ,0x00000002);  // tXP = 2
	WRITE_MEM(DMCX_APB_Based+PL341_T_XSR,0x000000c8); //
	WRITE_MEM(DMCX_APB_Based+PL341_T_ESR,0x000000c8); //
	WRITE_MEM(DMCX_APB_Based+PL341_T_MRD,0x00000002);

	WRITE_MEM(DMCX_APB_Based+PL341_MEMORY_CFG, 0x0001001a); //1 chip 14 row 10 column
	WRITE_MEM(DMCX_APB_Based+PL341_MEMORY_CFG2,0x00000030);

	WRITE_MEM(DMCX_APB_Based+0x200,0x00001ff0); //BA-RA-CA

	for(i=0; i<sizeof(PL341_Direct_Cmd)/4;i++){
		WRITE_MEM(DMCX_APB_Based+PL341_DIRECT_CMD,PL341_Direct_Cmd[i]);
	}

	WRITE_MEM(DMCX_APB_Based+PL341_MEMC_CMD,0x00000000);

	int r=READ_MEM(DMCX_APB_Based+PL341_MEMC_STATUS,v);
	//do{
	//	READ_MEM(DMCX_APB_Based+PL341_MEMC_STATUS,v);
	//		
	//} while (v&0x3 != 0x1);

}

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
