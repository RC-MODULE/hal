#include <stdio.h>
#include <hal.h>
#include <dma.h>

#pragma data_section ".data_imu1"
	//int dummy[8];
	int src_matrix[128*128];
#pragma data_section ".data_imu3"
	int dummy_2[2];
	int dst_matrix[128*128];
#pragma data_section ".data_shared"


int main(){

	halOpenDMA();//функция инициализирует окружение для работы DMA вызывается 1 раз для всех DMA

	printf("src=0x%x, dst=0x%x\n",src_matrix, dst_matrix);
	halSleep(2);
	halLed(1);
	
	//OK
	halInitMatrixDMA((int*)src_matrix,128,128,128,(int*)dst_matrix,128);
	while(halStatusDMA());

	halLed(4);
	//Error
	halInitStatusMatrixDMA((int*)src_matrix,128,128,128,(int*)dst_matrix,128);
	while(halStatusDMA());
	halLed(8);
	return 0;
} 