#include <stdio.h>
#include <hal.h>
#include <dma.h>

#pragma data_section ".data_imu4"
	int src[32+1][32];
#pragma data_section ".data_imu6"
	int dst[32+1][32];

int main(){
	int counter = 0;
	//код ниже обеспечивает гарантированное выравнивание по 16 32-битных слов
	int* src_matrix = (int*)((int)src >> 4);
	int* dst_matrix = (int*)((int)dst >> 4);
	src_matrix = (int*)((int)src_matrix << 4);
	dst_matrix = (int*)((int)dst_matrix << 4);

	for(int i = 0; i < 32*32; i+=32){
		for(int j = 0; j < 32; j++)
		src_matrix[i + j] = counter++;
	}

	halOpenStatusDMA();//функция инициализирует окружение для работы DMA вызывается 1 раз для всех DMA
	int ret = halCheckParamsStausMatrixDMA((int*)src_matrix,32,32,32,(int*)dst_matrix,32);
	if (ret){
		printf("ERORR: wrong parametrs in DMA initialisation exit code is %d\n",ret);
		return ret;
	}

	printf("src = 0x%x\n",(int)src_matrix);
	printf("dst = 0x%x\n",(int)dst_matrix);
	
	halInitStatusMatrixDMA((int*)src_matrix,32,32,32,(int*)dst_matrix,32);
	
	while(halStatusDMA()){}

	printf("Matrix DMA had finished relays on status function\n");
	for(int i = 0,offset = 0; i < 32; i++){
		printf("row[%d]  ",i);
		for(int j = 0; j < 32; j++){
			printf("%d  ",dst_matrix[j + offset]);
		}
		offset += 32;
		printf("\n");
	}


	return 0;
} 