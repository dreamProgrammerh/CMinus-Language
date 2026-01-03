# **Pointer Types (`T*`)**

Pointer types represent **raw memory addresses**.
They behave similarly to C pointers and provide no automatic safety.

Pointers are intentionally low-level and explicit.

---

## 1 Pointer Declaration

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

## 2 Pointer Initialization

```c
int x = 10;
int* p = &x;
```

The address-of operator (`&`) produces a pointer.

---

## 3 Null Pointers

Pointers may be null:

```c
int* p = null;
```

Dereferencing a null pointer is undefined behavior.

---

## 4 Dereferencing

```c
int value = *p;
```

Dereferencing reads or writes the pointed value.

If the pointer does not point to valid memory, behavior is undefined.

---

## 5 Pointer Assignment

```c
int a = 1;
int b = 2;

int* p = &a;
p = &b;   // allowed
```

Pointers are reassignable unless declared `const`.

---

## 6 Const and Pointer Placement

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

## 7 Pointer Arithmetic

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

## 8 Pointer Comparison

Pointers may be compared:

```c
p1 == p2;
p1 < p2;
p1 > p2;
```

Results are meaningful only when pointers refer to the same allocation region.

---

## 9 Pointer Safety Model

Pointers provide **no safety guarantees**:

* null dereference -> undefined behavior
* out-of-bounds access -> undefined behavior
* lifetime violations -> undefined behavior

All pointer safety must be enforced manually or via higher-level abstractions.

---

## 10 Pointer vs Reference

| Pointer (`T*`)       | Reference (`T&`) |
| -------------------- | ---------------- |
| nullable             | never null       |
| rebindable           | not rebindable   |
| arithmetic allowed   | no arithmetic    |
| unsafe               | safer            |
| explicit dereference | implicit         |

---

## 11 Lowering

Pointers lower directly to C pointers with identical semantics.

No metadata, tracking, or checks are added.

---

## **Summary**

* pointers represent raw memory addresses
* fully compatible with C
* unsafe by design
* explicit nullability
* explicit dereferencing
* no automatic checks
