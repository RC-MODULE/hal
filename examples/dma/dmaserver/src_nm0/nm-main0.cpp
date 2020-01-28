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
#include <string.h>
#include "sleep.h"
#include "hal.h"
#include "led.h"
#include "ringbuffert.h"
#include "dma.h"
#include "dmaserver.h"


long align64bit=0;
int src[252]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
int dst[256]={0xCCC0,0xCCC1,0xCCC2,0xCCC3,0xCCC4,0xCCC5,0xCCCC,0xCCCC,0xCCCC,0xCCCC};


	


int dmaServerCallback();

DmaServer<128,2> dmaServer(dmaServerCallback);
DmaClient<128>   dmaClient0;
DmaClient<128>   dmaClient1;

volatile static int count=0;
int dmaServerCallback(){
	halLed(	dmaServer.currentTask->status>>4); // выполняем какие-то завершающие действия над текущей задачей (необязательно) 
	dmaServer.currentTask->status=0x777;		// выполняем какие-то завершающие действия над текущей задачей (необязательно) 
	dmaServer.currentChannel->incTail();		// сдвигаем tail в текущем канале
	dmaServer.startNextTask();					// запускаем следующую задачу
}

int main()
{
	printf("START\n");
	printf("[NMC0]:start\n");
	
	nmc_malloc_set_heap(0);
	HalRingBufferData<DmaTask,128>* channel0= dmaServer.getChannelBuffer(0);	// получаем адрес канала-0
	HalRingBufferData<DmaTask,128>* channel1= dmaServer.getChannelBuffer(1);	// получаем адрес канала-1
	
	
	dmaClient0.bind(channel0);	// подключаем клиент к каналу 0 DMA сервера
	dmaClient1.bind(channel1);	// подключаем клиент к каналу 0 DMA сервера
	
	volatile int taskNo=0;
	
	while(1){
		for (int i=0;i<101; i++){
			DmaTask task0={src,dst,2,taskNo++};		
			dmaClient0.add(task0);
		}
		dmaServer.startJob(1);
		for (int i=0;i<100; i++){
			DmaTask task1={src+2,dst+2,2,taskNo--};		
			dmaClient1.add(task1);
		}
		dmaServer.startJob();
	}
}

