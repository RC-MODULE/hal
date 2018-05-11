#include <stdio.h>
#include <hal.h>
#include <dma.h>

#pragma data_section ".data_imu0"
	int src_matrix[32][32];
#pragma data_section ".data_imu1"
	int dst_matrix[32][32];
int done = 1;
int callback(){
	done = 0;
	halLed(0xaa);
	return 0;
}


int main(){
	int counter = 0;
	for(int i = 0; i < 32; i++){
		for(int j = 0; j < 32; j++)
		src_matrix[i][j] = counter++;
	}

	halOpenDMA();//функция инициализирует окружение для работы DMA вызывается 1 раз для всех DMA

	halSetCallbackDMA((DmaCallback)callback);//устанавливает адрес callback функции 
	
	//Устанавленный выше callback будет действовать всегда до тез пор пока он не будет или пеереписан другим вызовом halSetCallbackDMA()
	//или сброшен при помощи halSetCallbackDMA(0);

	int ret = halInitMatrixDMA((int*)src_matrix,32,32,32,(int*)dst_matrix,32);
	if (ret){
		printf("ERORR: wrong parametrs in DMA initialisation exit code is %d\n",ret);
		return ret;
	}

	while(done){

	}
	printf("Matrix DMA had finished relays on callback function\n");
	printf("\n");
	for(int i = 0; i < 32; i++){
		printf("row[%d]  ",i);
		for(int j = 0; j < 32; j++){
			printf("%d  ",dst_matrix[i][j]);
		}
		printf("\n");
	}

	printf("\n");
	for(int i = 0; i < 32; i++){
		for(int j = 0; j < 32; j++)
		src_matrix[i][j] = counter--;
	}

	halInitMatrixDMA((int*)src_matrix,32,32,32,(int*)dst_matrix,32);

	while(halStatusDMA()){

	}
	printf("Matrix DMA had finished relays on callback function\n");
	printf("\n");
	for(int i = 0; i < 32; i++){
		printf("row[%d]  ",i);
		for(int j = 0; j < 32; j++){
			printf("%d  ",dst_matrix[i][j]);
		}
		printf("\n");
	}

	return 0;
} 