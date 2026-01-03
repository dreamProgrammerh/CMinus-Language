# **Compile-Time Type Properties**

CMinus provides a set of **compile-time property operators** used to query information about types and constant values.

These operators:

* are evaluated at compile time
* never generate runtime code
* return constant values
* may be used in type expressions, generic constraints, and initializers

They behave similarly to built-in compile-time functions.

---

## Overview

Compile-time property operators include:

* `sizeof(T)`
* `alignof(T)`
* `lengthof(T)`
* `nameof(T)`
* `typeof(expr)`
* `typeid(T)`
* `minof(T)`
* `maxof(T)`
* `defaultof(T)`

All of them return compile-time constants.

---

## `sizeof(T)`

Returns the size of a type in bytes.

```c
sizeof(int)      // 4
sizeof(string)   // implementation-defined
```

Rules:

* evaluated at compile time
* equivalent to C `sizeof`
* valid only for complete types

---

## `alignof(T)`

Returns the required alignment of a type in bytes.

```c
alignof(int)
alignof(Point)
```

Used for layout control and low-level memory handling.

---

## `lengthof(X)`

Returns the length of a compile-time-known container.

Applicable to:

* fixed-size arrays
* tuples
* strings with known capacity
* tables

Examples:

```c
lengthof(int[10])        // 10
lengthof((int, int))     // 2
lengthof(string<32>)     // 32
```

---

## `nameof(T)`

Returns the compile-time name of a type or symbol.

```c
nameof(int)        // "int"
nameof(MyStruct)   // "MyStruct"
```

Used for diagnostics, debugging, and code generation.

---

## `typeof(expr)`

Returns the type of an expression.

```c
typeof(1 + 2)       // int
typeof(value)       // inferred type
```

Used mainly in generics and compile-time logic.

---

## `typeid(T)`

Returns a unique compile-time identifier representing a type.

```c
typeid(int)
```

Properties:

* constant
* comparable
* stable within a compilation
* not a runtime RTTI object

Used for:

* compile-time maps
* specialization logic
* static dispatch decisions

---

## `minof(T)` and `maxof(T)`

Return the minimum and maximum representable values of a numeric type.

```c
minof(i32)   // -2147483648
maxof(i32)   //  2147483647
```

Applicable to:

* integer types
* floating-point types
* custom numeric types that define bounds

---

## `defaultof(T)`

Returns the default value for a type.

```c
defaultof(int)        // 0
defaultof(bool)       // false
defaultof(string)     // empty string
```

Used for:

* initialization
* generics
* compile-time fallback logic

---

## Usage in Constraints

These operators may appear inside generic constraints:

```c
<uint N -> N < maxof(u16)>
```

Or:

```c
<T -> sizeof(T) <= 64>
```

---

## Summary

* all operators are compile-time only
* produce constant values
* usable in generics and constraints
* introduce no runtime cost
* enable expressive static reasoning
* never allocate or execute code
