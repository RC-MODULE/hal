#ifndef MULTIHEAP_INCLUDED
#define MULTIHEAP_INCLUDED
typedef unsigned size_t32;
typedef unsigned size_t32;






#define GUARD_BITS 0x600DB1D5
#define MAX_BUFFER32_SIZE 16*1024*1024/4 
/// класс буфер - заголовок в начале выделяемой данамической памяти 
class BufferHeader{
	public:
	int 			guardInfoBits0;
	size_t32		sizeBuffer32;	///<< размер массива данных в буфере в 32-р словах
	int				prevDisp32;		///<< смещение  на предыдущий буфер в списке 
	int				nextDisp32;		///<< смещение на следующий буфер в списке 
	bool			isLocked;		///<< (неактивен)флаг блокировки буфера, запрещаюий его удаление с помощью release()
	int				guardInfoBits1;
	
	
	/// Возвращает указатель на данные в буфере 
	int* dataStart(){
		//return ((int*)this);
		return ((int*)this+sizeof(BufferHeader)/sizeof(int));
	};

	/// Возвращает указатель на конец данных в буфере (следу)
	int* dataEnd(){
		int *p=((int*)this+sizeof(BufferHeader)/sizeof(int)+sizeBuffer32);
		return p;
		//return ((int*)this+sizeBuffer32);
	}

	/// Возвращает указатель на предыдущий заголовок
	BufferHeader* prev() {
		if (prevDisp32 == 0)
			return 0;
		return (BufferHeader*)((int*)this + prevDisp32);
	}

	/// Возвращает указатель на следующий заголовок
	BufferHeader* next() {
		if (nextDisp32 == 0)
			return 0;
		return (BufferHeader*)((int*)this + nextDisp32);
	}
	/// Связывает с предыдущим элементом
	void linkPrev(BufferHeader* prevHeader) {
		prevDisp32 = (int*)prevHeader - (int*)this;
	}
	/// Связывает со следующим элементом
	void linkNext(BufferHeader* nextHeader) {
		nextDisp32 = (int*)nextHeader - (int*)this;
	}

	/// Возвращает указатель на конечные защитные поля (2 слова)
	int* EndGuardBits(){
		return dataEnd()-2;
	}
	unsigned CheckGuardBits(){
		if (guardInfoBits0!=GUARD_BITS){
			return 0xBADB00F1;
		}
		if (guardInfoBits1!=GUARD_BITS){
			return 0xBADB00F2;
		}
		if (sizeBuffer32){
			int* pFinalGuardBits=EndGuardBits();
			if ((*pFinalGuardBits)!=GUARD_BITS){
				return 0xBADB00F3;
			}
			if (*(pFinalGuardBits+1)!=GUARD_BITS){
				return 0xBADB00F4;
			}
			if (sizeBuffer32<0)
				return 0xBADB00FF;

			if (sizeBuffer32>MAX_BUFFER32_SIZE)
				return 0xBADB00FE;
		}
		return 0;
	}
};


/// класс - куча
template<int SIZE32> class  Heap{
public:
	int				data[SIZE32];
	//BufferHeader*	pZeroBuffer;		///<< указатель на нулевой буфер в списке (c нулевым размером)
	//int*			pHeapEnd;			///<< указатель на слово следуюшее за концом кучи 
	int				sizeHeapAvail32;	///<< размер общей свободной памяти в куче
	bool			isHeapLocked;		///<< запрещает операции с кучей 
	int				status;
	BufferHeader* zeroBuffer() {
		return (BufferHeader*)data;
	}
	/// конструктор - создает кучу в указанной памяти
	Heap() {
		init();
	}
	void init(){
		isHeapLocked = false;
		status = 0;
		BufferHeader* pZeroBuffer= zeroBuffer();								// начальный адрес кучи
		pZeroBuffer->prevDisp32=0;								//
		pZeroBuffer->nextDisp32=0;
		pZeroBuffer->sizeBuffer32=0;
		pZeroBuffer->isLocked=true;
		pZeroBuffer->guardInfoBits0=GUARD_BITS;
		pZeroBuffer->guardInfoBits1=GUARD_BITS;

		//pHeapEnd   = (int*)addrHeap+size32Heap;								// конечный адрес кучи (следующий за последним байтом кучи)
		sizeHeapAvail32= SIZE32-(2*sizeof(BufferHeader)/4);
	}
	
	///// устанавливает принадлежность к куче
	int isMine(void* addr){
		if ((int*)pZeroBuffer < (int*)addr && (int*)addr < pHeapEnd)
			return 1;
		return 0;
	}

	/// Возвращает объем свободной памяти в пуле в 32р. словах
	//int HeapAvail(){
	//	return (size32FreeMem-sizeof(BufferHeader)/sizeof(int));
	//}

	//size_t32 TotalAvail(){
	//	return sizeHeapAvail32;
	//}
	
	/// Возвращает максимальный размер буфера  в 32р. словах, который можно выделить в куче
	size_t32 getMaxAvail(){
		int size32Max=0;
		BufferHeader* pBuffer=pZeroBuffer;
		for ( ; pBuffer->nextDisp32!=0; pBuffer=pBuffer->nextDisp32) {
			int sizeBetween32=(int*)pBuffer->nextDisp32 - pBuffer->dataEnd();
			if (sizeBetween32>size32Max) 
				size32Max=sizeBetween32;
		} 
		// находимся на последнем буфере
		int sizeBetween32=(int*)pHeapEnd - pBuffer->dataEnd();
		if (sizeBetween32>size32Max) 
			size32Max=sizeBetween32;
		size32Max-=sizeof(BufferHeader)/sizeof(int);
		return size32Max;
	}

	/// Выделяет буфер в куче
	int* allocate(size_t32 sizeBuffer32){
		if (isHeapLocked) 
			return 0;
		if (sizeBuffer32==0)
			return 0;
		sizeBuffer32+=(sizeBuffer32&1);	// округляем вверх до четного размера
		sizeBuffer32+=2;					// добавляем 2 слова для защитного поля
		BufferHeader* pCurrBuffer = zeroBuffer();
		int sizeBetween32=0;
		for(; pCurrBuffer->nextDisp32!=0; pCurrBuffer = pCurrBuffer->next()){
			// проверяем влезает ли массив в промежуток между тукущим и следующим
			BufferHeader* pNewBuffer  = (BufferHeader*)pCurrBuffer->dataEnd();
			BufferHeader* pNextBuffer = (BufferHeader*)pCurrBuffer->next();
			sizeBetween32=(int*)pNextBuffer - (int*)pNewBuffer;
			if (sizeBetween32>=sizeBuffer32+sizeof(BufferHeader)/sizeof(int)){
				// переустанавливаем связи в текущем буфере
				pCurrBuffer->linkNext(pNewBuffer);
				// переустанавливаем связи в следующем буфере
				pNextBuffer->linkPrev(pNewBuffer);
				// устнавливаем связи в новом буфере
				pNewBuffer->linkPrev(pCurrBuffer);
				pNewBuffer->linkNext(pNextBuffer);
				pNewBuffer->sizeBuffer32=sizeBuffer32;
				pNewBuffer->isLocked=false;
				// устанавливаем защитные биты
				pNewBuffer->guardInfoBits0=GUARD_BITS;
				pNewBuffer->guardInfoBits1=GUARD_BITS;
				//int* guardEndBits=pNewBuffer->EndGuardBits();
				//*guardEndBits    =GUARD_BITS;
				//*(guardEndBits+1)=GUARD_BITS;			
				// уменьшаем сободный размер				
				sizeHeapAvail32-=(sizeBuffer32+sizeof(BufferHeader)/sizeof(int));
				return pNewBuffer->dataStart();
			} 
		}
		// дошли до последнего буфера (у которого pCurrBuffer->nextDisp32=0)
		BufferHeader* pNewBuffer = (BufferHeader*)pCurrBuffer->dataEnd();
		sizeBetween32=SIZE32 - ((int*)pNewBuffer-(int*)zeroBuffer());
		if (sizeBetween32>=sizeBuffer32+sizeof(BufferHeader)/sizeof(int)){
			// создаем новый буфер
			// устнавливаем связи в новом буфере
			pNewBuffer->linkPrev(pCurrBuffer);
			pNewBuffer->nextDisp32=0;
			pNewBuffer->sizeBuffer32=sizeBuffer32;
			pNewBuffer->isLocked=false;
			// устанавливаем связи в текущем буфере
			pCurrBuffer->linkNext(pNewBuffer);

			// устанавливаем защитные биты
			pNewBuffer->guardInfoBits0=GUARD_BITS;
			pNewBuffer->guardInfoBits1=GUARD_BITS;
			//int* guardEndBits=pNewBuffer->EndGuardBits();
			//*guardEndBits    =GUARD_BITS;
			//*(guardEndBits+1)=GUARD_BITS;
			// уменьшаем сободный размер				
			sizeHeapAvail32 -= (sizeBuffer32 + sizeof(BufferHeader) / sizeof(int));
			return pNewBuffer->dataStart();
		} 
		return 0;
	}
	
	/// удаляет структуру буфера из списка 
	int remove(BufferHeader* pDelBuffer){
		if (isHeapLocked) 
			return 0;
		if (pDelBuffer->isLocked) 
			return 0;
		if (pDelBuffer->sizeBuffer32==0)
			return 0;
		
		if (pDelBuffer->next()) {										// если есть следующий
			pDelBuffer->prev()->linkNext(pDelBuffer->next());			// связываем ссылку предыдущего буфера на следующий 
			pDelBuffer->next()->linkPrev(pDelBuffer->prev());			// связываем ссылку следующего буфера на предыдущий 
		}
		else
			pDelBuffer->nextDisp32=0;	

		sizeHeapAvail32+=(pDelBuffer->sizeBuffer32+sizeof(BufferHeader)/sizeof(int));
		pDelBuffer->sizeBuffer32=-1;// необяз . флаг что удалили
		return 1;
	}
	
	/// осообождат память по адресу
	int release(void* p){
		if (isHeapLocked) 
			return 0;
		BufferHeader* pDelBuffer=(BufferHeader*)p-1;
		return remove(pDelBuffer);
	}
	
	/// блокирует указатель от удаления через release
	void lock(void* p){
		BufferHeader* pBuffer=(BufferHeader*)p-1;
		pBuffer->isLocked=true;
	}

	/// блокирует все указатели от удаления через release
	//void lockAll(){
	//	BufferHeader* pBuffer=pZeroBuffer->nextDisp32;
	//	while (pBuffer) {
	//		pBuffer->isLocked=true;
	//		pBuffer=pBuffer->nextDisp32;
	//	}
	//}
	/// разблокирует все указатели для удаления через release
	//void unlockAll(){
	//	BufferHeader* pBuffer=pZeroBuffer->nextDisp32;
	//	while (pBuffer) {
	//		pBuffer->isLocked=false;
	//		pBuffer=pBuffer->nextDisp32;
	//	}
	//}

	/// разблокирует все указатели для удаления через release
	//void unlock(void* p){
	//	BufferHeader* pBuffer=(BufferHeader*)p-1;
	//	pBuffer->isLocked=false;
	//	//numHeapLocked--;
	//}

	/// удаляет все указатели из кучи
	void releaseAll(){
		
		//lockHeap();
		if (isHeapLocked) 
			return ;
		while (BufferHeader* pBuffer = zeroBuffer()->next()){
			remove(pBuffer);
		}
		//unlockHeap();
	}
	/// Запрещает операции с кучей
	void lockHeap(){
		isHeapLocked=true;
	}
	/// Разрещает операции с кучей
	void unlockHeap(){
		isHeapLocked=false;
	}
	int check(){
		if (status)
			return status;

		if (pZeroBuffer->prevDisp32){
			status=0xBADB00F0; 
			return status;
		}
		if (pZeroBuffer->sizeBuffer32){
			status=0xBADB00FF; 
			return status;
		}
	//	if (status=pZeroBuffer->CheckGuardBits()){
	//		return status;
	//	}
		
		BufferHeader* pBuffer=pZeroBuffer->nextDisp32;

		int AllocatedMemory=0;
		int MaxHeapSize=(int*)pHeapEnd-(int*)pZeroBuffer;
		
		for(int i=0;i<100;i++){
			if (pBuffer==0)
				break;

			
			if (!isMine(pBuffer)){
				status =0xBADB00FA; 
				return status;
			}

			if (status=pBuffer->CheckGuardBits()){
				return status;
			}

			if (pBuffer->prevDisp32->nextDisp32!=pBuffer){
				status =0xBADB00FB; 
				return status;
			}

			AllocatedMemory+=sizeof(BufferHeader)/sizeof(int);
			AllocatedMemory+=pBuffer->sizeBuffer32;
			if (AllocatedMemory>MaxHeapSize){
				status=0xBADB00FD;
				return status;
			}
			if ((int)pBuffer&1){
				status=0xBADB00FC;
				return status;
			}
			pBuffer=pBuffer->nextDisp32;
		}
		return 0;
	}
};


#endif
