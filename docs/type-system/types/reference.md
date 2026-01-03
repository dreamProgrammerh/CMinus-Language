# **Reference Types (`T&`)**

References represent **non-null, stable aliases** to existing objects.
They provide safer access than pointers while preserving zero-cost semantics.

A reference always refers to a valid object and cannot be reseated.

---

## 1 Reference Declaration

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

## 2 Assignment Through References

```c
r = 20;
```

Modifies the referenced object (`x`).

---

## 3 Reference Rebinding Is Not Allowed

```c
int a = 1;
int b = 2;

int& r = a;
r = b;   // assigns value of b to a
```

This does **not** rebind the reference.

Rebinding is illegal and impossible.

---

## 4 Reference Lifetime Rules

A reference must not outlive the object it refers to.

Binding to a temporary or invalid object is forbidden:

```c
int& r = 10;   // error
```

---

## 5 Reference Semantics

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

## 6 References vs Pointers

| Feature             | Reference (`T&`) | Pointer (`T*`) |
| ------------------- | ---------------- | -------------- |
| Null                | ✕                | ✓              |
| Rebind              | ✕                | ✓              |
| Arithmetic          | ✕                | ✓              |
| Must be initialized | ✓                | ✕              |
| Dereference syntax  | implicit         | explicit       |
| Safety              | higher           | lower          |

---

## 7 Passing by Reference

```c
void inc(int& x) {
    x += 1;
}
```

This avoids copying and guarantees a valid target.

---

## 8 References and ABI

References lower to pointers at the ABI level, but the compiler enforces:

* non-null
* no rebinding
* valid lifetime

These rules exist only at compile time.

---

## **Summary**

* references are non-null aliases
* always bound at initialization
* cannot be reseated
* safer than pointers
* zero runtime cost
