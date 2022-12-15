#include <core/logger.hpp>
#include <core/asserts.hpp>

//TODO temporary
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

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
	
	//Red : \033[31m
	//Blue: \033[34m
	//Green : \033[32m
	//Yellow : \033[33m
	//Orange : \033[38; 5; 214m
	//Purple : \033[35m

	const char* level_strings[6] = { "[FATAL]: ", "[ERROR]: " , "[WARN]: " , "[INFO]: " , "[DEBUG]: " , "[TRACE]: " };
	uint8_t is_error = level < 2;

	char out_message[32000];
	memset(out_message, 0, sizeof(out_message));

	va_list arg_ptr = nullptr;
	va_start(arg_ptr, message);
	vsnprintf(out_message, 32000, message, arg_ptr);
	va_end(arg_ptr);

	char out_message2[32000];

	va_start(arg_ptr, message);

	printf("\033[38;5;214m%s:\033[0m ", level_strings[level]);
	vprintf(message, arg_ptr);
	printf("\n");

	va_end(arg_ptr);

	sprintf_s(out_message2, "%s%s\n", level_strings[level], out_message);

	// TODO: platform-specific output.
	printf_s("%s", out_message2);
}

void report_assertion_failure(const char* expression, const char* message, const char* file, uint32_t line)
{
	log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
	//log_output(LOG_LEVEL_FATAL, "Assertion failure: ", expression, "message: ", message, "in file: ", file, "line: ", line);
}
