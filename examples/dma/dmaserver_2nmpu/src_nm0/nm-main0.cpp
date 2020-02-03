//------------------------------------------------------------------------
//
//
//  Copyright (c) RC Module Inc.
//
//!
//! \file   main.cpp
//! \author S. Mushkaev
//! \brief  Example of DMA-server  usage
//!
//! \endif
//!
//------------------------------------------------------------------------


#include <stdio.h>
#include <string.h>
#include "sleep.h"
#include "hal.h"
#include "hal_target.h"
#include "led.h"
#include "ringbuffert.h"
#include "dma.h"
#include "dmaserver.h"
#include "time.h"

int dmaServerCallback();

#define NUM_TASKS 1024
#define NUM_CHANNELS 2


DmaServer<NUM_TASKS,NUM_CHANNELS> dmaServer(dmaServerCallback); 	// создаем DMA сервер, передаем обработчик 
																	// память под циклические буфера выделятся в куче по умолчанию т.е. в нулевой
DmaClient<NUM_TASKS>   dmaClient0;									// создаем DMA клиент на NMC0

unsigned long long totalCopiedSize=0;

// обработчик прерывания от DMA
int dmaServerCallback(){
	if (dmaServer.currentTask->status>0xF)
		halLed4Hi(dmaServer.currentTask->status>>4);	// выводим на Led номер задачи nmc1
	else 
		halLed4Lo(dmaServer.currentTask->status);		// выводим на Led номер задачи nm0
	totalCopiedSize+=dmaServer.currentTask->size;
	dmaServer.currentTask->status=0x777;				// выполняем какие то завершающие действия над текущей задачей (необязательно) 
	dmaServer.currentChannel->incTail();				//!(Важно) сдвигаем tail в текущем канале
	dmaServer.startNextTask();							//!(Важно) запускаем следующую задачу
	return 1;
}



int main()
{
	halSetProcessorNo(0);
	halHandshake(1);
	int size=16*1024; // maxSize
	nmc_malloc_set_heap(0);
	int* src=halMalloc32(size);	
	nmc_malloc_set_heap(2);
	int* dst=halMalloc32(size);
	printf("[NMC0:]src %x dst %x\n",src,dst);
	if (src==0 || dst==0){
		printf("[NMC0:]No buf memory\n");
		return -1;
	}

	printf("[NMC0:] NM6407 Dma-Server+Client\n");
	HalRingBufferData<DmaTask,1024>* channel0= dmaServer.getChannelBuffer(0);	// получаем адрес канала-0
	HalRingBufferData<DmaTask,1024>* channel1= dmaServer.getChannelBuffer(1);	// получаем адрес канала-1
	
	if (channel0==0 || channel1==0){
		printf("[NMC0:]No channel memory\n");
		return -1;
	}

	dmaClient0.bind(channel0);													// подключаем клиент к каналу 0 DMA сервера
	halSyncAddr(channel1,1);													// передаем адрес канала-1 на второе ядро
	
	for (int i=0; i<size; i++){
		src[i]=i;
		dst[i]=0;
	}
		
	DmaTask task0={src,dst,0,0};			
	clock_t t0=clock();
	clock_t t1,dt;
	unsigned taskNo=0;

	while(1){
		for (int i=1; i<=1024; i++){
			task0.status=((taskNo++)>>8)&0xF;
			task0.size= (taskNo%size) + (taskNo&1);							
			dmaClient0.add(task0);
		}
		//printf("[NMC0:]%d\n", taskNo);
		dmaServer.startJob();	
		// Замер скорости DMA сервера
		if (0) if (taskNo%4096==0){
			t1=clock();
			unsigned long long dmaSpeed=totalCopiedSize;
			totalCopiedSize=0;
			if (t1>t0)
				dt=t1-t0;
			else 
				dt=0xFFFFFFFF-t0+t1;
			t0=t1;
			dmaSpeed=dmaSpeed*500000000/(1024*1024)/unsigned(dt);
			printf("dma server speed: %d MB/s  tasks=%d\n", unsigned(dmaSpeed), unsigned(taskNo)) ;
		}
	}
	return 1;
}
