#ifndef HAL_RING_INCLUDED
#define HAL_RING_INCLUDED
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
#include "hal.h"
struct HalRingBuffer{
	int* 	data;			///<  физический адрес кольцевого буфера входных данных 
	size_t 	maxCount;		///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	size_t 	maxCountMinus1;	///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	size_t 	size;			///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	size_t 	head;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код MASTER уже записал в	буфер входных данных [заполняется MASTER]
	size_t 	tail;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код SLAVE  уже прочитал (обработал) 			 [заполняется SLAVE]
	size_t  pendingCount;

	FuncSingleCopy singleCopy;
	FuncDoubleCopy doubleCopy;
	FuncSetCallbackDMA setCallback;
};

static HalRingBuffer* activeRingBuffer=0;
	
int halRingBufferInit(HalRingBuffer* ringBuffer, void* buffer, size_t size, size_t count, FuncSingleCopy singleMemCopy, FuncDoubleCopy doubleMemCopy,  FuncSetCallbackDMA setCallback)
{
	if (buffer==0) return RING_BUFFER_MEM_ERROR;
	if ((count&(count-1))!=0) return RING_BUFFER_INCORRECT_COUNT;
	ringBuffer->data = (int*)buffer;
	ringBuffer->maxCount = count;
	ringBuffer->maxCountMinus1=count-1;
	ringBuffer->size = size;
	ringBuffer->head=0;
	ringBuffer->tail=0;
	ringBuffer->pendingCount=0;
	ringBuffer->singleCopy=singleMemCopy;
	ringBuffer->doubleCopy=doubleMemCopy;
	ringBuffer->setCallback=setCallback;

	return RING_BUFFER_OK;
}
extern "C"{
static int halRingBufferPushCallback(){
	activeRingBuffer->head += activeRingBuffer->pendingCount;
	activeRingBuffer->pendingCount =0;
	return 0;
}

static int halRingBufferPopCallback(){
	activeRingBuffer->tail += activeRingBuffer->pendingCount;
	activeRingBuffer->pendingCount =0;
	return 0;
}
};

void halRingBufferPush(HalRingBuffer* ringBuffer, void* src, size_t count){
	while (ringBuffer->tail+ringBuffer->maxCount-ringBuffer->head < count)
		halSleep(RING_BUFFER_SLEEP);
	
	if (ringBuffer->setCallback){
		while (ringBuffer->pendingCount)
			halSleep(RING_BUFFER_SLEEP);
		ringBuffer->pendingCount=count;
		ringBuffer->setCallback(halRingBufferPushCallback);
		activeRingBuffer=ringBuffer;
	}

	
	size_t tail    = ringBuffer->tail;
	size_t posHead = ringBuffer->head & ringBuffer->maxCountMinus1;			
	size_t posTail = tail & ringBuffer->maxCountMinus1;
	int* addrHead  = ringBuffer->data + posHead*ringBuffer->size;
	
	// [.......<Tail>******<Head>.....]
	if (posTail<posHead || ringBuffer->head==ringBuffer->tail){
		size_t countToEnd = ringBuffer->maxCount - posHead;
		if (count <= countToEnd){
			ringBuffer->singleCopy(src, addrHead ,count*ringBuffer->size);
		}
	// [*******<Head>......<Tail>*****]
		else {
			int firstSize =countToEnd*ringBuffer->size;
			int secondSize=(count-countToEnd)*ringBuffer->size;
			ringBuffer->doubleCopy(src, (int*)src+firstSize, addrHead, ringBuffer->data, firstSize, secondSize);
		}
	}
	else {
		ringBuffer->singleCopy(src, addrHead, count*ringBuffer->size);
	}

	if (ringBuffer->setCallback==0)
		ringBuffer->head += count;
}

void halRingBufferPop(HalRingBuffer* ringBuffer, void* dst, size_t count){
	while (ringBuffer->head-ringBuffer->tail < count)
		halSleep(RING_BUFFER_SLEEP);
	
	if (ringBuffer->setCallback){
		while (ringBuffer->pendingCount)
			halSleep(RING_BUFFER_SLEEP);
		ringBuffer->pendingCount=count;
		ringBuffer->setCallback(halRingBufferPopCallback);
		activeRingBuffer=ringBuffer;
	}

	size_t head    = ringBuffer->head;
	size_t posHead = head & ringBuffer->maxCountMinus1;			
	size_t posTail = ringBuffer->tail & ringBuffer->maxCountMinus1;
	int* addrTail  = ringBuffer->data + posTail*ringBuffer->size;
	
	// [.......<Tail>******<Head>.....]
	if (posTail<posHead){
		ringBuffer->singleCopy(addrTail, dst, count*ringBuffer->size);
	}
	// [*******<Head>......<Tail>*****]
	else {
		size_t countToEnd = ringBuffer->maxCount - posTail;
		if (count <= countToEnd){
			ringBuffer->singleCopy(addrTail, dst, count*ringBuffer->size);
		}
		else {
			int firstSize = countToEnd*ringBuffer->size;
			int secondSize= (count-countToEnd)*ringBuffer->size;
			ringBuffer->doubleCopy(addrTail, ringBuffer->data, dst, (int*)dst+firstSize, firstSize, secondSize);
		}
	}
	if (ringBuffer->setCallback==0)
		ringBuffer->tail += count;
}

void* halRingBufferHead(HalRingBuffer* ringBuffer){
	size_t posHead = ringBuffer->head & ringBuffer->maxCountMinus1;			
	int* addrHead  = ringBuffer->data + posHead*ringBuffer->size;
	return addrHead;
}

void* halRingBufferTail(HalRingBuffer* ringBuffer){
	size_t posTail = ringBuffer->tail & ringBuffer->maxCountMinus1;
	int* addrTail  = ringBuffer->data + posTail*ringBuffer->size;
	return addrTail;
}

int halRingBufferStatus(HalRingBuffer* ringBuffer){
	if (ringBuffer->setCallback==0)
		return false;
	return ringBuffer->pendingCount;
}
/*
template <class T> class HalRingBuffer{
public:	
	T* 		data;			///<  физический адрес кольцевого буфера входных данных 
	size_t 	size;			///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	size_t 	head;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код MASTER уже записал в	буфер входных данных [заполняется MASTER]
	size_t 	tail;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код SLAVE  уже прочитал (обработал) 			 [заполняется SLAVE]
	size_t* head_addr;
	size_t* tail_addr;
	size_t 	id;
	
	memcpy_ptr push_memcpy;
	memcpy_ptr pop_memcpy;

	// ------------- dma --------------
	memcpy_ptr dma_init;
	size_t(*dma_check)();
	T*     dma_ptr;
	size_t dma_left;
	size_t dma_size;
	//----------------------------------
	unsigned timeout;    // msec
	unsigned time2sleep; // msec
	int 	pad[16-3-5*sizeof(memcpy_ptr)/sizeof(int)];	///<  резервные поля
	
	
	
	int callback(){
		
		return 1;
	}
	HalRingBuffer(){
		
		//sizeof_size_t=sizeof(size_t);
		//sizeof_T=sizeof(T);
		init(0,0,(memcpy_ptr)memcpy,(memcpy_ptr)memcpy);
		//Init(0,0,0,0);
	}
	HalRingBuffer(T* buffer,size_t count, memcpy_ptr pushmemcpy, memcpy_ptr popmemcpy ){
		init(buffer,count,pushmemcpy,popmemcpy);
	}
	//void Init(T* buffer,size_t count, memcpy_ptr pushmemcpy, memcpy_ptr popmemcpy ){
	bool init(T* buffer,size_t count, memcpy_ptr pushmemcpy, memcpy_ptr popmemcpy ){
		if (buffer==0) return false;
		if (count&(count-1)!=0) return false ;
		data=buffer;
		size=count;
		head=0;
		tail=0;
		head_addr=&head;
		tail_addr=&tail;
		id=0x600DB00F;
		push_memcpy=(memcpy_ptr)pushmemcpy;
		pop_memcpy =(memcpy_ptr)popmemcpy;
		dma_init=0;
		dma_check=0;
		dma_ptr =0;
		dma_left=0;
		dma_size=0;
		time2sleep=1;	// msec
		timeout=100;	// 
		return true;
		
	}

	bool isFull(){
		return (head-tail==size);
	}
	bool isEmpty(){
		return (head==tail);
	}
	size_t writeAvail(){
		return (size-(head-tail));
	}
	size_t readAvail(){
		return (head-tail);
	}
	
	
	T* ptrHead(){
		if (pushAvail()==0)
			return 0;
		size_t headPointer=head&(size-1);
		T* pElement=data+headPointer;
		return pElement;
	}
	T* ptrTail(){
		if (PopAvail()==0)
			return 0;
		size_t tailPointer=tail&(size-1);
		T* pElement=data+tailPointer;
		return pElement;
	}
	
	bool moveHead(int numElements){
		size_t h=head+numElements;
		if (h>tail+size)
			return RING_BUFFER_ERROR;
		head=h;
		return RING_BUFFER_OK;
	}

	bool moveTail(int numElements){
		size_t t=tail+numElements;
		if (t>head)
			return RING_BUFFER_ERROR;
		tail=t;
		return RING_BUFFER_OK;
	}
	

	size_t write(T* pSrcElements, size_t numElements)
	{
		size_t initHead=head;
		size_t count;
		size_t diff;
		clock_t t0=clock();
		size_t posSrc=0;
		while (numElements){
			size_t Tail=tail;
			size_t posHead=head&(size-1);			
			size_t posTail=Tail&(size-1);
			if (posTail<posHead || head==Tail)
				diff=size-posHead; 
			else if (posHead<posTail)
				diff=posTail-posHead; 
			else { 								// buffer is full
				halSleep(time2sleep);
				continue;
			} 
			count=MIN(numElements,diff);
			//if (
			//push_memcpy(data+posHead,pSrcElements+posSrc,count*sizeof32(T));
			push_memcpy(pSrcElements+posSrc,data+posHead,count*sizeof32(T));
			head+=count;
			posSrc+=count;
			numElements -=count;
			//}
		}
		return (head-initHead);
	}
	
	size_t writedma(T* pSrcElements, size_t numElements)
	{
		while (writeAvail()<numElements){
			halSleep(time2sleep);
		}
		size_t posHead=head&(size-1);			
		size_t posTail=tail&(size-1);
		dmahead=head+numElements;
		halSetCallbackDMA(writeCommit);
		if (posTail<posHead || head==tail){
			size_t diff=size-posHead; 
			if (numElements<=diff){
				halInitSingleDMA(pSrcElements,data+posHead,numElements*sizeof(T));
			} else {
				halInitDoubleDMA(pSrcElements,pSrcElements+diff,data+posHead,data,diff*sizeof(T),(numElements-diff)*sizeof(T));
			}
		}
		else { //if (posHead<posTail){
			halInitSingleDMA(pSrcElements,data+posHead,numElements*sizeof(T));
		}
		return 0;
	}

	size_t write(T pSrcElement)
	{
		return write(&pSrcElement,1);
	}
	size_t pushDMA(){
	}

	
	
	size_t read(T* pDstElements, size_t numElements, int ExitMode= EXIT_ON_COMPLETED)
	{
		size_t initTail=tail;
		size_t count;
		size_t diff;
		clock_t t0=clock();
		size_t posDst=0;
		while (numElements){
			size_t Head=head;
			size_t posHead=Head&(size-1);
			size_t posTail=tail&(size-1);
			if (posTail<posHead )
				diff=posHead-posTail; 
			else if (posHead<=posTail && Head!=tail)
				diff=size-posTail; 
			else if (ExitMode&EXIT_ON_EMPTY_BUFFER) // buffer is empty
				return (tail-initTail);	
			else if (ExitMode&EXIT_ON_TIMEOUT){
				if ((clock()-t0)>timeout)
					return (tail-initTail);	
				halSleep(time2sleep);
				continue;
			} else {
				//Sleep(time2sleep);
				continue;
			}
			
			count=MIN(numElements,diff);
			//pop_memcpy(pDstElements,posDst*bytesizeof(T),data,posTail*bytesizeof(T),count*bytesizeof(T));
			pop_memcpy(data+posTail,pDstElements+posDst,count*sizeof32(T));
			tail+=count;
			posDst+=count;
			numElements -=count;
		}
		return (tail-initTail);
	}

	size_t readdma(T* pDstElements, size_t numElements, int ExitMode= EXIT_ON_COMPLETED)
	{
		size_t initTail=tail;
		size_t count;
		size_t diff;
		clock_t t0=clock();
		size_t posDst=0;
		while (numElements){
			size_t Head=head;
			size_t posHead=Head&(size-1);
			size_t posTail=tail&(size-1);
			if (posTail<posHead )
				diff=posHead-posTail; 
			else if (posHead<=posTail && Head!=tail)
				diff=size-posTail; 
			else if (ExitMode&EXIT_ON_EMPTY_BUFFER) // buffer is empty
				return (tail-initTail);	
			else if (ExitMode&EXIT_ON_TIMEOUT){
				if ((clock()-t0)>timeout)
					return (tail-initTail);	
				halSleep(time2sleep);
				continue;
			} else {
				//Sleep(time2sleep);
				continue;
			}

			count=MIN(numElements,diff);
			//pop_memcpy(pDstElements,posDst*bytesizeof(T),data,posTail*bytesizeof(T),count*bytesizeof(T));
			pop_memcpy(data+posTail,pDstElements+posDst,count*sizeof32(T));
			tail+=count;
			posDst+=count;
			numElements -=count;
		}
		return (tail-initTail);
	}
*/
//};


#endif
