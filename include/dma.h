#define DMA_BUSY  1
#define DMA_ERROR 2
#define DMA_READY 0 
#define DMA_OK    0 

#ifndef MAX_CHAIN_BUF_SIZE
#define MAX_CHAIN_BUF_SIZE 256
#endif 


typedef int(*DmaCallback2)();

#ifdef __cplusplus
		extern "C" {
#endif

	typedef int(*DmaCallback)();
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
	//int xdmacinit( int* dstptr,  int* srcptr, int n ); // в адресах ARM
	
	/**
	 *  \brief Brief
	 *  
	 *  \return Return_Description
	 *  
	 *  \details Details
	 */
	int xdmac0(); // возвращает текущий счетчик (0 если передача окончена )
	
	
	int dmaCheck(void* src, void* dst, int size32);
	int dmaInit (void* src, void* dst, int size32, DmaCallback func);
	//void dmaInitBias(int n, unsigned src, unsigned dstq, int row2read, int bias2read, int row2wrt, int bias2wrt);
	void dmaMatrixInit(void* src, int width, int height, int srcStride,  void* dst, int dstStride );
	int  dmaStatus();

	typedef  int (*FuncSingleCopy)(void*  src,  void*  dst,  int  size32);
	typedef  int (*FuncDoubleCopy)(void*  src0, void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1);
	typedef void (*FuncSetCallbackDMA)(DmaCallback user_callback);
	//---------------------------------
	//int  halInitSingleDMA(void*  src,  void*  dst,  int  size32,  DmaCallback* func, int channel=0);
	//int  halInitPacketDMA(void** src,  void** dst,  int* size32,  DmaCallback* func, int channel=0);
	//int  halInitDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1, DmaCallback func, int channel=0);
	//int  halInitMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32, DmaCallback func, int channel=0);
	//int  halStatusDMA();
	
	/**
	 *  \brief функция инициализирует модуль ПДП
	 *  \param [in] адрес источника
	 *  \param [in] адрес приемника
	 *  \param [in] количество 32 битных слов для пересылки
	 *	\restr 
	 *  src адрес должен быть четным в противном случае ПДП зависнет
	 *	<p>dst адрес должен быть четным в противном случае ПДП зависнет 
	 *	<p>size32 должен быть четным в противном случае ПДП скопирует на 1 меньше (ближайший наименший четный) 
	 */

	int  halInitSingleDMA(void*  src,  void*  dst,  int  size32);
	/**
	 *  \brief функция инициализирует модуль ПДП для использования без прерываний
	 *  \param [in] адрес источника
	 *  \param [in] адрес приемника
	 *  \param [in] количество 32 битных слов для пересылки
	 *	\restr 
	 *  src адрес должен быть четным в противном случае ПДП зависнет
	 *	<p>dst адрес должен быть четным в противном случае ПДП зависнет 
	 *	<p>size32 должен быть четным в противном случае ПДП скопирует на 1 меньше (ближайший наименший четный) 
	 */
	int halInitStatusSingleDMA(void*  src,  void*  dst,  int  size32);


	/**
	 *  \brief функция инициализирует модуль ПДП для записи двух массивов
	 *  \param [in] адрес первого источника
	 *  \param [in] адрес второго источника
	 *  \param [in] адрес первого приемника
	 *  \param [in] адрес второго приемника
	 *  \param [in] количество 32 битных слов для пересылки первой пары массивов
	 *  \param [in] количество 32 битных слов для пересылки второй пары массивов
	 *	\restr 
	 *	src0 адрес должен быть четным в противном случае ПДП зависнет
	 *	<p>dst0 адрес должен быть четным в противном случае ПДП зависнет  
	 *	<p>src1 адрес должен быть четным в противном случае ПДП зависнет
	 *	<p>dst1 адрес должен быть четным в противном случае ПДП зависнет  
	 *	<p>intSize0 должен быть четным в противном случае ПДП скопирует на 1 меньше (ближайший меньший четный) 
	 *	<p>intSize1 должен быть четным в противном случае ПДП скопирует на 1 меньше (ближайший меньший четный) 
	 */
	int  halInitDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1);
	/**
	 *  \brief функция инициализирует модуль ПДП для записи пакета массивов
	 *  \param [in] адрес массива, содержащего адреса источников
	 *  \param [in] адрес массива, содержащего адреса приемников
	 *  \param [in] количество 32 битных слов для пересылки первой пары массивов
	 *  \param [in] количество 32 битных слов для пересылки второй пары массивов
	 *	\restr 
	 *  src0 адрес должен быть четным в противном случае ПДП зависнет
	 *	<p>dst0 адрес должен быть четным в противном случае ПДП зависнет  
	 *	<p>src1 адрес должен быть четным в противном случае ПДП зависнет
	 *	<p>dst1 адрес должен быть четным в противном случае ПДП зависнет
	 *	<p>size32 должен быть четным в противном случае ПДП скопирует на 1 меньше (ближайший четный) 
	 */
	int  halInitPacketDMA(void** src,  void** dst,  int* size32, int  amm);
	/**
	 *  \brief функция инициализирует модуль ПДП для копирования матрицы
	 *  \param [in] адрес массива, содержащего адреса источников
	 *  \param [in] длинна строки матрицы в 32 битных словах (ширина матрицы в 32 битных словах)
	 *  \param [in] количество строк матрицы (высота матрицы)
	 *  \param [in] расстояние между соседними строками матрицы в массиве источника (src stride в 32 биьных словах)
	 *  \param [in] адрес массива приемника
	 *  \param [in] расстояние между соседними строками матрицы в массиве приемника (dst stride в 32 биьных словах)
	 *	\restr 
	 *  src адрес должен быть четным в противном случае ПДП зависнет
	 *	<p>width должен быть четным в противном случае ПДП скопирует на 1 меньше (ближайший четный) 
	 *	<p>srcStride32 должен быть четным в противном случае ПДП скопирует на 1 меньше (ближайший четный) 
	 *	<p>dst адрес должен быть четным в противном случае ПДП зависнет  
	 *	<p>dstStride32 должен быть четным в противном случае ПДП скопирует на 1 меньше (ближайший четный) 
	 *	
	 */
	int  halInitMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32);
	/**
	 *  \brief функция инициализирует модуль ПДП для копирования матрицы
	 *  \param [in] адрес массива, содержащего адреса источников
	 *  \param [in] длинна строки матрицы в 32 битных словах (ширина матрицы в 32 битных словах)
	 *  \param [in] количество строк матрицы (высота матрицы)
	 *  \param [in] расстояние между соседними строками матрицы в массиве источника (src stride в 32 биьных словах)
	 *  \param [in] адрес массива приемника
	 *  \param [in] расстояние между соседними строками матрицы в массиве приемника (dst stride в 32 биьных словах)
	 *	\restr 
	 *  Ограничения функции halInitStatusMatrixDMA включают в себя ограничения для функции halInitMatrixDMA, но кроме них есть так же другие ограничения
	 *	<p>Все параметры функции кроме height, должны быть выровнены так, что последние 4 бита = 0 (другими словами при 32-битной адресации все параметры должны быть кратны 16 32-битным словам)	
	 *	в противном случаи возможно зависание канала ПДП (особенно если dst адрес принадлежит DDR)
	 *   
	 */
	int  halInitStatusMatrixDMA(void* src, int width, int height, int srcStride32, void* dst, int dstStride32);

	/**
		* \brief Функция устанавливает функцию, которая вызовется после отработки ПДП (callBack).
		*	<p> для выключения callback можно передать в функцию 0, тогда callback не будет вызываться.
		* <p>Для работы callback необходимо разрешить внешние прерывания для процессора вызвав функцию
		* halEnbExtInt() и на плате 12101 разрешить обработку прерываний от ПДП в обработчике прерываний
		* вызвав функцию halMaskIntContMdma_mc12101().
		*
	*/
	void halSetCallbackDMA(DmaCallback user_callback);
	/**
		* \brief Функция выполняет инициализацию программы обработки прерывания и должна быть вызвана один раз перед запуском ПДП 
		* в случаи необходимости использования callBack. 
		*	<p>Данная функция анализирует регистр pswr и в случае если он запрещает внешние прерывания возвращает 1 
		* \return
		* Если внешние прерывания разрешены на ядре возвращает 0 в противном случае 1
	*/
	int  halOpenDMA();
	//int  halOpenDMA();
	/**
		* \brief Функция разрешает обработку внешних прерывания у процессорного ядра на котором она (функция) была вызвана
	*/

	void halEnbExtInt();
	/**
		* \brief Функция запрещает обработку внешних прерывания у процессорного ядра на котором она (функция) была вызвана
	*/
	void halDisExtInt();
	/**
		* \brief Функция возвращает 0 в случае если ПДП закончил работу
		* 
	*/
	int  halStatusDMA();
	/**
		* \brief Функция разрешает обработку прерывания от ПДП (MDMA) в обработчике прерывания на плате 12101 
	*/
	void halMaskIntContMdma_mc12101();
	// test of params
	// error code description
	////////single DMA
	// 1 stands for src is odd 
	// 2 stands for dst is odd
	// 3 stands for size is odd
	
	/**
		* \brief Функция возвращает код ошибки для параметров функции halInitSingleDMA() 
		* \return 
		* Возвращает код ошибки
		* <p>1 - нечетный адрес массива источника
		* <p>2 - нечетный адрес массива приемника
		* <p>3 - нечетное количество 32 битных слов для копирования
	*/
	int halCheckParamsSingleDMA(void* src, void* dst, int size);
	/**
		* \brief Функция возвращает код ошибки для параметров функции halInitDoubleDMA() 
		* \return
		* Возвращает код ошибки
		* <p>1 - нечетный адрес массива первого источника
		* <p>2 - нечетный адрес массива второго источника
		* <p>3 - нечетный адрес массива первого приемника
		* <p>4 - нечетный адрес массива второго приемника
		* <p>5 - нечетное количество 32 битных слов для копирования первой пары массивов
		* <p>6 - нечетное количество 32 битных слов для копирования первой второй массивов
	*/

	// error code description
	///// double dma
	// 1 stands for src0 is odd
	// 2 stands for src1 is odd
	// 3 stands for dst0 is odd
	// 4 stands for dst1 is odd
	// 5 stands for intSize0 is odd
	// 6 stands for intSize1 is odd
	int halCheckParamsDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1);
	/**
		* \brief Функция возвращает код ошибки для параметров функции halInitMatrixDMA() 
		* \return
		* Возвращает код ошибки
		* <p>1 - нечетный адрес массива первого источника
		* <p>2 - длина строки матрицы не четное число
		* <p>3 - нечетное расстояние между строками у матрицы источника (srcStride32)
		* <p>4 - нечетный адрес массива приемника
		* <p>5 - нечетное расстояние между строками у матрицы приемника (dstStride32)
		* <p>6 - width > srcStride32 перекрытие адресов
		* <p>7 - width > dstStride32 перекрытие адресов
	*/
	// error code descriptoin 
	///// matrix DMA
	// 1 stands for src is odd
	// 2 stands for width is odd
	// 3 stands for srcStride32 is odd
	// 4 stands for dst is odd
	// 5 stands for dstStride32 is odd
	// 6 stands for address overlap wdth more than stride
	int halCheckParamsMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32);
	/**
		* \brief Функция возвращает код ошибки для параметров функции halInitStatusMatrixDMA() 
		* \return
		* Возвращает код ошибки
		* <p>1 - не выровненный по 16 32 битных слов адрес массива источника
		* <p>2 - длина строки матрицы не выровнена
		* <p>3 - расстояние между строками у матрицы источника (srcStride32) не выровнено по 16 32-битных слов
		* <p>4 - не выровненный по 16 32 битных слов адрес массива приемника
		* <p>5 - расстояние между строками у матрицы приемника (dstStride32) не выровнено по 16 32-битных слов
		* <p>6 - width > srcStride32 перекрытие адресов
		* <p>7 - width > dstStride32 перекрытие адресов
	*/
	int halCheckParamsStausMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32);

	//error code description
	///// packet DMA
	//// function returns error code in 4 LSB and from 4th bits number of chaine were erroneous parametr was detected
 	// 1 stands for odd address of src array
 	// 2 stands for odd address of dst array
 	// 4 stands for odd size32 of array 
	/**
		* \brief Функция возвращает код ошибки для параметров функции halInitPacketDMA().
		* <p>Сам код ошибки находится непосредственно в первых 4х битах (LSB), а индекс цепочки хранится в битах начиная с 4 (нумерация битов начинается с 0)   
		* /return
		* Возвращает код ошибки
		* <p>1 - нечетный адрес массива источника
		* <p>2 - нечетный адрес массива приемника
		* <p>2 - нечетное количество 32 битных слов для копирования
		*
		*
	*/
	int  halCheckParamsPacketDMA(void** src,  void** dst,  int* size32); 
	void SetFlagDMA(int value);
	void halUnlockDMA(); 
	void halLockDMA();		
	/**
		* \brief Функция позволяет узнать занято ли DMA.
		* /return
		* 0 если не заблокировано 0xffffffff в противном случаи
	*/
	int halIsBusyDMA();
	int halGetCoreId();
	int halWhereMirror();
	void halSetMirror(); 	
	int halReadCoreID();
	
	void halEnterCriticalSection();
  void halExitCriticalSection();
  void halOpenStatusDMA();

#ifdef __cplusplus
		};
#endif

