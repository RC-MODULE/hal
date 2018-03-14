#ifndef chain7707tools
#define  chain7707tools
struct chain7707
{
    int *root;
    int *prev;
    int *current;
}; // chain7707
unsigned int  first7707chain=0;
chain7707  chain;
void initChain7707(int *buf)
{  unsigned int uu;
    chain.root=buf;
    uu=(unsigned int)(chain.root);
    uu=(uu+3)&0xfffffffc;
    chain.root=(int*)uu;	
    chain.prev=chain.root; chain.current=chain.root;
}
int add2Chain( int armdst, int armsrc, int bytecnt)
{ unsigned int u;
   u=((unsigned int)chain.current)<<2; // Ёто уже армовый адрес
   u &= 0xfffffff0;   // сносим 4 младших
   u |= 1;     // и добавл€ем бит DCN "next valid chain"
   if (chain.current==chain.root) first7707chain=u; else chain.prev[5]=u;
   *chain.current++=bytecnt-1;	
   *chain.current++=armsrc;
   *chain.current++=armdst;	
   *chain.current++=0x30f00;	
   *chain.current++=0x30f00;	
   *chain.current++=0; // terminate chain	
   chain.current+=2;
   chain.prev=chain.current-8;	
 	return 0; 	
} // add2Chain
#endif
