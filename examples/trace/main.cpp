//----------------------------------------------------------------------------//
//                                                                            //
//                           MC12101 board examples                           //
//                                                                            //
//                                Demo program                                //
//                         calculate factorial of 10.                         //
//                                                                            //
//                  2016 (c) RC Module Inc., Moscow, Russia                   //
//                                                                            //
//----------------------------------------------------------------------------//

int fact(int n)
{
	if (n == 1)
		return 1;

	int res = fact(n - 1) * n;

	return res;
}
extern "C" {
	void start_trace();
	void stop_trace();
}

int main()
{
	start_trace();
	int res = 0;
	res = fact(10);
	stop_trace();
	return res;
}
