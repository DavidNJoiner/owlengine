#pragma once

#include "define.hpp"

//Disable assertions by commenting out the below line
#define LVE_ASSERTIONS_ENABLED

#ifdef  LVE_ASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif //  LVE_ASSERTIONS_ENABLED

LVE_API void report_assertion_failure(const char* expression, const char* message, const char* file, int line);

#define LVE_ASSERT(expr)                                                \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            report_assertion_failure(#expr, "", __FILE__, __LINE__); \
            debugBreak();                                            \
        }                                                            \
    }

#define LVE_ASSERT_MSG(expr, message)                                        \
    {                                                                     \
        if (expr) {                                                       \
        } else {                                                          \
            std::cout << expr << " / " << #expr << std::endl;             \
            report_assertion_failure(#expr, message, __FILE__, __LINE__); \
            debugBreak();                                                 \
        }                                                                 \
    }

#ifdef _DEBUG
#define LVE_ASSERT_DEBUG(expr)                                          \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            std::cout << expr << " / " << #expr << std::endl;        \
            report_assertion_failure(#expr, "", __FILE__, __LINE__); \
            debugBreak();                                            \
        }                                                            \
    }
#else
#define LVE_ASSERT_DEBUG(expr)  // Does nothing at all
#endif

#else
#define LVE_ASSERT(expr)               // Does nothing at all
#define LVE_ASSERT_MSG(expr, message)  // Does nothing at all
#define LVE_ASSERT_DEBUG(expr)         // Does nothing at all
#endif