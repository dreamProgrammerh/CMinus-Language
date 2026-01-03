# **Generics (Compile-Time Parametric Types)**

Generics in CMinus provide **compile-time parametrization** of types, functions, and other constructs.
They are resolved entirely at compile time and **never exist at runtime**.

CMinus uses **monomorphization**, not type erasure.

---

## 1 What Generics Are

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

## 2 Where Generics Are Allowed

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

## 3 Generic Parameters Syntax

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

## 4 Generic Functions

```c
void swap<T>(T& a, T& b) {
    T& tmp = a;
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

## 5 Type Inference for Generics

In many cases, generic arguments are inferred automatically:

```c
swap(1, 2);          // T = int
swap("a", "b");      // T = string
```

If inference fails, explicit arguments are required.

---

## 6 Generic Constraints (Interfaces)

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

## 7 Restricted Generic Sets

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

---

## 8 Generic Constants

Generic constants are **compile-time values** used to configure types.

They behave like generic parameters, but represent **values instead of types**.

```c
<uint Capacity>
```

They may include compile-time constraints:

```c
<uint Capacity -> Capacity > 0 && Capacity < 1024>
```

Generic constants are:

* evaluated at compile time
* immutable
* erased after compilation
* usable in type definitions
* required to satisfy their constraints

---

### **Mixing Constants and Types**

Generic constants can be combined with type parameters:

```c
<uint Capacity, T>
```

With constraints:

```c
<uint Capacity -> Capacity > 0, T::str | string | char[]>
```

---

### Example: Generic String Type

```c
struct string<
    u32 Capacity = 256 -> Capacity > 32,
    Char = c8 : Character
> {
    u32 capacity = Capacity;
    u32 length = 0;
    Char* data;
}
```

### Usage

```c
string str1 = "one";              // Capacity = 256, Char = c8
string<32> str2 = "one";          // Capacity = 32
string<32, c32> str3 = u32"one";  // Capacity = 32, Char = c32
```

Invalid:

```c
string<16> s = "one"; // error: constraint not satisfied
```

---

### Rules Summary

* generic constants are compile-time only
* must satisfy declared constraints
* may have default values
* may be used in layout and initialization
* generate no runtime data
* participate in monomorphization
* vanish after compilation

---

## 8 Generic Behavior by Construct Type

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

## 9 Generics and Code Generation

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

## 10 Generics and Performance

Generics introduce:

* no dynamic dispatch
* no boxing
* no heap overhead
* no runtime cost

All decisions are compile-time.

---

## 11 Invalid Generic Usage

Errors include:

```c
T f<T>(T x) {
    x + 1; // error: operator not guaranteed
}
```

open to discuss if it should be warning instead of error.

Unless constrained:

```c
void f<T: Numeric>(T x) {
    x + 1; // ok
}
```

---

## 12 Generic Defaults (Future)

Default generic arguments may be added later:

```c
struct Vec<T = float> {
    T x;
    T y;
}
```

This is not currently required but planned.

---

## 13 Summary

* generics are compile-time only
* monomorphized, not erased
* usable on most language constructs
* support interface constraints
* support restricted type sets
* produce zero runtime overhead
* integrate naturally with C output
