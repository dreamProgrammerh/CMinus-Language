# Type System (Overview)

This document defines the **CMinus (C-) type system**.

The type system is designed to:

* stay close to C’s mental model
* avoid hidden runtime costs
* preserve performance predictability
* make ownership, allocation, and safety explicit
* provide expressive abstractions without semantic surprises

> This document is a **language contract**. The behavior described here is intentional and stable unless explicitly stated otherwise.

---

## Design Goals

The CMinus type system aims to:

* map cleanly and predictably to **C code**
* avoid implicit behavior and silent conversions
* make unsafe operations visible in syntax
* allow zero-cost abstractions
* keep performance characteristics obvious to the programmer

---

## Core Principles

### Explicit Cost

If an operation allocates memory, copies data, performs a runtime check, or introduces indirection, it must be **visible in code**.

The compiler may optimize implementations, but must not change observable behavior or introduce hidden work.

---

### C Compatibility

Every CMinus type must:

* have a direct C representation, or
* lower deterministically into C constructs

Generated C code is intended to be readable, inspectable, and debuggable.

---

### Explicit Safety

CMinus provides safety mechanisms, but never forces them.
Unsafe behavior is allowed, but must be written explicitly and locally.

---

## Predefined Type Aliases

For familiarity and interoperability, CMinus defines the following aliases:

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

These aliases introduce **no new semantics** and exist purely for readability and C compatibility.

---

## Primitive Types

Primitive types map directly to C primitives and have fixed, predictable behavior.

| Type       | Description                    | C Mapping  |
| ---------- | ------------------------------ | ---------- |
| `i8`–`i64` | signed integers                | `int*_t`   |
| `u8`–`u64` | unsigned integers              | `uint*_t`  |
| `f32`      | 32-bit floating point          | `float`    |
| `f64`      | 64-bit floating point          | `double`   |
| `isize`    | pointer-sized signed integer   | `int64_t`  |
| `usize`    | pointer-sized unsigned integer | `uint64_t` |
| `bool`     | boolean value                  | `uint8_t`  |
| `trnry`    | ternary logic                  | `int8_t`   |
| `void`     | no value                       | `void`     |

No implicit widening, narrowing, signedness, or boolean conversions occur.

---

## `const` and `final`

CMinus distinguishes **immutability** from **compile-time evaluation**.

### `const`

```c
const int x = runtimeValue();
```

* the value cannot be modified after initialization
* the value may depend on runtime data
* primarily used to express intent and enforce safety

The compiler may optimize `const` values when possible, but such optimizations must not affect program semantics.

### `final`

```c
final int y = 10;
```

* the value is known at compile time
* usable in compile-time contexts (types, tables, options, indices)
* guaranteed to be constant

The compiler may internally treat some `const` values as `final` when no runtime data is involved, but this is an optimization detail and not part of the language semantics.

---

## Pointer Types

Pointers behave like C pointers and follow C semantics.

```c
int* ptr;
const int* readOnly;
int* const fixed;
```

Properties:

* pointers may be null
* pointer arithmetic is allowed
* dereferencing invalid or null pointers is undefined behavior

No implicit runtime checks are introduced.

---

## Reference Types

References represent **non-null, stable aliases** to existing objects.

```c
int& ref = value;
```

Semantics:

* references are never null
* always refer to a valid object
* cannot be rebound to refer to a different object

References lower to `T* const` in C, but with stronger semantic guarantees enforced by the compiler.

---

## Optional Types (`T?`)

Optional types represent values that may fail or be absent.

```c
string? readFile(str path);
```

Optional is **not** a variant type.

Conceptually, an optional is equivalent to:

```c
struct Optional<T> {
    T value;
    u8 errorCode;
}
```

* `errorCode == 0` indicates success
* non-zero values represent explicit failure reasons

```c
return none[12];
```

Optional values must be explicitly handled by the caller. They are never implicitly removed, ignored, or optimized away.

---

## Allocation and `new`

CMinus separates **value semantics** from **allocation semantics**.

```c
let a = value;      // value semantics
let b = new value;  // explicit heap allocation
```

Rules:

* `new` always allocates on the heap
* assignment may copy or move values depending on type-defined behavior
* allocation without `new` is explicit and type-defined

### Strings

* `string` is always heap-allocated and resizable
* `str` is stack-allocated by default
* `str*` may reference heap-allocated string data

---

## Arrays

### Fixed-Size Arrays

```c
int[10] values;
```

* contiguous storage
* compile-time size
* direct C array mapping

### Length-Prefixed Arrays

```c
int[~10] arr;
```

* stores length alongside data
* O(1) length access
* safer alternative to raw C arrays

---

## Slices

```c
int[] slice;
```

Slices consist of:

* a pointer
* a length

Slices do not own memory and never allocate implicitly.

---

## Struct Types

```c
struct Point {
    int x;
    int y;
}
```

* predictable memory layout
* field order preserved
* padding only when required by ABI or optimization

---

## Union Types

```c
union Color {
    u32 rgba;
    struct { u8 r; u8 g; u8 b; u8 a; };
}
```

* overlapping storage
* predictable layout
* no hidden tagging

---

## Variant Types

```c
variant{int, string} value;
```

* tagged union
* exactly one active value at a time
* explicit type checks and casts required

Variants do not include error semantics; they represent data alternatives only.

---

## Data-Oriented Types (`data`)

```c
data Player {
    hot vec3<float> position;
    hot vec3<float> velocity;
    warm i64 score;
    cold string name;
}
```

Data types describe logical structures that are split into multiple physical layouts based on access frequency.

Properties:

* cache-friendly layout
* automatic padding and grouping optimization
* semantic transparency to the programmer

---

## Class Types

```c
class Person {
    string name;
    uint age;
}
```

* classes are struct-based
* inheritance supported
* dispatch resolved at compile time
* no hidden runtime polymorphism

---

## Function Types

```c
int add(int a, int b);
```

Functions are first-class values with statically known signatures.

---

## Generics

Generics are available on:

* structs
* classes
* functions
* typedefs
* tables
* options

Runtime-generating constructs are monomorphized per used type.
Compile-time-only constructs use generics for validation and vanish after compilation.

---

## Compile-Time Constructs

### `typedef`

```c
typedef Name = Complex<Type>;
```

* compile-time only
* no runtime cost
* may be generic
* used to improve readability and abstraction

---

### `options`

Scoped named constants bound to a base type.

```c
options<i8>{less=-1, equal=0, more=1}
```

* compile-time only
* zero runtime cost
* auto-counter assignment when applicable

---

### `interface`

Interfaces define **compile-time prototypes** for structs and classes.

```c
interface Drawable {
    void draw();
}
```

* must be explicitly implemented
* fields and methods allowed
* default implementations supported
* no runtime behavior or layout impact

---

### `table`

Compile-time static lookup tables.

```c
table<u32> Colors {
    "red" = 0xFF0000,
};
```

* keys must be compile-time constants
* values stored as a static `T[]`
* all lookups lower to index-based access

---

## Performance Model

The CMinus type system guarantees:

* no garbage collection
* no hidden reference counting
* no runtime reflection 
* predictable lowering to C

---

## Non-Goals

The type system intentionally does not provide:

* implicit memory management
* runtime polymorphism
* hidden lifetimes
* automatic safety checks

All such behavior must be explicitly requested and visible in code.