//test of DMA на плате 5103.  NM part
// SetCallBack( int vectoraddr, int ProcAddress); Устанавливает
// обработчик прерываний от окончания любого из каналов DMA.
// vectoraddr=58h для локальной шины и 60h для глобальной шины.
// return value является функцией, которая возвращает 0 если к
// этому времени DMA закончила пересылку. Если приоритет у DMA
// то она всегда возвращает ноль, т.к. не может получить управление
// до окончания пересылки.

// Здесь один обработчик на обе шины, но он только отмечает время.
// cnt_func dmainit( int dmahigh,  int *dstptr,  int *srcptr, int n );
// Даёт старт тому каналу DMA, который соответствует адресам чтения
//и записи. dstptr и srcptr должны быть на одной шине.
// Если dmahigh=1 то приоритет у DMA, если 0 то у ЦПУ.
// N количество 64р слов. Расход времени 1.5 такта на 64р слово
// если приоритет у DMA. Файл функций dma.asm
//==============================================================
#include "hal.h"
#include "dma.h"
#include "time.h"
#include "stdio.h"
#include "malloc.h"

// if dmahigh=1 then dma имеет высший приоритет else it has cpu
clock_t t0,t1;
int DMA_Finished=false;

void BellFromDma()// Обработчик DMA прерывания (вектор 58h)
{ 
	t1=clock();
	DMA_Finished=true;
}

#define SIZE32 32000

#pragma data_section ".data_int.bss"
	#define OFFSET 0x40000
	long long aligner0;
	int src[SIZE32];	
	
#pragma data_section ".data_ext.bss"

	long long aligner1;
	int dst[SIZE32];


int compare(int* a, int* b, int size ){
	for (int i=0; i<size;i++){
		if (a[i]!=b[i])
			return i;
	}
	return -1;
}

void initInc(int* arr, int startVal, int increment, int size){
	for (int i=0; i<size;i++){
		arr[i]=startVal;
		startVal+=increment;
	}
}
	
int main()
{ 
	
	printf("Start\n");
	
	*((int*)0x40001000) |= 0x10;
	initInc((int*)src,1,1,SIZE32);
	
	initInc((int*)dst,2,3,SIZE32);
	printf("SRC:%x DST:%x\n",src,dst);
	for(int i=0; i<10; i++){
		printf("%d %d\n",src[i],dst[i]);
	}
	//SetCallBack(0x58, (int)BellFromDma);
	//SetCallBack(0x60, (int)BellFromDma);

	t0=clock();
	//fd=(cnt_func)dmainit( 0, (int*)src, (int*)dst, SIZE_dst);
	//cnt_func fd=(cnt_func)dmainit( 0, (int*)src, (int*)dst, SIZE_dst);
	
	printf("Init DMA:[%x] => [%x]\n",src, dst);	
	int err;
	//err=dmaInit( (int*)src+0x40000, dst, SIZE32, 0);
	//err=dmaInit( (int*)src, dst, SIZE32, 0);
	//err=dmaInit( dst, (int*)src+0x40000,  SIZE32, 0);
	err=dmaInit( dst, (int*)src,  SIZE32, 0);
	//int err=dmaInit( dst, (int*)src, SIZE32, 0);
	//return err;
	//int err=dmaInit( (int*)src+0x40000,dst, SIZE32, 0);
	//printf("DMA ERROR:%x",err);
	
	if (err){
		printf("ERROR:DMA init  %x=>%x\n",src, dst);
		return err;
	}
	
	
	
	//printf("DMA_working... \n");
	int d=3;
	//while (((d=dmaStatus())&2)==0){
	while (dmaStatus()){
		printf(".");
	}
	//while(1){
		//if(dmaStatus()==0){
			//break;
		//}
	//}

	t1=clock();
	
	
	
	
	printf("\nDMA_Finished=%d\n",DMA_Finished);
	
	err=compare((int*)src,(int*)dst,SIZE32);
	if (err!=-1){
		printf("Error in src[%d]=%x dst[%d]=%x  \n",err,src[err],err,dst[err]);
	}
	else {
		printf("Test  DMA:OK!\n");
	}
	
	float tt=float(t1-t0)/SIZE32;
	printf("Src=>Dst: %d words transfertd at %d clocks , %f ticks/per 32-bit word\n",SIZE32,t1-t0,tt);
	for(int i=0; i<10; i++){
		printf("%d %d\n",src[i],dst[i]);
	}
	return 0;
	
	
} 