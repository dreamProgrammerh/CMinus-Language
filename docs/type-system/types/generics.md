# **Generics**

Generics allow writing reusable definitions parameterized by types.
CMinus supports generics for both **runtime-generating constructs** and **compile-time–only constructs**, with different semantics.

---

## 1 Where Generics Are Allowed

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

## 2 Generic Syntax

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

## 3 Multiple Generic Parameters

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

## 4 Generic Functions

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

## 5 Generic Constraints (Interface Bounds)

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

We are planing to introduce generic constants:

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

## 6 Runtime vs Compile-Time Generics

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

## 7 Generic + typedef

```c
typedef Result<T> = variant{T, Error};
```

Used as:

```c
Result<int> r;
```

This creates **no runtime type**, only a rewritten type expression.

---

## 8 Generic + options

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

## 9 Generic + table

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

## 10 Generic Instantiation Rules

* every concrete type combination is independent
* no runtime polymorphism
* no boxing
* no vtables
* no implicit casting between instantiations

```c
Box<int> != Box<float>
```

---

## 11 Monomorphization Model

CMinus uses **full monomorphization**:

* each concrete use produces its own version
* optimized independently
* fully inlined where possible

This ensures:

* predictable performance
* zero abstraction overhead
* C-like output

---

## 12 What Generics Do *Not* Do

Generics do NOT:

* introduce inheritance
* introduce runtime dispatch
* erase types
* allocate memory
* imply polymorphism

They are purely compile-time expansion tools.

---

## **Summary**

* Generics exist for both runtime and compile-time constructs
* Runtime generics generate code per type
* Compile-time generics vanish after validation
* Constraints enforce structure and intent
* No runtime cost or hidden behavior
* Designed to remain C-like and predictable
