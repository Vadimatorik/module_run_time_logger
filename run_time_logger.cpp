#include "run_time_logger.h"

#ifdef MODULE_RUN_TIME_LOGGER_ENABLED

#include <stdio.h>

const uint8_t INIT_OK[]					= "[ INIT_OK ]\t\t";
const uint8_t INIT_ISSUE[]				= "[ INIT_ISSUE ]\t\t";
const uint8_t INIT_ERROR[]				= "[ INIT_ERROR ]\t\t";
const uint8_t RUN_MESSAGE_OK[]			= "[ RUN_MESSAGE_OK ]\t";
const uint8_t RUN_MESSAGE_ISSUE[]		= "[ RUN_MESSAGE_ISSUE ]\t";
const uint8_t RUN_MESSAGE_ERROR[]		= "[ RUN_MESSAGE_ERROR ]\t";

const uint8_t* start_string[6] = {
	INIT_OK, INIT_ISSUE, INIT_ERROR, RUN_MESSAGE_OK, RUN_MESSAGE_ISSUE, RUN_MESSAGE_ERROR
};

RunTimeLogger::RunTimeLogger( const run_time_logger_cfg* const cfg ) : cfg( cfg ) {
	this->m = USER_OS_STATIC_MUTEX_CREATE( &mb );
	this->color[ 0 ] = this->cfg->color.init_ok_color_string;
	this->color[ 1 ] = this->cfg->color.init_issue_color_string;
	this->color[ 2 ] = this->cfg->color.init_error_color_string;
	this->color[ 3 ] = this->cfg->color.run_message_ok_color_string;
	this->color[ 4 ] = this->cfg->color.run_message_issue_color_string;
	this->color[ 5 ] = this->cfg->color.run_message_error_color_string;
}

BASE_RESULT RunTimeLogger::sendMessage( RTL_TYPE_M type, const char* string ) {
	if ( this->m != nullptr)				USER_OS_TAKE_MUTEX( this->m, portMAX_DELAY );
	BASE_RESULT r;
	snprintf( this->message_buffer, 512, "%s %s %s \n\r", this->color[ ( uint8_t )type ], start_string[ ( uint8_t )type ], string );
	r = this->cfg->out_buffer( this->message_buffer );
	if ( this->m != nullptr)				USER_OS_GIVE_MUTEX( this->m );
	return r;
}

#endif
