#include "ddr.h"

#ifndef MEM
#define MEM(addr)  (*((unsigned volatile*)(addr)))
#endif
//#include "common.h"
//#include "defs.h"
//#include "tube.h"
volatile int DDR_EM0_Inited=DDR_NOT_INITED;		
volatile int DDR_EM1_Inited=DDR_NOT_INITED;

int i;
//#include <malloc.h>
void INIT_DDR_EMX(unsigned DMCX_APB_Based){
	//malloc(10);
	int i;
	volatile static unsigned PL341_Direct_Cmd[]={
		0x000c0000,    //nop
		0x00000000,    //precharge
		0x00040000,    //autorefresh
		0x00040000,    //autorefresh
		0x00080852,    //modereg 0
		0x00090004,    //modereg 1 - ODT 75
	};
	
	MEM(DMCX_APB_Based+PL341_MEMC_CMD)=0x00000003;
	while ((MEM(DMCX_APB_Based+PL341_MEMC_STATUS)&0x3) != 0x2) {}
  
	MEM(DMCX_APB_Based+PL341_MEMC_CMD)=0x00000004;
	while ((MEM(DMCX_APB_Based+PL341_MEMC_STATUS)&0x3) != 0x0) {}
    
	MEM(0x180200)=MEM(DMCX_APB_Based+PL341_MEMC_STATUS);
	
	MEM(DMCX_APB_Based+PL341_REFRESH_PRD)=0x00000a28; //

	MEM(DMCX_APB_Based+PL341_CAS_LATENCY)=0x0000000a; //CAS=4
	
	MEM(DMCX_APB_Based+PL341_TRD_EN)=0x00000003;

// tCK=1/324 MHz = 0.003086 * 10^-6 = 3.086 ns= 3 ns
// tACLK = 1/162= 0.00617 * 10^-6 = 6.17 ns = 6 ns
	MEM(DMCX_APB_Based+PL341_T_RAS)=0x0000000F; // tRAS = 45 ns / tCK = 15 = 0xF
	MEM(DMCX_APB_Based+PL341_T_RC)=0x00000014;  // tRC  = 60 ns / tCK = 20 = 0x14
	MEM(DMCX_APB_Based+PL341_T_RCD)=0x00000305; // tRCD = 15 ns / tCK = 5  = 0x5
	MEM(DMCX_APB_Based+PL341_T_RFC)=0x0000132B; // tRFC=127,5 ns/tACLK - 3 = 19 = 0x13; tRFC = 127,5 ns/tACLK= 42,5 = 0x2B;
	MEM(DMCX_APB_Based+PL341_T_RP)=0x00000005;  // tRP  = 15 ns / tCK = 5 = 0x5; tRP  = 15 ns / tACLK -3 = 2,5 - 3 =0
	MEM(DMCX_APB_Based+PL341_T_RRD)=0x00000004; // tRRD = 10 ns / tCK = 3,33=4
	MEM(DMCX_APB_Based+PL341_T_WR)=0x00000005;  // tWR  = 15 ns / tCK = 5
	MEM(DMCX_APB_Based+PL341_T_WTR)=0x00000003; // tWTR = 7,5 ns/ tCK = 2,5=3
	MEM(DMCX_APB_Based+PL341_T_XP)=0x00000002;  // tXP = 2
	MEM(DMCX_APB_Based+PL341_T_XSR)=0x000000c8; //
	MEM(DMCX_APB_Based+PL341_T_ESR)=0x000000c8; //
	MEM(DMCX_APB_Based+PL341_T_MRD)=0x00000002;
  
	MEM(DMCX_APB_Based+PL341_MEMORY_CFG)=0x0001001a; //1 chip 14 row 10 column
	MEM(DMCX_APB_Based+PL341_MEMORY_CFG2)=0x00000030;
	
	MEM(DMCX_APB_Based+0x200)=0x00001ff0; //BA-RA-CA
	
	for(i=0; i<sizeof(PL341_Direct_Cmd)/4;i++){
		MEM(DMCX_APB_Based+PL341_DIRECT_CMD)=PL341_Direct_Cmd[i];
	}

	MEM(DMCX_APB_Based+PL341_MEMC_CMD)=0x00000000;
	while ((MEM(DMCX_APB_Based+PL341_MEMC_STATUS)&0x3) != 0x1) {}
	
}

void Init_DDR_EM0(){

	if (DDR_EM0_Inited==DDR_NOT_INITED){
		MEM(0x2003c050)=0x1c; //DDR PHY ODT ON
		//tube_msg("INIT DDR EM0 ...");
		INIT_DDR_EMX(DMC0_APB_Based);
		DDR_EM0_Inited=DDR_INITED;
	}

	
}
	


void Init_DDR_EM1(){
	
	if (DDR_EM1_Inited==DDR_NOT_INITED){
		MEM(0x2003c064)=0x1c; //DDR PHY ODT ON
		//tube_msg("INIT DDR EM1 ...");
		INIT_DDR_EMX(DMC1_APB_Based);
		DDR_EM1_Inited=DDR_INITED;
	}

}
