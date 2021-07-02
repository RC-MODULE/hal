#define DMA_BUSY  1
#define DMA_ERROR 2
#define DMA_READY 0 
#define DMA_OK    0 
#define DMA_MAX_SIZE    131070		///максимальный размер пересылаемых данных в 32-разрядных словах на mc12101

#ifndef MAX_CHAIN_BUF_SIZE
#define MAX_CHAIN_BUF_SIZE 256
#endif 


typedef int(*DmaCallback2)();

#ifdef __cplusplus
		extern "C" {
#endif

	typedef int(*DmaCallback)();
	
	extern DmaCallback halDmaUserCallbackPtr;
	
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

	typedef  int (*FuncSingleCopy)(const void*  src,  void*  dst,  int  size32);
	typedef  int (*FuncDoubleCopy)(const void*  src0, const void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1);
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
	 *	<p>size32 должен быть четным в противном случае ПДП скопирует на 1 меньше (ближайший наименьший четный) 
	 */

	int  halInitSingleDMA(const void*  src,  void*  dst,  int  size32);
	/**
	 *  \brief функция инициализирует модуль ПДП для использования без прерываний
	 *  \param [in] адрес источника
	 *  \param [in] адрес приемника
	 *  \param [in] количество 32 битных слов для пересылки
	 *	\restr 
	 *  src адрес должен быть четным в противном случае ПДП зависнет
	 *	<p>dst адрес должен быть четным в противном случае ПДП зависнет 
	 *	<p>size32 должен быть четным в противном случае ПДП скопирует на 1 меньше (ближайший наименьший четный) 
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
	 *  \param [in] длина строки матрицы в 32 битных словах (ширина матрицы в 32 битных словах)
	 *  \param [in] количество строк матрицы (высота матрицы)
	 *  \param [in] расстояние между соседними строками матрицы в массиве источника (src stride в 32 битных словах)
	 *  \param [in] адрес массива приемника
	 *  \param [in] расстояние между соседними строками матрицы в массиве приемника (dst stride в 32 битных словах)
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
	 *  \param [in] длина строки матрицы в 32 битных словах (ширина матрицы в 32 битных словах)
	 *  \param [in] количество строк матрицы (высота матрицы)
	 *  \param [in] расстояние между соседними строками матрицы в массиве источника (src stride в 32 битных словах)
	 *  \param [in] адрес массива приемника
	 *  \param [in] расстояние между соседними строками матрицы в массиве приемника (dst stride в 32 битных словах)
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
	void halDmaSetCallback(DmaCallback user_callback);
	void halSetDummyCallbackDMA();
	DmaCallback halGetCallbackDMA();
	DmaCallback halDmaGetCallback();
	/**
		* \brief Функция выполняет первоначальную инициализацию ПДП для работы с callBack функцией и вызывается 1 раз. 
	*/
	int  halOpenDMA();
	int  halDmaInit();
	int  halDmaInitC();
	/**
		* \brief Функция выполняет первоначальную инициализацию ПДП для работы без callBack функции и вызывается 1 раз.
		* Если до этого была вызвана функция halOpenDMA, то вызывать функцию halOpenStatusDMA не нужно
	*/
	void halOpenStatusDMA();
	
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
	/**
		* \brief Функция проверяет корректность параметров для функции halInitSingleDMA() 
		* \return 
		* Возвращает код ошибки
		* <p>0 - параметры корректны
		* <p>1 - нечетный адрес массива источника
		* <p>2 - нечетный адрес массива приемника
		* <p>3 - нечетное количество 32 битных слов для копирования
	*/
	int halCheckParamsSingleDMA(void* src, void* dst, int size);
	/**
		* \brief Функция проверяет корректность параметров для функции halInitDoubleDMA() 
		* \return
		* Возвращает код ошибки
		* <p>0 - параметры корректны
		* <p>1 - нечетный адрес массива первого источника
		* <p>2 - нечетный адрес массива второго источника
		* <p>3 - нечетный адрес массива первого приемника
		* <p>4 - нечетный адрес массива второго приемника
		* <p>5 - нечетное количество 32 битных слов для копирования первой пары массивов
		* <p>6 - нечетное количество 32 битных слов для копирования первой второй массивов
	*/

	int halCheckParamsDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1);
	/**
		* \brief Функция проверяет корректность параметров для функции halInitMatrixDMA() 
		* \return
		* Возвращает код ошибки
		* <p>0 - параметры корректны
		* <p>1 - нечетный адрес массива первого источника
		* <p>2 - длина строки матрицы не четное число
		* <p>3 - нечетное расстояние между строками у матрицы источника (srcStride32)
		* <p>4 - нечетный адрес массива приемника
		* <p>5 - нечетное расстояние между строками у матрицы приемника (dstStride32)
		* <p>6 - width > srcStride32 перекрытие адресов
		* <p>7 - width > dstStride32 перекрытие адресов
	*/
	int halCheckParamsMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32);
	/**
		* \brief Функция проверяет корректность параметров для функции halInitStatusMatrixDMA() 
		* \return
		* Возвращает код ошибки
		* <p>0 - параметры корректны
		* <p>1 - невыровненный по 16 32 битных слов адрес массива источника
		* <p>2 - длина строки матрицы не выровнена
		* <p>3 - расстояние между строками у матрицы источника (srcStride32) не выровнено по 16 32-битных слов
		* <p>4 - невыровненный по 16 32 битных слов адрес массива приемника
		* <p>5 - расстояние между строками у матрицы приемника (dstStride32) не выровнено по 16 32-битных слов
		* <p>6 - width > srcStride32 перекрытие адресов
		* <p>7 - width > dstStride32 перекрытие адресов
	*/
	int halCheckParamsStausMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32);
	/**
		* \brief Функция проверяет корректность параметров для функции halInitPacketDMA().
		* <p>Сам код ошибки находится непосредственно в первых 4х битах (LSB), а индекс цепочки хранится в битах начиная с 4 (нумерация битов начинается с 0)   
		* /return
		* Возвращает код ошибки
		* <p>0 - параметры корректны
		* <p>1 - нечетный адрес массива источника
		* <p>2 - нечетный адрес массива приемника
		* <p>2 - нечетное количество 32 битных слов для копирования
		*
		*
	*/
	int  halCheckParamsPacketDMA(void** src,  void** dst,  int* size32, int amm);
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
	/**
		* \brief Функция позволяет узнать смещение по адресам памяти для DMA.
		* /return
		* Memory offset
	*/ 	
	int halWhereMirror();
	void halSetMirror();
	/**
		* \brief Функция позволяет узнать номер ядра.
		* /return
		* CoreID
	*/ 	
	int halReadCoreID();
	
	void halEnterCriticalSection();
	void halExitCriticalSection();
	
	void halEnterCriticalSection0();
	void halExitCriticalSection0();
	void halEnterCriticalSection1();
	void halExitCriticalSection1();


	/**
	 *  \brief Запуск DMA 
	 *  
	 *  \param [in] src Массив источник
	 *  \param [in] dst Массив Приемник
	 *  \param [in] size32 Размер в 32-р. словах
	 *  
	 *  \details Назначение суффиксов: 
	 [C]ritical - запуск DMA с использованием критических секций. Необходимо при запуске DMA сразу с нескольких ядер. Использование критических секций замедляет запуск примерно на 100 тактов.
	 Инициализация DMA должна производится функций halDmaInitC также с суффиксом C.
	 [M]apped - предполагается, что адреса аргументов для внутренней памяти подаются  отмапированными в зеркало памяти (в память свыше 0x40000). Если [M] отсутствует, то
	 внутри функции производится проверка на принадлежность адресов массивов к внутреннему адресному пространству, вызывающего ядра и производится мапирование в глобальное адресное пространство в случае необходимости.
	 Вызов DMA по адресам  локальной памяти приведет к ошибке.
	 [A]ligned - предполагается, что адреса src и dst выровнены по границе 16-ти 32.р слов (4 младших бита адреса равны нулю). 
	 Проверка выровненности адресов не производится и DMА всегда запускается в одномерном режиме с пакетными запросами. Вызов DMA в этом режиме по невыровненным адресам - приведет к ошибке. 
	 Если [А] отсутствует - производится проверка на выровненность аргументов. 
	 В случае невыровненности хотя бы одного аргумента DMA запускается в двумерном режиме с одиночными запросами. Скорость DMA в таком режиме ниже.
	 
	 Для запуска DMA требуется однократная инициализация halDmaInit или halDmaInitС при работе с критическими секциями . 
	 */
	//! \{
	void halDmaStart   		(const void* src, void* dst, unsigned size32);
	void halDmaStartA  		(const void* src, void* dst, unsigned size32);
	void halDmaStartM  		(const void* src, void* dst, unsigned size32);
	void halDmaStartMA 		(const void* src, void* dst, unsigned size32);
	void halDmaStartC  		(const void* src, void* dst, unsigned size32);
	void halDmaStartCA 		(const void* src, void* dst, unsigned size32);
	void halDmaStartCM 		(const void* src, void* dst, unsigned size32);
	void halDmaStartCMA		(const void* src, void* dst, unsigned size32);
	
	//! \}	
	typedef void (*DmaStart)(const void* src, void* dst, unsigned size32);
	/**
	 *  \brief Запуск DMA 
	 *  
	 *  \param [in] src Массив источник (должно быть кратно 2)
	 *  \param [in] dst Массив Приемник (должно быть кратно 2)
	 *  \param [in] size32 Размер копируемой матрицы в 32-р. словах (width*heght)  (должно быть кратно 2)
	 *  \param [in] width Ширина матрицы  в 32-р. словах  (должно быть кратно 2)
	 *  \param [in] srcStride32 Смещение между строками входной матрицы в 32-р. словах  (должно быть кратно 2)
	 *  \param [in] dstStride32 Смещение между строками выходной матрицы в 32-р. словах  (должно быть кратно 2)
	 *  
	 *  \details Назначение суффиксов: 
	 [C]ritical - запуск DMA с использованием критических секций. Необходимо при запуске DMA сразу с нескольких ядер. Использование критических секций замедляет запуск примерно на 100 тактов.
	 Инициализация DMA должна производится функций halDmaInitC также с суффиксом C.
	 [M]apped - предполагается, что адреса аргументов для внутренней памяти подаются  отмапированными в зеркало памяти (в память свыше 0x40000). Если [M] отсутствует, то
	 внутри функции производится проверка на принадлежность адресов массивов к внутреннему адресному пространству, вызывающего ядра и производится мапирование в глобальное адресное пространство в случае необходимости.
	 Вызов DMA по адресам  локальной памяти приведет к ошибке.
	 [A]ligned - предполагается, что адреса src и dst  выровнены по границе 16-ти 32-р. слов (4 младших бита адреса равны нулю) и все аргументы кратны 16. 
	 Проверка аргументов на кратность и выровненность не производится и DMA запускается сразу в двухмерном режиме с пакетными запросами (самый быстрый). 
	 Если [А] отсутствует - производится проверка на выровненность аргументов. Если хотя бы один адрес не выровнен или аргумент  не кратен 16,
	 то запуск DMA производится серией одиночных запросов, что снижает скорость передачи .  

	 Для запуска DMA требуется однократная инициализация halDmaInit или halDmaInitС при работе с критическими секциями . 
	 */
	//! \{
	
	void halDma2D_Start   	(const void* src, void* dst, unsigned size32, unsigned width, unsigned srcStride32, unsigned dstStride32);
	void halDma2D_StartA  	(const void* src, void* dst, unsigned size32, unsigned width, unsigned srcStride32, unsigned dstStride32);
	void halDma2D_StartM  	(const void* src, void* dst, unsigned size32, unsigned width, unsigned srcStride32, unsigned dstStride32);
	void halDma2D_StartMA 	(const void* src, void* dst, unsigned size32, unsigned width, unsigned srcStride32, unsigned dstStride32);
	void halDma2D_StartC  	(const void* src, void* dst, unsigned size32, unsigned width, unsigned srcStride32, unsigned dstStride32);
	void halDma2D_StartCA 	(const void* src, void* dst, unsigned size32, unsigned width, unsigned srcStride32, unsigned dstStride32);
	void halDma2D_StartCM 	(const void* src, void* dst, unsigned size32, unsigned width, unsigned srcStride32, unsigned dstStride32);
	void halDma2D_StartCMA	(const void* src, void* dst, unsigned size32, unsigned width, unsigned srcStride32, unsigned dstStride32);
//! \}	
	
	
	
	/**
	 *  \brief Проверка статуса завершения DMA 
	 *  
	 *  \return  возвращает 1 если передача завершена и 0 если нет.
	 *  
	 */
	//! \{
	int halDma2D_IsCompleted();
	int halDmaIsCompleted();
//! \}		
	
	

#ifdef __cplusplus
		};
#endif

