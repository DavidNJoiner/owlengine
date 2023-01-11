#include <core/logger.hpp>
#include <core/asserts.hpp>

//TODO temporary
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>
#include <iostream>

// C libs
#include <stdarg.h>

int initialize_logging()
{
	// TODO : create log file.
	return TRUE;
}

void shutdown_logging()
{
	// TODO: cleanup logging/write queued entries.
}

void log_output(log_level level, const char* message ...)
{
	//TODO: Implement color for each level

	const char* level_strings[6] = { "[FATAL]: ", "[ERROR]: " , "[WARN]: " , "[INFO]: " , "[DEBUG]: " , "[TRACE]: " };
	int msg_color = 15;
	uint8_t is_error = level < 2;
	

	switch (level) {
		case 1:
			msg_color = 196;
			break;
		case 2:
			msg_color = 208;
			break;
		case 3:
			msg_color = 226;
			break;
		case 4:
			msg_color = 46;
			break;
		case 5:
			msg_color = 21;
			break;
		case 6:
			msg_color = 129;
			break;
	}
	
	const uint32_t msg_length = 32000;
	char out_message[msg_length];
	memset(out_message, 0, sizeof(out_message));

	va_list arg_ptr = nullptr;

	va_start(arg_ptr, message);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), msg_color);

	printf(level_strings[level]);
	vprintf(message, arg_ptr);
	printf("\n");

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	va_end(arg_ptr);
}

void report_assertion_failure(const char* expression, const char* message, const char* file, uint32_t line)
{
	log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
	//log_output(LOG_LEVEL_FATAL, "Assertion failure: ", expression, "message: ", message, "in file: ", file, "line: ", line);
}
