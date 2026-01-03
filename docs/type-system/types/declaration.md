# **Variable Declarations, Modifiers, and Binding Rules**

CMinus separates **type**, **mutability**, and **compile-time guarantees**.
Variable declarations consist of:

* an optional **modifier** (`let`, `const`, `final`)
* an optional **explicit type**
* a required **initializer** (except in specific cases)

---

## 1 Default Declaration (`let`)

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

## 2 Untyped Declarations (Type Inference)

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

## 3 `const` — Runtime Immutability

```c
const int a = readValue();
```

* value cannot be reassigned
* value may depend on runtime data
* enforces immutability for safety and intent

`const` does **not** imply compile-time evaluation.

The compiler may optimize `const` values when provably safe, but this is not a semantic guarantee.

---

## 4 `final` — Compile-Time Constant

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

## 5 Reassignment Rules

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

## 6 Explicit Type Overrides Inference

When a type is specified, inference is bypassed.

```c
let int a = 1;       // explicit
let int a = 1.0;     // error
```

Explicit typing always wins over inferred typing.

---

## 7 Binding vs Allocation

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

## **Summary**

* `let` is default and implicit
* `const` enforces runtime immutability
* `final` enforces compile-time availability
* inference is local, single-shot, and strict
* no implicit conversions
* no dynamic typing
