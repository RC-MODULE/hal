#include "VShell.h"
#include <stdlib.h>
//#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <hal_host.h>
#include <hal.h>




int main(int argc, char *argv[])
{
	if (!VS_Init())
		return 0;

	//if (!VS_Bind("D:/Video/Films/AVI/DV1.avi"))
	//return 0;

	printf("Openning...\n");
	if (halOpen("main.abs", NULL)) {			// Load executable file to board, connect to shared memory
		halSleep(2000);
		return -1;
	}

	int width = 352;// VS_GetWidth(VS_SOURCE);
	int height = 240;// VS_GetHeight(VS_SOURCE);
	int size = width*height;


	VS_CreateImage("Src", 1, width, height, VS_RGB8, 0);
	VS_CreateImage("Dst", 2, width, height, VS_RGB8, 0);


	int* src =  (int*)malloc(size);
	int* dst =  (int*)malloc(size);
	

	int sync = halSync(0x8086,0);			// Receive handshake (0x6406)
	
	sync = halSync(width,0);
	sync = halSync(height,0);

	unsigned srcAddr = halSync(0xBABA,0);
	unsigned dstAddr = halSync(0xDEDA,0);

	if (srcAddr == 0 || dstAddr == 0)
		return -1;

	for (int i = 0; i < size; i++) {
		((char*)src)[i] = i%width;
	}
	while (VS_Run()) {
		VS_GetGrayData(VS_SOURCE, dst);
		VS_SetData(1, src);


		int ok1=halWriteMemBlock((unsigned int*)src, srcAddr, size / 4,0);
		sync = halSync(0x600DB00F,0);		// Get nmc status

										//nmppsSubC_8s(src, 128, src, size);
										//average4to1_nm8s(src, dst, tmp, size);
										//nmppsAddC_8s(dst, 128, dst, size/4);
		sync = halSync(0x600DB00F,0);		// Get nmc status

		int ok2=halReadMemBlock((unsigned int*)dst, dstAddr, size  / 4,0); // read data from board
		

		
		VS_SetData(2, dst);
		VS_Draw(VS_DRAW_ALL);
		if (dst[0]!= ~src[0]) {
			printf("%x %x %x %x %x %x \n", ok1, ok2, srcAddr, dstAddr, src[0], dst[0]);
			//VS_OpReset();
			break;
			
		}

	}
	while (1);
    return 0;
}
