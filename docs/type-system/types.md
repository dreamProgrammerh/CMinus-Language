# Type System

This document defines the **CMinus (C-) type system**.

The type system is designed to:
- stay close to C’s mental model
- avoid hidden or surprising runtime behavior
- preserve performance predictability
- provide a rich set of primitive and composite types

> This is a **design document**, not a compiler reference.

---

## Goals

The CMinus type system aims to:
- map cleanly to **C types**
- be **explicit**, not implicit
- make unsafe operations visible
- allow zero-cost abstractions where possible

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

## Overview of Types

CMinus provides the following categories of types:

- Primitive types
- Pointer types
- Constant types
- Length-prefixed types
- Array and slice types
- String and str types
- Struct and union types
- Variant and optional types
- Data and class types
- Function types

Compile-time-only types:
- `final`
- `enum`
- `typedef`
- `generic`
- `options`
- `interface`
- `table`

---

## Primitive Types

Primitive types map directly to C primitives.

| CMinus Type   | Description                     | C Mapping     |
|---------------|---------------------------------|---------------|
| **Integers**  |||
| `i8`          | 8-bit signed integer            | `int8_t`      |
| `i16`         | 16-bit signed integer           | `int16_t`     |
| `i32`         | 32-bit signed integer           | `int32_t`     |
| `i64`         | 64-bit signed integer           | `int64_t`     |
| **Unsigned Integers** |||  
| `u8`          | 8-bit unsigned integer          | `uint8_t`     |
| `u16`         | 16-bit unsigned integer         | `uint16_t`    |
| `u32`         | 32-bit unsigned integer         | `uint32_t`    |
| `u64`         | 64-bit unsigned integer         | `uint64_t`    |
| **Floating Point** |||  
| `f32`         | 32-bit float                    | `float`       |
| `f64`         | 64-bit float                    | `double`      |
| **Size Types** |||
| `isize`       | pointer-sized signed integer    | `intptr_t`    |
| `usize`       | pointer-sized unsigned integer  | `uintptr_t`   |
| **Characters** |||
| `char`        | UTF-8 byte                      | `uint8_t`     |
| `wchar`       | UTF-16 unit                     | `uint16_t`    |
| `rune`        | UTF-32 unit                     | `uint32_t`    |
| `bchar`       | byte chunk encoding             | `uint8_t`     |
| **Logic** |||
| `bool`        | boolean value                   | `uint8_t`     |
| `trnry`       | ternary logic value             | `int8_t`      |
| `void`        | no value                        | `void`        |

Example:

```c
i32 count = 10;
f64 pi = 3.14159;
bool enabled = true;
void f() {}
```

No implicit widening or narrowing conversions occur.

---

## Pointer Types

Pointers behave like C pointers (subject to refinement).

```c
int* ptr;
const int* freeze;
int* const reference = &a;
const int* const locked = &a;
```

Example:

```c
int a = 10;
int b = 15;

int* ptr = &a;                  // `ptr` is an int pointer
*ptr = 20;                      // Allowed: modifying the value at the address
ptr = &b;                       // Allowed: changing the address stored in `ptr`

const int* freeze = &a;         // `freeze` is a const int pointer
*freeze = 20;                   // Error: cannot modify the value at the address
freeze = &b;                    // Allowed: changing the address stored in `freeze`

int* const reference = &a;      // `reference` is a const pointer to int
*reference = 20;                // Allowed: modifying the value at the address
reference = &b;                 // Error: cannot change the address stored in `reference`

const int* const locked = &a;   // `locked` is a const pointer to const int
*locked = 20;                   // Error: cannot modify the value at the address
locked = &b;                    // Error: cannot change the address stored in `locked`
```

Pointer rules follow standard C semantics regarding mutability and address stability.

### Reference Types

```c
int& ref = a;
ref = 20;                       // Allowed: modifying the value at the address
ref = b;                        // Allowed: changing the address stored in `ref`

const int& cref = a;
cref = 20;                      // Error: cannot modify the value at the address
cref = b;                       // Error: cannot change the address stored in `cref`
```

Reference rules follow standard C semantics regarding mutability, address stability, and they can't be null.

Same as:

```c
int* const ref = &a;
```

### Safety

* Reference arithmetic is allowed
* Null references are allowed (pointers only)
* Dereferencing invalid references is undefined behavior

CMinus does **not** add runtime reference checks by default.

---

## Constant types

```c
const int a = 5;
a = 10;             // Error: cannot modify constant value
```

Constant types are immutable and cannot be modified after initialization.

* Unmodifiable variable
* good for optimization  


## Array Types

### Fixed-Size Arrays+++

```c
int[10] values;
```

* Stored contiguously
* Size known at compile time
* Maps directly to C arrays

### Length-Prefixed Arrays

```c
int[~10] arr = [1, 2, 3];
```

* Length stored explicitly
* O(1) length access
* Safer than raw C arrays

---

## Slice Types

```c
int[] slice1 = arr.subarr(0, 5);
int[] slice2 = {*arr[0], 5};
```

A slice consists of:

* a pointer
* a length

Slices do **not** own memory.

---

## String Types

### Dynamic Strings

```c
string str = new "Hello, World!";
string<32> str2 = new "string with 32 byte capacity";
```

* Mutable
* Can grow or shrink
* Convertible to/from C strings
* O(1) byte access
* Encoding-aware character access

### Fixed-Size Strings

```c
str<16> fixedStr = "Fixed string";
str str = "Auto detected string length";
```

* Immutable
* Size known at compile time
* No dynamic allocation unless explicitly requested
* can act as `string` as long as it is not modified

### Null-Terminated Strings

```c
char* nullStr = "Null terminated string\0";
char[32] nullTermStr = "Null terminated string\0";
```

* C-compatible
* `char*` is immutable
* `char[]` is mutable
* Null-terminated

---

## Struct Types

```c
struct Point {
    int x;
    int y;
}
```

* Predictable layout
* Field order preserved unless layout optimization is enabled
* No hidden padding except ABI-required cases

---

## Union Types

```c
union Color {
    u32 rgba;
    struct {
        byte r;
        byte g;
        byte b;
        byte a;
    };
}
```

* Predictable layout
* No hidden padding unless required by ABI

---

## Variant Types

```c
variant{int, string} value;
```

Variants are tagged unions:

* One active type at a time
* Explicit type checks required
* Padding added only when needed

```c
if (value is int) {
    int x = value as int;
}
```

---

## Optional Types

```c
int? maybeInt = none;
maybeInt = 42;
```

Optional types represent a value that may or may not be present.

* Safe default value
* Explicit checks required

```c
if (maybeInt != none) {
    int x = maybeInt;
}
```

---

## Data Types (Experimental)

```c
data Player {
    hot vec3<float> position;
    hot vec3<float> velocity;
    warm i64 score;
    cold string name;
    u32 id;
}
```

Data types are split into multiple internal layouts optimized by access frequency.

* Improved cache locality
* Reduced padding
* Efficient iteration
* Access remains transparent to the user

---

## Class Types

```c
class Person {
    string name;
    u32 age;

    main(this.name, this.age);
}
```

Classes are structs with inheritance support.

* Optional layout optimization
* Compile-time inheritance
* No hidden runtime dispatch unless explicitly enabled

---

## Function Types

```c
i32 add(i32 a, i32 b) {
    return a + b;
}
```

* Known return types
* Predictable behavior
* Compile-time optimization friendly

---

## The `new` Keyword

The `new` keyword explicitly requests heap allocation.

```c
let s  = "allocate on heap when needed";
let s2 = new "use the new keyword to allocate directly on heap";

new 10;
new "string";
new Person("nobody");
```

* `new` makes allocation **explicit**
* Allocation behavior without `new` is intentionally conservative
* Exact allocation rules are **subject to discussion**

The goal is to:

* avoid surprises
* keep code easy to reason about
* make memory behavior visible

---

## Compile-Time Constants (`final`)

```c
final int threshold = 10;
final float pi = 3.14159265358979323846;
```

Final are compile-time constant, you cannot use runtime values inside them.

* Can only contain compile-time constants
* No runtime values allowed
* Get resolved during compilation
* No runtime overhead

---

## Named magic Numbers (`enum`)

```c
enum Color {
    Red = 0xFF0000,
    Green = 0x00FF00,
    Blue = 0x0000FF,
};

enum: u8 Direction {
    Up,
    Down,
    Left,
    Right,
};

Color color = Color::Red;               // 0xFF0000
Direction direction = Direction::Up;    // 0
```

Enums are a compile-time constant, you cannot use runtime values inside them.

* Can only contain compile-time constants
* No runtime values allowed
* Get resolved during compilation
* No runtime overhead

---

## Type Aliases (`typedef`)

`typedef` introduce a compile-time alias for an existing type.

```c
typedef flags = u64;
typedef any = variant{i64, f64, bool, trnry, rune, string};
typedef vec3<T> = (T x, T y, T z);
typedef callback = void();
typedef color = union {
    u32 rgba = 0;
    struct {
        byte r, g, b, a;
    }
}
```

**Type aliases:**
* do **not** create new types
* have **no runtime cost**
* are fully erased during compilation
* exist only to improve readability and maintainability

> Aliased types are always interchangeable with their underlying types.

### Scope and Visibility

type aliases follow normal scoping and module rules.

Visibility control (such as public or private aliases) is handled by the module system, not by `typedef` itself.

---

## Generic 

Generics in CMinus allow types and functions to be parameterized by other types.

Generics are a **compile-time-only feature**.  
All generic parameters are resolved during compilation and produce concrete C code.

There is **no runtime representation** of generics.

### Basic Usage

```c
List<int> numbers;
Map<string, int> table;

typedef vec3<t> = (t, t, t);
typedef mat4<t> = t[4][4];

vec3<float> position;
mat4<int> camera;
```
  
Generic parameters must be fully known at compile time.

**Semantics**
* Generic instantiations generate concrete types
* No type information exists at runtime
* No dynamic dispatch is introduced
* Predictable performance
* Each specialization is compiled independently

---

## Scoped Type Wrapper (`options`)

`options` defines a **scoped set of named constants** associated with a base type.

The declared names exist **only within the scope where the option is used** and are erased during compilation.

**Definition scope:**
```c
options<int>{up, down, left, right} direction = up;
// `up`, `down`, `left`, `right` are visible only here
// underlying type is `int`
// `up` maps to `0`, `down` to `1`, etc.
```

**Completion result:**
```c
options<u32>{red=0xFF0000, green=0x00FF00} color = green;

// after compilation
u32 color = 0x00FF00;
```

**Type aliases scope:**
```c
typedef ID = options<variant{str, int}>{s="8496814", i=544658};
ID id = s;
```

**Function scope:**
```c
options<i8>{less=-1, equal=0, more=1}
compare(str s1, str s2) {
    return less when s1 < s2;
    return more when s1 > s2;
    return equal;
}
```

* `options` wraps an existing type
* Each option maps to a constant value of that type
* Option names are scoped and non-exported
* All names are resolved at compile time
* No runtime metadata is generated

### **Design Intent**

**Properties**
* Can be used with type aliases
* Has zero runtime cost
* Improves readability and intent
* Acts as a lightweight, inline alternative to enums

`options` exists to:
* avoid global enums
* prevent name pollution
* express intent locally
* remain fully C-compatible

It is intentionally minimal and does not introduce new types or runtime behavior.

---

## Interfaces (Compile-Time Prototypes)

An `interface` defines a **compile-time prototype** for types.

It describes the **required structure and behavior** of a type, without introducing inheritance, runtime dispatch, or metadata.

Interfaces exist only at compile time and are fully erased during compilation.

**Basic Usage**
```c
interface Drawable {
    void draw();
}
```
```c
struct Sprite : Drawable {
    vec2 position;
    void draw() {
        canvas.drawImage(image, position.x, position.y);
    }
}

class Player : Drawable  {
    string name;
    void draw() {
        // draw player
    }
}
```

- An interface defines a prototype, not a base type
- Implementing an interface means:
  - required fields exist (if specified)
  - required functions exist with matching signatures
- All checks are performed at compile time
- No runtime indirection is introduced

### Relationship to Structs and Classes
- Structs and classes may implement interfaces
- Interfaces do not imply inheritance
- Multiple interfaces may be implemented by the same type
- Interface implementation does not change the underlying type

The interface is used only for verification, not composition.

---

## Static Tables (`table`)

A `table` defines a **compile-time static lookup table**.

Tables are resolved entirely during compilation and stored in **static memory**.
All accesses are lowered to **direct index-based access** in the generated C code.

```c
table<string> Colors {
    "red"    = "red color",
    0x00FF00 = "green hex",
    10       = "special",
    'A'      = "capital",
};
````

* A table maps **compile-time constant keys** to values
* Keys are resolved and indexed at compile time
* Values are stored in a contiguous static array
* No runtime hashing or lookup is performed
* Tables are immutable after compilation

---

### Table Value Type

The type parameter in `table<T>` specifies the **value type** stored in the table.

At compilation, the table is lowered to a static array of that type:

```c
table<string> Colors { ... };

// lowered form
string Colors[];
```

All values in the table must be compatible with `T`.

#### Variant Values

To store values of different types, a variant may be used as the table type:

```c
table<variant{string, int}> Mixed {
    "name" = "mmd",
    "age"  = 18,
};
```

The generated storage remains a single static array.

---

### Keys

#### Allowed Key Types

Table keys may be **any compile-time constant**, including:

* string literals
* character literals
* numeric literals (decimal, hex, binary, octal)
* boolean values
* `null`, `none`, and similar literals
* `final` compile-time bindings

```c
final key = "red";
Colors[key];     // equivalent to Colors["red"]
```

Runtime values are not allowed as keys.

---

### Access Forms

#### Key-Based Access

```c
Colors["red"];        // "red color"
Colors[0x00FF00];     // "green hex"
```

If the key is known at compile time, access is resolved to a constant index.

---

#### Compile-Time Index Access

```c
Colors::"red";        // compile-time index of "red"
```

The `::` operator retrieves the index assigned to a key during compilation.

---

#### Index-Based Access

```c
Colors![3];           // unchecked access, panics if out of bounds
Colors?[99];          // safe access, returns optional
```

* `!` performs unchecked access
* `?` performs safe access and returns an optional value

---

### Key Normalization (Internal)

All table keys are normalized into internal string identifiers during compilation.
This process is deterministic and exists solely to ensure stable indexing.

Examples of internal normalization:

```text
"red"        -> "$$red"
'x'          -> "$x"
1            -> "1"
-1           -> "_1"
0xA          -> "10"
true         -> "Ztrue"
false        -> "Zfalse"
null         -> "Znull"
```

> Key normalization is an **internal compiler detail**.
> Programs must not depend on the exact normalized representation.

---

### Properties

* Stored in static memory
* Zero runtime lookup cost
* Predictable performance
* Fully C-compatible output
* Suitable for systems and embedded code

---

### Error Handling

* Duplicate keys result in a compile-time error
* Invalid runtime keys are rejected at compile time
* Out-of-bounds index access behavior is explicit (`!` or `?`)

---

### Design Intent

Static tables exist to:

* replace runtime maps with compile-time data
* eliminate hashing and dynamic allocation
* provide fast, index-based lookup
* keep behavior explicit and predictable

Tables intentionally do **not**:

* allocate memory at runtime
* support mutation
* perform runtime key comparison
* introduce hidden control flow

---

### Example: Complete Usage

```c
table<variant{string, int}> Data {
    "name" = "mmd",
    "age"  = 18,
    1      = "one",
};

println(Data["name"]);
println(Data![1]);
println(Data?[99] ?? "none");
```

All accesses compile down to direct array indexing.

---

## `const` vs `final`

`const` and `final` are both compile-time constants, but they have different characteristics:

* `const` is a variable that cannot be modified after initialization (runtime fold).
* `final` is a constant that cannot be modified after initialization (compile-time fold).

### **Const**

```c
final int z = 10;

int a = 14;
const int b = 5;
const int c = b * 2;
const int d = a + z + 1;
const int e = func();

final int f = d;                // Error: runtime values cannot be used in final's
a = 10;                         // Error: cannot modify constant value
int[b] array = [1, 2, 3];       // Error: cannot use runtime values inside const
struct s { int a = d }          // Error: cannot use runtime values as default value
```

**What is possible with `const` variables?**

* Use runtime values, function calls, and expressions to initialize `const` values
* Use one `const` variable to initialize another `const` variable
* Use runtime variables to initialize `const` variables
* Use `final` variables to initialize `const` variables

**What is not possible with `const` variables?**

* `const` variables cannot be used to initialize `final` variables
* `const` variables cannot be modified after initialization
* `const` variables cannot be used as default parameter values

### **Final**

```c
final int a = 10;
final int b = a * 2;

const int c = b / 2;
int d = a + c - 1;
final int e = c + 1;            // Error: cannot use runtime values inside final

int[b] array = [1, 2, 3];       // Valid
struct s { int a = d }          // Valid
```

**What is possible with `final` variables?**
* Use compile-time constants and other `final` variables to initialize new `final` variables
* `final` variables can be used to declare array sizes and similar compile-time constructs
* `final` variables can be used as default values in structs and function parameters
* `final` variables can be used in runtime expressions

**What is not possible with `final` variables?**
* `final` variables cannot be initialized with runtime values, function calls, or non-constant expressions
* `final` variables cannot be modified after initialization
* `final` variables cannot be used to initialize other `final` variables if they depend on runtime values


### **Key difference:**
- **`const`**: Can be initialized with runtime values, but becomes immutable thereafter
- **`final`**: Must be initialized with compile-time constants only, but then behaves like a constant

---

## Type Safety Model

CMinus safety is **explicit and local**.

### Safe Operations

* Type-checked assignments
* Explicit casts
* Compile-time type checking
* Optional bounds checking

### Unsafe Operations

* Raw pointer arithmetic
* Manual memory management
* Unsafe casts

Unsafe operations must be written explicitly.

---

## Performance Model

The CMinus type system aims to provide:

* No garbage collection
* No hidden reference counting
* No runtime reflection
* Predictable allocation behavior

Exact allocation strategies are intentionally left open for discussion.

---

## Use Cases

### Systems Programming

* Predictable layouts
* Manual control
* Low overhead abstractions

### Game Development

* Data-oriented design
* Cache-friendly structures

### Embedded Systems

* Portable C output
* No mandatory runtime

---

## Non-Goals

The CMinus type system intentionally does **not** provide:

* Automatic garbage collection
* Runtime reflection
* Implicit polymorphism
* Hidden object lifetimes

These may exist as optional layers, but not in the core language.

---

## Conclusion

CMinus provides a safe and predictable type system that is suitable for a wide range of applications. By explicitly managing memory and avoiding hidden object lifetimes, CMinus ensures that developers have full control over their code's behavior.