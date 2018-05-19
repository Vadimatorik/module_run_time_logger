#include "run_time_logger.h"

#ifdef MODULE_RUN_TIME_LOGGER_ENABLED

#include <stdio.h>
#include <string.h>

const char INIT_OK[]					= "[ INIT_OK ]\t\t";
const char INIT_ISSUE[]				= "[ INIT_ISSUE ]\t\t";
const char INIT_ERROR[]				= "[ INIT_ERROR ]\t\t";
const char RUN_MESSAGE_OK[]			= "[ RUN_MESSAGE_OK ]\t";
const char RUN_MESSAGE_ISSUE[]		= "[ RUN_MESSAGE_ISSUE ]\t";
const char RUN_MESSAGE_ERROR[]		= "[ RUN_MESSAGE_ERROR ]\t";

const char* startString[6] = {
	INIT_OK, INIT_ISSUE, INIT_ERROR, RUN_MESSAGE_OK, RUN_MESSAGE_ISSUE, RUN_MESSAGE_ERROR
};

RunTimeLogger::RunTimeLogger( const runTimeLoggerCfg* const cfg ) : cfg( cfg ) {
	this->m = USER_OS_STATIC_MUTEX_CREATE( &mb );
	this->color[ 0 ] = this->cfg->color.initOkColorString;
	this->color[ 1 ] = this->cfg->color.initIssueColorString;
	this->color[ 2 ] = this->cfg->color.initErrorColorString;
	this->color[ 3 ] = this->cfg->color.runMessageOkColorString;
	this->color[ 4 ] = this->cfg->color.runMessageIssueColorString;
	this->color[ 5 ] = this->cfg->color.runMessageErrorColorString;

	/// В очереди хранятся указатели на строки.
	this->qMessage	=	USER_OS_STATIC_QUEUE_CREATE( MAX_COUNT_MESSAGE_IN_QUEUE, sizeof( char* ), ( uint8_t* )this->qbMessage, &this->qsMessage );

	/// Создаем задачу, которая будет разбирать запросы.
	USER_OS_STATIC_TASK_CREATE(	RunTimeLogger::task,
								"runTimeLogger",
								RUN_TIME_LOGGER_TASK_SIZE,
								( void* )this,
								RUN_TIME_LOGGER_PRIO,
								this->tbTask,
								&this->tsTask );
}

void RunTimeLogger::task ( void* pObj ) {
	RunTimeLogger* o = ( RunTimeLogger* )pObj;
	char*	message;

	while( true ) {
		/// Забираем строку для отправки.
		USER_OS_QUEUE_RECEIVE(o->qMessage, &message, portMAX_DELAY );

		/// Передаем до победного.
		while( o->cfg->outBuffer( message ) != BASE_RESULT::OK );

		/// Убираем за собой.
		vPortFree( message );
	}
}


void RunTimeLogger::sendMessage( RTL_TYPE_M type, const char* string ) {
	USER_OS_TAKE_MUTEX( this->m, portMAX_DELAY );

	uint32_t lenString = 0;
	lenString += strlen( this->color[ ( uint8_t )type ] ) + 2;
	lenString += strlen( startString[ ( uint8_t )type ] ) + 2;
	lenString += strlen( string ) + 2;
	lenString += 3;

	char* messageString = nullptr;

	/// Ждем, пока появится место в куче.
	while( true ) {
		messageString = ( char* )pvPortMalloc( lenString );

		if ( messageString != nullptr )
			break;

		USER_OS_DELAY_MS( 1 );
	}

	/// Копируем строчку.
	snprintf( messageString, lenString, "%s %s %s \n\r", this->color[ ( uint8_t )type ], startString[ ( uint8_t )type ], string );

	USER_OS_QUEUE_SEND( this->qMessage, &messageString, portMAX_DELAY );

	USER_OS_GIVE_MUTEX( this->m );

	return;
}

#endif
