typedef int(*cnt_func)();
typedef void(*bell_func)();
extern "C"
{ 

    void SetCallBack( int vectoraddr, int ProcAddress);
	cnt_func dmainit( int dmahigh,  int *dstptr,  int *srcptr, int n );
	cnt_func halDmaStart( int *q );
	int dmac0();
	int dmac1();
	extern cnt_func statusFunc;
	void dmaopen5103();
//int halInitSingleDMA(void*  src,  void*  dst,  int  size32,
    //bell_func DmaCallback, int channel);
//int  halInitDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1,
//     int intSize0, int intSize1);
//int  halInitPacketDMA(void** src,  void** dst,  int* size32);
//int  halInitMatrixDMA(void*  src,  int  width,  int  height, int srcStride32,
 //    void* dst, int dstStride32, bell_func DmaCallback, int channel);
	void BellFromDmaMatrix();
	void BellFromDmaPacket();

}
