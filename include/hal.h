
#define nm64u unsigned __int64
#define __int64 __int64
#define nm32u unsigned int
#define nm32s int

    /**
    \defgroup HAL_Fill HAL_Fill
    \ingroup vInit
    \brief
        \ru Функция инициализации элементов массива постоянным значением. 
        \en Function of array initialization with a constant value.  
    
		\~
    
	\f[
        pVec[i]  =  nVal, 
    \f]
    
	\f[
        i = \overline{0 \ldots nSize-1}
    \f]
    
    \param nSize    
        \ru Размер вектора в элементах. 
        \en Size of vec in elements. 
		\~
    \param nVal     
        \ru Константа. Диапазон значений nVal должен соответсвовать типу результирующего вектора. 
        \en A constant. Range of nVal must correspond to type of result vec
		\~
    \retval pVec    
        \ru Результирующий вектор. 
        \en The result vec. 
		\~
    \return \e void 
     
        
    \par
    \xmlonly
        <testperf>
	        <param> pVec </param> <values> L G </values>
	        <param> nVal </param> <values> n </values>
	        <param> nSize  </param> <values> 10240 </values>
        </testperf>
        <testperf>
	        <param> pVec </param> <values> G </values>
	        <param> nVal </param> <values> n </values>
	        <param> nSize  </param> <values> 8 128 1024 10240 </values>
        </testperf>
    \endxmlonly
    */
    //! \{
void HAL_Fill32(void* pVec, int nVal, int nSize);



    /**
    \defgroup HAL_Rand HAL_Rand
    \ingroup vInit
    \brief
        \ru Инициализация массива случайными числами. 
        \en Initialization of an array with random numbers. 
		\~
    \param nSize		
        \ru Размер вектора. 
        \en Vector size. 
		\~
    \param nRandomize    
        \ru Произвольное число для инициализации 
                            генератора случайных чисел. 
        \en Arbitrary number for initialization 
                            of a random-number generator. 
		\~
    \retval pDstVec     
        \ru Результирующий вектор. 
        \en Result vec. 
		\~
    \return \e void
    
    \par
    \xmlonly
        <testperf> 
             <param> pDstVec </param> <values> L G </values>
             <param> nRandomize </param> <values> 1 </values>
             <param> nSize </param> <values> 10240 </values>
        </testperf>
        <testperf> 
             <param> pDstVec </param> <values> G </values>
             <param> nRandomize </param> <values> 1 </values>
             <param> nSize </param> <values> 8 128 1024 10240 </values>
        </testperf>
    \endxmlonly
    */
    //! \{
/**
	/ru  Инициализация массива 32-разрядными случайными числами. 
	/en Random initialization of 32-bit buffer
	/~
*/
void HAL_Rand64(__int64* pDstVec, int nSize, int nRandomize = 1);


//*****************************************************************************

    /**
    \defgroup HAL_Copy HAL_Copy
    \ingroup vInit
    \brief
        \ru Копирование вектора. 
        \en Vector copying. 
    
		\~
    
	\f[
        pDstVec[i]  =  pSrcVec[i],
    \f]
    
	\f[
        i = \overline{0 \ldots nSize-1}
    \f]
    
    \param pSrcVec  
        \ru Входной вектор. 
        \en Input vec. 
		\~
    \param nSize    
        \ru Размер векторов в элементах. 
        \en Vector size in elements. 
		\~
    \retval pDstVec 
        \ru Результирующий вектор. 
        \en The result vec. 
		\~
    \return \e void
    

    
        */
    //! \{
void HAL_Copy32( void* pSrcVec, void* pDstVec, int nSize);
    //! \}
	
	
 /**
	\internal
    \defgroup HAL_Crc HAL_Crc
    \ingroup vStat
		\brief 
		    \ru Подсчет циклического кода.
		    \en Cyclic code calculation.

		\~
	  \~
		\param pSrcVec
			\ru Входной вектор.
			\en Input vec.
		\~
		\param nSize
			\ru Размер входного вектора.
			\en Input vec size.

		\~
		\retval nCrc
			\ru Циклический код.
			\en Cyclic code.

		\~
    \par
    \xmlonly
				<testperf>
          <param> pSrcVec </param> <values> L G 		</values>
          <param> nCrc  </param> <values> nn 		</values>
          <param> nSize     </param> <values> 10240 	</values>
				</testperf>
				<testperf>
          <param> pSrcVec </param> <values> G 		</values>
          <param> nCrc  </param> <values> nn 		</values>
          <param> nSize     </param> <values> 8 128 1024 10240 	</values>
				</testperf>

    \endxmlonly
    */
    //! \{
void halCrc32(void* pSrcVec, int nSize, unsigned int* nCrc);
	
