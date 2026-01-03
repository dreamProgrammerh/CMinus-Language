# **Variant Types**

Variant types represent a value that can hold **one of several explicitly listed types**, with a runtime tag indicating which one is active.

Variants are *tagged unions* and provide controlled, type-safe alternatives to raw unions.

---

## 1 Declaring a Variant

```c
variant{int, string} value;
```

This declares a variable that can hold either an `int` or a `string`.

Internally, a variant consists of:

* a storage region large enough for the largest member
* a discriminator (tag) identifying the active type

---

## 2 Assigning to a Variant

```c
value = 10;
value = "hello";
```

Assignment sets both the stored value and the active tag.

Only types listed in the variant are allowed.

---

## 3 Type Checking

To access the stored value, the active type must be checked explicitly.

```c
if (value is int) {
    int x = value as int;
}
```

Rules:

* `is` checks the active variant type
* `as` extracts the value
* extraction without checking is a compile-time error (maybe change to warning)

---

## 4 Exhaustiveness

All possible variant alternatives must be considered when required by context.

```c
variant{int, string} v;

if (v is int) {
    ...
} else {
    // guaranteed to be string
}
```

---

## 5 Variant Assignment Semantics

Assigning a new value:

* destroys the previous value (if needed)
* updates the active tag
* stores the new value

No implicit conversions occur between variant alternatives.

---

## 6 Variants vs Optionals

Variants represent **data alternatives**, not failure states.

```c
variant{int, none}
```

is not equivalent to:

```c
int?
```

Key differences:

| Variant                   | Optional                |
| ------------------------- | ----------------------- |
| many alternatives         | success/failure only    |
| tagged by type            | tagged by error code    |
| explicit pattern checking | explicit error handling |
| general data modeling     | error/result modeling   |

---

## 7 Variants vs Unions

| Variant            | Union             |
| ------------------ | ----------------- |
| tagged             | untagged          |
| checked access     | unchecked         |
| safer              | unsafe            |
| small runtime cost | zero runtime cost |

---

## **Summary**

* variants are tagged unions
* only declared types are allowed
* access requires explicit checks
* safer alternative to unions
* not used for error signaling
