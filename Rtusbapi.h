//---------------------------------------------------------------------------
#ifndef __RTUSBAPIH__
#define __RTUSBAPIH__
//---------------------------------------------------------------------------
	#include <windows.h>

	// версия библиотеки
	#define 	VERMAJOR_RTUSBAPI 				(0x1)   	// только одна цифра
	#define 	VERMINOR_RTUSBAPI 				(0x5)		// только одна цифра
	#define 	CURRENT_VERSION_RTUSBAPI     	((VERMAJOR_RTUSBAPI << 0x10) | VERMINOR_RTUSBAPI)

	// вместо ReleaseDevice будем использовать ReleaseInstance
	#define ReleaseDevice ReleaseInstance

	// дальше только для C++
	#ifdef __cplusplus

	// экспортируемые функции
	extern "C" DWORD WINAPI RtGetDllVersion(void);
	extern "C" LPVOID WINAPI RtCreateInstance(PCHAR const DeviceName);

	// USB интерфейс для модулей от R-Technology
	struct IRTUSB
	{
		virtual BOOL WINAPI OpenDevice(WORD VirtualSlot) = 0;
		virtual BOOL WINAPI CloseDevice(void) = 0;
		virtual HANDLE WINAPI GetModuleHandle(void) = 0;
		virtual BOOL WINAPI GetUsbSpeed(BYTE * const UsbSpeed) = 0;
		virtual BOOL WINAPI GetModuleName(PCHAR const ModuleName) = 0;
		virtual BOOL WINAPI GetModuleSerialNumber(PCHAR const SerialNumber) = 0;
		virtual BOOL WINAPI GetAvrVersion(PCHAR const AvrVersion) = 0;
		virtual BOOL WINAPI ReleaseInstance(void) = 0;
	};

	// USB и DSP интерфейс для модулей от R-Technology
	template <class Type>
	struct IRTUSBDSP
	{
		// функции для работы с USB интерфейсом модуля
		virtual BOOL WINAPI OpenDevice(WORD VirtualSlot) = 0;
		virtual BOOL WINAPI CloseDevice(void) = 0;
		virtual HANDLE WINAPI GetModuleHandle(void) = 0;
		virtual BOOL WINAPI GetUsbSpeed(BYTE * const UsbSpeed) = 0;
		virtual BOOL WINAPI GetModuleName(PCHAR const ModuleName) = 0;
		virtual BOOL WINAPI GetModuleSerialNumber(PCHAR const SerialNumber) = 0;
		virtual BOOL WINAPI GetAvrVersion(PCHAR const AvrVersion) = 0;
		virtual BOOL WINAPI ReleaseInstance(void) = 0;

		// функции для работы с DSP модуля
		virtual BOOL WINAPI RESET_DSP(void) = 0;
		virtual BOOL WINAPI LOAD_DSP(PCHAR const FileName = NULL) = 0;
		virtual BOOL WINAPI MODULE_TEST(void) = 0;
		virtual BOOL WINAPI GET_DSP_INFO(Type * const DspInfo) = 0;
		virtual BOOL WINAPI SEND_COMMAND(WORD Command) = 0;

		// функции для работы с памятью DSP модуля
		virtual BOOL WINAPI PUT_VAR_WORD(WORD Address, SHORT Data) = 0;
		virtual BOOL WINAPI GET_VAR_WORD(WORD Address, SHORT * const Data) = 0;
		virtual BOOL WINAPI PUT_DM_WORD(WORD Address, SHORT Data) = 0;
		virtual BOOL WINAPI GET_DM_WORD(WORD Address, SHORT * const Data) = 0;
		virtual BOOL WINAPI PUT_PM_WORD(WORD Address, LONG Data) = 0;
		virtual BOOL WINAPI GET_PM_WORD(WORD Address, LONG * const Data) = 0;
		virtual BOOL WINAPI PUT_DM_ARRAY(WORD BaseAddress, WORD NPoints, SHORT * const Data) = 0;
		virtual BOOL WINAPI GET_DM_ARRAY(WORD BaseAddress, WORD NPoints, SHORT * const Data) = 0;
		virtual BOOL WINAPI PUT_PM_ARRAY(WORD BaseAddress, WORD NPoints, LONG * const Data) = 0;
		virtual BOOL WINAPI GET_PM_ARRAY(WORD BaseAddress, WORD NPoints, LONG * const Data) = 0;
	};

	// интерфейс для работы в режиме ввода данных на модулях от R-Technology
	template <class Type>
	struct IRTREAD
	{
		virtual BOOL WINAPI GET_INPUT_PARS(Type * const ap) = 0;
		virtual BOOL WINAPI SET_INPUT_PARS(Type * const ap) = 0;
		virtual BOOL WINAPI START_READ(void) = 0;
		virtual BOOL WINAPI STOP_READ(void) = 0;
		virtual BOOL WINAPI READ_KADR(SHORT * const Data) = 0;
		virtual BOOL WINAPI READ_SAMPLE(WORD Channel, SHORT * const Sample) = 0;
		virtual BOOL WINAPI ReadData(SHORT * const lpBuffer, DWORD * const nNumberOfWordsToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped) = 0;
	};

	// интерфейс для работы в режиме вывода данных на модулях от R-Technology
	template <class Type>
	struct IRTWRITE
	{
		virtual BOOL WINAPI GET_OUTPUT_PARS(Type * const dp) = 0;
		virtual BOOL WINAPI SET_OUTPUT_PARS(Type * const dp) = 0;
		virtual BOOL WINAPI START_WRITE(void) = 0;
		virtual BOOL WINAPI STOP_WRITE(void) = 0;
		virtual BOOL WINAPI WRITE_SAMPLE(WORD Channel, SHORT * const Sample) = 0;
		virtual BOOL WINAPI WriteData(SHORT * const lpBuffer, DWORD * const nNumberOfWordsToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) = 0;
	};

	// интерфейс для работы с областью пользовательского ППЗУ на модулях от R-Technology
	template <class Type>
	struct IRTFLASH
	{
		virtual BOOL WINAPI ENABLE_FLASH_WRITE(BOOL EnableFlashWrite) = 0;
		virtual BOOL WINAPI PUT_FLASH(Type * const fi) = 0;
		virtual BOOL WINAPI GET_FLASH(Type * const fi) = 0;
	};

	// интерфейс для работы с ТТЛ линиями на модулях от R-Technology
	struct IRTTTL
	{
		virtual BOOL WINAPI ENABLE_TTL_OUT(BOOL EnabledTtlOut) = 0;
		virtual BOOL WINAPI TTL_OUT(WORD TtlOut) = 0;
		virtual BOOL WINAPI TTL_IN(WORD * const TtlIn) = 0;
	};

	//==============================================================================
	//  О Б Н А Р У Ж Е Н И Е   U S B - М О Д У Л Е Й  от  R - T E C H N O L O G Y
	//==============================================================================
	// интерфейс для обнаружения (мониоринга) USB-модулей от R-Technology
	struct IRTUSBDETECT :	IRTUSB
	{
		// функция выдачи строки с последней ошибкой
		virtual int WINAPI GetLastErrorString(LPTSTR const lpBuffer, DWORD nSize) = 0;
	};




	//==============================================================================
	//                       М О Д У Л Ь       U S B - 2 1 8 5
	//==============================================================================

	// пространство имён для работы с модулем USB-2185
	namespace RTUSB2185
	{
		// адрес начала сегмента блока данных в памяти программ DSP
		const WORD VarsBaseAddress = 0x30;
		// тактовая частота работы DSP в кГц
		const DWORD DSP_CLOCK_OUT = 72000;

		// номера доступных пользовательских запросов для USB (vendor request)
		enum { 	V_RESET_DSP,
					V_PUT_ARRAY, V_GET_ARRAY,
					V_START_READ, V_START_WRITE,
					V_COMMAND_IRQ,
					V_GET_USB_SPEED,
					V_PUT_FLASH = 8, V_GET_FLASH = 9,
					V_GET_MODULE_NAME = 11,
					V_GET_MODULE_SERIAL_NUMBER = 13
			};

		// номера доступных команд штатного драйвера DSP
		enum { 	C_TEST,
					C_START_READ, C_STOP_READ,
					C_START_WRITE, C_STOP_WRITE,
					C_LAST_COMMAND
				};

		// возможные индексы скорости работы шины USB
		enum {	USB11, USB20, INVALID_USB_SPEED };

		#pragma pack(1)
		// структура пользовательского ППЗУ
		struct FLASH
		{
			BYTE FlashBytes[256];
		};

		// структура, содержащая информацию о версии драйвера AVR
		struct AVR_VERSION
		{
			BYTE avrstr[10];
			BYTE avrprefix[4];
			BYTE avrmajor;
			BYTE avrminor;
		};

		// структура, содержащая информацию о версии драйвера DSP
		struct DSP_INFO
		{
			BYTE Target[10];
			BYTE Label[6];
			BYTE DspMajor;
			BYTE DspMinor;
		};

		#pragma pack()

		// адреса переменных штатного драйвера DSP (располагаются в памяти программ DSP)
		const WORD D_PROGRAM_BASE_ADDRESS 		= (VarsBaseAddress + 0x0);
		const WORD D_TARGET			   			= (VarsBaseAddress + 0x1);
		const WORD D_LABEL			   			= (VarsBaseAddress + 0x6);
		const WORD D_VERSION			   			= (VarsBaseAddress + 0x9);
		const WORD D_TEST_VAR1				 		= (VarsBaseAddress + 0xA);
		const WORD D_TEST_VAR2				 		= (VarsBaseAddress + 0xB);
		const WORD D_TEST_INTR_VAR			 		= (VarsBaseAddress + 0xC);
		const WORD D_MODULE_READY	 		 		= (VarsBaseAddress + 0xD);
		const WORD D_COMMAND					 		= (VarsBaseAddress + 0xE);

		const WORD D_READ_RATE						= (VarsBaseAddress + 0x10);
		const WORD D_READ_ENABLED					= (VarsBaseAddress + 0x11);
		const WORD D_READ_FIFO_BASE_ADDRESS		= (VarsBaseAddress + 0x12);
		const WORD D_READ_FIFO_LENGTH				= (VarsBaseAddress + 0x13);

		const WORD D_WRITE_RATE						= (VarsBaseAddress + 0x20);
		const WORD D_WRITE_ENABLED					= (VarsBaseAddress + 0x21);
		const WORD D_WRITE_FIFO_BASE_ADDRESS	= (VarsBaseAddress + 0x22);
		const WORD D_WRITE_FIFO_LENGTH			= (VarsBaseAddress + 0x23);
	}

	// интерфейс модуля USB2185
	struct IRTUSB2185:	IRTUSBDSP<RTUSB2185::DSP_INFO>
	{
		// функции для организации чтения данных из модуля
		virtual BOOL WINAPI START_READ(void) = 0;
		virtual BOOL WINAPI STOP_READ(void) = 0;
		virtual BOOL WINAPI ReadData(SHORT * const lpBuffer, DWORD * const nNumberOfWordsToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped) = 0;

		// функции для организации передачи данных в модуль
		virtual BOOL WINAPI START_WRITE(void) = 0;
		virtual BOOL WINAPI STOP_WRITE(void) = 0;
		virtual BOOL WINAPI WriteData(SHORT * const lpBuffer, DWORD * const nNumberOfWordsToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) = 0;

		// функции для работы с пользовательским ППЗУ
		virtual BOOL WINAPI GET_FLASH(RTUSB2185::FLASH * const fi) = 0;
		virtual BOOL WINAPI PUT_FLASH(RTUSB2185::FLASH * const fi) = 0;

		// функция выдачи строки с последней ошибкой
		virtual int WINAPI GetLastErrorString(LPTSTR const lpBuffer, DWORD nSize) = 0;
	};




	//==============================================================================
	//                       М О Д У Л Ь       U S B - 3 0 0 0
	//==============================================================================

	// пространство имён для работы с модулем USB-3000
	namespace RTUSB3000
	{
		// адрес начала сегмента блока данных в памяти программ DSP
		const WORD VarsBaseAddress = 0x30;
		// тактовая частота работы DSP в кГц
		const DWORD DSP_CLOCK_OUT = 72000;

		// возможные источники тактовых импульсов для ввода данных
		enum {	INTERNAL_INPUT_CLOCK, EXTERNAL_INPUT_CLOCK, INVALID_INPUT_CLOCK };
		// возможные типы синхронизации вводимых с модуля даных
		enum {	NO_SYNCHRO, TTL_START_SYNCHRO, TTL_KADR_SYNCHRO, ANALOG_SYNCHRO, INVALID_INPUT_SYNCHRO };
		// возможные типы вводимых с модуля даных
		enum {	EMPTY_DATA, ADC_DATA, TTL_DATA, MIXED_DATA, INVALID_INPUT_DATA };
		// возможные индексы скорости работы шины USB
		enum {	USB11, USB20, INVALID_USB_SPEED };

		#pragma pack(1)

		// структура, задающая режимы ввода данных для модуля USB-3000
		struct INPUT_PARS
		{
			WORD size;
			BOOL InputEnabled;			  		// флажок разрешение/запрещение ввода данных (только при чтении)
			BOOL CorrectionEnabled;				// управление корректировкой входных данных (с АЦП)
			WORD InputClockSource;				// источник тактовых импульсов для ввода данных
			WORD InputType;						// тип вводимых с модуля даных (АЦП или ТТЛ)
			WORD SynchroType;						// тип синхронизации вводимых с модуля даных
			WORD SynchroAdType;					// тип аналоговой синхронизации
			WORD SynchroAdMode; 					// режим аналоговой сихронизации
			WORD SynchroAdChannel;  			// синхроканал АЦП при аналоговой синхронизации
			SHORT SynchroAdPorog; 				// порог срабатывания АЦП при аналоговой синхронизации
			WORD ChannelsQuantity;				// число активных логических каналов
			WORD ControlTable[128];				// управляющая таблица с активными логическими каналами
			WORD InputFifoBaseAddress;			// базовый адрес FIFO буфера ввода в DSP модуля
			WORD InputFifoLength;	  			// длина FIFO буфера ввода в DSP модуля
			double InputRate;	  			  		// тактовая частота ввода данных в кГц
			double InterKadrDelay;		  		// межкадровая задержка в мс
			double ChannelRate;					// частота ввода фиксированного логического канала
			double AdcOffsetCoef[8]; 			// корректировочные коэф. смещение нуля для АЦП
			double AdcScaleCoef[8];				// корректировочные коэф. масштаба для АЦП
		};

		// структура, задающая режимы вывода данных для модуля USB-3000
		struct OUTPUT_PARS
		{
			WORD size;
			BOOL OutputEnabled;					// разрешение/запрещение вывода данных
			double OutputRate;	  		  		// частота вывода данных в кГц
			WORD OutputFifoBaseAddress;  		// базовый адрес FIFO буфера вывода
			WORD OutputFifoLength;				// длина FIFO буфера вывода
		};


		// структура пользовательского ППЗУ
		struct FLASH
		{
			WORD CRC16;								// контрольная сумма
			WORD size;								// размер данной структуры в байтах
			BYTE SerialNumber[9];				// серийный номер модуля
			BYTE Name[11];							// название модуля
			BYTE Revision;							// ревизия модуля
			BYTE DspType[17];						// тип установленного DSP
			BYTE IsDacPresented; 				// флажок наличия ЦАП
			DWORD DspClockout; 					// тактовая частота DSP в Гц
			float AdcOffsetCoef[8];				// корректировочные коэф. смещения нуля для АЦП
			float AdcScaleCoef[8];				// корректировочные коэф. масштаба для АЦП
			float DacOffsetCoef[2];				// корректировочные коэф. смещения нуля для ЦАП
			float DacScaleCoef[2];				// корректировочные коэф. масштаба для ЦАП
			BYTE ReservedByte[129];				// зарезервировано
		};

		// структура, содержащая информацию о версии драйвера AVR
		struct AVR_INFO
		{
			BYTE avrstr[10];
			BYTE avrprefix[4];
			BYTE avrmajor;
			BYTE avrminor;
		};

		// структура, содержащая информацию о версии драйвера DSP
		struct DSP_INFO
		{
			BYTE Target[10];						// модуль, для которого предназначен данный драйвер DSP
			BYTE Label[6];							// метка разработчика драйвера DSP
			BYTE DspMajor;							// старшие число версии драйвера DSP
			BYTE DspMinor;							// младшее число версии драйвера DSP
		};

		#pragma pack()

		// адреса переменных штатного драйвера DSP (располагаются в памяти программ DSP)
		const WORD D_PROGRAM_BASE_ADDRESS 		= (VarsBaseAddress + 0x0);
		const WORD D_TARGET			   			= (VarsBaseAddress + 0x1);
		const WORD D_LABEL			   			= (VarsBaseAddress + 0x6);
		const WORD D_VERSION			   			= (VarsBaseAddress + 0x9);
		const WORD D_TEST_VAR1				 		= (VarsBaseAddress + 0xA);
		const WORD D_TEST_VAR2				 		= (VarsBaseAddress + 0xB);
		const WORD D_TEST_INTR_VAR			 		= (VarsBaseAddress + 0xC);
		const WORD D_MODULE_READY	 		 		= (VarsBaseAddress + 0xD);
		const WORD D_COMMAND					 		= (VarsBaseAddress + 0xE);
		const WORD D_INPUT_CLOCK_SOURCE 			= (VarsBaseAddress + 0x10);

		const WORD D_CONTROL_TABLE_LENGHT  		= (VarsBaseAddress + 0x20);
		const WORD D_INPUT_SAMPLE					= (VarsBaseAddress + 0x21);
		const WORD D_INPUT_CHANNEL					= (VarsBaseAddress + 0x22);
		const WORD D_INPUT_RATE						= (VarsBaseAddress + 0x23);
		const WORD D_INTER_KADR_DELAY				= (VarsBaseAddress + 0x24);
		const WORD D_FIRST_SAMPLE_DELAY			= (VarsBaseAddress + 0x25);
		const WORD D_INPUT_ENABLED					= (VarsBaseAddress + 0x26);
		const WORD D_INPUT_FIFO_BASE_ADDRESS	= (VarsBaseAddress + 0x27);
		const WORD D_INPUT_FIFO_LENGTH	  		= (VarsBaseAddress + 0x28);
		const WORD D_CUR_INPUT_FIFO_LENGTH		= (VarsBaseAddress + 0x29);

		const WORD D_CORRECTION_ENABLED 	  		= (VarsBaseAddress + 0x2B);

		const WORD D_INPUT_TYPE				  		= (VarsBaseAddress + 0x2C);
		const WORD D_SYNCHRO_TYPE			  		= (VarsBaseAddress + 0x2D);
		const WORD D_SYNCHRO_AD_TYPE		  		= (VarsBaseAddress + 0x2E);
		const WORD D_SYNCHRO_AD_MODE		  		= (VarsBaseAddress + 0x2F);
		const WORD D_SYNCHRO_AD_CHANNEL	  		= (VarsBaseAddress + 0x30);
		const WORD D_SYNCHRO_AD_POROG 	  		= (VarsBaseAddress + 0x31);

		const WORD D_OUTPUT_SAMPLE			 		= (VarsBaseAddress + 0x40);
		const WORD D_OUTPUT_SCLK_DIV		 		= (VarsBaseAddress + 0x41);
		const WORD D_OUTPUT_RATE  					= (VarsBaseAddress + 0x42);
		const WORD D_OUTPUT_ENABLED  				= (VarsBaseAddress + 0x43);
		const WORD D_OUTPUT_FIFO_BASE_ADDRESS	= (VarsBaseAddress + 0x44);
		const WORD D_OUTPUT_FIFO_LENGTH		 	= (VarsBaseAddress + 0x45);
		const WORD D_CUR_OUTPUT_FIFO_LENGTH	 	= (VarsBaseAddress + 0x46);

		const WORD D_ENABLE_TTL_OUT 	  			= (VarsBaseAddress + 0x4D);
		const WORD D_TTL_OUT 	  					= (VarsBaseAddress + 0x4E);
		const WORD D_TTL_IN 	  						= (VarsBaseAddress + 0x4F);

		const WORD D_ADC_SCALE 	 	  				= (VarsBaseAddress + 0x50);
		const WORD D_ADC_ZERO 	 	  				= (VarsBaseAddress + 0x58);
		const WORD D_CONTROL_TABLE 	 	  		= (0x100);
	}

	// интерфейс модуля USB3000                                       `
	struct IRTUSB3000 :	IRTUSBDSP<RTUSB3000::DSP_INFO>
	{
		// функции для работы в режиме ввода данных из модуля в РС
		virtual BOOL WINAPI GET_INPUT_PARS(RTUSB3000::INPUT_PARS * const ap) = 0;
		virtual BOOL WINAPI SET_INPUT_PARS(RTUSB3000::INPUT_PARS * const ap) = 0;
		virtual BOOL WINAPI START_READ(void) = 0;
		virtual BOOL WINAPI STOP_READ(void) = 0;
		virtual BOOL WINAPI READ_KADR(SHORT * const Data) = 0;
		virtual BOOL WINAPI READ_SAMPLE(WORD Channel, SHORT * const Sample) = 0;
		virtual BOOL WINAPI ReadData(SHORT * const lpBuffer, DWORD * const nNumberOfWordsToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped) = 0;

		// функции для работы c выводом данных из РС в модуль
		virtual BOOL WINAPI GET_OUTPUT_PARS(RTUSB3000::OUTPUT_PARS * const dp) = 0;
		virtual BOOL WINAPI SET_OUTPUT_PARS(RTUSB3000::OUTPUT_PARS * const dp) = 0;
		virtual BOOL WINAPI START_WRITE(void) = 0;
		virtual BOOL WINAPI STOP_WRITE(void) = 0;
		virtual BOOL WINAPI WRITE_SAMPLE(WORD Channel, SHORT * const Sample) = 0;
		virtual BOOL WINAPI WriteData(SHORT * const lpBuffer, DWORD * const nNumberOfWordsToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) = 0;

		// функции для работы с ТТЛ линиями
		virtual BOOL WINAPI ENABLE_TTL_OUT(BOOL EnableTtlOut) = 0;
		virtual BOOL WINAPI TTL_OUT(WORD TtlOut) = 0;
		virtual BOOL WINAPI TTL_IN(WORD * const TtlIn) = 0;

		// функции для работы с пользовательским ППЗУ
		virtual BOOL WINAPI ENABLE_FLASH_WRITE(BOOL EnableFlashWrite) = 0;
		virtual BOOL WINAPI PUT_FLASH(RTUSB3000::FLASH * const fi) = 0;
		virtual BOOL WINAPI GET_FLASH(RTUSB3000::FLASH * const fi) = 0;

		// функция выдачи строки с последней ошибкой
		virtual int WINAPI GetLastErrorString(LPTSTR const lpBuffer, DWORD nSize) = 0;
	};

	#define	MAKE_PM_BUFFER_RTUSBAPI(PmPoints, Buffer)		(WORD)(PmPoints), Transform_Buffer((WORD)(PmPoints), (LONG *)(Buffer))

	#endif

#endif

