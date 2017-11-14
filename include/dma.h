extern "C"
{
	typedef int(*DmaCallBack)();
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
	int dmaInit (void* src, void* dst, int size32, DmaCallBack* func);
	//void dmaInitBias(int n, unsigned src, unsigned dstq, int row2read, int bias2read, int row2wrt, int bias2wrt);
	void dmaMatrixInit(void* src, int width, int height, int srcStride,  void* dst, int dstStride );
	int  dmaStatus();

};
