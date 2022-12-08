#pragma once

#include "core/define.hpp"


namespace platform_layer {

	struct platform_state {
		void* internal_state;
	};

	uint8_t platform_startup(
		platform_state* plat_state,
		const char* application_name,
		uint32_t x,
		uint32_t y,
		uint32_t width,
		uint32_t height);

	void platform_shutdown(platform_state* plat_state);

	uint8_t platform_pump_message(platform_state* plat_state);

	void* platform_allocate(uint64_t size, uint8_t aligned);
	void platform_free(void* block, uint8_t aligned);
	void* platform_zero_memory(void* block, uint64_t size);
	void* platform_copy_memory(void* dest, const void* source, uint64_t size);
	void* platform_set_memory(void* dest, uint32_t value, uint64_t size);

	void platform_console_write(const char* message, uint8_t colour);
	void platform_console_write_error(const char* message, uint8_t colour);

	double platform_get_absolute_time();

	// Sleep on the thread for the provided ms. This blocks the main thread.
	// Should only be used for giving time back to the OS for unused update power.
	// Therefore it is not exported.
	void platform_sleep(uint64_t ms);

}