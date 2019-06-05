#include "stdio.h"
#include "hal.h"
//#include "nmtype.h"
//#include "nmpp.h"
extern "C" void printMatrix_16s(char* text, nm16s* matrix, int height,int width){
	char address[16];
	sprintf (address,"%x",matrix);
	printf("%s [%s]\n",text,address);
	for (int y=0,i=0; y<height; y++){
		for(int x=0; x<width; x++,i++){
			int val=halGet_16s(matrix,i);
			printf("%4d ",val);
		}
		printf("\r\n");
	}
}
