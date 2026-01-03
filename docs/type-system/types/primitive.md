# **Primitive Types and Literal Semantics**

Primitive types are the lowest-level value types in CMinus.
They map directly and predictably to C primitives and introduce **no hidden behavior**.

---

## 1 Integer Types

Signed integers:

| Type  | Width  | C Mapping |
| ----- | ------ | --------- |
| `i8`  | 8-bit  | `int8_t`  |
| `i16` | 16-bit | `int16_t` |
| `i32` | 32-bit | `int32_t` |
| `i64` | 64-bit | `int64_t` |

Unsigned integers:

| Type  | Width  | C Mapping  |
| ----- | ------ | ---------- |
| `u8`  | 8-bit  | `uint8_t`  |
| `u16` | 16-bit | `uint16_t` |
| `u32` | 32-bit | `uint32_t` |
| `u64` | 64-bit | `uint64_t` |

Pointer-sized integers:

| Type    | Description             |
| ------- | ----------------------- |
| `isize` | signed, pointer-sized   |
| `usize` | unsigned, pointer-sized |

Rules:

* no implicit signed/unsigned conversion
* no implicit widening or narrowing
* overflow behavior follows the target C compiler and ABI

---

## 2 Floating-Point Types

| Type  | Description  | C Mapping |
| ----- | ------------ | --------- |
| `f32` | 32-bit float | `float`   |
| `f64` | 64-bit float | `double`  |

Rules:

* no implicit integer <-> float conversion
* all conversions must be explicit
* floating-point semantics follow the target platform

---

## 3 Boolean and Logic Types

```c
bool enabled = true;
trnry ternary = down;
```

| Type    | Description   |
| ------- | ------------- |
| `bool`  | binary logic  |
| `trnry` | ternary logic |

`bool`:

* valid values: `true`, `false`
* lowers to `uint8_t`

`trnry`:

* valid values: `down`, `middle`, `up`, `down[0-127]`, `up[0-127]`
* lower to `int8_t`
* represents three-state logic or expanded level of up/down logic
* semantics defined by usage, not enforced by the type system

No implicit truthiness exists:

```c
if (1) {}           // error
if (true) {}        // OK
if (1 is true) {}   // OK
```

---

## 4 Character Types

| Type    | Description        |
| ------- | ------------------ |
| `c8`    | UTF-8 byte         |
| `c16`   | UTF-16 unit        |
| `c32`   | UTF-32 codepoint   |

Character types are **numeric** and do not imply encoding validation.

---

## 5 `void`

```c
void f();
```

* represents the absence of a value
* cannot be stored, assigned, or passed
* only valid as a function return type or placeholder

---

## 6 Predefined Type Aliases

For familiarity and interoperability, the following aliases are provided:

| Alias    | Underlying Type |
| -------- | --------------- |
| `byte`   | `i8`            |
| `short`  | `i16`           |
| `int`    | `i32`           |
| `long`   | `i64`           |
| `ubyte`  | `u8`            |
| `ushort` | `u16`           |
| `uint`   | `u32`           |
| `ulong`  | `u64`           |
| `float`  | `f32`           |
| `double` | `f64`           |
| `char`   | `c8`            |
| `dchar`  | `c16`           |
| `qchar`  | `c32`           |

Aliases introduce **no new semantics** and exist only for readability.

---

## 7 Literal Typing Rules

| Literal                     | Type     |
| --------------------------- | -------- |
| `1`                         | `i32`    |
| `1u`                        | `u32`    |
| `1.0`                       | `f64`    |
| `1.0f`                      | `f32`    |
| `'a'`                       | `c8`     |
| `true` / `false`            | `bool`   |
| `up` / `middle` / `down`    | `trnry`  |
| `up[0-127]` / `down[0-127]` | `trnry`  |

Literals do adapt to type:

```c
i64 x = 1;    // will be inferred as i64
i64 x = 1i64; // same (explicit)
```

---

## 8 Literal Variants

Integers:

| Literal                          | Description                          |
| -------------------------------- | ------------------------------------ |
| `1`                              | Signed 32-bit integer                |
| `1i`                             | Signed 32-bit integer                |
| `1u`                             | Unsigned 32-bit integer              |
| `1i8`                            | (Explicit) Signed 8-bit integer      |
| `1u8`                            | (Explicit) Unsigned 8-bit integer    |
| `0xA`                            | Hexadecimal 8-bit integer            |
| `0b11`                           | Binary 8-bit integer                 |
| `0o67`                           | Octal 8-bit integer                  |
| `0xC3i`                          | Hexadecimal 8-bit signed integer     |
| `0o72u`                          | Octal 8-bit unsigned integer         |
| `0b1101i32`                      | (Explicit) Binary 32-bit integer     |


Floats:

| Literal                          | Description                          |
| -------------------------------- | ------------------------------------ |
| `1.0`                            | Signed 64-bit floating-point number  |
| `1.0f`                           | Signed 32-bit floating-point number  |
| `1.0f32`                         | Signed 32-bit floating-point number  |
| `1.0f64`                         | Signed 64-bit floating-point number  |

Characters:

| Literal                          | Description                          |
| -------------------------------- | ------------------------------------ |
| `'a'`                            | Signed 8-bit character               |
| `b'a'`                           | Signed 8-bit character  (BYTE)       |
| `w'a'`                           | Signed 16-bit character (WORD)       |
| `d'a'`                           | Signed 32-bit character (DWORD)      |

Booleans:

| Literal                          | Description                          |
| -------------------------------- | ------------------------------------ |
| `true=1` / `false=0`             | Boolean value                        |

Ternary:

| Literal                          | Description                          |
| -------------------------------- | ------------------------------------ |
| `down=-1` / `middle=0` / `up=1`  | Ternary value                        |
| `down[10] = -10`                 | Ternary negative value               |
| `up[10]   =  10`                 | Ternary positive value               |

String:

| Literal                          | Description                          |
| -------------------------------- | ------------------------------------ |
| `"hello"`                        | String literal                       |
| `t8"UTF-8"`                      | UTF-8 (default)                      |
| `t16"UTF-16"`                    | UTF-16                               |
| `t32"UTF-32"`                    | UTF-32                               |
| `bc"ByteChunk"`                  | [ByteChunk](../byte-chunk/encoding.md) network encoding           |
| `x"32CCEF2a"`                    | Hex bytes                            |
| `b"01110001"`                    | Binary bytes                         |

---

## **Summary**

* primitives are fixed-size, implicit & explicit, and predictable
* no implicit conversions of any kind
* aliases do not affect semantics
* literal types are strict and context-independent

---