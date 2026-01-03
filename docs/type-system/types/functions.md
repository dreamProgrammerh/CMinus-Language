# **Function Types**

Functions in CMinus are **first-class values** with statically known signatures.
They can be stored, passed, and returned, while remaining fully predictable and analyzable.

---

## 1 Function Declaration

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

## 2 Function Signature

A **function signature** describes the *name* of the function together with its **parameter types** and **return type**.  
It is the unique identity used by the compiler for overload resolution and type checking.

Example:

```c
int add(int a, int b);
```

Signature: `add(int,int) -> int`

* The **name** is `add`  
* The **parameters** are `(int, int)`  
* The **return type** is `int`

Signatures are always statically known and cannot change at runtime.  
Defaults and variadics are handled at compile time, but do not alter the canonical signature.

> For more depth and full rules, see **function-signature.md**.

---

## 3 Function Type Syntax

The type of a function is written as:

```c
int(int, int)
```

Meaning:

> a function taking `(int, int)` and returning `int`

Example:

```c
int(int, int) f = func add(int, int);
```
`func` keyword to access a function reference or pointer at **compile time** based on its signature.

reads like “give me the function add(int,int)"

---

## 4 Calling Functions

```c
int x = add(1, 2);
```

Function calls are direct and statically resolved.

---

## 5 Function Values

Functions can be assigned to variables:

```c
int(int, int) op = func add(int, int);
```

They may be passed as arguments:

```c
void apply(int(int, int) f) {
    f(1, 2);
}
```

---

## 6 Function Pointers vs Function Values

Internally, function values lower to function pointers.

However, semantically:

* functions are immutable
* function values cannot be modified
* only referenced

---

## 7 Returning Functions

```c
int(int, int) choose(bool add) {
    if (add) return func addFn(int, int);
    return func subFn(int, int);
}
```

---

## 8 No Closures (by default)

Functions do **not** capture outer variables.

```c
int x = 10;
let f = \() -> x;       // ✕ not allowed
let f = \[x]() -> x;    // explicit capture
let f = \[&x]() -> x;   // explicit capture by reference
```

By default only explicitly declared parameters are accessible.

This keeps functions predictable and ABI-friendly.

---

## 9 Function Overloading

Functions may be overloaded by parameter types.

```c
int add(int a, int b);
float add(float a, float b);
```

Resolution is compile-time.

---

## 10 Function Types and Pointers

Function types can be used wherever types are allowed:

```c
typedef Adder = int(int, int);
```

They can be used in structs, tables, or generics.

---

## **Summary**

* functions are first-class values
* statically typed and resolved
* no closures
* no runtime dispatch
* compatible with C function pointers
