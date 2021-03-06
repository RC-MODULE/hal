#define DMA_BUSY  1
#define DMA_ERROR 2
#define DMA_READY 0 
#define DMA_OK    0 

#ifndef MAX_CHAIN_BUF_SIZE
#define MAX_CHAIN_BUF_SIZE 256
#endif 

#define LINK_OUTPUT 0
#define LINK_INPUT 1

typedef void LinkCallback();

///У процессора nm6407 у каждого ядра (NMPU0 и NMPU1) есть по три линка. На плате mc12101 наружу выходит 4 линка, LINK0 и LINK1 
///процессорной системы NMPU0 (Выводы X5, X6), и LINK0 и LINK1 процессорной системы NMPU1 (Выводы X7, X8 соответсвенно).
///
///LINK2 является межпроцессорным линком и связывает ядра между собой.
///
///По умолчанию после сброса LINK0 обоих процессорных систем настроен на прием данных, а LINK1 - на передачу.
///Для изменения настройки линков по направлению необходимо замкнуть  контакты разъемов Х12 – Х15 (X12, X13 отвечают за LINK0 и LINK1 
///процессорной системы NMPU0; X14, X15 отвечают за LINK0 и LINK1 процессорной системы NMPU1 соответственно)
///LINK2 может работать как на прием, так и на передачу

#ifdef __cplusplus
		extern "C" {
#endif

	/**
	 *  \brief Инициализация линка
	 *  
	 *  \param port [in] Номер линка (Для платы mc12101 принимает значения 0..2)
	 *  \param direction [in] Указание направления порта(0 - если порт передающий, 1 - если порт принимающий)
	 *  
	 *  Для платы mc12101 направление линков не устанавливается а лишь указывается. Для изменения направления настройки по направлению,
	 *  необходимо замкнуть соответствующие контакты (см.выше). Не касается LINK2
	 */
	void halLinkInit(int port, int direction);
	
	/**
	 *  \brief Запуск линка
	 *  
	 *  \param vector [in,out] Указатель на массив. Для передающего порта это массив источник, для принимающего - массив приемник
	 *  \param size32 [in] Размер в 32-разрядных словах (должно быть кратно 2)
	 *  \param port [in] Номер линка
	 */
	void halLinkStart(void* vector, int size32, int port);
	
	/**
	 *  \brief Запуск двумерного линка
	 *  
	 *  \param vector [in,out] Указатель на массив. Для передающего порта это массив источник, для принимающего - массив приемник
	 *  \param size32 [in] Размер копируемой матрицы в 32-р. словах (width*heght)  (должно быть кратно 2)
	 *  \param width [in] Ширина матрицы  в 32-р. словах  (должно быть кратно 2)
	 *  \param stride [in] Смещение между строками матрицы в 32-р. словах  (должно быть кратно 2)
	 *  \param port [in] Номер линка
	 */
	void halLinkStart2D(void* vector, int size32, int width, int stride, int port);
	
	/**
	 *  \brief Проверка статуса завершения линка
	 *  
	 *  \param port [in] Номер линка
	 *  \retval Возвращает 1 если передача или прием завершен и 0, если нет
	 */
	int halLinkIsCompleted(int port);
	
	/**
	 * \brief Функция устанавливает функцию, которая вызовется после отработки ПДП (callBack).
	 *	<p> для выключения callback можно передать в функцию 0, тогда callback не будет вызываться.
	 *  callback устанавливается один на линк
	 *  
	 *  \param callback [in] Функция
	 *  \param port [in] Номер линка
	 *  \retval Return description
	 *  
	 */
	void halLinkSetCallback(LinkCallback callback, int port);
	

#ifdef __cplusplus
		};
#endif

