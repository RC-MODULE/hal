#ifndef DISABLE_PRINTX
extern "C" void print_32x(char* text,unsigned x);
extern "C" void printMatrix_32x(char* text, int* matrix, int height,int width);
extern "C" void printMatrix_32s(char* text, int* matrix, int height,int width);

extern "C" void printMatrixS_32x(char* text, int* matrix, int height,int width, int stride);
extern "C" void printMatrixS_32s(char* text, int* matrix, int height,int width, int stride);

extern "C" void printMatrix_16x(char* text, nm16s* matrix, int height,int width);
extern "C" void printMatrix_16s(char* text, nm16s* matrix, int height,int width);


#define PRINT_MATRIX_32X(text, matrix,  height, width) printMatrix_32x(text, (int*)matrix,  height, width)
#define PRINT_MATRIX_32S(text, matrix,  height, width) printMatrix_32s(text, (int*)matrix,  height, width)

#define PRINT_MATRIX_16X(text, matrix,  height, width) printMatrix_16x(text, (nm16s*)matrix,  height, width)
#define PRINT_MATRIX_16S(text, matrix,  height, width) printMatrix_16s(text, (nm16s*)matrix,  height, width)
#else 

#define PRINT_MATRIX_32X(text, matrix,  height, width) 
#define PRINT_MATRIX_32S(text, matrix,  height, width) 

#define PRINT_MATRIX_16X(text, matrix,  height, width) 
#define PRINT_MATRIX_16S(text, matrix,  height, width) 

#endif