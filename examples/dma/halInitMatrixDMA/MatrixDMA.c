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

	halOpenDMA();//this function writes the interruption vector into interruption controller and initialise some variables needed to provide a functionnality of dma on both core 

	halSetCallbackDMA((DmaCallback)callback);//set callback function to be called after dma is done. 
	//Pay attention that once set up call back will call after dma has finished forever untill it flashed or changhed 
	// to flash the last callback function call halSetCallBack(0);
	//load the paramentrs into matrix DMA
	int ret=halInitMatrixDMA((int*)src_matrix,32,32,32,(int*)dst_matrix,32);
	if (ret)
		return ret;

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