#include "stdio.h"
extern "C" void printMatrix_32s(char* text, int* matrix, int height,int width){
	char address[16];
	sprintf (address,"%x",matrix);
	printf("%s [%s]\n",text,address);
	for (int y=0,i=0; y<height; y++){
		for(int x=0; x<width; x++,i++){
			printf("%8d ",matrix[i]);
		}
		printf("\n");
	}
}
