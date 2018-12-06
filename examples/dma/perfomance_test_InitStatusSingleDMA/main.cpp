//***************************************************************************/
//*                     RC Module Inc., Moscow, Russia                      */
//*                     NeuroMatrix(r) NM640x Software                      */
//*                                                                         */
//*   Автор:  Бродяженко Андрей                                      		*/
//*                                                                         */
//*   Данная программа является примером параллельной работы устройсва DMA  */
//*   с процессором (в данном случае с RISC, но аналогично можно			*/
//*	  использовать векторный сопроцессор).									*/
//*   Варианты копирования (массива целых чисел), описанные в программе:	*/
//*   1. Копирование массива из SRAM в DDR, после чего на					*/
//*   RISC-ядре выполняется умножение 2-х векторов.							*/
//*   2. Копирование массива из DDR в SRAM, после чего на					*/
//*   RISC-ядре выполняется умножение 2-х векторов.							*/
//*   3. Копирование массива из SRAM в DDR. Умножение 2-х векторов на		*/
//*   RISC-ядре выполняется параллельно копированию.						*/
//*   4. Копирование массива из DDR в SRAM. Умножение 2-х векторов на		*/
//*   RISC-ядре выполняется параллельно копированию.						*/
//*   Размер копируемого вектора равен SIZE.								*/
//*	  Размер умножаемых векторов SIZE >> 9.									*/
//*   Для каждого варианта измеряется время выполнения (в тактах)			*/
//*   и выводится в консоль.												*/
//    ВАЖНО!																*/
//*   Если адрес входного для DMA массива не кратен 16, DMA 				*/
//*	  копирует данные медленнее.											*/
//***************************************************************************/

#include "time.h"
#include "nmpp.h"
#include "dma.h"
#include <stdio.h>
#include <string.h>

#define					SIZE							512
#define					DMA_SIZE						SIZE
#define					MUL_SIZE						SIZE >> 9

//------------Объявление массивов-------------//
#pragma data_section ".data_imu1"
	int mult1_imu1[SIZE];
	int src_imu1[SIZE];				// Адрес этого входного для DMA массива кратен 16, поэтому выравнивать не надо
#pragma data_section ".data_imu2"
	int mult2_imu2[SIZE];
	int product_imu2[SIZE];
	int dst_imu2[SIZE];
#pragma data_section ".data_imu3"
	int product_imu3[SIZE];
	int dst_imu3[SIZE];
#pragma data_section ".data_imu4"
	int src_imu4[SIZE];				// Адрес этого входного для DMA массива кратен 16, поэтому выравнивать не надо
	int mult1_imu4[SIZE];
	int dst_imu4[SIZE];
#pragma data_section ".data_ddr"
	long algn[6];					// Этот массив делает кратным 16 адрес входного массива src_ddr для DMA (особенность DMA)
	int src_ddr[SIZE];
	int dst_ddr[SIZE];

//---------Функция перемножения 2-х векторов целых чисел на RISC-ядре NeuroMatrix-----------//
void MulVec(const int* pSrcVec1, const int* pSrcVec2, int* pDstVec, int size)
{
	for(int i = 0; i < size; i++) {
		pDstVec[i] = pSrcVec1[i] * pSrcVec2[i];
	}
}

//---------Функция для вывода массива целых чисел в консоль-----------//
void PrintArray(const int* pArray, int size)
{
	for(int i = 0; i < size; i++) {
		printf("Element[%d] = %d;\n", i, pArray[i]);
	}
}

int main()
{
	clock_t t[8];

//------------Инициализация массивов для копирования и умножения-----------//
	memset(src_imu1, 0, SIZE);
	memset(src_imu4, 1, SIZE);
	memset(src_ddr, 2, SIZE);
	memset(mult1_imu1, 3, SIZE);
	memset(mult2_imu2, 4, SIZE);
	memset(mult1_imu4, 5, SIZE);

//------------Инициализация устройства DMA------------//
	halOpenDMA();

//------------Вариант 1, SRAM -> DDR------------//
	t[0] = clock();
	halInitStatusSingleDMA((void*)(src_imu1), (void*)(dst_ddr), DMA_SIZE);
	while (halStatusDMA());

//------------Вариант 2, DDR -> SRAM------------//
	t[1] = clock();
	halInitStatusSingleDMA((void*)(src_ddr), (void*)(dst_imu2), DMA_SIZE);
	while (halStatusDMA());

//------------Умножение 2-х векторов, которое выполняется после завершения работы DMA------------//
	t[2] = clock();
	MulVec(mult1_imu1, mult2_imu2, product_imu3, MUL_SIZE);

//------------Вариант 3, SRAM(imu1) -> DDR, умножение 2-х векторов выполняется на фоне работы DMA------------//
	t[3] = clock();
	halInitStatusSingleDMA((void*)(src_imu1), (void*)(dst_ddr), DMA_SIZE);
	MulVec(mult1_imu1, mult2_imu2, product_imu3, MUL_SIZE);
	while (halStatusDMA());

//------------Вариант 3, SRAM(imu4) -> DDR, умножение 2-х векторов выполняется на фоне работы DMA------------//
	t[4] = clock();
	halInitStatusSingleDMA((void*)(src_imu4), (void*)(dst_ddr), DMA_SIZE);
	MulVec(mult1_imu1, mult2_imu2, product_imu3, MUL_SIZE);
	while (halStatusDMA());

//------------Вариант 4, DDR -> SRAM(imu2), умножение 2-х векторов выполняется на фоне работы DMA------------//
	t[5] = clock();
	halInitStatusSingleDMA((void*)(src_ddr), (void*)(dst_imu2), DMA_SIZE);
	MulVec(mult1_imu1, mult2_imu2, product_imu3, MUL_SIZE);
	while (halStatusDMA());

//------------Вариант 4, DDR -> SRAM(imu4), умножение 2-х векторов выполняется на фоне работы DMA------------//
	t[6] = clock();
	halInitStatusSingleDMA((void*)(src_ddr), (void*)(dst_imu4), DMA_SIZE);
	MulVec(mult1_imu1, mult2_imu2, product_imu3, MUL_SIZE);
	while (halStatusDMA());
	t[7] = clock();

	//PrintArray(product_imu3, SIZE);

//------------Вывод в консоль времени работы для каждого варианта------------//
	printf("DMA (SRAM -> DDR):                          %d\n", t[1] - t[0]);
	printf("DMA (DDR  -> SRAM):                         %d\n", t[2] - t[1]);
	printf("Mul (SRAM):                                 %d\n", t[3] - t[2]);
	printf("DMA (SRAM(imu1) -> DDR) and Mul(imu1,2,3):  %d\n", t[4] - t[3]);
	printf("DMA (SRAM(imu4) -> DDR) and Mul(imu1,2,3):  %d\n", t[5] - t[4]);
	printf("DMA (DDR -> SRAM(imu2)) and Mul(imu1,2,3):  %d\n", t[6] - t[5]);
	printf("DMA (DDR -> SRAM(imu4)) and Mul(imu1,2,3):  %d\n", t[7] - t[6]);
	return 0;
}

// DMA_SIZE = 8192 (int)
// MUL_SIZE = 16 (int)

// DMA (SRAM -> DDR):                          5586
// DMA (DDR  -> SRAM):                         5634
// Mul (SRAM):                                 1982
// DMA (SRAM(imu1) -> DDR) and Mul(imu1,2,3):  5557
// DMA (SRAM(imu4) -> DDR) and Mul(imu1,2,3):  5689
// DMA (DDR -> SRAM(imu2)) and Mul(imu1,2,3):  5599
// DMA (DDR -> SRAM(imu4)) and Mul(imu1,2,3):  5740
