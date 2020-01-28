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
#include "hal_target.h"
#include "led.h"
#include "ringbuffert.h"
#include "dma.h"
#include "dmaserver.h"


#define NUM_TASKS 1024
DmaClient<NUM_TASKS>   dmaClient1; // создаем DMA клиент на NMC1

int main()
{

	halSetProcessorNo(1);
	halHandshake(0);
	printf("[NMC1:] NM6407 Dma-client\n");
	int size=16*1024;
	nmc_malloc_set_heap(0);
	int* src=halMalloc32(size);
	nmc_malloc_set_heap(2);
	int* dst=halMalloc32(size);
	printf("[NMC1:]src %x dst %x\n",src,dst);
	if (src==0 || dst==0)
		return -1;

	void* channel1=halSyncAddr(0,0);			// получаем адрес канала-1 на второе ядро	
	HalRingBufferData<DmaTask, 1024> *ch = (HalRingBufferData<DmaTask, 1024>*) channel1;
	dmaClient1.bind(channel1);			// подключаем клиент к каналу 1 DMA сервера
	
	for (int i=0; i<size; i++){
		src[i]=0xBABABABA;
		dst[i]=0xDEDADEDA;
	}
	int taskNo=0;
	
	DmaTask task1={halMapAddrTo(src,0),halMapAddrTo(dst,0),256,0};		// мапируем адреса в пространство nmc1
	
	while(1){
		for(int i=1; i<=1024;i++){		
			task1.status=((taskNo++)>>8)<<4;
			task1.size = (taskNo%size) + (taskNo & 1);
			dmaClient1.add(task1);
		}
		//printf("[NMC1:]%d\n", taskNo);
	}
}

