struct SpecDMA7707
{
  int *root;
  int *prev;
  int *current;
}; // SpecDMA7707
void openDma(void *buf);
int add2Chain( int armdst, int armsrc, int bytecnt);
extern SpecDMA7707 chain_7707;
