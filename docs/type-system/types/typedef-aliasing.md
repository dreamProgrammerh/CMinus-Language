# **`typedef` — Type Aliasing and Compile-Time Types**

`typedef` defines a **compile-time alias** for a type expression.
It introduces *no runtime symbol*, allocates no memory, and generates no code.

Its purpose is:

* readability
* abstraction
* reuse
* expressing intent
* simplifying complex types

---

## 1 Basic Syntax

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

## 2 `typedef` Is Compile-Time Only

A `typedef`:

* does **not** create a new type
* does **not** affect layout
* does **not** exist at runtime
* does **not** generate symbols
* is fully substituted during compilation

Think of it as a named macro for types, but type-checked.

---

## 3 Complex Type Aliases

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

## 4 Typedef With Generics

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

## 5 Typedef With Constraints

You may constrain generic parameters:

```c
typedef Number<T: Numeric> = T;
```

This restricts valid substitutions at compile time.

---

## 6 Typedef vs Struct

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

## 7 Typedef and Readability

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

## 8 Typedef With Pointers

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

## 9 Typedef With Function Types

```c
typedef Comparator<T> = int(T, T);
```

Usage:

```c
Comparator<int> cmp;
```

---

## 10 Typedef With Arrays

```c
typedef Matrix = float[4][4];
```

Or generic:

```c
typedef Array<T, N> = T[N];
```

---

## 11 Typedef With Optional and Variant

```c
typedef Maybe<T> = T?;
typedef Result<T> = variant{T, Error};
```

These are aliases only; behavior comes from the underlying type.

---

## 12 Typedef Visibility and Scope

* `typedef` follows normal scope rules
* may be declared at:

  * file level
  * inside modules
  * inside blocks

Inner definitions shadow outer ones.

---

## 13 Typedef and Code Generation

During compilation:

1. typedefs are resolved
2. substituted into full types
3. removed entirely
4. remaining code is compiled normally

There is no trace of `typedef` in generated C.

---

## 14 When to Use `typedef`

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

### **Summary**

* `typedef` creates compile-time aliases
* zero runtime cost
* supports generics and constraints
* expands fully before codegen
* ideal for readability and abstraction
* not a real type
