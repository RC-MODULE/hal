#include <stdio.h>
#include <hal.h>
#include "nmpp.h"

//этот пример показывает как использовать doubleDMA
#define SIZE 100

int user_callback(){
	halLed(0xaa);
	return 0;
}
int done = 1;
int user_callback_to_stop(){
	halLed(0xbb);
	done = 0;
	return 0;
}

int main(){
	int ret;
	int size0 = 128;
	int size1 = 16;
	nmppsMallocSetRoute16(0xF0101);
	
	int* src0 = nmppsMalloc_32s(size0);
	int* dst0 = nmppsMalloc_32s(size0);
	int* src1 = nmppsMalloc_32s(size1);
	int* dst1 = nmppsMalloc_32s(size1);
	
	if (src0==0 || src1==0 || dst0==0 || dst1==0 )
		return -1;
	
	for(int i = 0; i < size0; i++){
		src0[i] = i;
		dst0[i] = 0;
	}	
	for(int i = 0; i < size1; i++){
		src1[i] = 0x10000+i;
		dst1[i] = 0;
	}	
	
	halOpenDMA();//функция инициализирует окружение для работы DMA вызывается 1 раз для всех DMA
	
	halSetCallbackDMA((DmaCallback)user_callback);//устанавливает адрес callback функции 
	
	//Устанавленный выше callback будет действовать всегда до тез пор пока он не будет или переписан другим вызовом halSetCallbackDMA()
	//или сброшен при помощи halSetCallbackDMA(0);

	ret = halCheckParamsDoubleDMA(src0,src1,dst0,dst1,size0,size1);
	
	if (ret){
		printf("ERORR: wrong parametrs in DMA initialisation exit code is %d\n",ret);
		return ret;
	}
	
	halInitDoubleDMA(src0,src1,dst0,dst1,size0,size1);

	
	//когда DMA закончит передачу halStatusDMA() вернет 0
	while(halStatusDMA()){

	}
	
	printf("DMA had finished relays on halStausDMA\n");
	printf("\n");
	for(int i = 0; i < 16; i++){
		printf("src0[%d] = %x \t dst0[%d] = %x\n",i,src0[i],i,dst0[i]);
	}
	for(int i = 0; i < 16; i++){
		printf("src1[%d] = %x \t dst1[%d] = %x\n",i,src1[i],i, dst1[i]);
	}

	for(int i = 0; i < size0; i++){
		src0[i] = i;
		dst0[i] = 0;
	}	
	for(int i = 0; i < size1; i++){
		src1[i] = 0x10000+i;
		dst1[i] = 0;
	}	
	
	halSetCallbackDMA((DmaCallback)user_callback_to_stop);
	halInitDoubleDMA(src0,src1,dst0,dst1,size0,size1);
	//ещё один способ узнать о завершение передачи это использовать функцию callback и переменную
	//в которую в callback пишется определенное значение
	while(done){

	}
	printf("\n");
	printf("DMA had finished relays on CallBack function\n");
	printf("\n");
	for(int i = 0; i < 16; i++){
		printf("src0[%d] = %x \t dst0[%d] = %x\n",i,src0[i],i,dst0[i]);
	}
	for(int i = 0; i < 16; i++){
		printf("src1[%d] = %x \t dst1[%d] = %x\n",i,src1[i],i, dst1[i]);
	}

	
	return 0;
	
}