#include "vic.h"	
#include "tube.h"
#include "sctl.h"
//#include "ddr.h"
#include "config.h"
//#include "defs.h"
//#include "mem_map.h"

void INIT_NMC3();
void START_NMC3();
void CHECK_NMC3();

extern int NMC3_IM1[];


#define NMC3_TIMEOUT 0x1000

int volatile NMC_High_Handled=VECT_WAITING;
int volatile NMC_Low_Handled =VECT_WAITING;
	
//#define  NMC3_CHECKSUM_LABEL	0x00130420			// IM0 memory  def in mem_map.h
#define  NMC3_CHECKSUM_VALUE	0x0baba	
#define  NMC_TIMEOUT 2000
/*	
int Wait_NMC3(){
	int t;
	for(t=0; t<NMC3_TIMEOUT; t++){
		if (NMC3_Handled){
			tube_msg("NMC3 HANDLED");
			return 1;
		}
	}
	tube_msg("NMC3 TIMEOUT");
	return 0;
}*/
void INTERRUPT VECT_HANDLER_NMC_HIGH ()
{
	NMC_High_Handled=VECT_HANDLED;
	
	MEM(SCTL_Base+0x34)=0xffffffff; // Reset interrupt from NMC3
	MEM(SCTL_Base+0x3C)=0x1; 		// Set interrupt from ARM to NMC3
	MEM(VIC_Base0+VIC_VECTADDR)=1;
	INLINE_TUBE_MSG("*** VECT_HANDLER_NMC_HIGH ***");
	
}
void INTERRUPT VECT_HANDLER_NMC_LOW ()
{
	NMC_Low_Handled=VECT_HANDLED;
	MEM(VIC_Base0+VIC_VECTADDR)=1;
	
	MEM(SCTL_Base+0x30)=0xffffffff; // Reset interrupt from NMC3
	MEM(SCTL_Base+0x38)=0x1; 		// Set interrupt from ARM to NMC3
	INLINE_TUBE_MSG("*** VECT_HANDLER_NMC_LOW ***");
}


void VECT_HANDLER_NMC_HIGH_();
void VECT_HANDLER_NMC_LOW_();

void Init_VIC_NMC3(){
	MEM(VIC_Base0+VIC_VECTADDR14)=(unsigned)VECT_HANDLER_NMC_HIGH;
	MEM(VIC_Base0+VIC_VECTADDR15)=(unsigned)VECT_HANDLER_NMC_LOW;
	unsigned IrqMask=MEM(VIC_Base0+VIC_INTENABLE);
	MEM(VIC_Base0+VIC_INTENABLE)=IrqMask|IRQ_NMC_HIGH|IRQ_NMC_LOW;
}		

void Init_NMC3(){
	dmacpy((void*)0x00140000, (void*)NMC3_IM1, 4*4762);
	//memcpy_w((void*)0x00140000, (void*)NMC3_IM1, 4*4762/4);
	INIT_NMC3();
}

void Start_NMC3(){
	START_NMC3();
}
int Wait_NMC3(){
	int t;
	for(t=0; t<NMC_TIMEOUT; t++){
		if ((NMC_Low_Handled==VECT_HANDLED)||(NMC_High_Handled==VECT_HANDLED)){
			if (NMC_Low_Handled==VECT_HANDLED)
				tube_msg("NMC LOW HANDLED");
			if (NMC_High_Handled==VECT_HANDLED)
				tube_msg("NMC High HANDLED");
			return 1;
		}
	}
	tube_hex(NMC_Low_Handled);
	tube_hex(NMC_High_Handled);
	
	tube_msg("NMC TIMEOUT");
	return 0;
}
int Check_NMC3(){
	int t;
	for(t=0; t<NMC_TIMEOUT; t++){
		if (MEM(NMC3_CHECKSUM_LABEL)==NMC3_CHECKSUM_VALUE){
			tube_msg("CHECK NMC - OK!");
			return TEST_OK;
		}
	}
	tube_msg("CHECK NMC - ERROR!");
	return TEST_ERROR;
}
/*
void memcpy_w_interlive(void* dst, void* src, int count)
{
	int i;
	int* d=(int*)dst;
	int* s=(int*)src;
	for(i=0; i<count; i+=2){
		*d=*s;
		d+=2;
		s++;
	}
}*/
int Test_NMC3(){
	int i;
	int val;
	tube_msg("START TEST NMC3...  ");
// 	Init_DDR_EM0();
	
	tube_msg("Init VIC NMC3...");
	Init_VIC_NMC3();
	/*
	for(i=0; i<16;i++){
		tube_msg("val=");
		val=MEM(0x00141000+i*4);
		tube_hex(val);
	}
	*/
	tube_msg("init NMC3...");
INIT_NMC3();
	tube_msg("start NMC3...");
START_NMC3();	
tube_msg("END NMC3...");
return TEST_OK;
	
	tube_msg("INIT NMC3...  ");

	Init_NMC3();
	tube_msg("START NMC3...");
	Start_NMC3();
	tube_msg("WAIT NMC3....");
	Wait_NMC3();
	tube_msg("CHECK NMC3....");
	int test=Check_NMC3();
	if (test==TEST_OK){
		tube_msg("TEST NMC3 - OK!");
		return TEST_OK;
	}
	else {
		tube_msg("TEST NMC3 - ERROR!");
		return TEST_ERROR;
	}
	//tube_msg("CHECK_NMC3...\n");
	//CHECK_NMC3();
/*
	tube_msg("INIT_NMC3...");
	INIT_NMC3();
	
	tube_msg("START_NMC3...");
	START_NMC3();
	tube_msg("WAIT_NMC3...");
	WAIT_NMC3();
	tube_msg("CHECK_NMC3...");
	CHECK_NMC3();
*/

}