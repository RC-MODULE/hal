extern "C"{
int ubcOpen(char* absfile=0, ...);
//int ubcGetShared(unsigned* addr,int *sh);
int ubcSync(int val, unsigned processor=0);
int ubcReadMemBlock (unsigned* dstHostAddr, unsigned srcBoardAddr, unsigned size32, unsigned processor=0);
int ubcWriteMemBlock(unsigned* srcHostAddr, unsigned dstBoardAddr, unsigned size32, unsigned processor=0);
int ubcGetResult(unsigned* result,  unsigned processor=0);
int ubcClose();
};