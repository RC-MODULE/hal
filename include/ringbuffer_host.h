#ifndef HAL_HOST_RING_INCLUDED
#define HAL_HOST_RING_INCLUDED
#include <string.h>
#include <time.h>

#ifdef __NM__ 
#undef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 320000000
#endif


#define MIN(a,b) ((a) > (b) ? (b) : (a))

#define RING_BUFFER_DECLARED	0xDEC1A8ED
#define RING_BUFFER_ALLOCATED	0xA10CA7ED
#define RING_BUFFER_MEM_ERROR	0xDEADF00D
#define RING_BUFFER_ERROR		0xBEDABEDA
#define RING_BUFFER_OK			0x0
#define RING_BUFFER_INCORRECT_COUNT -1
#define RING_BUFFER_SLEEP 100
#define EXIT_ON_TIMEOUT       1
#define EXIT_ON_COMPLETED     0
#define EXIT_ON_FULL_BUFFER   2
#define EXIT_ON_EMPTY_BUFFER  2

#ifdef __NM__
#define bytesizeof(T) (sizeof(T)<<2)
#else
#define bytesizeof(T) sizeof(T)
#endif 

#ifdef __NM__
#define sizeof32(t) sizeof(t)
#else
#define sizeof32(t) (sizeof(t)*4)
#endif


typedef  void (*memcpy_ptr)(const void *, void *, size_t) ;
typedef  void *(*t_bytecpy)(void *to, int toIndex, void const *from, int fromIndex, size_t size) ;


//HalRingBuffer<int> * pbuffer;
//int writeCommit(){
//	pbuffer->head=pbuffer->dmahead;
//}
//int readCommit(){
//	pbuffer->tail=pbuffer->dmatail;
//}
#include "dma.h"
#include "sleep.h"
#include "stddef.h"
#include "ringbuffer.h"


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



struct HalHostRingBuffer{
	unsigned 	data;			///<  физический адрес кольцевого буфера входных данных в памяти NM
	unsigned 	maxCount;		///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	size_t 	maxCountMinus1;	///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	size_t 	size;			///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	size_t 	head;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код MASTER уже записал в	буфер входных данных [заполняется MASTER]
	size_t 	tail;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код SLAVE  уже прочитал (обработал) 			 [заполняется SLAVE]

	unsigned remoteHeadAddr;	///<  сколько элементов ОТ НАЧАЛА ПОТОКА код MASTER уже записал в	буфер входных данных [заполняется MASTER]
	unsigned remoteTailAddr;	//<  сколько элементов ОТ НАЧАЛА ПОТОКА код SLAVE  уже прочитал (обработал) 			 [заполняется SLAVE]
	int processor;

	FuncSingleCopy singleCopy;
	FuncDoubleCopy doubleCopy;

};

	
int halHostRingBufferInit(HalHostRingBuffer* ringBuffer, unsigned remoteRingBufferAddress, int core=0)
{
	/*
	halReadMemBlock((unsigned*)&ringBuffer->data,	remoteRingBufferAddress+0, 1, core) ;
	halReadMemBlock(&ringBuffer->maxCount,			remoteRingBufferAddress+1, 1, core) ;
	halReadMemBlock(&ringBuffer->maxCountMinus1,	remoteRingBufferAddress+2, 1, core) ;
	halReadMemBlock(&ringBuffer->size,				remoteRingBufferAddress+3, 1, core) ;
	halReadMemBlock(&ringBuffer->head,				remoteRingBufferAddress+4, 1, core) ; ringBuffer->remoteHeadAddr = remoteRingBufferAddress+4;
	halReadMemBlock(&ringBuffer->tail,				remoteRingBufferAddress+5, 1, core) ; ringBuffer->remoteTailAddr = remoteRingBufferAddress+5;
	ringBuffer->processor=core;
	*/
	halReadMemBlock((unsigned*)&ringBuffer->data,	remoteRingBufferAddress+offsetof(HalRingBuffer,data)/4, 1, core) ;
	halReadMemBlock(&ringBuffer->maxCount,			remoteRingBufferAddress+offsetof(HalRingBuffer,maxCount)/4, 1, core) ;
	halReadMemBlock(&ringBuffer->maxCountMinus1,	remoteRingBufferAddress+offsetof(HalRingBuffer,maxCountMinus1)/4, 1, core) ;
	halReadMemBlock(&ringBuffer->size,				remoteRingBufferAddress+offsetof(HalRingBuffer,size)/4, 1, core) ;
	ringBuffer->remoteHeadAddr	=					remoteRingBufferAddress+offsetof(HalRingBuffer,head)/4;
	ringBuffer->remoteTailAddr =					remoteRingBufferAddress+offsetof(HalRingBuffer,tail)/4;
	halReadMemBlock(&ringBuffer->head,				ringBuffer->remoteHeadAddr, 1, core) ; 
	halReadMemBlock(&ringBuffer->tail,				ringBuffer->remoteTailAddr, 1, core) ; 
	
	ringBuffer->processor=core;

	return RING_BUFFER_OK;
}


void halHostRingBufferPush(HalHostRingBuffer* ringBuffer, void* src, size_t count){
	while(1){
		halReadMemBlock(&ringBuffer->tail,ringBuffer->remoteTailAddr,1, ringBuffer->processor);
		if (ringBuffer->tail+ringBuffer->maxCount-ringBuffer->head >= count)
			break;
		halSleep(RING_BUFFER_SLEEP);
	}
	
	size_t tail    = ringBuffer->tail;
	size_t posHead = ringBuffer->head & ringBuffer->maxCountMinus1;			
	size_t posTail = tail & ringBuffer->maxCountMinus1;
	unsigned  addrHead  = ringBuffer->data + posHead*ringBuffer->size;
	
	// [.......<Tail>******<Head>.....]
	if (posTail<posHead || ringBuffer->head==ringBuffer->tail){
		size_t countToEnd = ringBuffer->maxCount - posHead;
		if (count <= countToEnd){
			//ringBuffer->singleCopy(src, addrHead ,count*ringBuffer->size);
			halWriteMemBlock((unsigned*)src, addrHead ,count*ringBuffer->size, ringBuffer->processor);
		}
	// [*******<Head>......<Tail>*****]
		else {
			int firstSize =countToEnd*ringBuffer->size;
			int secondSize=(count-countToEnd)*ringBuffer->size;
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
		halSleep(RING_BUFFER_SLEEP);
	}
	
	size_t head    = ringBuffer->head;
	size_t posHead = head & ringBuffer->maxCountMinus1;			
	size_t posTail = ringBuffer->tail & ringBuffer->maxCountMinus1;
	unsigned addrTail  = ringBuffer->data + posTail*ringBuffer->size;
	
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
			int firstSize = countToEnd*ringBuffer->size;
			int secondSize= (count-countToEnd)*ringBuffer->size;
			//ringBuffer->doubleCopy(addrTail, ringBuffer->data, dst, (int*)dst+firstSize, firstSize, secondSize);
			halReadMemBlock((unsigned*)dst,addrTail, firstSize, ringBuffer->processor);
			halReadMemBlock((unsigned*)dst+firstSize,ringBuffer->data, secondSize, ringBuffer->processor);
		}
	}
	ringBuffer->tail += count;
	halWriteMemBlock(&ringBuffer->tail,ringBuffer->remoteTailAddr,1, ringBuffer->processor);
}



#endif
