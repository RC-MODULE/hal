#include <string.h>
#include <time.h>
#include "hal.h"
#include "hal_host.h"

#include "dma.h"
#include "sleep.h"
#include "stddef.h"
#include "ringbuffer_host.h"


#ifdef __NM__
#define bytesizeof(T) (sizeof(T)<<2)
#else
#define bytesizeof(T) sizeof(T)
#endif 

//#ifdef __NM__
//#define sizeof32(t) sizeof(t)
//#else
//#define sizeof32(t) (sizeof(t)*4)
//#endif


typedef  void (*memcpy_ptr)(const void *, void *, size_t) ;
typedef  void *(*t_bytecpy)(void *to, int toIndex, void const *from, int fromIndex, size_t size) ;



#ifndef offsetof
#ifdef __cplusplus
#ifdef _WIN64
#define offsetof(s,m)   (size_t)( (ptrdiff_t)&reinterpret_cast<const volatile char&>((((s *)0)->m)) )
#else  /* _WIN64 */
#define offsetof(s,m)   (size_t)&reinterpret_cast<const volatile char&>((((s *)0)->m))
#endif  /* _WIN64 */


#else  /* __cplusplus */

#ifdef _WIN64
#define offsetof(s,m)   (size_t)( (ptrdiff_t)&(((s *)0)->m) )
#else  /* _WIN64 */
#define offsetof(s,m)   (size_t)&(((s *)0)->m)
#endif  /* _WIN64 */

#endif  /* __cplusplus */

#endif // offdetoff


int halHostRingBufferIsFull(HalHostRingBuffer* ringBuffer) {
	size_t head, tail;
	halReadMemBlock(&head, ringBuffer->remoteHeadAddr, 1, ringBuffer->processor);
	halReadMemBlock(&tail, ringBuffer->remoteTailAddr, 1, ringBuffer->processor);
	return head == tail+ ringBuffer->maxCount;
}

int halHostRingBufferIsEmpty(HalHostRingBuffer* ringBuffer) {
	size_t head, tail;
	halReadMemBlock(&head, ringBuffer->remoteHeadAddr, 1, ringBuffer->processor);
	halReadMemBlock(&tail, ringBuffer->remoteTailAddr, 1, ringBuffer->processor);
	return head == tail;
}

#ifdef VIRTUAL_BOARD
// если коннектимся к виртуальной плате то адресация в 4 раза меньше
#define SIZE_OF_INT 4
#else 
// если коннектимся к nmc плате то адресация в 4 раза меньше
#define SIZE_OF_INT 1
#endif


int halHostRingBufferInit(HalHostRingBuffer* ringBuffer, unsigned remoteRingBufferAddress, int core)
{
	/*
	halReadMemBlock(&(ringBuffer->data),	remoteRingBufferAddress+0, 1, core) ;
	halReadMemBlock(&ringBuffer->maxCount,			remoteRingBufferAddress+1, 1, core) ;
	halReadMemBlock(&ringBuffer->maxCountMinus1,	remoteRingBufferAddress+2, 1, core) ;
	halReadMemBlock(&ringBuffer->size,				remoteRingBufferAddress+3, 1, core) ;
	halReadMemBlock(&ringBuffer->head,				remoteRingBufferAddress+4, 1, core) ; ringBuffer->remoteHeadAddr = remoteRingBufferAddress+4;
	halReadMemBlock(&ringBuffer->tail,				remoteRingBufferAddress+5, 1, core) ; ringBuffer->remoteTailAddr = remoteRingBufferAddress+5;
	ringBuffer->processor=core;
	*/
	
	halReadMemBlock(&ringBuffer->data,				remoteRingBufferAddress+offsetof(HalRingBuffer,data)/4*SIZE_OF_INT, 1, core) ;
	halReadMemBlock(&ringBuffer->maxCount,			remoteRingBufferAddress+offsetof(HalRingBuffer,maxCount)/ 4 * SIZE_OF_INT, 1, core) ;
	halReadMemBlock(&ringBuffer->maxCountMinus1,	remoteRingBufferAddress+offsetof(HalRingBuffer,maxCountMinus1)/ 4 * SIZE_OF_INT, 1, core) ;
	halReadMemBlock(&ringBuffer->size,				remoteRingBufferAddress+offsetof(HalRingBuffer,size)/ 4 * SIZE_OF_INT, 1, core) ;
	ringBuffer->remoteHeadAddr	=					remoteRingBufferAddress+offsetof(HalRingBuffer,head)/ 4 * SIZE_OF_INT;
	ringBuffer->remoteTailAddr =					remoteRingBufferAddress+offsetof(HalRingBuffer,tail)/ 4 * SIZE_OF_INT;
	halReadMemBlock(&ringBuffer->head,				ringBuffer->remoteHeadAddr, 1, core) ; 
	halReadMemBlock(&ringBuffer->tail,				ringBuffer->remoteTailAddr, 1, core) ; 
	
	ringBuffer->processor=core;
	ringBuffer->sleep = 10;

	return RING_BUFFER_OK;
}


void halHostRingBufferPush(HalHostRingBuffer* ringBuffer, void* src, size_t count){
	while(1){
		halReadMemBlock(&ringBuffer->tail,ringBuffer->remoteTailAddr,1, ringBuffer->processor);
		if (ringBuffer->tail+ringBuffer->maxCount-ringBuffer->head >= count)
			break;
		halSleep(ringBuffer->sleep);
	}
	
	size_t tail    = ringBuffer->tail;
	size_t posHead = ringBuffer->head & ringBuffer->maxCountMinus1;			
	size_t posTail = tail & ringBuffer->maxCountMinus1;
	ARCH_ADDR  addrHead = ringBuffer->data +posHead*ringBuffer->size*SIZE_OF_INT;
	
	// [.......<Tail>******<Head>.....]
	if (posTail<posHead || ringBuffer->head==ringBuffer->tail){
		size_t countToEnd = ringBuffer->maxCount - posHead;
		if (count <= countToEnd){
			//ringBuffer->singleCopy(src, addrHead ,count*ringBuffer->size);
			halWriteMemBlock((unsigned*)src, addrHead ,count*ringBuffer->size, ringBuffer->processor);
		}
	// [*******<Head>......<Tail>*****]
		else {
			size_t firstSize =countToEnd*ringBuffer->size;
			size_t secondSize=(count-countToEnd)*ringBuffer->size;
			//ringBuffer->doubleCopy(src, (int*)src+firstSize, addrHead, ringBuffer->data, firstSize, secondSize);
			halWriteMemBlock((unsigned*)src, addrHead ,firstSize);
			halWriteMemBlock((unsigned*)src+firstSize, ringBuffer->data ,secondSize, ringBuffer->processor);
		}
	}
	else {
		//ringBuffer->singleCopy(src, addrHead, count*ringBuffer->size);
		halWriteMemBlock((unsigned*)src, addrHead ,count*ringBuffer->size,  ringBuffer->processor);
	}
	ringBuffer->head += count;
	halWriteMemBlock(&ringBuffer->head,ringBuffer->remoteHeadAddr,1, ringBuffer->processor);
}

void halHostRingBufferPop(HalHostRingBuffer* ringBuffer, void* dst, size_t count){
	while (1){
		halReadMemBlock(&ringBuffer->head,ringBuffer->remoteHeadAddr,1, ringBuffer->processor);
		if (ringBuffer->head-ringBuffer->tail >= count)
			break;
		halSleep(ringBuffer->sleep);
	}
	
	size_t head    = ringBuffer->head;
	size_t posHead = head & ringBuffer->maxCountMinus1;			
	size_t posTail = ringBuffer->tail & ringBuffer->maxCountMinus1;
	ARCH_ADDR addrTail  = ringBuffer->data + posTail*ringBuffer->size*SIZE_OF_INT;
	
	// [.......<Tail>******<Head>.....]

	if (posTail<posHead){
		//ringBuffer->singleCopy(addrTail, dst, count*ringBuffer->size);
		halReadMemBlock((unsigned*)dst,addrTail, count*ringBuffer->size, ringBuffer->processor);
	}
	// [*******<Head>......<Tail>*****]
	else {
		size_t countToEnd = ringBuffer->maxCount - posTail;
		if (count <= countToEnd){
			//ringBuffer->singleCopy(addrTail, dst, count*ringBuffer->size);
			halReadMemBlock((unsigned*)dst,addrTail, count*ringBuffer->size, ringBuffer->processor);
		}
		else {
			size_t firstSize = countToEnd*ringBuffer->size;
			size_t secondSize= (count-countToEnd)*ringBuffer->size;
			//ringBuffer->doubleCopy(addrTail, ringBuffer->data, dst, (int*)dst+firstSize, firstSize, secondSize);
			halReadMemBlock((unsigned*)dst,addrTail, firstSize, ringBuffer->processor);
			halReadMemBlock((unsigned*)dst+firstSize,ringBuffer->data, secondSize, ringBuffer->processor);
		}
	}
	ringBuffer->tail += count;
	halWriteMemBlock(&ringBuffer->tail,ringBuffer->remoteTailAddr,1, ringBuffer->processor);
}


