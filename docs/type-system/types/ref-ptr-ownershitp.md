# **References, Pointers, and Ownership Semantics**

This section defines how **addresses, references, ownership, and mutation** work in CMinus.
These rules are intentionally close to C, but made more explicit and safer by syntax.

---

## 1 Overview

CMinus distinguishes between:

* **values**
* **references (`&`)**
* **pointers (`*`)**
* **ownership / allocation (`new`)**

Each has different guarantees and responsibilities.

---

## 2 Values (Baseline)

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

## 3 Pointers (`T*`)

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

## 4 References (`T&`)

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

## 5 References vs Pointers

| Feature                 | Reference           | Pointer           |
| ----------------------- | ------------------- | ----------------- |
| Nullable                | ✕                   | ✓                 |
| Rebindable              | ✕                   | ✓                 |
| Requires initialization | ✓                   | ✕                 |
| Syntax dereference      | implicit            | `*`               |
| Typical use             | parameters, aliases | ownership, memory |

---

## 6 Heap Allocation (`new`)

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

## 7 Value Assignment vs Heap Allocation

```c
let a = 10;       // stack value
let b = new 10;   // heap allocation
```

Assignment behavior depends on the type:

* value types → copied
* reference types → reference semantics
* heap objects → pointer copy

---

## 8 Strings and Allocation

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

## 9 Ownership Model (High-Level)

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

## 10 Copy vs Move Semantics

By default:

* assignment copies values
* references copy aliases
* pointers copy addresses

Move semantics may be defined per-type in the future.

---

## 11 Passing Values to Functions

```c
void f(int x) { }
```

* `x` is copied

```c
void g(int& x) { }
```

* receives reference

```c
void h(int* x) { }
```

* receives pointer

---

## 12 Returning References and Pointers

Allowed but dangerous:

```c
int& bad() {
    int x = 10;
    return x; // invalid
}
```

Compiler may warn or reject.

---

## 13 Lifetime Responsibility

The language does not track lifetimes.

The programmer must ensure:

* references never outlive referents
* pointers are valid when dereferenced
* heap memory is eventually released

---

## 14 Safety Summary

* references are safer than pointers
* pointers allow full control
* `new` is explicit
* no hidden allocations
* undefined behavior is allowed but explicit

---

## **Summary**

* `T` → value
* `T*` → pointer (nullable, unsafe)
* `T&` → non-null reference
* `new` allocates on heap
* references cannot be null or rebound
* pointers behave like C
* ownership is explicit and manual
