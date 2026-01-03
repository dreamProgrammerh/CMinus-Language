# **Union Types**

Union types define a value that can occupy **one of several overlapping representations**.
All members share the same memory region.

Unions are low-level constructs and map directly to C `union`.

---

## 1 Defining a Union

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

## 2 Memory Layout

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

## 3 Access Rules

```c
Color c;
c.rgba = 0xFF00FF00;

u8 red = c.r;
```

The language does not track which field is “active”.

Reading a field different from the last written one is allowed but semantically unsafe unless explicitly intended.

---

## 4 Safety and Responsibility

Unions provide **no safety guarantees**:

* no active-member tracking
* no runtime checks
* no automatic initialization
* no destruction semantics

Correct usage is entirely the programmer’s responsibility.

---

## 5 Use Cases

* bit-level reinterpretation
* ABI compatibility
* packed data formats
* performance-critical code
* low-level memory access

---

## 6 Union vs Variant

| Union         | Variant                  |
| ------------- | ------------------------ |
| no tag        | tagged                   |
| unsafe        | checked                  |
| zero overhead | small overhead           |
| C-compatible  | higher-level abstraction |

Use a `union` when layout and control matter more than safety.

---

## **Summary**

* unions overlap memory
* no runtime checks
* layout is predictable
* matches C behavior exactly
* intended for low-level work
