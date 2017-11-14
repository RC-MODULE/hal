
#include "memcfg.h"


extern "C" {
	void Init_Cache();
	void Init_VIC();
	void Init_DDR_EM0();
	void Init_DDR_EM1();
	int init_gpio_ARM();
};

int main(void) {

	//--------------- startup ------------------------
	MEM(ARM_HELLO)  =0xC0DE1879;
	MEM(ARM_INFO)   =0xCCCCCCCC;		
	MEM(ARM_DEBUG)  =0xCCCCCCCC;		
	MEM(DDR_INFO)   =0xDEADB00F;		
	for(int i=0; i<32; i++)
		MEM(ARM2NM_MSG_ADDR+i*4)  =0xCCCCCCCC;
	
	//MEM(ARM_DEBUG_ADDR)=0xDEBAC003;
	//--------------- init DDR ------------------------
	Init_DDR_EM0();
	Init_DDR_EM1();
	//MEM(ARM_DEBUG_ADDR)=0xDEBAC004;
	//--------------- test DDR_EM0 ---------------------
	
	MEM(0x40000000)=0x12345678;
	MEM(0xC0000000)=0x12345678;
	if ((MEM(0x40000000)==0x12345678) && (MEM(0xc0000000)==0x12345678)){
		MEM(DDR_INFO)=0x600DB00F;		
	} else 	return -1;

	//MEM(ARM_DEBUG_ADDR)=0xDEBAC004;
	//--------------- fill DDR --------------------------
	for(int i=0; i<1024;i++)
		MEM(0x40000000+i*4)=0xCCCCCCCC;
	for(int i=0; i<1024;i++)
		MEM(0xc0000000+i*4)=0xDDDDDDDD;
	//--------------------------------------------------
	//MEM(ARM_DEBUG_ADDR)=0xDEBAC005;
	//--------- синхронизиция с NM -----------------
	MEM(ARM2NM_MSG_ADDR)=HANDSHAKE_MSG;					// шлем хэндшейк на NM
	while (MEM(NM2ARM_MSG_ADDR)!=HANDSHAKE_MSG);		// ждем подтверждение от NM

	//--------- OK ---------------------------------
	MEM(ARM_INFO)=0x600D1879;

	
	
	
	return 1;
}
