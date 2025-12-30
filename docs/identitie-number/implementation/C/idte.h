#pragma once

#include <stdint.h>

#define IDTE_SIZE       64
#define IDTE_SUCCESS     0
#define IDTE_INVALID     1

typedef int64_t  i64;
typedef int32_t  i32;
typedef int16_t  i16;
typedef int8_t   i8;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

// Fixed-length 11-character encoding for 64-bit hash
u32 IDTE_encodeHash(u64 value, char out[12]);
uint8_t IDTE_decodeHash(const char in[12], u64* out_value);

// Variable-length encoding: returns number of characters written (excluding null terminator)
u32 IDTE_encode(u64 value, char* out);

// Variable-length decoding: takes length of input string
u8 IDTE_decode(const char* in, u32 length, u64 *out);