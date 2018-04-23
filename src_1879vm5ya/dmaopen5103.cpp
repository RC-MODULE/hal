extern "C"{
int nonfirst, chain[10], matrixcnt, packetcnt;
int ofdmainit;
void dmaopen5103()
{ nonfirst=0; chain[9]=0;
  matrixcnt=0;
  packetcnt=-1;
  ofdmainit=0;
}

}