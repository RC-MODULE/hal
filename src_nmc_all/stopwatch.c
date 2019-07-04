#include "nmprofiler.h"
extern "C"{
volatile ProfilerData *pHeadStopwatchProfile=0; // указатель на первый профайл в списке
volatile ProfilerData *pTailStopwatchProfile=0; // указатель на последний профайл в списке
};
