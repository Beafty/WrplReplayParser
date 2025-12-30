

#ifndef MYEXTENSION_CONST_H
#define MYEXTENSION_CONST_H

#include <cstdint>
#include "array"
#include <cstring>
#include "dag_assert.h"


#define EASTL_LIMITS_MAX_U(T) ((T)~(T)0)

#define BitSize_t uint32_t
#define BITS_TO_BYTES(x)              (((x) + 7) >> 3)
#define BITS_TO_BYTES_WORD_ALIGNED(x) ((((x) + 31) >> 5) << 2)
#define BYTES_TO_BITS(x)              ((x) << 3)

template<typename T, size_t N>
void mem_copy_from(std::array<T, N> &dst, const T *src)
{
  std::memcpy(dst.data(), src, sizeof(T) * N);
}

template<typename T, size_t N>
void mem_set_0(std::array<T, N> &dst)
{
  std::memset(dst.data(), 0, sizeof(T) * N);
}

template<typename T, size_t N>
void mem_set_FF(std::array<T, N> &dst)
{
  std::memset(dst.data(), 0, sizeof(T) * N);
}

#if defined(__cplusplus) && !defined(__GNUC__)
template <typename T, size_t N>
char (&_countof__helper_(T (&array)[N]))[N];
#define countof(x) (sizeof(_countof__helper_(x)))
#else
#define countof(x) (sizeof(x) / sizeof((x)[0]))
#endif
#define c_countof(x) (sizeof(x) / sizeof((x)[0]))

#endif //MYEXTENSION_CONST_H
