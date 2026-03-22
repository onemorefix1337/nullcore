#pragma once

#ifdef _WIN32
#ifdef _WIN64
#define NC_PLATFORM_WINDOWS
#else
#error "x86 not supported!"
#endif
#endif

#define NC_ENABLE_ASSERTS

#ifdef NC_ENABLE_ASSERTS
#define NC_ASSERT(x, msg) { if(!(x)) { NC_CORE_ERROR(msg); __debugbreak(); } }
#define NC_CORE_ASSERT(x, msg) { if(!(x)) { NC_CORE_ERROR(msg); __debugbreak(); } }
#else
#define NC_ASSERT(x, msg)
#define NC_CORE_ASSERT(x, msg)
#endif

#define BIT(x) (1 << x)  // ← это должно быть здесь