extern "C"
{
	/**
	 *  \brief Brief
	 *  
	 *  \param [in] dstptr Parameter_Description
	 *  \param [in] srcptr Parameter_Description
	 *  \param [in] n Parameter_Description
	 *  \return Return_Description
	 *  
	 *  \details Details
	 */
	int xdmacinit( int* dstptr,  int* srcptr, int n ); // в адресах ARM
	
	/**
	 *  \brief Brief
	 *  
	 *  \return Return_Description
	 *  
	 *  \details Details
	 */
	int xdmac0(); // возвращает текущий счётчик (0 если передача окончена )

};


//#define dmah 0
typedef int(*cnt_func)();
extern "C"
{ 
	/**
	 *  \brief Brief
	 *  
	 *  \param [in] vectoraddr Parameter_Description
	 *  \param [in] ProcAddress Parameter_Description
	 *  \return Return_Description
	 *  
	 *  \details Details
	 */
	 void SetCallBack( int vectoraddr, int ProcAddress);
	
	/**
	 *  \brief Brief
	 *  
	 *  \param [in] dmahigh Parameter_Description
	 *  \param [in] dstptr Parameter_Description
	 *  \param [in] srcptr Parameter_Description
	 *  \param [in] n Parameter_Description
	 *  \return Return_Description
	 *  
	 *  \details Details
	 */
	 cnt_func dmainit( int dmahigh,  int *dstptr,  int *srcptr, int n );
	
	
	/**
	 *  \brief Brief
	 *  
	 *  \return Return_Description
	 *  
	 *  \details Details
	 */
	 int dmac0(); // возвращает текущий счётчик (0 если передача окончена )
};