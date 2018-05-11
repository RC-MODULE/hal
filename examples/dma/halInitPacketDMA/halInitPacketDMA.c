#include <stdio.h>
#include <malloc.h>
#include <hal.h>

#define SIZE 100

#pragma data_section ".data_imu0"
	int src_1[SIZE];
#pragma data_section ".data_imu1"
	int src_2[SIZE];
	int dst_1[SIZE];
#pragma data_section ".data_ext"	
	int dst_2[SIZE];
	int dst_3[SIZE];


int done = 1;
int user_callback(){
	done = 0;
	halLed(0xaa);
	return 0;
}


int main(){
	halOpenDMA();//функция инициализирует окружение для работы DMA вызывается 1 раз для всех DMA

	halSetCallbackDMA((DmaCallback)user_callback);//устанавливает адрес callback функции 
	
	//Устанавленный выше callback будет действовать всегда до тез пор пока он не будет или пеереписан другим вызовом halSetCallbackDMA()
	//или сброшен при помощи halSetCallbackDMA(0);

	int* arr_src[4];
	int* arr_dst[4];
	int arr_size[4] = {SIZE,SIZE,SIZE,SIZE};
	//массив адресов источника
	nmppsMallocSetRoute16(0xF0101); 
	arr_src[0] = nmppsMalloc_32s(SIZE);
	arr_src[1] = nmppsMalloc_32s(SIZE);
	arr_src[2] = nmppsMalloc_32s(SIZE);
	arr_src[3] = nmppsMalloc_32s(SIZE);
	nmppsMallocSetRoute16(0xF1010);
	//массив адресов приемника 
	arr_dst[0] = nmppsMalloc_32s(SIZE);
	arr_dst[1] = nmppsMalloc_32s(SIZE);
	arr_dst[2] = nmppsMalloc_32s(SIZE);
	arr_dst[3] = nmppsMalloc_32s(SIZE);


	for(int i = 0; i < SIZE; i++){
		src_1[i] = i;
	}
	for(int i = 0; i < SIZE; i++){
		src_2[i] = -i;
	}
	int* arr = arr_src[2];
	for(int i = 0; i < SIZE; i++){
		arr[i] = 123;
	}
	arr = arr_src[3];
	for(int i = 0; i < SIZE; i++){
		arr[i] = 23;
	}
	int ret = halInitPacketDMA((void**)arr_src,(void**)arr_dst,arr_size,4);
	if (ret){
		printf("ERORR: wrong parametrs in DMA initialisation exit code is %d\n",ret);
		return ret;
	}

	int time = 0;
	while(done){
		time++;
		if(time > 1000000){
			printf("TIME IS OVER\n");
			return 3;
		}
	}

	printf("ARRAY 1\n");
	for(int i = 0; i < 10; i++){
		printf("[%d] = %d \n",i,*(arr_dst[0] + i));
	}
	printf("ARRAY 2\n");
	for(int i = 0; i < 10; i++){
		printf("[%d] = %d \n",i,*(arr_dst[1] + i));
	}
	printf("ARRAY 3\n");
	for(int i = 0; i < 10; i++){
		printf("[%d] = %d \n",i,*(arr_dst[2] + i));
	}
	printf("ARRAY 4\n");
	for(int i = 0; i < 10; i++){
		printf("[%d] = %d \n",i,*(arr_dst[3] + i));
	}
	return 0;
}
