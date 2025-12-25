/*
 * CMinus Language Standard
 * Version: 1.0.0 (Standard 2026)
 * ======================================================================
 * Language: C
 * Standard: C99 or later
 * Author: ...
 * 
 * Type definitions and constants for explicit-width integer and floating
 * point types, character types, boolean, and ternary logic.
 * 
 * Features:
 * - Fixed-width integer types (i8, i16, i32, i64, u8, u16, u32, u64)
 * - Floating point types (f32, f64)
 * - Character types (c8, c16, c32)
 * - Boolean type (bool) with true/false constants
 * - Ternary type (trnry) with down(n)/up(n)/middle intensity macros
 * - Platform-dependent pointer-sized integers (isize, usize)
 * - Min/max constants for all numeric types
 * 
 * Usage: #include "cm-types.h"
 * Compile with: -std=c99 or higher
 * 
 * Note: This is a standalone header with no dependencies.
 * ======================================================================
*/
#pragma once

typedef signed char           byte;
typedef unsigned char         ubyte;

typedef signed char           i8;
typedef signed short          i16;
typedef signed int            i32;
typedef signed long long      i64;

typedef unsigned char         u8;
typedef unsigned short        u16;
typedef unsigned int          u32;
typedef unsigned long long    u64;

typedef float                 f32;
typedef double                f64;

typedef unsigned char         c8;
typedef unsigned short        c16;
typedef unsigned int          c32;

typedef unsigned char         bool;
typedef signed char           trnry;

// Byte min
#define minof_byte   0

// Integer minimums
#define minof_i8     -128
#define minof_i16    -32768
#define minof_i32    -2147483648
#define minof_i64    -9223372036854775808LL

#define minof_u8     0
#define minof_u16    0
#define minof_u32    0
#define minof_u64    0

// Characters minimums
#define minof_c8     0
#define minof_c16    0
#define minof_c32    0

// Floating-point minimums
#define minof_f32    (-3.402823466e+38f)
#define minof_f64    (-1.7976931348623157e+308)

// Byte max
#define maxof_byte   255

// Integer maximums
#define maxof_i8     127
#define maxof_i16    32767
#define maxof_i32    2147483647
#define maxof_i64    9223372036854775807LL

#define maxof_u8     255
#define maxof_u16    65535
#define maxof_u32    4294967295U
#define maxof_u64    18446744073709551615ULL

// Floating-point maximums
#define maxof_f32    3.402823466e+38f
#define maxof_f64    1.7976931348623157e+308

// Characters maximums
#define maxof_c8     255
#define maxof_c16    65535
#define maxof_c32    4294967295U

// Floating-point epsilon
#define epsilon_f32  1.192092896e-07f
#define epsilon_f64  2.2204460492503131e-16

// Smallest positive normalized floating-point values
#define minpos_f32   1.175494351e-38f
#define minpos_f64   2.2250738585072014e-308

// Infinity values
#define inf_f32      (1.0f / 0.0f)
#define inf_f64      (1.0 / 0.0)

// NaN values
#define nan_f32      (0.0f / 0.0f)
#define nan_f64      (0.0 / 0.0)

// Special character constants
#define nullchar8    ((c8)'\0')
#define nullchar16   ((c16)0)
#define nullchar32   ((c32)0)

// Booleans literals
#define true       1
#define false      0

#define truel(n)   (n)

// Ternary literals
#define down       -1
#define middle     0  
#define up         1

#define downl(n)   (-(n))
#define upl(n)     (n)

// Platform-dependent pointer-sized integers
// usize: unsigned size sufficient to hold any array index
// isize: signed version for pointer differences
#if defined(__LP64__) || defined(_WIN64) || defined(__x86_64__) || \
    defined(__ppc64__) || defined(__aarch64__)
    // 64-bit platforms
    typedef i64 isize;
    typedef u64 usize;
    
    #define minof_isize minof_i64
    #define minof_usize minof_u64
    
    #define maxof_isize maxof_i64
    #define maxof_usize maxof_u64
    
    #define ISIZE_BITS 64
#else
    // 32-bit platforms (default assumption)
    typedef i32 isize;
    typedef u32 usize;
    
    #define minof_isize minof_i32
    #define minof_usize minof_u32
    
    #define maxof_isize maxof_i32
    #define maxof_usize maxof_u32
    
    #define ISIZE_BITS 32
#endif
