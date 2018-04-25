#include "chain7707.h"
SpecDMA7707 chain_7707;
void openDma(void *buf)
{  unsigned int uu;
    chain_7707.root=(int*)buf;
    uu=(unsigned int)(chain_7707.root);
    uu=(uu+3)&0xfffffffc;
    chain_7707.root=(int*)uu;	
    chain_7707.prev=chain_7707.root; chain_7707.current=chain_7707.root;
}
int add2Chain( int armdst, int armsrc, int bytecnt)
{ unsigned int u;//chain_7707.current ����� � ����� ������� ����������
   u=((unsigned int)chain_7707.current)<<2; // ��� ��� ������� �����
   u &= 0xfffffff0;   // ������ 4 �������
   u |= 1;     // � ��������� ��� DCN "next valid chain"
   if (chain_7707.current-chain_7707.root) chain_7707.prev[5]=u;
     chain_7707.prev=chain_7707.current;	
   *chain_7707.current++=bytecnt-1;	
   *chain_7707.current++=armsrc;
   *chain_7707.current++=armdst;	
   *chain_7707.current++=0x30f00;	
   *chain_7707.current++=0x30f00;	
   *chain_7707.current++=0; // terminate chain	
   *chain_7707.current++=0;  // non using
   *chain_7707.current++=0;  // non using
 	return 0; 	
} // add2Chain
