#pragma once

#ifdef __cplusplus

#include <stdint.h>
#include "user_os.h"

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
struct color_message_struct {
	const char*		init_ok_color_string;
	const char*		init_issue_color_string;
	const char*		init_error_color_string;
	const char*		run_message_ok_color_string;
	const char*		run_message_issue_color_string;
	const char*		run_message_error_color_string;
};

struct run_time_logger_cfg {
	color_message_struct		color;
    void ( *out_buffer )     ( char* string );
};

class run_time_logger {
public:
	run_time_logger( const run_time_logger_cfg* const cfg );

	void send_message( RTL_TYPE_M type, const char* string );
private:
	mutable	char message_buffer[ 512 ];											// Символов на строку.
	const run_time_logger_cfg* const cfg;
	mutable USER_OS_STATIC_MUTEX                    	m = nullptr;
	mutable USER_OS_STATIC_MUTEX_BUFFER             	mb;
	mutable	const char*									color[6];
};

#endif
