

#ifdef __cplusplus
    extern "C" {
#endif
unsigned halCrcAcc_32u(const unsigned int* pSrcVec, int nSize, unsigned int* crcAccumulator);

unsigned int halCrc_32u(
    const unsigned int* pSrcVec, 	// Input Vector			:long Any[Size/2]
    int nSize             	// Vector size        	:[0,1,2...]
    )
{
	unsigned crc=0xCCCCCCCC;
	halCrcAcc_32u(pSrcVec,nSize,&crc);
	return crc;
}

#ifdef __cplusplus
    };
#endif
