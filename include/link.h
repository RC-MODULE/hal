
#define LINK_OUTPUT 0
#define LINK_INPUT 1

typedef void LinkCallback();

///У процессора nm6407 у каждой процессорной системы есть по три линка(в сумме выхоидт 6). 
///На плате mc12101 наружу выходит 4 линка, LINK0 и LINK1 процессорной системы NMPU0 (Выводы X5 и X6), 
///и LINK0 и LINK1 процессорной системы NMPU1 (Выводы X7 и X8 соответсвенно).
///
///LINK2 обоих процессорных систем подсоединены друг к другу, таким образом выходит, что LINK2 является межпроцессорным линком
///
///По умолчанию после сброса LINK0 обоих процессорных систем настроен на прием данных, а LINK1 - на передачу.
///Для изменения направления передачи данных необходимо замкнуть  контакты разъемов Х12 – Х15 (X12, X13 отвечают за LINK0 и LINK1 
///процессорной системы NMPU0; X14, X15 отвечают за LINK0 и LINK1 процессорной системы NMPU1 соответственно)
///LINK2 может работать как на прием, так и на передачу

#ifdef __cplusplus
		extern "C" {
#endif

	/**
	 *  \brief Инициализация линка
	 */
	void halLinkInit();
	
	/**
	 *  \brief Запуск линка
	 *  
	 *  \param vector [in,out] Указатель на массив. Для передающего порта это массив источник, для принимающего - массив приемник
	 *  \param size32 [in] Размер в 32-разрядных словах (должно быть кратно 2)
	 *  \param port [in] Номер линка
	 */
	void halLinkStart(void* vector, int size32, int port, int direction);
	
	/**
	 *  \brief Запуск двумерного линка
	 *  
	 *  \param vector [in,out] Указатель на массив. Для передающего порта это массив источник, для принимающего - массив приемник
	 *  \param size32 [in] Размер копируемой матрицы в 32-р. словах (width*heght)  (должно быть кратно 2)
	 *  \param width [in] Ширина матрицы  в 32-р. словах  (должно быть кратно 2)
	 *  \param stride [in] Смещение между строками матрицы в 32-р. словах  (должно быть кратно 2)
	 *  \param port [in] Номер линка
	 */
	void halLinkStart2D(void* vector, int size32, int width, int stride, int port, int direction);
	
	/**
	 *  \brief Проверка статуса завершения линка
	 *  
	 *  \param port [in] Номер линка
	 *  \retval Возвращает 1 если передача или прием завершен и 0, если нет
	 */
	int halLinkIsCompleted(int port, int direction);
	
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
	void halLinkSetCallback(LinkCallback callback, int port, int direction);
	
	/**
	 * \brief      { function_description }
	 *
	 * \param[in]  port       The port
	 * \param[in]  direction  The direction
	 *
	 * \return     { description_of_the_return_value }
	 * \details Для передающей части
	 * 			01h – Idle – бездействие,
				02h – ReceiveWrite – приём и запись данных,
				04h – UncompleteWrite – приём завершён, только запись,
				08h – Complete – запись завершена,
				1Ch – DataMiss – режим очистки буфера данных.

				Для принимающей части
				01h – Idle – бездействие,
				02h – ReceiveWrite – приём и запись данных,
				04h – UncompleteWrite – приём завершён, только запись,
				08h – Complete – запись завершена,
				1Ch – DataMiss – режим очистки буфера данных.
	 */
	int halLinkGetState(int port, int direction);




	/**
	 * \brief      Счётчик активных запросов для 
	 *
	 * \param[in]  port       The port
	 * \param[in]  direction  The direction
	 *
	 * \return     { description_of_the_return_value }
	 */
	int halLinkGetActiveDataCounter(int port, int direction);

#ifdef __cplusplus
		};
#endif

