#pragma once
#include <stdint.h>

// Properly define static assertions.
#if defined(__cplusplus) || defined(_gcc_)
#define STATIC_ASSERT static_assert
#elif defined(__clang__) || defined(__gcc__)
#define STATIC_ASSERT _Static_assert
#endif

// Ensure all types are of the correct size.
STATIC_ASSERT(sizeof(uint8_t) == 1, "Expected uint8_t to be 1 byte.");
STATIC_ASSERT(sizeof(uint16_t) == 2, "Expected uint16_t to be 2 bytes.");
STATIC_ASSERT(sizeof(uint32_t) == 4, "Expected uint32_t to be 4 bytes.");
STATIC_ASSERT(sizeof(uint64_t) == 8, "Expected uint64_t to be 8 bytes.");

STATIC_ASSERT(sizeof(int8_t) == 1, "Expected int8_t to be 1 byte.");
STATIC_ASSERT(sizeof(int16_t) == 2, "Expected int16_t to be 2 bytes.");
STATIC_ASSERT(sizeof(int32_t) == 4, "Expected int32_t to be 4 bytes.");
STATIC_ASSERT(sizeof(int64_t) == 8, "Expected int64_t to be 8 bytes.");

STATIC_ASSERT(sizeof(float) == 4, "Expected float to be 4 bytes / 32 bits.");
STATIC_ASSERT(sizeof(double) == 8, "Expected double to be 8 bytes / 64 bits.");

#define TRUE 1
#define FALSE 0

// Platform detection (only windows is supported)
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) 
#define LVE_PLATFORM_WINDOWS = 1;
#ifndef _WIN64
#error "64-bit is required on Windows!"
#endif
// Catch anything not caught by the above.
#define LVE_PLATFORM_UNIX = 1;
#elif defined(_POSIX_VERSION)
#else
#error "No compatible platform found!"
#endif

#ifdef LVE_EXPORT
// Exports
#ifdef _MSC_VER
#define LVE_API __declspec(dllexport)
#else
#define LVE_API __attribute__((visibility("default")))
#endif
#else
// Imports
#ifdef _MSC_VER
#define LVE_API __declspec(dllimport)
#else
#define LVE_API
#endif
#endif