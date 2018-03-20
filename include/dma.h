#define DMA_BUSY  1
#define DMA_ERROR 2
#define DMA_READY 0 
#define DMA_OK    0 

typedef int(*DmaCallback2)();

#ifdef __cplusplus
		extern "C" {
#endif

	typedef int(*DmaCallback)();
	/**
	 *  \brief Brief
	 *  
	 *  \param [in] dstptr Parameter_Description
	 *  \param [in] srcptr Parameter_Description
	 *  \param [in] n Parameter_Description
	 *  \return Return_Description
	 *  
	 *  \details Details
	 */
	int xdmacinit( int* dstptr,  int* srcptr, int n ); // в адресах ARM
	
	/**
	 *  \brief Brief
	 *  
	 *  \return Return_Description
	 *  
	 *  \details Details
	 */
	int xdmac0(); // возвращает текущий счётчик (0 если передача окончена )
	
	
	int dmaCheck(void* src, void* dst, int size32);
	int dmaInit (void* src, void* dst, int size32, DmaCallback func);
	//void dmaInitBias(int n, unsigned src, unsigned dstq, int row2read, int bias2read, int row2wrt, int bias2wrt);
	void dmaMatrixInit(void* src, int width, int height, int srcStride,  void* dst, int dstStride );
	int  dmaStatus();

	//---------------------------------
	//int  halInitSingleDMA(void*  src,  void*  dst,  int  size32,  DmaCallback* func, int channel=0);
	//int  halInitPacketDMA(void** src,  void** dst,  int* size32,  DmaCallback* func, int channel=0);
	//int  halInitDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1, DmaCallback func, int channel=0);
	//int  halInitMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32, DmaCallback func, int channel=0);
	//int  halStatusDMA();
	
	int  halInitSingleDMA(void*  src,  void*  dst,  int  size32);
	int  halInitPacketDMA(void** src,  void** dst,  int* size32);
	int  halInitDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1);
	int  halInitMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32);
	void halSetCallbackDMA(DmaCallback user_callback);
	int  halInitDMA();
	void halEnbExtInt();
	void halDisExtInt();
	int  halStatusDMA();
	void halMaskIntContMdma_mc12101();
	// test of params
	// error code description
	////////single DMA
	// 1 stands for src is odd 
	// 2 stands for dst is odd
	// 3 stands for size is odd
	int halTestParamSingleDMA(void* src, void* dst, int size);
	// error code description
	///// double dma
	// 1 stands for src0 is odd
	// 2 stands for src1 is odd
	// 3 stands for dst0 is odd
	// 4 stands for dst1 is odd
	// 5 stands for intSize0 is odd
	// 6 stands for intSize1 is odd
	int halTestParamDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1);
	// error code descriptoin 
	///// matrix DMA
	// 1 stands for src is odd
	// 2 stands for width is odd
	// 3 stands for srcStride32 is odd
	// 4 stands for dst is odd
	// 5 stands for dstStride32 is odd
	int halTestParamMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32);

#ifdef __cplusplus
		};
#endif

