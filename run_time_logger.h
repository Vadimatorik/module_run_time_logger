#pragma once

#ifdef MODULE_RUN_TIME_LOGGER_ENABLED

#ifdef __cplusplus

#include <stdint.h>
#include "user_os.h"
#include "mc_hardware_interfaces_base.h"

enum class RTL_TYPE_M {
	INIT_OK				= 0,							// Инициализация идет нормально.
	INIT_ISSUE			= 1,							// Есть некоторые затруднения, но можно и без этого жить.
	INIT_ERROR			= 2,							// Критическая проблема. Без этого дальнейшее действие уже невозможно (или откад для повторной попытки/пропуск).
	RUN_MESSAGE_OK		= 3,							// Сообщение времени выполнения.
	RUN_MESSAGE_ISSUE	= 4,							// Что-то пошло не так во время выполнения, но в целом все хорошо.
	RUN_MESSAGE_ERROR	= 5								// Что-то в реальном времени пошло не так.
};

const char ANSI_RED[] = "\x1B[31m";
const char ANSI_GRN[] = "\x1B[32m";
const char ANSI_YEL[] = "\x1B[33m";
const char ANSI_BLU[] = "\x1B[34m";
const char ANSI_MAG[] = "\x1B[35m";
const char ANSI_CYN[] = "\x1B[36m";
const char ANSI_WHT[] = "\x1B[37m";

const char ANSI_RESET[] = "\x1B[0m";

// Цвета для разных типов сообщений.
struct colorMessageStruct {
	const char*		initOkColorString;
	const char*		initIssueColorString;
	const char*		initErrorColorString;
	const char*		runMessageOkColorString;
	const char*		runMessageIssueColorString;
	const char*		runMessageErrorColorString;
};

struct runTimeLoggerCfg {
	colorMessageStruct					color;
	BASE_RESULT ( *outBuffer )			( const char* string );
};

#define MAX_COUNT_MESSAGE_IN_QUEUE				20
#define RUN_TIME_LOGGER_TASK_SIZE				200
#define	RUN_TIME_LOGGER_PRIO					2

class RunTimeLogger {
public:
	RunTimeLogger( const runTimeLoggerCfg* const cfg );

	void sendMessage( RTL_TYPE_M type, const char* string );

private:
	static	void	task						( void* pObj );

	const runTimeLoggerCfg* const cfg;

	USER_OS_STATIC_MUTEX						m = nullptr;
	USER_OS_STATIC_MUTEX_BUFFER					mb;
	const char*									color[6];

	USER_OS_STATIC_QUEUE						qMessage;
	USER_OS_STATIC_QUEUE_STRUCT					qsMessage;
	char*										qbMessage[ MAX_COUNT_MESSAGE_IN_QUEUE ];

	USER_OS_STATIC_STACK_TYPE					tbTask[ RUN_TIME_LOGGER_TASK_SIZE ];
	USER_OS_STATIC_TASK_STRUCT_TYPE				tsTask;
};

#endif

#endif
