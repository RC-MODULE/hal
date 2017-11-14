#include "hal.h"
#include "dma.h"
#include "time.h"
#include "memcfg.h"
#include "buffers.h"


//#pragma data_section ".data"

#define NM2ARM(addr)((int*)(((int)addr)*4))


clock_t t0,t1;

//int DMA_Finished=false;
//void BellFromDma()// Обработчик DMA прерывания (вектор 58h)
//{ 
//	t1=clock();
//	DMA_Finished=true;
//}

int main()
{ 
	MEM(NMC_HELLO/4)=0xC0DE6406;
	MEM(NMC_INFO/4) =0xCCCCCCCC;
	
	
	//--------- синхронизиция с ARM -----------------
	while (MEM(ARM2NM_MSG_ADDR/4)!=HANDSHAKE_MSG);	// ждем хэндшейк от ARM 
	MEM(NM2ARM_MSG_ADDR/4)=HANDSHAKE_MSG;			// шлем подтверждение на ARM
	
	MEM(NMC_INFO/4)=0x600D6406;
	
	unsigned SrcCrc=0; 
	unsigned DstCrc=0;
	
	HAL_Rand64(pool0,SIZE_POOL0/2);
	HAL_Fill32(pool4,0, SIZE_POOL0);
	HAL_Crc32( pool0,SIZE_POOL0,SrcCrc);
	
	
	//SetCallBack(0x58, (int)BellFromDma); Not implemented yet 
	
	t0=clock();
	
	int k=xdmacinit(NM2ARM(pool4),NM2ARM(pool0),  SIZE_POOL0*4 ); // армовые адреса и байтовый счётчик      
	
	int t,timeout=0x123456;
	for (t=0; t<timeout; t++){ 
		MEM(NM2ARM_MSG_ADDR/4)=t;
		if (xdmac0()==0){
			t1=clock();
			MEM(ARM2NM_MSG_ADDR/4+4)=t;
			break;
		}
	}
	
	HAL_Crc32( pool4,SIZE_POOL0,DstCrc);	
			
	MEM(DEBUG_DATA/4)=SrcCrc;
	MEM(DEBUG_DATA/4+1)=DstCrc;
	int tt=(t1-t0)*10;
	tt/=SIZE_POOL0;
	MEM(DEBUG_DATA/4+2)=tt;
	MEM(NMC_INFO/4)=0x600D6407;
	
	return SrcCrc-DstCrc;
} 



