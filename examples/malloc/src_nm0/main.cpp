// #include "math.h"
#include "malloc.h"
#include "stdio.h"


#define SIZE 256

__attribute__ ((section(".mem_bank0"))) volatile double static0[SIZE];
__attribute__ ((section(".mem_bank1"))) volatile double static1[SIZE];
__attribute__ ((section(".mem_bank2"))) volatile double static2[SIZE];
__attribute__ ((section(".mem_bank3"))) volatile double static3[SIZE];
__attribute__ ((section(".mem_bank4"))) volatile double static4[SIZE];
__attribute__ ((section(".mem_bank5"))) volatile double static5[SIZE];
__attribute__ ((section(".mem_bank6"))) volatile double static6[SIZE];
__attribute__ ((section(".mem_bank7"))) volatile double static7[SIZE];


// OLD SCHOOL
// #pragma data_section ".data_imu1"
//     double x1[SIZE];
// #pragma data_section ".data_imu2"
//     double x2[SIZE];
// #pragma data_section ".data_imu3"
//     double y1[SIZE];
// #pragma data_section ".data_imu4"
//     double y2[SIZE];

#include "malloc.h"
//int nmc_malloc_set_heap(int){}

int main()
{
	printf("static0 at %x\n",  static0) ;
	printf("static1 at %x\n",  static1) ;
	printf("static2 at %x\n",  static2) ;
	printf("static3 at %x\n",  static3) ;
	printf("static4 at %x\n",  static4) ;
	printf("static5 at %x\n",  static5) ;
	printf("static6 at %x\n",  static6) ;
	printf("static7 at %x\n\n",static7) ;
	
    for (int active_heap=0; active_heap<11; active_heap++){
		nmc_malloc_set_heap(active_heap);
    	double* dynamic= (double *)malloc( SIZE * sizeof(double));
		printf( "dynamic%d at %x\n",active_heap,dynamic );
		free(dynamic);
    }
	

    return 1;
}
