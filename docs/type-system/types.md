# Type System

This document defines the **CMinus (C-) type system**.

The type system is designed to:
- stay close to C’s mental model
- avoid hidden or surprising runtime behavior
- preserve performance predictability
- provide a rich set of primitive and composite types

> This is a **design document**, not a compiler reference.


## Goals of the CMinus Type System

The CMinus type system is designed with a small set of strict goals that guide all design decisions.
Anything not listed here is intentionally out of scope.

### 1. Predictable Mapping to C

Every construct in the type system must either:

* map directly to a C construct, or
* lower deterministically into C code

Generated C should be readable, debuggable, and understandable by humans (when show c enabled).

---

### 2. Explicit Cost Model

All operations that may:

* allocate memory
* copy data
* perform runtime checks
* introduce indirection

must be **visible in source code**.

Nothing expensive happens implicitly.

---

### 3. No Hidden Runtime Systems

The type system must not introduce:

* garbage collection
* reference counting
* hidden allocators
* hidden metadata
* implicit lifetime tracking

Any runtime behavior must be explicitly requested.

---

### 4. Strong but Honest Safety

CMinus supports safety mechanisms, but never enforces them silently.

* unsafe operations are allowed
* safe alternatives exist
* safety is opt-in and explicit
* unsafe behavior must be visible in syntax

The language favors **clarity over protection**.

---

### 5. Zero-Cost Abstractions

High-level constructs must compile away cleanly.

* abstractions should not cost more than hand-written C
* generics are monomorphized
* interfaces vanish after validation
* helpers exist only at compile time

If abstraction adds runtime cost, it must be explicit.

---

### 6. Deterministic Compilation

The same input must always generate the same output.

* no reflection
* no runtime type discovery
* no hidden dispatch
* no environment-dependent behavior

---

### 7. Explicit Ownership and Allocation

Memory ownership is never inferred.

* stack vs heap is explicit
* `new` is required for heap allocation
* lifetimes are programmer-managed
* references and pointers are distinct

---

### 8. Composability Over Magic

Features should compose cleanly instead of relying on special cases.

* types compose with generics
* options compose with tables
* interfaces compose with generics
* safety operators compose with all access forms

---

### 9. Practical Systems Focus

The type system is designed for:

* systems programming
* engines and tooling
* embedded development
* performance-critical software
* C interoperability

Not for scripting or dynamic environments.

---

### 10. Language as a Tool, Not a Policy Engine

CMinus does not attempt to:

* enforce programming paradigms
* prevent all errors
* impose architectural styles
* enforce ownership models

It provides tools — not ideology.

---

## Design Principles

### 1. No Hidden Cost

If an operation allocates memory, performs a copy, or introduces indirection,  
it should be **visible in the code** whenever possible.

> This principle is applied by design, but may be refined through discussion.

### 2. C Compatibility First

Every CMinus type must have:
- a clear C representation, or
- a clearly documented transformation into C

### 3. Safety Is Optional, Not Mandatory

CMinus provides **safety tools**, but never forces them.
Unsafe behavior must be explicit and local.

---

## **0. Variable Declarations, Modifiers, and Binding Rules**

CMinus separates **type**, **mutability**, and **compile-time guarantees**.
Variable declarations consist of:

* an optional **modifier** (`let`, `const`, `final`)
* an optional **explicit type**
* a required **initializer** (except in specific cases)

---

### 0.1 Default Declaration (`let`)

`let` is the **default modifier** and may be omitted.

```c
int a = 5;
let int a = 5;   // identical
```

If no modifier is specified, `let` is implicitly applied.

Properties of `let`:

* mutable
* runtime value
* type fixed at declaration

---

### 0.2 Untyped Declarations (Type Inference)

If no type is specified, the compiler infers the type from the right-hand side.

```c
let a = 1;       // i32
const b = 1;     // i32
final c = 1;     // i32
```

Inference rules:

* performed **once**, at declaration
* based solely on the initializer
* never changes afterward
* no dynamic typing

If inference is ambiguous or impossible, it is a compile-time error.

```c
let x = null;    // error
```

---

### 0.3 `const` — Runtime Immutability

```c
const int a = readValue();
```

* value cannot be reassigned
* value may depend on runtime data
* enforces immutability for safety and intent

`const` does **not** imply compile-time evaluation.

The compiler may optimize `const` values when provably safe, but this is not a semantic guarantee.

---

### 0.4 `final` — Compile-Time Constant

```c
final int size = 32;
```

* value is known at compile time
* usable in compile-time contexts (types, options, tables, indices)
* cannot depend on runtime values

```c
final int x = readValue(); // error
```

The compiler may internally promote some `const` values to `final` when no runtime data is involved, but this is an optimization detail and not observable behavior.

---

### 0.5 Reassignment Rules

```c
let a = 1;
a = 2;       // allowed

const b = 1;
b = 2;       // error

final c = 1;
c = 2;       // error
```

* `let`: reassignment allowed
* `const`: reassignment forbidden
* `final`: reassignment forbidden and compile-time bound

---

### 0.6 Explicit Type Overrides Inference

When a type is specified, inference is bypassed.

```c
let int a = 1;       // explicit
let int a = 1.0;     // error
```

Explicit typing always wins over inferred typing.

---

### 0.7 Binding vs Allocation

Declaration and binding **do not imply allocation strategy**.

```c
let a = value;
```

* does not guarantee stack or heap allocation
* allocation behavior is determined by:

  * the type
  * assignment semantics
  * use of `new`

Allocation rules are defined in the allocation section.

---

### 0.8 Summary

* `let` is default and implicit
* `const` enforces runtime immutability
* `final` enforces compile-time availability
* inference is local, single-shot, and strict
* no implicit conversions
* no dynamic typing

---

## **1. Primitive Types and Literal Semantics**

Primitive types are the lowest-level value types in CMinus.
They map directly and predictably to C primitives and introduce **no hidden behavior**.

---

### 1.1 Integer Types

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

### 1.2 Floating-Point Types

| Type  | Description  | C Mapping |
| ----- | ------------ | --------- |
| `f32` | 32-bit float | `float`   |
| `f64` | 64-bit float | `double`  |

Rules:

* no implicit integer <-> float conversion
* all conversions must be explicit
* floating-point semantics follow the target platform

---

### 1.3 Boolean and Logic Types

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

* valid values: `up`, `down`, `middle`, `up[0-127]`, `down[0-127]`
* lower to `int8_t`
* represents three-state logic or expanded level of up/down logic
* semantics defined by usage, not enforced by the type system

No implicit truthiness exists:

```c
if (1) {}     // error
if (true) {}  // OK
```

---

### 1.4 Character Types

| Type    | Description        |
| ------- | ------------------ |
| `c8`    | UTF-8 byte         |
| `c16`   | UTF-16 unit        |
| `c32`   | UTF-32 codepoint   |

Character types are **numeric** and do not imply encoding validation.

---

### 1.5 `void`

```c
void f();
```

* represents the absence of a value
* cannot be stored, assigned, or passed
* only valid as a function return type or placeholder

---

### 1.6 Predefined Type Aliases

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

### 1.7 Literal Typing Rules

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

### 1.8 Literal Variants

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
| `u8"UTF-8"`                      | UTF-8 (default)                      |
| `u16"UTF-16"`                    | UTF-16                               |
| `u32"UTF-32/Runes"`              | UTF-32                               |
| `bc"ByteChunk"`                  | ByteChunk network encoding           |
| `x"32CCEF2a"`                    | Hex bytes                            |
| `b"01110001"`                    | Binary                               |

---

### 1.9 Summary

* primitives are fixed-size, implicit & explicit, and predictable
* no implicit conversions of any kind
* aliases do not affect semantics
* literal types are strict and context-independent

---

## **2. Tuples (Inline Struct Types)**

Tuples in CMinus are **anonymous, inline struct types**.
They exist to group values without defining a named `struct`.

A tuple is a **value type** with a fixed layout determined at compile time.

---

### 2.1 Basic Tuple Syntax

```c
(int, string)
```

This is equivalent to the anonymous struct:

```c
struct {
    int $0;
    string $1;
}
```

* fields are stored in order
* layout is predictable
* no hidden padding beyond ABI requirements

---

### 2.2 Tuple Values

```c
let t = (1, "ok");
```

Type inferred as:

```c
(int, string)
```

Field access uses positional names:

```c
t.$0;   // int
t.$1;   // string
```

---

### 2.3 Named Tuples

Tuple fields may be explicitly named.

```c
(int code, string state)
```

Lowers to:

```c
struct {
    int code;
    string state;
}
```

Rules:

* if **any** field is named, **all fields must be named**
* partial naming is a compile-time error

```c
(int code, string)   // error
```

---

### 2.4 Tuple Construction

```c
(int, int) p = (10, 20);
```

Order and type must match exactly.

```c
(int, int) p = (20, 10);     // OK
(int, int) p = (10, 20, 30); // error
```

---

### 2.5 Tuple Assignment and Copying

Tuples follow normal value semantics:

```c
let a = (1, "x");
let b = a;    // value copy
```

Copy behavior depends on the contained types.

---

### 2.6 Tuples in Function Returns

Tuples are commonly used for multiple return values.

```c
(int, string) parse();
```

Usage:

```c
let result = parse();
println(result.$0);
println(result.$1);
```

Named tuple returns improve readability:

```c
(int code, string message) parse();
```

---

### 2.7 Tuples vs Structs

| Tuples                  | Structs             |
| ----------------------- | ------------------- |
| anonymous               | named               |
| inline                  | reusable            |
| positional by default   | named fields        |
| no separate declaration | explicit definition |

Tuples do not support:

* methods
* inheritance
* layout directives

---

### 2.8 Summary

* tuples are inline, anonymous structs
* field order defines layout
* naming is all-or-nothing
* no runtime overhead
* ideal for grouping temporary or return values

---

## **3. Struct Types**

Structs define **named aggregate types** with a fixed layout and explicit fields.
They are the primary mechanism for modeling structured data in CMinus.

Structs are value types and map directly to C `struct`.

---

### 3.1 Defining a Struct

```c
struct Point {
    int x;
    int y;
}
```

This defines a new type `Point` with two fields.

Equivalent C representation:

```c
struct Point {
    int x;
    int y;
};
```

---

### 3.2 Field Access

```c
Point p;
p.x = 10;
p.y = 20;
```

Field access is direct and has no runtime overhead.

---

### 3.3 Default Values & Constant

```c
struct Point {
    int x = 0;
    int y = 10;
}

Point p; // x = 0, y = 10

struct Point3D {
    const int x;
    const int y = 10;
    int z = 20;
}

Point3D p3d{ x: 10, y: 20, z: 30 };
p3d.x = 100; // error: x is const
p3d.y = 200; // error: y is const
p3d.z = 300; // ok
```

---

### 3.3 Initialization

Structs may be initialized using positional or named initialization.

```c
Point p1 = Point{ 10, 20 };
Point p2 = Point{ x: 10, y: 20 };
Point p2{ x: 10, y: 20 };   // short-hand
```

Rules:

* positional initialization follows field order
* named initialization may appear in any order
* initialization cannot be mixed is either positional or named
* missing fields are an error unless defaults exist
* constants cannot change after initialization

---

### 3.4 Value Semantics

Structs are value types.

```c
Point a = Point{1, 2};
Point b = a;   // full copy
```

Copying copies all fields recursively.

No implicit heap allocation occurs.

---

### 3.5 Layout and Padding

Struct layout is:

* deterministic
* field order preserved
* padding inserted only when required by ABI or alignment rules

The compiler may apply layout optimizations **only if explicitly enabled**.

---

### 3.6 Structs and Assignment

Assignment replaces the entire value:

```c
p = Point{3, 4};
```

Partial assignment is not allowed unless explicitly supported via field access.

---

### 3.7 Structs as Parameters and Returns

```c
Point make() {
    return Point{1, 2};
}
```

Passing and returning structs follows value semantics unless optimized by the compiler.

---

### 3.8 Nested Structs

```c
struct Rect {
    Point min;
    Point max;
}
```

Nested structs are laid out inline.

---

### 3.9 Structs vs Tuples

| Struct               | Tuple                           |
| -------------------- | ------------------------------- |
| named type           | anonymous type                  |
| reusable             | inline only                     |
| explicit field names | positional by default           |
| suitable for APIs    | suitable for temporary grouping |

---

### 3.10 Summary

* structs define named, reusable aggregate types
* layout is predictable and C-compatible
* copied by value
* no hidden allocation
* foundation for higher-level abstractions

---

## **4. Union Types**

Union types define a value that can occupy **one of several overlapping representations**.
All members share the same memory region.

Unions are low-level constructs and map directly to C `union`.

---

### 4.1 Defining a Union

```c
union Color {
    u32 rgba;
    struct {
        u8 r, g, b, a;
    };
}
```

This defines a single memory block that can be viewed in multiple ways.

---

### 4.2 Memory Layout

* all fields start at offset 0
* total size equals the size of the largest field
* alignment follows the strictest member
* no hidden tag or metadata

Equivalent C:

```c
union Color {
    uint32_t rgba;
    struct {
        uint8_t r, g, b, a;
    };
};
```

---

### 4.3 Access Rules

```c
Color c;
c.rgba = 0xFF00FF00;

u8 red = c.r;
```

The language does not track which field is “active”.

Reading a field different from the last written one is allowed but semantically unsafe unless explicitly intended.

---

### 4.4 Safety and Responsibility

Unions provide **no safety guarantees**:

* no active-member tracking
* no runtime checks
* no automatic initialization
* no destruction semantics

Correct usage is entirely the programmer’s responsibility.

---

### 4.5 Use Cases

* bit-level reinterpretation
* ABI compatibility
* packed data formats
* performance-critical code
* low-level memory access

---

### 4.6 Union vs Variant

| Union         | Variant                  |
| ------------- | ------------------------ |
| no tag        | tagged                   |
| unsafe        | checked                  |
| zero overhead | small overhead           |
| C-compatible  | higher-level abstraction |

Use a `union` when layout and control matter more than safety.

---

### 4.7 Summary

* unions overlap memory
* no runtime checks
* layout is predictable
* matches C behavior exactly
* intended for low-level work

---

## **5. Variant Types**

Variant types represent a value that can hold **one of several explicitly listed types**, with a runtime tag indicating which one is active.

Variants are *tagged unions* and provide controlled, type-safe alternatives to raw unions.

---

### 5.1 Declaring a Variant

```c
variant{int, string} value;
```

This declares a variable that can hold either an `int` or a `string`.

Internally, a variant consists of:

* a storage region large enough for the largest member
* a discriminator (tag) identifying the active type

---

### 5.2 Assigning to a Variant

```c
value = 10;
value = "hello";
```

Assignment sets both the stored value and the active tag.

Only types listed in the variant are allowed.

---

### 5.3 Type Checking

To access the stored value, the active type must be checked explicitly.

```c
if (value is int) {
    int x = value as int;
}
```

Rules:

* `is` checks the active variant type
* `as` extracts the value
* extraction without checking is a compile-time error (can be change)

---

### 5.4 Exhaustiveness

All possible variant alternatives must be considered when required by context.

```c
variant{int, string} v;

if (v is int) {
    ...
} else {
    // guaranteed to be string
}
```

---

### 5.5 Variant Assignment Semantics

Assigning a new value:

* destroys the previous value (if needed)
* updates the active tag
* stores the new value

No implicit conversions occur between variant alternatives.

---

### 5.6 Variants vs Optionals

Variants represent **data alternatives**, not failure states.

```c
variant{int, none}
```

is not equivalent to:

```c
int?
```

Key differences:

| Variant                   | Optional                |
| ------------------------- | ----------------------- |
| many alternatives         | success/failure only    |
| tagged by type            | tagged by error code    |
| explicit pattern checking | explicit error handling |
| general data modeling     | error/result modeling   |

---

### 5.7 Variants vs Unions

| Variant            | Union             |
| ------------------ | ----------------- |
| tagged             | untagged          |
| checked access     | unchecked         |
| safer              | unsafe            |
| small runtime cost | zero runtime cost |

---

### 5.8 Summary

* variants are tagged unions
* only declared types are allowed
* access requires explicit checks
* safer alternative to unions
* not used for error signaling

---

## **6. Optional Types (`T?`)**

Optional types represent **operations that may fail** or **values that may be absent**, using an explicit and lightweight error model.

They are *not* variants and are *not* nullable pointers.

---

### 6.1 Conceptual Model

An optional type:

```c
T?
```

is conceptually equivalent to:

```c
struct Optional<T> {
    T value;
    u8 error;
}
```

Rules:

* `error == 0` -> success
* `error != 0` -> failure code
* the value is only valid when `error == 0`

This model is fixed and intentional.

---

### 6.2 Creating Optional Values

## Success value

```c
return value;
```

implicitly means:

```c
return value with error = 0;
```

## Failure value

```c
return none[12];
```

This produces an optional with:

* no usable value
* error code = `12`

Error codes are user-defined and meaningful only by convention.

---

### 6.3 Declaring Optional Types

```c
string? readFile(str path);
```

This declares a function that may fail.

The caller **must handle** the optional.

---

### 6.4 Mandatory Handling

Optional values cannot be used directly.

```c
string? s = readFile("a.txt");

print(s);     // ✕ error
```

They must be explicitly handled using one of the access operators.

This rule is enforced even if the compiler can prove no failure occurs.

---

### 6.5 Access Operators

Optional access uses explicit operators:

## Safe access

```c
s?.length
```

* returns another optional
* propagates failure automatically

---

## Forced access (panic on failure)

```c
s!.length
```

* assumes success
* triggers panic if error != 0

---

## Conditional handling

```c
if (s == none[12]) {
    println("file not found");
}
```

Comparison is allowed only against `none[...]`.

---

### 6.6 Using Optionals in Control Flow

```c
string? s = readFile("a.txt");

if (s != none) {
    println(s!.length);
}
```

You must explicitly acknowledge failure paths.

---

[]]6.7 Optional vs Variant

Optional is **not** a variant:

```c
variant{string, none}  // ✕ not equivalent
```

Differences:

| Optional               | Variant                |
| ---------------------- | ---------------------- |
| fixed error channel    | arbitrary alternatives |
| compact representation | tagged union           |
| error-oriented         | data-oriented          |
| mandatory handling     | pattern matching       |

---

### 6.8 Optional vs Null

CMinus does not use `null` for error signaling.

* pointers may be null
* optionals encode failure explicitly
* `none` is not a value, but an error marker

---

### 6.9 Compile-Time Behavior

* `T?` is a distinct type
* cannot be implicitly converted to `T`
* cannot be erased or optimized away semantically
* compiler may optimize representation if safe

---

### 6.10 Summary

* `T?` represents success or failure
* uses error codes, not variants
* forces explicit handling
* lightweight and predictable
* not nullable pointers
* not variants

---

## **7. Pointer Types (`T*`)**

Pointer types represent **raw memory addresses**.
They behave similarly to C pointers and provide no automatic safety.

Pointers are intentionally low-level and explicit.

---

### 7.1 Pointer Declaration

```c
int* p;
```

This declares a pointer to an `int`.

The pointer may be:

* uninitialized
* null
* pointing to valid memory
* pointing to invalid memory

No checks are inserted automatically.

---

### 7.2 Pointer Initialization

```c
int x = 10;
int* p = &x;
```

The address-of operator (`&`) produces a pointer.

---

### 7.3 Null Pointers

Pointers may be null:

```c
int* p = null;
```

Dereferencing a null pointer is undefined behavior.

---

### 7.4 Dereferencing

```c
int value = *p;
```

Dereferencing reads or writes the pointed value.

If the pointer does not point to valid memory, behavior is undefined.

---

### 7.5 Pointer Assignment

```c
int a = 1;
int b = 2;

int* p = &a;
p = &b;   // allowed
```

Pointers are reassignable unless declared `const`.

---

### 7.6 Const and Pointer Placement

Const placement follows C semantics.

```c
const int* p;       // pointer to const int
int* const p;       // const pointer to int
const int* const p; // const pointer to const int
```

Meaning:

* `const int*` -> value cannot be modified
* `int* const` -> pointer cannot be reassigned
* both together -> neither can change

---

### 7.7 Pointer Arithmetic

Pointer arithmetic is allowed.

```c
p + 1;
p - 2;
```

Semantics:

* scaled by the pointed type size
* identical to C behavior
* no bounds checking

---

### 7.8 Pointer Comparison

Pointers may be compared:

```c
p1 == p2;
p1 < p2;
p1 > p2;
```

Results are meaningful only when pointers refer to the same allocation region.

---

### 7.9 Pointer Safety Model

Pointers provide **no safety guarantees**:

* null dereference -> undefined behavior
* out-of-bounds access -> undefined behavior
* lifetime violations -> undefined behavior

All pointer safety must be enforced manually or via higher-level abstractions.

---

### 7.10 Pointer vs Reference

| Pointer (`T*`)       | Reference (`T&`) |
| -------------------- | ---------------- |
| nullable             | never null       |
| rebindable           | not rebindable   |
| arithmetic allowed   | no arithmetic    |
| unsafe               | safer            |
| explicit dereference | implicit         |

---

### 7.11 Lowering

Pointers lower directly to C pointers with identical semantics.

No metadata, tracking, or checks are added.

---

### 7.12 Summary

* pointers represent raw memory addresses
* fully compatible with C
* unsafe by design
* explicit nullability
* explicit dereferencing
* no automatic checks

---

## **8. Reference Types (`T&`)**

References represent **non-null, stable aliases** to existing objects.
They provide safer access than pointers while preserving zero-cost semantics.

A reference always refers to a valid object and cannot be reseated.

---

### 8.1 Reference Declaration

```c
int x = 10;
int& r = x;
```

This binds `r` to `x`.

Rules:

* references must be initialized
* initialization must bind to a valid object
* references cannot be null

---

### 8.2 Assignment Through References

```c
r = 20;
```

Modifies the referenced object (`x`).

---

### 8.3 Reference Rebinding Is Not Allowed

```c
int a = 1;
int b = 2;

int& r = a;
r = b;   // assigns value of b to a
```

This does **not** rebind the reference.

Rebinding is illegal and impossible.

---

### 8.4 Reference Lifetime Rules

A reference must not outlive the object it refers to.

Binding to a temporary or invalid object is forbidden:

```c
int& r = 10;   // error
```

---

### 8.5 Reference Semantics

* always non-null
* always points to a valid object
* cannot change target
* behaves like an alias

Conceptually equivalent to:

```c
T* const
```

but with stronger compile-time guarantees.

---

### 8.6 References vs Pointers

| Feature             | Reference (`T&`) | Pointer (`T*`) |
| ------------------- | ---------------- | -------------- |
| Null                | ✕                | ✓              |
| Rebind              | ✕                | ✓              |
| Arithmetic          | ✕                | ✓              |
| Must be initialized | ✓                | ✕              |
| Dereference syntax  | implicit         | explicit       |
| Safety              | higher           | lower          |

---

### 8.7 Passing by Reference

```c
void inc(int& x) {
    x += 1;
}
```

This avoids copying and guarantees a valid target.

---

### 8.8 References and ABI

References lower to pointers at the ABI level, but the compiler enforces:

* non-null
* no rebinding
* valid lifetime

These rules exist only at compile time.

---

### 8.9 Summary

* references are non-null aliases
* always bound at initialization
* cannot be reseated
* safer than pointers
* zero runtime cost

---

## **9. Allocation Model and the `new` Keyword**

CMinus separates **value semantics** from **allocation semantics**.
Declaring or assigning a value does *not* imply where it is stored. Allocation behavior is explicit and controlled.

---

### 9.1 Value Semantics vs Allocation

By default, expressions produce **values**, not heap objects.

```c
let a = 10;
```

This does **not** imply heap allocation.

Where and how data lives depends on:

* the type
* the context
* whether `new` is used
* copy / move behavior defined by the type

---

### 9.2 The `new` Keyword

`new` explicitly allocates storage on the heap.

```c
let p = new Point(1, 2);
```

Properties:

* always allocates on the heap
* returns a value whose storage is heap-backed
* never implicit
* visible in syntax

---

### 9.3 Assignment and Allocation Are Separate Concepts

```c
let a = value;
```

means:

* evaluate `value`
* assign it to `a`

It does *not* imply allocation.

```c
let b = new value;
```

means:

* allocate storage
* construct value inside it

Assignment semantics depend on the type being assigned.

---

### 9.4 Copy vs Move Semantics

When assigning:

```c
a = b;
```

the behavior depends on the type:

* trivial types -> copied
* complex types -> may define move/copy behavior
* heap-backed values -> copy references or content depending on type rules

This behavior is explicit in type design, not implicit language magic.

---

### 9.5 Allocation and Strings

## `string`

```c
string s = "hello";
```

* always heap-allocated
* resizable
* owns its memory

Using `new` explicitly:

```c
string s = new "hello";
```

is equivalent but emphasizes allocation intent.

---

## `str`

```c
str s = "hello";
```

* stack-allocated
* fixed size
* immutable length
* may reference heap data if assigned explicitly

```c
str s = new "hello"; // heap-backed string
```

---

## Pointer Access to Heap Data

```c
str* p = new "hello";
```

Stores the address of heap-allocated string data.

---

### 9.6 Allocation and Operator Overloading

Because CMinus supports operator overloading:

```c
a = b;
```

may internally:

* copy data
* move data
* allocate memory
* release memory

All such behavior is controlled by the type definition.

The language itself introduces **no implicit heap allocation** unless:

* `new` is used, or
* the type explicitly defines allocation behavior

---

### 9.7 Stack vs Heap Is Not a Type Property

Types do not inherently live on the stack or heap.

Instead:

* values live wherever their binding requires
* `new` forces heap allocation
* references and pointers may refer to either

---

### 9.8 Example

```c
struct Box {
    int value;
}

let a = Box{1};       // stack value
let b = new Box{2};   // heap allocation
```

Both have the same type but different storage behavior.

---

### 9.9 Summary

* allocation is explicit
* `new` always allocates on heap
* assignment does not imply allocation
* types define copy/move behavior
* stack vs heap is not part of the type
* allocation intent is visible in code

---

## **10. Arrays**

CMinus provides two primary array forms:

1. **Fixed-size arrays**
2. **Length-prefixed arrays**

Both are value types with predictable memory layout and no hidden allocation.

---

### 10.1 Fixed-Size Arrays

#### Declaration

```c
int[10] values;
```

This declares an array of 10 integers.

Properties:

* size known at compile time
* stored contiguously
* layout identical to C arrays
* no runtime bounds metadata

Equivalent C:

```c
int values[10];
```

---

### 10.2 Initialization

```c
int[3] a = [1, 2, 3];
```

Partial initialization is allowed only if explicitly defined by the language later; otherwise, missing values are an error.

---

### 10.3 Access

```c
a[0] = 10;
int x = a[1];
```

Access has no bounds checking by default.

---

### 10.4 Fixed Arrays as Values

Fixed arrays:

* are copied by value
* can be passed to functions
* can be embedded inside structs

```c
struct S {
    int[4] data;
}
```

---

### 10.5 Length-Prefixed Arrays

Length-prefixed arrays store their length alongside their elements.

```c
int[~10] arr;
```

Meaning:

* up to 10 elements
* first field stores length
* remaining memory stores elements

Logical layout:

```c
struct {
    usize length;
    int data[10];
}
```

---

### 10.6 Length Access

Length is available in O(1):

```c
arr.length
```

---

### 10.7 Initialization

```c
int[~10] arr = [1, 2, 3];
```

The stored length becomes `3`.

---

### 10.8 Access

```c
arr[0]    // no bounds check
arr![1]   // panic when out of bounds
arr?[2]   // return optional, none when out of bound
```

---

### 10.9 Safety Characteristics

* bounds known at compile time
* length tracked explicitly
* no automatic bounds checks unless requested
* safer than raw C arrays

---

### 10.10 Comparison to Fixed Arrays

| Feature           | Fixed Array | Length-Prefixed |
| ----------------- | ----------- | --------------- |
| Compile-time size | ✓           | ✓ (capacity)    |
| Runtime length    | ✕           | ✓               |
| Safer             | ✕           | ✓               |
| Extra storage     | ✕           | ✓ (length)      |

---

### 10.11 Summary

* fixed arrays match C exactly
* length-prefixed arrays track size
* both are value types
* no implicit allocation
* no automatic bounds checks

---

## **11. Slices**

Slices represent a **non-owning view over a contiguous sequence of elements**.
They consist of a pointer plus a length and do not allocate or own memory.

Slices are designed for safe, explicit access to array-like data without copying.

---

### 11.1 Slice Type Syntax

```c
int[] slice;
// may change to `int[*] slice;`
```

A slice always has two components:

```c
struct {
    int* data;
    usize length;
}
```

This representation is conceptual; the exact layout is ABI-compatible.

---

### 11.2 Creating Slices

#### From a length-prefixed array

```c
int[~10] arr = [1, 2, 3];
int[] s = arr[..];
```

The slice references the array’s data and length.

---

#### From a fixed array

```c
int[4] a = [1, 2, 3, 4];
int[] s = {*a, 4};
```

Explicit construction is required because fixed arrays do not store length.

---

#### From a subrange

```c
int[] sub1 = arr.subarr(1, 3);
int[] sub2 = arr[1..3];
```

Creates a slice referencing elements `[1..3)`.

---

### 11.3 Slice Ownership

Slices:

* do **not** own memory
* never allocate
* do not free
* are invalid if the backing storage becomes invalid

The programmer is responsible for ensuring the underlying storage lives long enough.

---

### 11.4 Accessing Slice Elements

```c
int x = s[0];
```

By default:

* indexing is unchecked
* out-of-bounds access is undefined behavior

Safe access is available via safety operators (covered later).

---

### 11.5 Slice Length

```c
usize n = s.length;
```

Length access is O(1).

---

### 11.6 Copying Slices

```c
int[] a = s;
```

Copies only the pointer and length, not the data.

---

### 11.7 Mutability

Mutability depends on the element type:

```c
const int[] s;   // elements are read-only
int[] s;         // elements are mutable
```

The slice itself may still be reassigned unless declared `const`.

---

### 11.8 Slices vs Arrays

| Feature         | Slice    | Fixed Array | Length-Prefixed |
| --------------- | -------- | ----------- | --------------- |
| Owns data       | ✕        | ✓           | ✓               |
| Stores length   | ✓        | ✕           | ✓               |
| Heap allocation | ✕        | ✕           | ✕               |
| Copy cost       | O(1)     | O(n)        | O(n)            |
| Bounds safety   | optional | none        | partial         |

---

### 11.9 Common Use Cases

* passing arrays to functions
* working with subranges
* zero-copy views
* API boundaries

---

### 11.10 Summary

* slices are `(pointer, length)`
* non-owning and lightweight
* no implicit allocation
* explicit safety
* ideal for APIs

---

## **12. String Types**

CMinus provides multiple string representations with explicit ownership, mutability, and allocation semantics.
There is no single “magic string” type; each form exists for a specific purpose.

---

### 12.1 Overview of String Forms

| Type     | Allocation      | Mutable         | Owns memory | Notes                  |
| -------- | --------------- | --------------- | ----------- | ---------------------- |
| `string` | heap            | yes             | yes         | growable, dynamic      |
| `str`    | stack (default) | no (size-fixed) | yes         | fixed-capacity         |
| `str*`   | heap or stack   | depends         | no          | pointer to string      |
| `char*`  | external        | mutable         | no          | C-style string         |
| `char[]` | inline          | mutable         | yes         | null-terminated buffer |

---

### 12.2 `string` — Dynamic Heap String

```c
string s = "hello";
```

Properties:

* always heap allocated
* dynamically resizable
* owns its memory
* mutable
* length and capacity tracked
* dynamic encoding (UTF-8 16 32, ByteChuck, User Defined)

Equivalent conceptual layout:

```c
struct string {
    char* data;
    usize length;
    usize capacity;
}
```

#### Behavior

```c
s += " world";
```

May reallocate internally.

Access:

```c
s[0];
s.length;
```

---

### 12.3 Explicit Heap Allocation with `new`

```c
string s = new "hello";
```

This explicitly allocates the string on the heap.

Semantically equivalent to the implicit heap allocation that `string` already performs, but makes allocation intent explicit.

---

### 12.4 `str` — Fixed-Size String

```c
str name = "John";
```

Properties:

* fixed size
* stack-allocated by default
* immutable length
* owns its memory
* encoding-aware

The capacity is inferred from the initializer unless specified explicitly:

```c
str<16> buf = "hello";
string<256> str = "builder";
```

---

### 12.5 Heap-Backed `str`

`str` can also be backed by heap storage:

```c
str s = new "hello";
```

In this case:

* memory lives on heap
* capacity fixed
* still immutable in size

---

### 12.6 String Pointers

```c
str* p = new "hello";
```

* pointer to a string object
* does not own the string
* may point to stack or heap string

---

### 12.7 C-Style Strings

#### `char*`

```c
char* s = "hello\0";
```

* null-terminated
* mutable
* no length tracking
* interoperable with C

#### `char[]`

```c
char buf[32] = "hello";
```

* inline storage
* mutable
* null-terminated
* fixed size

---

### 12.8 Length and Character Access

| Type     | Length Access | Character Access      |
| -------- | ------------- | --------------------- |
| `string` | O(1)          | O(1) byte / O(n) char |
| `str`    | O(1)          | O(1) byte / O(n) char |
| `char*`  | O(n)          | O(1)                  |
| `char[]` | O(n)          | O(1)                  |

---

### 12.9 Encoding Notes

* `char` represents a UTF-8 byte or ByteChuck
* `dchar` represents UTF-16 units
* `qchar` represents UTF-32 code points
* no implicit transcoding occurs

---

### 12.10 String Conversion

Conversions must be explicit.

```c
string a = "hi";
str b = a;
char* c = a;
```

Each conversion may:

* allocate
* copy
* or reinterpret
  depending on the target type.

---

### 12.11 Summary

* multiple string types exist by design
* ownership and allocation are explicit
* `string` is dynamic and heap-backed
* `str` is fixed-size and value-oriented
* C strings are supported for interop
* no implicit conversions or hidden allocation

---

## **13. Data-Oriented Types (`data`)**

`data` types describe **logical entities whose physical layout is optimized automatically** for cache efficiency and iteration performance.
They are designed for data-oriented programming while remaining usable like normal structs.

A `data` declaration describes *what fields exist*, not how they are physically arranged.

---

### 13.1 Basic Declaration

```c
data Player {
    hot vec3<float> position;
    hot vec3<float> velocity;
    warm i64 score;
    cold string name;
}
```

This declares a logical type `Player`, but its storage is **not a single struct**.

---

### 13.2 Logical vs Physical Representation

A `data` type is conceptually one object, but physically it is split into multiple internal structures based on access frequency.

Conceptual view:

```c
Player {
    position;
    velocity;
    score;
    name;
}
```

Physical layout (conceptual example):

```c
struct Player_hot {
    vec3<float> position;
    vec3<float> velocity;
}

struct Player_warm {
    i64 score;
}

struct Player_cold {
    string name;
}

struct Player_data {
    Key key;
}

List<Player_hot>  player_group_hot;
List<Player_warm> player_group_warm;
List<Player_cold> player_group_cold;
```

The compiler automatically generates and manages these layouts.

---

### 13.3 Access Semantics

Despite the internal split, access looks like a normal struct:

```c
Player p;
p.position = ...;
p.name = "Alice";
```

The compiler rewrites accesses to the correct internal storage.

---

### 13.4 Access Frequency Qualifiers

#### `hot`

* accessed very frequently
* optimized for cache locality
* grouped together

#### `warm`

* moderate access frequency

#### `cold`

* rarely accessed
* separated to reduce cache pollution

These qualifiers affect **layout only**, not semantics.

---

### 13.5 Performance Motivation

`data` exists to:

* reduce cache misses
* improve memory locality
* support data-oriented design
* avoid manual struct-of-arrays rewriting

Especially useful in:
`
* game engines
* simulations
* ECS-style systems
* large collections

---

### 13.6 Iteration Behavior

When iterating over collections of `data` objects, the compiler may transform loops into structure-of-arrays (SoA) access patterns automatically.

Example (conceptual):

```c
for (Player p of players) {
    update(p.position);
}
```

May compile into iteration over `Player_hot.position[]`.

---

### 13.7 Semantics and Guarantees

* field access is transparent
* no observable semantic difference from structs
* layout may change between compilations
* ABI stability is not guaranteed
* not suitable for FFI boundaries

---

### 13.8 Interaction With Other Types

* can contain structs, primitives, strings
* can be used in arrays and slices
* can be passed to functions
* cannot rely on memory layout
* cannot take raw pointers to internal fields safely

---

### 13.9 Restrictions

* cannot be used in C ABI directly
* address of internal fields may not be stable
* layout must not be assumed
* unsuitable for serialization without explicit control

---

### 13.10 Summary

* `data` describes *what data exists*, not how it is laid out
* compiler splits storage automatically
* optimized for cache and iteration
* accessed like normal structs
* layout intentionally abstracted

---

## **14. Class Types**

Classes represent **structured types with inheritance and behavior**, built on top of structs but extended with controlled polymorphism.
They remain explicit, predictable, and compile-time–driven, without hidden runtime mechanisms.

---

### 14.1 Defining a Class

```c
class Person {
    string name;
    uint age;
}
```

A class declaration defines:

* a named type
* a fixed set of fields
* optional inheritance
* optional methods

Classes are still *data-first* and map closely to structs.

---

### 14.2 Constructors

A class may define one or more constructors using `main`.

```c
class Person {
    string name;
    uint age;

    main(this.name, this.age);
}
```

Constructor rules:

* `main` defines how the class is initialized
* parameters are assigned explicitly
* no implicit default constructor unless declared
* overloading may be supported later

---

### 14.3 Instantiation

```c
Person p = Person("Alice", 20);
```

Allocation behavior depends on context:

```c
Person p1 = Person("A", 20);      // value
Person* p2 = new Person("B", 30); // heap allocation
Person p3{"C", 40};               // short-hand on stack 
```

The class itself does not imply heap allocation.

---

### 14.4 Inheritance

Classes may extend a single base class.

```c
class Student extends Person {
    string major;

    main(base.name, base.age, this.major);
}
```

Rules:

* single inheritance only
* base class fields are included first
* explicit constructor chaining required
* no implicit base initialization

---

### 14.5 Field Access and Shadowing

```c
student.name;
student.major;
```

* derived classes inherit all base fields
* name conflicts are disallowed unless explicitly resolved
* base fields accessed using `base.field`

---

### 14.6 Methods

Methods are functions associated with a class.

```c
class Counter {
    int value;

    void inc() {
        this.value += 1;
    }
}
```

Properties:

* receive `this` implicitly
* resolved at compile time
* no hidden virtual dispatch

---

### 14.7 Polymorphism Model

CMinus uses **compile-time polymorphism**, not runtime virtual dispatch.

* method calls are resolved statically
* no vtables
* no RTTI
* no implicit dynamic dispatch

This ensures:

* predictable performance
* zero runtime overhead

---

### 14.8 Overriding and Method Compatibility

Derived classes may define methods with the same name and signature.

Rules:

* method signatures must match exactly
* resolution is static
* no runtime override lookup

This behaves as *shadowing*, not virtual overriding.

---

### 14.9 Classes vs Structs

| Feature              | Struct   | Class     |
| -------------------- | -------- | --------- |
| Named type           | ✓        | ✓         |
| Fields               | ✓        | ✓         |
| Methods              | ✕        | ✓         |
| Inheritance          | ✕        | ✓         |
| Layout control       | explicit | inherited |
| Runtime polymorphism | ✕        | ✕         |

---

### 14.10 Classes and Memory Layout

* class layout is deterministic
* base fields precede derived fields
* no hidden metadata
* compatible with C layout rules when inheritance is not used

---

### 14.11 Summary

* classes extend structs with behavior
* inheritance is explicit and single
* no runtime polymorphism
* predictable layout and performance
* allocation controlled explicitly

---

## **15. Function Types**

Functions in CMinus are **first-class values** with statically known signatures.
They can be stored, passed, and returned, while remaining fully predictable and analyzable.

---

### 15.1 Function Declaration

```c
int add(int a, int b) {
    return a + b;
}
```

A function has:

* a name
* parameter list
* return type
* body

---

### 15.2 Function Type Syntax

The type of a function is written as:

```c
int(int, int)
```

Meaning:

> a function taking `(int, int)` and returning `int`

Example:

```c
int(int, int) f = func(add, int, int);
```

`func` statement generates the function signature name at compile time, based on name and parameter types.

---

### 15.3 Calling Functions

```c
int x = add(1, 2);
```

Function calls are direct and statically resolved.

---

### 15.4 Function Values

Functions can be assigned to variables:

```c
int(int, int) op = func(add, int, int);
```

They may be passed as arguments:

```c
void apply(int(int, int) f) {
    f(1, 2);
}
```

---

### 15.5 Function Pointers vs Function Values

Internally, function values lower to function pointers.

However, semantically:

* functions are immutable
* function values cannot be modified
* only referenced

---

### 15.6 Returning Functions

```c
int(int, int) choose(bool add) {
    if (add) return func(addFn, int, int);
    return func(subFn, int, int);
}
```

---

### 15.7 No Closures (by default)

Functions do **not** capture outer variables.

```c
int x = 10;
let f = \() -> x; // ✕ not allowed
```

By default only explicitly declared parameters are accessible.

This keeps functions predictable and ABI-friendly.

---

### 15.8 Function Overloading

Functions may be overloaded by parameter types.

```c
int add(int a, int b);
float add(float a, float b);
```

Resolution is compile-time.

---

### 15.9 Function Types and Pointers

Function types can be used wherever types are allowed:

```c
typedef Adder = int(int, int);
```

They can be used in structs, tables, or generics.

---

### 15.10 Summary

* functions are first-class values
* statically typed and resolved
* no closures
* no runtime dispatch
* compatible with C function pointers

---

## **16. Generics**

Generics allow writing reusable definitions parameterized by types.
CMinus supports generics for both **runtime-generating constructs** and **compile-time–only constructs**, with different semantics.

---

### 16.1 Where Generics Are Allowed

Generics may be used on:

Runtime-generating:

* `struct`
* `class`
* `function`

Compile-time–only:

* `typedef`
* `options`
* `table`

All of them share the same syntax, but not the same behavior.

---

### 16.2 Generic Syntax

Basic form:

```c
<T>
```

Example:

```c
struct Box<T> {
    T value;
}
```

Usage:

```c
Box<int> a;
Box<string> b;
```

Each used type generates a distinct concrete version.

---

### 16.3 Multiple Generic Parameters

```c
struct Pair<A, B> {
    A first;
    B second;
}
```

Usage:

```c
Pair<int, string> p;
```

---

### 16.4 Generic Functions

```c
T identity<T>(T value) {
    return value;
}
```

Usage:

```c
int x = identity<int>(10);
```

Type inference may be added later, but explicit form is always valid.

---

### 16.5 Generic Constraints (Interface Bounds)

Generic parameters may be constrained using interfaces:

```c
<T: Drawable>
```

Meaning:

* `T` must explicitly implement `Drawable`

Multiple constraints:

```c
<T: Drawable | Serializable>
```

---

### Extended form (future control syntax)

We plan to support explicit generation control:

```c
<T:: int | float>
```

or combined:

```c
<T:: int | float : Numeric>
```

Meaning:

* only listed concrete types are allowed
* optional interface enforcement

This affects **code generation scope**, not runtime behavior.

### Generic Constant (future)

We plan to support generic constants:

```c
<uint Capacity>
```

with compile time condition:

```c
<uint Capacity -> Capacity > 0 && Capacity < 1024>
```

combined:

```c
<uint Capacity -> Capacity > 0, T::str | string | char[]>
```

Meaning:

* constant value that used to initialize the type
* inline constant with strict passing compile-time condition


---

### 16.6 Runtime vs Compile-Time Generics

### Runtime-generating generics

These generate real code per type:

* `struct`
* `class`
* `function`

Example:

```c
struct Box<T> { T v; }
```

Produces (conceptually):

```c
struct Box_$int { int v; }
struct Box_$string { string v; }
```

---

### Compile-time–only generics

These exist only during compilation and vanish afterward:

* `typedef`
* `options`
* `table`

Example:

```c
typedef Vec<T> = T[];
```

After expansion, no runtime symbol exists.

---

### 16.7 Generic + typedef

```c
typedef Result<T> = variant{T, Error};
```

Used as:

```c
Result<int> r;
```

This creates **no runtime type**, only a rewritten type expression.

---

### 16.8 Generic + options

```c
options<T>{ zero, one }
```

The generic parameter only affects the underlying type.

Example:

```c
options<u8>{ red=1, blue=2 }
```

Generics do not introduce storage or runtime behavior.

---

### 16.9 Generic + table

```c
table<T> Values {
    "a" = ...,
}
```

The generic determines the value array type:

```c
T[]
```

Keys remain compile-time constants.

---

### 16.10 Generic Instantiation Rules

* every concrete type combination is independent
* no runtime polymorphism
* no boxing
* no vtables
* no implicit casting between instantiations

```c
Box<int> != Box<float>
```

---

### 16.11 Monomorphization Model

CMinus uses **full monomorphization**:

* each concrete use produces its own version
* optimized independently
* fully inlined where possible

This ensures:

* predictable performance
* zero abstraction overhead
* C-like output

---

### 16.12 What Generics Do *Not* Do

Generics do NOT:

* introduce inheritance
* introduce runtime dispatch
* erase types
* allocate memory
* imply polymorphism

They are purely compile-time expansion tools.

---

### 16.13 Summary

* Generics exist for both runtime and compile-time constructs
* Runtime generics generate code per type
* Compile-time generics vanish after validation
* Constraints enforce structure and intent
* No runtime cost or hidden behavior
* Designed to remain C-like and predictable

---

## **17. `typedef` — Type Aliasing and Compile-Time Types**

`typedef` defines a **compile-time alias** for a type expression.
It introduces *no runtime symbol*, allocates no memory, and generates no code.

Its purpose is:

* readability
* abstraction
* reuse
* expressing intent
* simplifying complex types

---

### 17.1 Basic Syntax

```c
typedef Name = Type;
```

Example:

```c
typedef Index = u32;
```

Usage:

```c
Index i = 10;
```

This is exactly equivalent to:

```c
u32 i = 10;
```

There is no runtime difference.

---

### 17.2 `typedef` Is Compile-Time Only

A `typedef`:

* does **not** create a new type
* does **not** affect layout
* does **not** exist at runtime
* does **not** generate symbols
* is fully substituted during compilation

Think of it as a named macro for types, but type-checked.

---

### 17.3 Complex Type Aliases

`typedef` is most useful for complex or nested types.

```c
typedef Buffer = u8[1024];
typedef Callback = bool(int, string);
```

Instead of repeating:

```c
bool(int, string)
```

you write:

```c
Callback cb;
```

---

### 17.4 Typedef With Generics

`typedef` supports generics:

```c
typedef Pair<T, U> = (T, U);
```

Usage:

```c
Pair<int, string> p;
```

This expands to:

```c
(int, string)
```

No struct or runtime object is generated.

---

### 17.5 Typedef With Constraints

You may constrain generic parameters:

```c
typedef Numeric<T: Number> = T;
```

This restricts valid substitutions at compile time.

---

### 17.6 Typedef vs Struct

### Struct:

```c
struct Vec2 {
    float x;
    float y;
}
```

* creates a real runtime layout
* has identity
* can be forward-declared
* can be extended

### Typedef:

```c
typedef Vec2 = (float, float);
```

* no runtime identity
* inline expansion
* purely syntactic
* zero overhead

Use `typedef` when you want **meaning**, not identity.

---

### 17.7 Typedef and Readability

Typedefs are especially useful to:

* name semantic concepts
* hide verbose generics
* standardize APIs
* clarify intent

Example:

```c
typedef UserId = u64;
typedef Timestamp = i64;
```

These prevent confusion even though both are integers.

---

### 17.8 Typedef With Pointers

```c
typedef Ptr<T> = T*;
```

Usage:

```c
Ptr<int> p;
```

Equivalent to:

```c
int* p;
```

---

### 17.9 Typedef With Function Types

```c
typedef Comparator<T> = int(T, T);
```

Usage:

```c
Comparator<int> cmp;
```

---

### 17.10 Typedef With Arrays

```c
typedef Matrix = float[4][4];
```

Or generic:

```c
typedef Array<T, N> = T[N];
```

---

### 17.11 Typedef With Optional and Variant

```c
typedef Maybe<T> = T?;
typedef Result<T> = variant{T, Error};
```

These are aliases only; behavior comes from the underlying type.

---

### 17.12 Typedef Visibility and Scope

* `typedef` follows normal scope rules
* may be declared at:

  * file level
  * inside modules
  * inside blocks

Inner definitions shadow outer ones.

---

### 17.13 Typedef and Code Generation

During compilation:

1. typedefs are resolved
2. substituted into full types
3. removed entirely
4. remaining code is compiled normally

There is no trace of `typedef` in generated C.

---

### 17.14 When to Use `typedef`

Use `typedef` when you want:

* cleaner syntax
* semantic naming
* reusable type expressions
* abstraction without cost
* generic aliases
* documentation through naming

Do **not** use `typedef` when you need:

* identity
* behavior
* storage
* layout control

---

### 17.15 Summary

* `typedef` creates compile-time aliases
* zero runtime cost
* supports generics and constraints
* expands fully before codegen
* ideal for readability and abstraction
* not a real type

---

## **18. `options` — Scoped Named Constants (Inline Enums)**

`options` define a **set of named constant values bound to a base type**.
They behave like *compile-time enums*, but are more flexible, more expressive, and fully zero-cost.

They exist only at compile time and disappear after lowering.

---

### 18.1 Basic Syntax

```c
options<int>{up, down, left, right}
```

This declares four named constants associated with the base type `int`.

By default, values are assigned incrementally starting from `0`:

```text
up    = 0
down  = 1
left  = 2
right = 3
```

---

### 18.2 Using `options` in Variables

```c
options<int>{up, down, left, right} dir = up;
```

Outside this declaration scope, the names `up`, `down`, etc. **do not exist**.

Only the resulting value remains.

Lowered to:

```c
int dir = 0;
```

---

### 18.3 Explicit Values

You may explicitly assign values:

```c
options<i8>{less=-1, equal=0, more=1}
```

Mixing explicit and implicit values is allowed:

```c
options<u8>{a, b, c=10, d}
```

Results in:

```text
a = 0
b = 1
c = 10
d = 11
```

---

### 18.4 Counter Rules

Automatic numbering follows these rules:

* starts from `0`
* increments by `1`
* applies only to numeric base types
* cannot decrement
* must not overflow
* produces a compile-time error on overflow

Example (invalid):

```c
options<u8>{a=254, b, c}
```

`c` would overflow -> compile-time error.

---

### 18.5 Supported Base Types

`options<T>` requires `T` to be a **compile-time literal-compatible type**, such as:

* integers
* unsigned integers
* characters
* enums / numeric aliases
* variants
* strings (with explicit values)

Unsupported types cause a compile-time error.

---

### 18.6 String-Based Options

```c
options<string>{
    red   = "red",
    green = "green",
    blue  = "blue"
}
```

Compiles to string constants.

Used when semantic clarity matters more than numeric identity.

---

### 18.7 Variant-Based Options

Options can wrap a variant:

```c
options<variant{int, string}>{
    id = 1,
    name = "user"
}
```

Each option expands to a valid value of the variant.

---

### 18.8 Options Inside Functions

Options may appear inside function scope:

```c
options<int>{less=-1, equal=0, more=1}
compare(int a, int b) {

    if (a < b) return less;
    if (a > b) return more;
    return equal;
}
```

Scope rules:

* names exist only inside the block
* values lower directly to constants
* no runtime structure is created

---

### 18.9 Options With Type Aliases

Options work cleanly with `typedef`:

```c
typedef Order = i8;

options<Order>{buy=1, sell=2}
```

---

### 18.10 Options With Functions

Options are often used as symbolic return values:

```c
typedef result = options<i8>{ok=0, error=1};

result process() {
    if (fail()) return error;
    return ok;
}
```

---

### 18.11 Options Do Not Create Types

Important distinction:

```c
options<int>{a, b}
```

does **not** define a new type.

It only defines named constants of type `int`.

This means:

```c
int x = a;   // valid
```

There is no type-checking barrier like enums in some languages.

---
Below is a **complete, precise version of section 18.12**, written in the same tone and rigor as your other sections.
It explains intent, rules, precedence, and edge cases without over-engineering.

---

### 18.12 Option Name Resolution and Scope Priority

When using `options`, names defined inside the option set may collide with variables or symbols from the surrounding scope.

CMinus resolves this ambiguity using **scope priority rules**.

---

#### Default Name Resolution Rule

When an identifier used inside an `options` expression matches both:

* a local variable name, and
* an option constant name

**the local variable always wins**.

Example:

```c
int x = 2;

options<int>{x, y} a = x;
```

Interpretation:

* `x` refers to the variable `x`
* value assigned is `2`

The option constant `x` is ignored in this context.

This rule ensures that normal lexical scoping always has priority and avoids accidental shadowing.

---

#### Accessing Option Constants Explicitly (`:` operator)

To explicitly refer to an option constant, use the **option access operator** `:`.

```c
options<int>{x, y} a = :x;
```

This forces lookup inside the option set.

Result:

* `:x` refers to the option constant
* assigned value is the option’s numeric value (`0` in this case)

---

#### Meaning of the `:` Operator

The `:` operator:

* bypasses normal name resolution
* explicitly selects an option constant
* ignores variables, parameters, and fields with the same name
* is only valid inside option-related expressions

It always refers to the **current option scope**.

---

### 18.13 Options vs Enums (Conceptually)

| Feature           | options | traditional enum |
| ----------------- | ------- | ---------------- |
| Runtime presence  | ✕ none  | usually yes      |
| Scoped            | ✓       | sometimes        |
| Base type control | ✓       | limited          |
| String support    | ✓       | rare             |
| Variant support   | ✓       | no               |
| Zero cost         | ✓       | not always       |
| Compile-time only | ✓       | often no         |

---

### 18.14 Options and Readability

Options exist to:

* name magic numbers
* express intent
* avoid raw constants
* document logic
* keep code readable
* avoid enum overhead

They are especially useful for:

* return codes
* comparison results
* flags
* symbolic states
* temporary scoped constants

---

### 18.15 Summary

* `options<T>` define named constants
* scoped and compile-time only
* zero runtime cost
* support auto-increment
* support explicit values
* support strings and variants
* disappear entirely after compilation
* ideal replacement for lightweight enums

---

## **19. `table` — Static Compile-Time Lookup Tables**

A `table` defines a **compile-time–constructed lookup structure** that lowers into a static array.
All keys are resolved at compile time and mapped to numeric indices.

A table has **no runtime hashing**, no dynamic lookup, and no allocation.

---

### 19.1 Basic Syntax

```c
table<string> Table {
    "red"    = "color",
    'A'      = "capital char",
    10       = "integer",
    0x00FF00 = "green hex",
};
```

Syntax form:

```c
table<T> Name {
    <constant-key> = <value>,
    ...
}
```

Where:

* `T` is the **value type**
* keys must be **compile-time constants**
* values must be assignable to `T`

---

### 19.2 What a Table Compiles To

A table is lowered into:

```c
static T Name_table[];
```

and a compile-time–generated mapping:

```
key -> index
```

Access is always transformed into **index-based access**.

There is no hash table, map, or runtime structure.

---

### 19.3 Table Value Type

The type inside `<>` defines the stored array type:

```c
table<string> Names;
```

Low-level representation:

```c
string[];
```

This means:

* all values must be convertible to `string`
* values are stored contiguously
* normal indexing rules apply internally

### Using variant values

```c
table<variant{int, string}> Mixed {
    "a" = 1,
    "b" = "hello"
}
```

---

### 19.4 Access Table Length

to access length of table at compile-time use `.length`

```c
Names.length;
```

length of table is determined by the compiler,
zero-cost table length access.


---

### 19.5 Keys — General Rules

Keys may be **any compile-time constant**, including:

* string literals
* character literals
* integers (decimal, hex, binary, octal)
* `true`, `false`, `null`
* `final` constants
* literals used in `options`
* compile-time expressions

Each key is converted into a **canonical internal string representation**.

---

### 19.6 Key Canonicalization Rules

Keys are normalized at compile time into internal identifiers:

### Strings

```
"red" -> "$$red"
```

### Characters

```
'A' -> "$A"
```

### Integers

```
10     -> "10"
0xFF   -> "255"
-1     -> "_1"
```

### Booleans

```
true  -> "Ztrue"
false -> "Zfalse"
```

### Null

```
null -> "Znull"
```

This representation is internal only and never exposed to users.

---

### 19.7 Compile-Time Key Validation

All keys must be:

* compile-time known
* unique after normalization
* valid for the table

Duplicate or conflicting keys produce a **compile-time error**.

---

### 19.8 Access Forms

## **1. Compile-Time Key Access**

```c
Colors["red"];
```

Rules:

* key must be compile-time constant
* lookup resolved at compile time
* lowered to array indexing

Equivalent to:

```c
Colors_table[INDEX];
```

---

## **2. Compile-Time Index Access**

```c
Colors::"red";
```

Returns the index of the key.

Useful when you want to store or pass the index explicitly.

```c
int idx = Colors::"red";
```

---

### 19.8 Runtime Index Access (Unsafe)

```c
Colors:[3]
```

* access index at runtime
* no bounds check
* can cause undefined behavior if index is invalid

Use it when you fully sure the index gonna be valid.

---

### 19.9 Runtime Index Access (Safe)

```c
Colors![3];
```

* runtime index
* performs bounds check
* panics on invalid index

Used when index comes from user input or computation.

---

### 19.10 Runtime Index Access (Safe)

```c
Colors?[3];
```

Returns an optional value:

```c
string? value = Colors?[i];
```

* returns `none` if index is out of range
* never panics
* always runtime

---

### 19.11 Using `final` Keys

Keys may be stored in `final` variables and reused:

```c
final key = "red";

Colors[key];     // valid
```

Rules:

* key must be `final`
* must resolve to a valid table key
* otherwise compile-time error

Non-final variables are rejected:

```c
let k = "red";
Colors[k]; // error
```

---

### 19.12 Why Runtime Keys Are Disallowed

Tables are **not hash maps**.

Allowing runtime string keys would imply:

* hashing
* comparison loops
* runtime memory access
* unpredictable cost

To preserve zero-cost semantics, tables only accept:

* compile-time keys
* runtime *indices*

---

### 19.13 Relationship to Arrays

A table behaves like a named, indexed static array:

```c
table<int> T {
    "a" = 10,
    "b" = 20
}
```

Is equivalent to:

```c
static int T_data[] = { 10, 20 };
```

Plus a compile-time mapping:

```
"a" -> 0
"b" -> 1
```

---

### 19.14 Why Tables Exist

Tables are designed to:

* replace hardcoded switch logic
* replace magic constants
* avoid runtime maps
* provide readable symbolic access
* allow deterministic codegen
* keep lookup O(1)
* stay fully static

---

### 19.15 Typical Use Cases

### Named configuration values

```c
table<int> Errors {
    "ok" = 0,
    "not_found" = 404,
    "denied" = 403,
}
```

---

### Mapping symbolic names to resources

```c
table<string> Shaders {
    "basic" = "basic.glsl",
    "ui"    = "ui.glsl"
}
```

---

### Runtime-safe indexed access

```c
int i = getIndex();
string? s = Shaders?[i];
```

---

### 19.16 Summary

* `table<T>` creates a static indexed array
* keys are compile-time constants only
* all lookups lower to index access
* no hashing, no allocation
* safe and unsafe runtime access available
* supports any value type, including variants
* ideal for symbolic lookup tables

---

## **20. Interfaces (Compile-Time Prototypes)**

An **interface** in CMinus is a *compile-time contract* used to describe the required **shape** of a type.
It does **not** generate runtime data, vtables, or metadata.

Interfaces exist only to:

* validate structure at compile time
* constrain generics
* define required fields and functions
* provide optional default implementations

They are closer to **struct/class blueprints** than to runtime polymorphism.

---

### 20.1 Purpose of Interfaces

Interfaces are used to:

* enforce that a type provides certain fields or methods
* describe behavior without defining storage
* constrain generic parameters
* enable static polymorphism
* share default logic safely

They do **not**:

* allocate memory
* introduce runtime dispatch
* generate hidden metadata
* affect layout directly

---

### 20.2 Basic Interface Declaration

```c
interface Drawable {
    void draw();
}
```

This defines a contract requiring a `draw()` method.

---

### 20.3 Implementing an Interface

Interfaces must be implemented **explicitly**.

```c
struct Sprite : Drawable {
    int x;
    int y;

    void draw() {
        // drawing logic
    }
}
```

Rules:

* implementation must be explicit (`: Interface`)
* all required members must exist
* names and signatures must match
* missing members are compile-time errors

---

### 20.4 Interfaces With Fields

Interfaces may define **required fields** in addition to methods.

```c
interface Entity {
    int id;
    string name;
}
```

A conforming type must define those fields:

```c
struct Player : Entity {
    int id;
    string name;
    int score;
}
```

Field names and types must match exactly.

---

### 20.5 Default Fields and Methods

Interfaces may define defaults using the `default` keyword.

```c
interface Drawable {
    default void draw() {
        // do nothing
    }
}
```

Default fields are also allowed:

```c
interface Colored {
    default Color color = black;
}
```

Rules:

* defaults are optional to override
* if not overridden, the default is used
* defaults do not force memory layout
* defaults do not allocate automatically

Defaults are injected at compile time.

---

### 20.6 Constructor Requirements

Interfaces may declare constructor requirements.

```c
interface Named {
    main(string name);
}
```

Implementing types must provide a compatible constructor.

---

### 20.7 Interface Inheritance

Interfaces may extend other interfaces:

```c
interface Movable {
    void move(int dx, int dy);
}

interface DrawableMovable : Movable {
    void draw();
}
```

Implementers must satisfy **all inherited requirements**.

---

### 20.8 Multiple Interface Implementation

A type may implement multiple interfaces:

```c
struct Button : Clickable, Drawable, Focusable {
    ...
}
```

There is no ambiguity because:

* no runtime dispatch exists
* method resolution is static
* names must be unique or explicitly resolved

---

### 20.9 Interfaces and Generics

Interfaces are commonly used as **generic constraints**.

```c
void render<T: Drawable>(T obj) {
    obj.draw();
}
```

Rules:

* `T` must implement `Drawable`
* verification is done at compile time
* no runtime checking

---

### Multiple Constraints

```c
result process<T: Drawable | Serializable>(T obj) {
    ...
}
```

Meaning:

* `T` must satisfy both interfaces

---

### 20.10 Predefined Primitive Type Interfaces
- `Numeric` for all kinds of numbers
- `Integer` for all kinds of signed integers
- `Unteger` for all kinds of unsigned integers
- `Floating` for all kinds of floating point numbers
- `Character` for all kinds of characters
- `Comparable` for all kinds of comparable types
- `Iterable` for all kinds of iterable types

| Interface       | Primitives & Types                    | Description                    |
|-----------------|---------------------------------------|--------------------------------|
| `Integer`       | `i8, i16, i32, i64`                   | all kinds of signed integers   |
| `Unteger`       | `u8, u16, u32, u64`                   | all kinds of unsigned integers |
| `Floating`      | `f32, f64`                            | all kinds of floating numbers  |
| `Numeric`       | `i8-i64, u8-u64, f32-f64`             | all kinds of numbers           | 
| `Character`     | `c8, c16, c32`                        | all kinds of characters        |
| `Logical`       | `bool, trnry, void`                   | all logic kind type            |
| `Comparable`    | `Numeric, trnry`                      | support `< > == != <= >=` etc. |
| `Checkable`     | `Numeric, Character, bool, trnry`     | support `== !=`                |


---

### 20.11 Interfaces Do Not Affect Layout

Interfaces:

* do not insert fields
* do not add padding
* do not generate vtables
* do not change memory layout

They only validate structure and signatures.

---

### 20.12 Interfaces vs Classes vs Structs

| Feature          | Interface          | Struct | Class |
| ---------------- | ------------------ | ------ | ----- |
| Allocates memory | No                 | Yes    | Yes   |
| Fields           | Yes (requirements) | Yes    | Yes   |
| Methods          | Yes                | Yes    | Yes   |
| Defaults         | Yes                | Yes    | Yes   |
| Inheritance      | Yes                | No     | Yes   |
| Runtime dispatch | No                 | No     | No    |
| Code generation  | No                 | Yes    | Yes   |

---

### 20.13 Interfaces Are Compile-Time Only

Interfaces are erased after type checking.

They:

* do not exist in generated C
* cannot be inspected at runtime
* cannot be stored as values
* cannot be referenced dynamically

They exist solely to improve correctness and structure.

---

### 20.14 Why Interfaces Exist in CMinus

Interfaces solve problems such as:

* enforcing APIs without inheritance
* validating generic usage
* documenting expectations
* enabling safe composition
* avoiding runtime polymorphism

They provide structure **without cost**.

---

### 20.15 Summary

* interfaces define required fields and methods
* they are compile-time only
* no runtime overhead
* explicit implementation required
* support defaults
* support inheritance
* usable as generic constraints
* never affect memory layout

---

## **21. Generics (Compile-Time Parametric Types)**

Generics in CMinus provide **compile-time parametrization** of types, functions, and other constructs.
They are resolved entirely at compile time and **never exist at runtime**.

CMinus uses **monomorphization**, not type erasure.

---

### 21.1 What Generics Are

A generic allows you to write code once and reuse it for multiple concrete types.

```c
struct Box<T> {
    T value;
}
```

Each used specialization generates a concrete version:

```c
Box<int>
Box<string>
```

These become independent generated types in C.

---

### 21.2 Where Generics Are Allowed

Generics can be used on:

* `struct`
* `class`
* `function`
* `typedef`
* `options`
* `table`
* `interface`

They may appear anywhere a type definition exists.

---

### 21.3 Generic Parameters Syntax

```c
<T>
<T, U>
<T, U, V>
```

Example:

```c
struct Pair<T, U> {
    T first;
    U second;
}
```

---

### 21.4 Generic Functions

```c
void swap<T>(T a, T b) {
    T tmp = a;
    a = b;
    b = tmp;
}
```

Usage:

```c
swap<int>(1, 2);
swap("a", "b");
```

Each instantiation generates a specialized version.

---

### 21.5 Type Inference for Generics

In many cases, generic arguments are inferred automatically:

```c
swap(1, 2);          // T = int
swap("a", "b");      // T = string
```

If inference fails, explicit arguments are required.

---

### 21.6 Generic Constraints (Interfaces)

Generics may be constrained using interfaces:

```c
void drawAll<T: Drawable>(T item) {
    item.draw();
}
```

Rules:

* `T` must implement `Drawable`
* verified at compile time
* no runtime checks

---

### Multiple Constraints

```c
void f<T: A | B>(T x) {
    ...
}
```

Meaning:

* `T` must satisfy **all** listed interfaces

---

### 21.7 Restricted Generic Sets

CMinus allows restricting generics to explicit types:

```c
T add<T::int | float>(T a, T b) {
    return a + b;
}
```

This means:

* `T` may only be `int` or `float`
* compiler generates versions only for those types

You may combine this with interface constraints:

```c
<T::int | float : Numeric>
```

### 21.8 Generics Constant

generic constant are default strict constant used to initialize the type.

here's an example of how it used in built in string:

```c
struct string<u32 Capacity = 256 -> Capacity > 32, Char = c8 : Character> {
    u32 capacity = Capacity;
    u32 length = 0;
    Char* data;

    operator =  (string s)   { ... }
    operator +  (string s)   { ... }
    operator +  (Numeric n)  { ... }
    operator >  (string s)   { ... }
    operator <  (string s)   { ... }
    operator == (string s)   { ... }
    operator >= (string s)   { ... }
    operator <= (string s)   { ... }
}
```

Usage:

```c
string str1 = "one";                // Capacity 256, Char c8
string<32> str2 = "one";            // Capacity 32, Char c8
string<32, c32> str3 = u32"one";    // Capacity 32, Char c32

string<16> str4 = "one";            // Error Capacity don't match the requirements
```

Rules:

* Capacity must be 32-bit unsigned integer
* Capacity must be higher than 32
* Char must be a character type

---

### 21.8 Generic Behavior by Construct Type

### Structs / Classes

* each specialization generates a new layout
* layout depends on substituted types
* no shared runtime metadata

### Functions

* specialized per type
* inlined aggressively when possible

### Typedefs

* purely compile-time
* expand and vanish after substitution

### Options / Tables

* generics only affect validation
* no generated runtime types

---

### 21.9 Generics and Code Generation

Monomorphization rules:

* each unique set of generic arguments creates a unique generated version
* unused specializations are not emitted
* generic logic disappears after lowering
* generated C code contains only concrete types

Example:

```c
Box<int>
Box<float>
```

-> generates two structs in C.

---

### 21.10 Generics and Performance

Generics introduce:

* no dynamic dispatch
* no boxing
* no heap overhead
* no runtime cost

All decisions are compile-time.

---

### 21.11 Invalid Generic Usage

Errors include:

```c
T f<T>(T x) {
    x + 1; // error: operator not guaranteed (may change to warning)
}
```

Unless constrained:

```c
void f<T: Numeric>(T x) {
    x + 1; // ok
}
```

---

### 21.12 Generic Defaults (Future)

Default generic arguments may be added later:

```c
struct Vec<T = float> {
    T x;
    T y;
}
```

This is not currently required but planned.

---

### 21.13 Summary

* generics are compile-time only
* monomorphized, not erased
* usable on most language constructs
* support interface constraints
* support restricted type sets
* produce zero runtime overhead
* integrate naturally with C output

---

## **22. Tuples (Inline Anonymous Structs)**

Tuples in CMinus are **anonymous, inline struct types** used to group multiple values without defining a named `struct`.

They are purely structural and compile directly into a struct layout.

---

### 22.1 Basic Tuple Syntax

```c
(int, string)
```

This defines an anonymous struct equivalent to:

```c
struct {
    int $0;
    string $1;
}
```

Each element is stored in order and accessed by index.

---

### 22.2 Tuple Values

```c
(int, string) pair = (10, "ok");
```

Accessing elements:

```c
pair.$0;   // 10
pair.$1;   // "ok"
```

Index names are fixed as `$0`, `$1`, `$2`, etc.

---

### 22.3 Named Tuple Fields

You may assign names to tuple elements:

```c
(int code, string state)
```

This expands to:

```c
struct {
    int code;
    string state;
}
```

Usage:

```c
(int code, string state) res = (200, "ok");

res.code;
res.state;
```

---

### 22.4 Naming Rules

If **any field is named**, then **all fields must be named**.

✕ Invalid:

```c
(int code, string)
```

✓ Valid:

```c
(int code, string state)
```

This avoids ambiguity and enforces clarity.

---

### 22.5 Tuple Literals

Tuple values use parentheses:

```c
let t = (1, "hello");
```

Type inference applies:

```c
// inferred as (int, string)
```

---

### 22.6 Tuple Type Inference

```c
let t = (1, 2.5, "x");
```

Infers:

```c
(int, double, string)
```

Inference rules:

* order is preserved
* each element infers independently
* no implicit coercion

---

### 22.7 Tuple Assignment Compatibility

Assignments must match structure exactly:

```c
(int, string) a = (1, "x");        // ok
(int, string) b = (1, 2);          // error
(int, string) c = ("x", 1);        // error
```

---

### 22.8 Tuple Destructuring

Tuples can be destructured:

```c
let (x, y) = (10, 20);
```

Named destructuring:

```c
let (code, msg) = getStatus();
```

Destructuring requires matching arity.

---

### 22.9 Tuples as Function Return Types

```c
(int, string) getStatus() {
    return (200, "OK");
}
```

Usage:

```c
let result = getStatus();
println(result.$0);
```

Or:

```c
let (code, msg) = getStatus();
```

---

### 22.10 Tuples as Parameters

```c
void process((int, string) input) {
    println(input.$0);
}
```

Or with names:

```c
void process((int code, string msg) input) {
    println(input.code);
}
```

---

### 22.11 Tuples vs Structs

| Feature        | Tuple              | Struct        |
| -------------- | ------------------ | ------------- |
| Named type     | No                 | Yes           |
| Anonymous      | Yes                | No            |
| Field naming   | Optional           | Required      |
| Reusable       | No                 | Yes           |
| Layout control | Limited            | Full          |
| Use case       | temporary grouping | data modeling |

---

### 22.12 Tuple Memory Layout

Tuples:

* are laid out like structs
* preserve order
* may include padding
* follow ABI rules
* have no hidden metadata

Example lowering:

```c
(int, string)
```

->

```c
struct {
    int _0;
    string _1;
};
```

---

### 22.13 Tuples and Generics

Tuples can be used inside generics:

```c
struct Pair<T> {
    (T, T) value;
}
```

Or as generic arguments:

```c
void f<T>((T, T) pair) { }
```

---

### 22.14 Tuples and Interfaces

Tuples do **not** implement interfaces.

They are structural values, not nominal types.

---

### 22.15 When to Use Tuples

Use tuples when:

* returning multiple values
* grouping temporary data
* avoiding small one-off structs
* clarity > reuse

Avoid tuples when:

* fields have long-term meaning
* API stability matters
* documentation clarity is important

---

### 22.16 Summary

* tuples are anonymous struct types
* `(T1, T2)` syntax
* optional named fields
* indexed or named access
* compile directly to structs
* no runtime overhead
* ideal for temporary grouping

---

## **23. References, Pointers, and Ownership Semantics**

This section defines how **addresses, references, ownership, and mutation** work in CMinus.
These rules are intentionally close to C, but made more explicit and safer by syntax.

---

### 23.1 Overview

CMinus distinguishes between:

* **values**
* **references (`&`)**
* **pointers (`*`)**
* **ownership / allocation (`new`)**

Each has different guarantees and responsibilities.

---

### 23.2 Values (Baseline)

A value is stored directly in its containing scope.

```c
int x = 10;
```

Properties:

* stored inline (stack or enclosing structure)
* copied on assignment
* no implicit heap allocation
* lifetime bound to scope

---

### 23.3 Pointers (`T*`)

Pointers behave like C pointers.

```c
int* p;
```

#### Pointer properties

* may be `null`
* may be reassigned
* may point to heap or stack
* support pointer arithmetic
* dereferencing invalid pointers is undefined behavior
* no automatic bounds or lifetime checks

---

#### Pointer qualifiers

```c
const int* p;   // pointer to const value
int* const p;   // const pointer
const int* const p;
```

Semantics match C exactly.

---

#### Pointer dereference

```c
*p = 10;
```

If `p` is invalid or null → undefined behavior.

---

### 23.4 References (`T&`)

References represent **non-null, stable aliases** to existing objects.

```c
int a = 10;
int& r = a;
```

#### Properties

* cannot be null
* must be initialized
* cannot be rebound
* always refer to a valid object
* act like implicit dereferenced pointers

Conceptually:

```c
T&  ≈  T* const  (with non-null guarantee)
```

---

#### Reference Usage

```c
r = 20;   // modifies `a`
```

References do not require dereferencing syntax.

---

#### Reference Restrictions

Invalid:

```c
int& r;        // error (must be initialized)
r = other;     // rebinding not allowed
```

---

### 23.5 References vs Pointers

| Feature                 | Reference           | Pointer           |
| ----------------------- | ------------------- | ----------------- |
| Nullable                | ✕                   | ✓                 |
| Rebindable              | ✕                   | ✓                 |
| Requires initialization | ✓                   | ✕                 |
| Syntax dereference      | implicit            | `*`               |
| Can be null             | ✕                   | ✓                 |
| Typical use             | parameters, aliases | ownership, memory |

---

### 23.6 Heap Allocation (`new`)

The `new` keyword explicitly allocates memory on the heap.

```c
int* p = new 10;
```

Equivalent conceptual lowering:

```c
int* p = malloc(sizeof(int));
*p = 10;
```

---

#### Allocation Rules

* `new` always allocates on the heap
* never implicit
* always visible
* allocation strategy is type-defined
* may invoke constructors

---

#### With complex types

```c
Person* p = new Person("Alice", 20);
```

---

### 23.7 Value Assignment vs Heap Allocation

```c
let a = 10;       // stack value
let b = new 10;   // heap allocation
```

Assignment behavior depends on the type:

* value types → copied
* reference types → reference semantics
* heap objects → pointer copy

---

### 23.8 Strings and Allocation

Special rules apply:

```c
string s = "hello";
```

* `string` is always heap-allocated
* dynamic capacity
* resizable

```c
str t = "hello";
```

* `str` is stack-allocated
* fixed-size
* may reference heap when assigned explicitly

```c
str* p = new "hello";
```

---

### 23.9 Ownership Model (High-Level)

CMinus does **not** enforce ownership rules automatically.

Instead:

* ownership is explicit
* lifetime is programmer-managed
* safety tools are optional

Ownership patterns can be built using:

* conventions
* wrappers
* interfaces
* future libraries

---

### 23.10 Copy vs Move Semantics

By default:

* assignment copies values
* references copy aliases
* pointers copy addresses

Move semantics may be defined per-type in the future.

---

### 23.11 Passing Values to Functions

```c
void f(int x) { }
```

* `x` is copied

```c
void g(int& x) { }
```

* receives reference

```c
func h(int* x) { }
```

* receives pointer

---

### 23.12 Returning References and Pointers

Allowed but dangerous:

```c
int& bad() {
    int x = 10;
    return x; // invalid
}
```

Compiler may warn or reject.

---

### 23.13 Lifetime Responsibility

The language does not track lifetimes.

The programmer must ensure:

* references never outlive referents
* pointers are valid when dereferenced
* heap memory is eventually released

---

### 23.14 Safety Summary

* references are safer than pointers
* pointers allow full control
* `new` is explicit
* no hidden allocations
* undefined behavior is allowed but explicit

---

### 23.15 Summary

* `T` → value
* `T*` → pointer (nullable, unsafe)
* `T&` → non-null reference
* `new` allocates on heap
* references cannot be null or rebound
* pointers behave like C
* ownership is explicit and manual

---

## 24. Memory Safety Operators (Overview Only)

CMinus provides **explicit safety operators** that allow the programmer to choose between:

* unchecked behavior (fast, unsafe)
* checked behavior (safe, explicit)
* optional propagation

These operators do **not** change the type system itself — they control *how access and calls behave*.

This section only introduces their meaning. Full semantics belong in `safety.md`.

---

### 24.1 Philosophy of Safety in CMinus

CMinus does **not enforce safety by default**.

Instead:

* unsafe behavior is allowed
* safe behavior must be explicitly requested
* every safety cost is visible in syntax

This ensures:

* predictable performance
* no hidden checks
* clear intent in code review

---

### 24.2 Categories of Safety Operators

CMinus provides operators for:

1. indexing
2. member access
3. pointer dereference
4. function calls
5. error propagation

They follow a consistent pattern:

| Operator | Meaning                            |
| -------- | ---------------------------------- |
| `[]`     | compile-time / unchecked access    |
| `:[]`    | runtime-time / unchecked access    |
| `![]`    | runtime-checked (panic on failure) |
| `?[]`    | safe optional access               |
| `.`      | unchecked member access            |
| `!.`     | checked member access              |
| `?.`     | optional member access             |
| `()`     | normal call                        |
| `!()`    | checked call                       |
| `!!()`   | drop call (panic on failure)       |
| `??()`   | result of call                     |
| `l ?? r` | nullish coalescing operator        |
| `l !! r` | left guard operator                |

---

### 24.3 Index Access Operators

#### Unchecked Indexing

```c
arr[i];
```

* no bounds checks
* undefined behavior if invalid
* fastest form

---

#### Checked Indexing (panic)

```c
arr![i];
```

* runtime bounds check
* panics on invalid index
* used when correctness > speed

---

#### Optional Indexing

```c
arr?[i];
```

Returns an optional:

```c
T?
```

* returns `none` if index invalid
* never panics
* requires handling

---

### 24.4 Member Access Operators

#### Normal member access

```c
obj.field
```

* assumes object is valid
* no checks

---

#### Checked member access

```c
obj!.field
```

* runtime validation
* panics if invalid
* mainly useful with pointers

---

#### Optional member access

```c
obj?.field
```

Returns `T?`.

Used when:

* object may be null
* object may be invalid
* failure should propagate safely

---

### 24.5 Function Call Operators

#### Normal call

```c
f(x)
```

Unchecked.

---

#### Checked call

```c
f!(x)
```

* panics on error
* used when failure is exceptional

---

#### Optional call

```c
f?(x)
```

* returns optional
* propagates failure
* requires handling

---

### 24.6 Optional Propagation

Optional values (`T?`) must be handled explicitly.

```c
string? s = readFile("a.txt");
```

Ways to handle:

#### Pattern check

```c
if (s != none) {
    println(s);
}
```

#### Match against specific error

```c
if (s == none[12]) { ... }
```

### Propagation via operators

```c
let len = s?.length;
```

---

### 24.7 Panic Semantics (Preview)

Panic represents **explicit fatal failure**.

```c
panic("message");
```

* aborts execution
* used only when explicitly requested
* never implicit

Some operators (`![]`, `!()`) may panic internally.

Full panic behavior is defined in `error-handling.md`.

---

### 24.8 Safety Is Local and Explicit

Key rule:

> If a line can fail, it must *look like it can fail*.

CMinus enforces this via syntax rather than runtime systems.

---

### 24.9 What Safety Does NOT Do

Safety operators do **not**:

* add garbage collection
* prevent undefined behavior globally
* track lifetimes
* add reference counting
* change ownership rules
* make code automatically safe

They only add *checks where explicitly requested*.

---

### 24.10 Relationship With Performance

* unchecked operators compile to raw C
* checked operators add branches
* optional operators add control flow
* nothing is implicit
* compiler never inserts checks silently

---

### 24.11 Summary

* safety is opt-in
* syntax makes cost explicit
* `!` → panic on failure
* `?` → return optional
* unchecked operators remain fastest
* safety and performance coexist

---

### Non-Goals of the CMinus Type System

The following are **explicitly not goals**, even if common in other modern languages.

---

#### 1. Automatic Memory Management

CMinus will not provide:

* garbage collection
* automatic reference counting
* implicit lifetime inference

Memory management is explicit and visible.

---

#### 2. Implicit Safety Guarantees

The language will not:

* auto-insert bounds checks
* auto-null-check pointers
* silently prevent undefined behavior
* rewrite unsafe logic

Safety must be requested using explicit syntax.

---

#### 3. Runtime Polymorphism

CMinus does not provide:

* virtual dispatch
* vtables
* runtime interface objects
* dynamic method lookup

Polymorphism is resolved at compile time.

---

#### 4. Reflection or Runtime Type Introspection

There is no:

* `typeOf(x)`
* runtime RTTI
* reflection API
* dynamic inspection

All type reasoning happens at compile time.

---

#### 5. Implicit Heap Allocation

The compiler will never:

* allocate memory automatically
* move values to heap silently
* convert stack values into heap objects

Heap allocation always requires `new`.

---

#### 6. Hidden Control Flow

CMinus avoids:

* implicit exception throwing
* hidden panics
* silent control flow changes
* invisible error propagation

Control flow must be explicit in syntax.

---

#### 7. Dynamic Typing

CMinus does not support:

* dynamically changing variable types
* runtime type mutation
* union-by-assignment semantics

All types are known at compile time.

---

#### 8. Implicit Conversions

No automatic:

* numeric widening
* numeric narrowing
* pointer coercion
* boolean conversion
* enum-to-int coercion

All conversions must be explicit.

---

#### 9. Language-Level Memory Safety Guarantees

CMinus does not attempt to prove:

* lifetime correctness
* alias safety
* race freedom
* thread safety

These may be built externally or via conventions.

---

#### 10. Magical Optimizations

The compiler will not:

* assume programmer intent
* rewrite semantics
* change visible behavior
* “do what you probably meant”

Optimizations must preserve exact meaning.