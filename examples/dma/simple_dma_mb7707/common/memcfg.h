//=== BANK =============//============= ARM-MAP MEMORY =================//================ NEURO-MATRIX MAP MEMORY =============//
// IM1-NMC  (256kB)		// (ARM:0x00140000-0x0017ffff)	0x4000(256kB)	//	(NM: 0x00000000-0x0000ffff),	len = 0x00010000;	//		
// IM3-NMC  (256kB)		// (ARM:0x00180000-0x001bffff)	0x4000(256kB)	//	(NM: 0x00010000-0x0001ffff),	len = 0x00010000;	//		
// IM0-ARM	(256kB)		// (ARM:0x00100000-0x0013ffff)	0x4000(256kB)	//	(NM: 0x00040000-0x0004ffff),	len = 0x00010000;	//	
// IM2-ARM	(256kB)		// (ARM:0x80100000-0x8013ffff)	0x4000(256kB)	// 	(NM: 0x20040000-0x2004ffff),	len = 0x00010000;	//	
// EM0-DDR	(16MB)		// (ARM:0x40000000-0x7fffffff) 16MB          	//	(NM: 0x10000000-0x10ffffff),	len = 0x01000000;	//	
// EM1-DDR	(16MB)		// (ARM:0xc0000000-0xcfffffff) 16MB          	// 	(NM: 0x30000000-0x30ffffff),	len = 0x01000000;	//
// BOOT-ROM				// (ARM:0x80000000 0x800fffff)               	//	(NM: 0x20000000,				len = 0x00040000;	//			
// =============================================================================================================================//

//-- ARM SPACE ADDRESS --------------
#define ARM_HELLO          0x80100000
#define ARM_INFO		   0x80100004	
#define ARM_DEBUG    	   0x80100008
#define DDR_INFO           0x8010000C

#define NMC_HELLO          0x80100010
#define NMC_INFO           0x80100014
#define NMC_DEBUG         	0x80100018
#define NMC_CRC       		0x8010001C

#define ARM2NM_MSG_ADDR    0x80100020	// в этой €чейке передаем тип сообщени€ от ARM к NM
#define ARM2NM_VAL_ADDR    0x80100024	// в этой €чейке передаем значение 		от ARM к NM
#define NM2ARM_MSG_ADDR    0x80100028	// в этой €чейке передаем тип сообщени€ от NM к ARM 
#define NM2ARM_VAL_ADDR    0x8010002C	// в этой €чейке передаем значение      от NM к ARM 

#define DEBUG_DATA     		0x80100030

#define INPUT_STREAM_ADDR 0xc0000000 	// address of input ac3-stream
#define OUTPUT_SOUND_ADDR 0x40000000	// address to play pcm data

#define HANDSHAKE_MSG 		0xABCDEF

#define ERROR_MSG					0xBEDA6406;

#ifndef MEM
#define MEM(addr)  (*((unsigned volatile*)(addr)))
#endif
