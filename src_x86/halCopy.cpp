#include <memory.h>
extern "C"{
	void*  halCopyRISC(const void* src,  void* dst,  unsigned size32){
		memcpy(dst,src,size32*4);
		return dst;
	}
	void*  halCopyDMA(const void* src,  void* dst,  unsigned size32){
		memcpy(dst,src,size32*4);
		return dst;
	}
	void*  halCopyInt(const void* src,  void* dst,  unsigned size32){
		memcpy(dst,src,size32*4);
		return dst;
	}
	void*  halCopyFlt(const void* src,  void* dst,  unsigned size32){
		memcpy(dst,src,size32*4);
		return dst;
	}
};