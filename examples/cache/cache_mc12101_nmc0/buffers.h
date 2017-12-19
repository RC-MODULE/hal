// массив для приема данных из циклического буфера

extern __int64 pool0[];
extern __int64 pool1[];
extern __int64 pool2[];
extern __int64 pool3[];
extern __int64 pool4[];
extern __int64 pool5[];

#define KB 1024/4
 
#define SIZE_POOL0 100*KB
#define SIZE_POOL1 100*KB
#define SIZE_POOL2 115*KB
#define SIZE_POOL3 128*KB
#define SIZE_POOL4 1320*KB 
#define SIZE_POOL5 1320*KB 

#define POOL0 0
#define POOL1 1
#define POOL2 2
#define POOL3 3

#define POOL_SHARED 4
#define POOL_EXT 5
