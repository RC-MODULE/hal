//extern int statusDMA;
#ifndef THALBUFFER_DEFINED
#define THALBUFFER_DEFINED
#include "dma.h"
#include "hal.h"


template<class T, int countOfT , int shieldSize32=1> class tHalRingBuffer{
public:

	size_t 		maxCount;		///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	size_t 		maxCountMinus1;	///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	size_t 		size;			///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	unsigned 	head;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код MASTER уже записал в	буфер входных данных [заполняется MASTER]
	unsigned	tail;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код SLAVE  уже прочитал (обработал) 			 [заполняется SLAVE]
	size_t		pendingCount;
	long long 	sheild0[(shieldSize32 +1)/2];
	T			data[countOfT];			///<  физический адрес кольцевого буфера входных данных 
	long long 	sheild1[(shieldSize32 +1)/2];
	FuncSingleCopy singleCopy;
	FuncDoubleCopy doubleCopy;
	FuncSetCallbackDMA setCallback;
	
	int init(FuncSingleCopy singleMemCopy, FuncDoubleCopy doubleMemCopy,  FuncSetCallbackDMA setCallback)
	{
		if ((countOfT&(countOfT -1))!=0) return RING_BUFFER_INCORRECT_COUNT;
		maxCount = countOfT;
		maxCountMinus1= countOfT -1;
		size = sizeof32(T);
		head=0;
		tail=0;
		pendingCount=0;
		singleCopy=singleMemCopy;
		doubleCopy=doubleMemCopy;
		setCallback=setCallback;
		return RING_BUFFER_OK;
	}

	void 	push(T* src, size_t count)
	{
		while (tail+maxCount-head < count)
			halSleep(RING_BUFFER_SLEEP);
		/*
		if (setCallback){
			while (pendingCount)
				halSleep(RING_BUFFER_SLEEP);
			pendingCount=count;
			setCallback(halRingBufferPushCallback);
			activeRingBuffer=ringBuffer;
		}
		*/
		
		size_t fixTail = tail;
		size_t posHead = head & maxCountMinus1;			
		size_t posTail = fixTail & maxCountMinus1;
		T*    addrHead = data + posHead;
		
		// [.......<Tail>******<Head>.....]
		if (posTail<posHead || head==fixTail){
			size_t countToEnd = maxCount - posHead;
			if (count <= countToEnd){
				singleCopy(src, addrHead ,count*size);
			}
		// [*******<Head>......<Tail>*****]
			else {
				int firstSize =countToEnd*size;
				int secondSize=(count-countToEnd)*size;
				doubleCopy(src, (int*)src+firstSize, addrHead, data, firstSize, secondSize);
			}
		}
		else {
			singleCopy(src, addrHead, count*size);
		}

		if (setCallback==0)
			head += count;
	}

	void 	pop(T* dst, size_t count){
		while (head-tail < count)
			halSleep(RING_BUFFER_SLEEP);
		/*
		if (setCallback){
			while (pendingCount)
				halSleep(RING_BUFFER_SLEEP);
			pendingCount=count;
			setCallback(halRingBufferPopCallback);
			activeRingBuffer=ringBuffer;
		}*/

		//size_t curHead    = ringBuffer->head;
		size_t posHead = head & maxCountMinus1;			
		size_t posTail = tail & maxCountMinus1;
		T* addrTail    = data + posTail;
		
		// [.......<Tail>******<Head>.....]
		if (posTail<posHead){
			singleCopy(addrTail, dst, count*size);
		}
		// [*******<Head>......<Tail>*****]
		else {
			size_t countToEnd = maxCount - posTail;
			if (count <= countToEnd){
				singleCopy(addrTail, dst, count*size);
			}
			else {
				int firstSize = countToEnd*size;
				int secondSize= (count-countToEnd)*size;
				doubleCopy(addrTail, data, dst, (int*)dst+firstSize, firstSize, secondSize);
			}
		}
		if (setCallback==0)
			tail += count;
	}
	T* 	getHead(){
		size_t posHead = head & maxCountMinus1;			
		T* addrHead  = data + posHead;
		return addrHead;
	}
	
	T* 	getTail(){
		size_t posTail = tail & maxCountMinus1;
		T* addrTail  = data + posTail*size;
		return addrTail;
	}
	int		getCount(){
		return head - tail;
	}
	int 	isBusy(){
		if (setCallback==0)
			return false;
		return pendingCount;	
	}
	int 	isEmpty(){
		return head == tail;
	}
	int 	isFull(){
		return head == tail + maxCount;
	}
};


/*
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

*/


#endif //THALBUFFER_DEFINED