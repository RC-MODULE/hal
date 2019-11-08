#include "memory.h"
extern "C" {

	void*  halCopyRISC(const void* src, void* dst, unsigned size32) {
		memcpy(dst, src, size32 * 4);		// Запускаем DMA с критической секцией
		return dst;
	};
};

