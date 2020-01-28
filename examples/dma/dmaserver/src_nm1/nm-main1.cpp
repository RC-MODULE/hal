//------------------------------------------------------------------------
//
//
//  Copyright (c) RC Module Inc.
//
//!
//! \file   main.cpp
//! \author S. Mushkaev
//! \brief  Example of universl-board-connect usage
//!
//! \endif
//!
//------------------------------------------------------------------------

#include <stdio.h>
#include "hal_target.h"
#include "sleep.h"
#include "hal.h"
#include "cache.h"
#include "string.h"
//DISABLE_PRINTF();
#include "ringbuffert.h"
#include "qdma.h"



struct DmaTask{
	void* src;
	void* dst;
	int size;
	int status;
};



template <int BUFFER_SIZE>	class DmaClient: HalRingBufferConnector<DmaTask,BUFFER_SIZE> {
	public:

	inline void bind(void* channelBuffer){
		HalRingBufferConnector<DmaTask,BUFFER_SIZE>::connect(channelBuffer);
	}
	inline void add(DmaTask& task){
		HalRingBufferConnector<DmaTask,BUFFER_SIZE>::push(&task,1);
	}
	void run(){
		
	}
};


long align64bit=0;
int src[252]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
int dst[256]={0xCCC0,0xCCC1,0xCCC2,0xCCC3,0xCCC4,0xCCC5,0xCCCC,0xCCCC,0xCCCC,0xCCCC};

DmaClient<128>   dmaClient;



int main()
{

	//halOpen();
	//halLedOff(5);
	//halSetProcessorNo(1);							// Set processor number 
	//halLedOn(4);
	halHandshake(0);
	//HalRingBufferConnector<DmaTask,128> dmaTaskBuffer;
	//HalRingBufferData<DmaTask,128>* container=(HalRingBufferData<DmaTask,128>*)halSyncAddr(0,0);
	HalRingBufferData<DmaTask,128>* channel = (HalRingBufferData<DmaTask,128>*)halSyncAddr(0,0);
	dmaClient.bind(channel);
	
	//int addr=halSync(0,1);
	//printf("[NMC1:]%x\n",container);
	printf("[NMC1:] dmaChannel[1] head %x\n",channel->head);
	printf("[NMC1:] dmaChannel[1] tail %x\n",channel->tail);
	printf("[NMC1:] dmaChannel[1] bufferId %x\n",channel->bufferId);
	
	//halSync(0,0);
	//return 0;
	//printf("[NMC1:]%x\n",addr);
	//halLedOn(3);

	int taskNo=0;
	while(1);
	while (0) 
		for (int i=0;i<10; i++){
			DmaTask task;
			task.src=halMapAddr(src+i*2);
			task.dst=halMapAddr(dst+i*2);
			task.size=2;
			task.status=taskNo++;
			dmaClient.add(task);
		}
	
	printf("[NMC1:] dmaChannel[1] head %x\n",channel->head);
	printf("[NMC1:] dmaChannel[1] tail %x\n",channel->tail);
	printf("[NMC1:] dmaChannel[1] bufferId %x\n",channel->bufferId);
	
	//printf("[NMC1] ready\n");
	//halSync(0,0);
	printf("[NMC1] echo\n");


//	ringBufferDMA[2]=(HalRingBufferData<CopyTask,128>*)halSync(0,0);
//	ringBufferDMA[3]=(HalRingBufferData<CopyTask,128>*)halSync(0,0);
//	
//	for(int i=0; i<256;i++){
//		A1[i]=1000+i;
//		B1[i]=0;
//	}
//	
//	
//	halDmaServerAddTask(2,halMapAddr(A1),		halMapAddr(B1),   10,0);
//	halDmaServerAddTask(2,halMapAddr(A1+10),	halMapAddr(B1+10),10,0);
//	halDmaServerAddTask(3,halMapAddr(A1+20),	halMapAddr(B1+20),10,0);
//
//	halSync((int)halMapAddr(B1),0);
	
	
	return 222;
}

