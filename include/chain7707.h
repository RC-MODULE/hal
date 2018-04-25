#ifndef _CHAIN7707_H_INCLUDED_
#define _CHAIN7707_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif


struct SpecDMA7707
{
  int *root;
  int *prev;
  int *current;
}; // SpecDMA7707
void openDma(void *buf);
int add2Chain( int armdst, int armsrc, int bytecnt);
extern SpecDMA7707 chain_7707;

#ifdef __cplusplus
};
#endif


#endif