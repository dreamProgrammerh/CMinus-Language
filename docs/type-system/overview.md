# Type System (Overview)

This document describes the **CMinus (C-) type system** and how each type behaves in practice.

The type system is intentionally explicit:
- no hidden allocations
- no silent conversions
- no invisible safety or performance costs

Everything described here is **observable in syntax**.

---

## Predefined Type Aliases

For familiarity and C compatibility, the following aliases exist:

```c
byte   = i8
short  = i16
int    = i32
long   = i64

ubyte  = u8
ushort = u16
uint   = u32
ulong  = u64

float  = f32
double = f64
````

These are pure aliases and introduce no new behavior.

---

## Variable Declarations

### `let`, `const`, `final`

CMinus has three declaration keywords:

```c
let     // mutable
const   // immutable (runtime)
final   // compile-time constant
```

`let` is the **default** and may be omitted.

```c
int a = 5;
let int a = 5;    // same as above
```

If no type is specified, it is inferred from the right-hand side:

```c
let a = 1;        // inferred i32
const b = 1;      // inferred i32, immutable
final c = 1;      // inferred i32, compile-time
```

Rules:

* inference happens once at declaration
* inferred type never changes
* ambiguous inference is a compile-time error

```c
let x = null; // error
```

---

## `const` vs `final`

### `const`

```c
const int x = runtimeValue();
```

* value cannot be reassigned
* value may depend on runtime data
* primarily a safety guarantee

### `final`

```c
final int y = 10;
```

* value is known at compile time
* usable in types, tables, options, array sizes, etc.
* required for any compile-time context

The compiler *may* fold some `const` values into `final`, but this is an optimization, not a language rule.

---

## Primitive Types

Primitive types map directly to C primitives.

| Type             | Notes             |
| ---------------- | ----------------- |
| `i8`--`i64`       | signed integers   |
| `u8`--`u64`       | unsigned integers |
| `f32`, `f64`     | floating point    |
| `isize`, `usize` | pointer-sized     |
| `bool`           | boolean           |
| `trnry`          | ternary logic     |
| `void`           | no value          |

There are:

* no implicit casts
* no implicit widening or narrowing
* no implicit boolean conversions

---

## Pointers

Pointers behave like C pointers.

```c
int* p;
const int* rp;
int* const fp;
```

Properties:

* may be null
* pointer arithmetic is allowed
* invalid dereference is undefined behavior
* no implicit runtime checks

---

## References (`&`)

References are **non-null aliases**.

```c
int& r = value;
```

Rules:

* never null
* always bound to a valid object
* cannot be rebound

Conceptually:

```c
int&   -> int* const   (with non-null guarantee)
```

---

## Optional Types (`T?`)

Optional types represent failure-capable values.

```c
string? readFile(str path);
```

Optional is **not** a variant.

Conceptually:

```c
struct Optional<T> {
    T  value;
    u8 error;
}
```

* `error == 0` → success
* non-zero error → failure reason

```c
return none[12];   // explicit error code
```

Optional values **must** be handled.
They are never silently ignored or optimized away.

---

## Allocation and `new`

CMinus separates **value semantics** from **allocation**.

```c
let a = value;      // stack / value semantics
let b = new value;  // explicit heap allocation
```

Rules:

* `new` always allocates on the heap
* assignment behavior is type-defined
* no implicit heap allocation

### Strings

* `string` → always heap allocated (growable)
* `str` → stack by default
* `str*` → pointer to string storage

---

## Arrays

### Fixed-Size Arrays

```c
int[10] values;
```

* contiguous
* compile-time size
* maps directly to C arrays

### Length-Prefixed Arrays

```c
int[~10] arr;
```

* length stored alongside data
* O(1) length access
* safer than raw C arrays

---

## Slices

```c
int[] slice;
```

A slice is:

* pointer
* length

Slices:

* do not own memory
* never allocate
* are safe only when used correctly

---

## Tuples

Tuples are **inline anonymous structs**.

```c
(int, string)
```

Equivalent to:

```c
struct {
    int    $0;
    string $1;
}
```

Named tuples:

```c
(int code, string state)
```

Equivalent to:

```c
struct {
    int    code;
    string state;
}
```

Rules:

* if one element is named, all must be named
* zero runtime cost
* purely structural

---

## Structs

```c
struct Point {
    int x;
    int y;
}
```

* predictable layout
* field order preserved
* padding only when required

---

## Unions

```c
union Color {
    u32 rgba;
    struct { u8 r, g, b, a; };
}
```

* overlapping storage
* no hidden tagging
* ABI-visible layout

---

## Variants

```c
variant{int, string} v;
```

* tagged union
* one active value at a time
* explicit checks required

```c
if (v is int) {
    int x = v as int;
}
```

Variants are **data-only**, not error types.

---

## Data Types (`data`)

```c
data Player {
    hot  vec3<float> position;
    hot  vec3<float> velocity;
    warm i64 score;
    cold string name;
}
```

Data types:

* split fields into multiple layouts
* optimize cache behavior
* behave like structs semantically

---

## Classes

```c
class Person {
    string name;
    uint age;
}
```

* struct-based
* inheritance supported
* compile-time dispatch
* no hidden runtime polymorphism

---

## Functions

```c
int add(int a, int b);
```

Functions:

* have fixed signatures
* are first-class values
* may be assigned, passed, and stored

---

## Generics

Generics exist on:

* structs
* classes
* functions
* typedefs
* tables
* options

Rules:

* runtime types are monomorphized
* compile-time-only constructs vanish after validation
* no runtime generic metadata

---

## `typedef`

```c
typedef Name = Complex<Type>;
```

* compile-time only
* zero runtime cost
* may be generic
* improves readability

---

## `options`

Scoped named constants bound to a type.

```c
options<i8>{less=-1, equal=0, more=1}
```

Properties:

* compile-time only
* zero runtime cost
* optional auto-counter behavior

### Scope Priority

```c
int x = 2;

options<int>{x, y} a = x;   // variable x → 2
options<int>{x, y} b = :x;  // option constant x → 0
```

`:` explicitly refers to the option constant.

---

## Interfaces

```c
interface Drawable {
    draw();
}
```

Interfaces:

* are compile-time only
* must be explicitly implemented
* may define fields, methods, constructors
* may provide `default` implementations
* introduce no runtime layout

---

## Tables

```c
table<string> Colors {
    "red"    = "red color",
    0x00FF00 = "green hex",
    10       = "special",
};
```

Rules:

* keys must be compile-time constants
* keys are mapped to indices at compile time
* values stored as static `T[]`

Access:

```c
Colors["red"];     // compile-time lookup
Colors::"red";    // index
Colors![3];       // runtime index (panic)
Colors?[3];       // runtime index (optional)
```

---

## Generic Constants (Minimal)

Generic constants are **compile-time values used to configure types**.

```c
struct string<u32 Capacity = 256 -> Capacity > 32> { ... }
```

Properties:

* must satisfy compile-time conditions
* never exist at runtime
* commonly used for capacities, limits, layout control

---

## Compile-Time Type Properties

Available in compile-time contexts:

```c
sizeof(T)
alignof(T)
nameof(T)
typeof(expr)
typeid(T)

minof(T)
maxof(T)
defaultof(T)
lengthof(array)
```

All of these:

* are evaluated at compile time
* have no runtime cost
* are forbidden in runtime-only contexts
