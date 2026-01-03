# **Tuples (Inline Anonymous Structs)**

Tuples in CMinus are **anonymous, inline struct types** used to group multiple values without defining a named `struct`.

They are purely structural and compile directly into a struct layout.

---

## 1 Basic Tuple Syntax

```c
(int, string)
```

This defines an anonymous struct equivalent to:

```c
struct {
    int $0;
    string $1;
}
```

Each element is stored in order and accessed by index.

---

## 2 Tuple Values

```c
(int, string) pair = (10, "ok");
```

Accessing elements:

```c
pair.$0;   // 10
pair.$1;   // "ok"
```

Index names are fixed as `$0`, `$1`, `$2`, etc.

---

## 3 Named Tuple Fields

You may assign names to tuple elements:

```c
(int code, string state)
```

This expands to:

```c
struct {
    int code;
    string state;
}
```

Usage:

```c
(int code, string state) res = (200, "ok");

res.code;
res.state;
```

---

## 4 Naming Rules

If **any field is named**, then **all fields must be named**.

✕ Invalid:

```c
(int code, string)
```

✓ Valid:

```c
(int code, string state)
```

This avoids ambiguity and enforces clarity.

---

## 5 Tuple Literals

Tuple values use parentheses:

```c
let t = (1, "hello");
```

Type inference applies:

```c
// inferred as (int, string)
```

---

## 6 Tuple Type Inference

```c
let t = (1, 2.5, "x");
```

Infers:

```c
(int, double, string)
```

Inference rules:

* order is preserved
* each element infers independently
* no implicit coercion

---

## 7 Tuple Assignment Compatibility

Assignments must match structure exactly:

```c
(int, string) a = (1, "x");        // ok
(int, string) b = (1, 2);          // error
(int, string) c = ("x", 1);        // error
```

---

## 8 Tuple Destructuring

Tuples can be destructured:

```c
let (x, y) = (10, 20);
```

Named destructuring:

```c
let (code, msg) = getStatus();
```

Destructuring requires matching arity.

---

## 9 Tuples as Function Return Types

```c
(int, string) getStatus() {
    return (200, "OK");
}
```

Usage:

```c
let result = getStatus();
println(result.$0);
```

Or:

```c
let (code, msg) = getStatus();
```

---

## 10 Tuples as Parameters

```c
void process((int, string) input) {
    println(input.$0);
}
```

Or with names:

```c
void process((int code, string msg) input) {
    println(input.code);
}
```

---

## 11 Tuples vs Structs

| Feature        | Tuple              | Struct        |
| -------------- | ------------------ | ------------- |
| Named type     | No                 | Yes           |
| Anonymous      | Yes                | No            |
| Field naming   | Optional           | Required      |
| Reusable       | No                 | Yes           |
| Layout control | Limited            | Full          |
| Use case       | temporary grouping | data modeling |

---

## 12 Tuple Memory Layout

Tuples:

* are laid out like structs
* preserve order
* may include padding
* follow ABI rules
* have no hidden metadata

> same **tuple type** use same **struct type**, *no duplicate* struct type.

Example lowering:

```c
(int, string)
```

->

```c
struct {
    int _0;
    string _1;
};
```

---

## 13 Tuples and Generics

Tuples can be used inside generics:

```c
struct Pair<T> {
    (T, T) value;
}
```

Or as generic arguments:

```c
void f<T>((T, T) pair) { }
```

---

## 14 Tuples and Interfaces

Tuples do **not** implement interfaces.

They are structural values, not nominal types.

---

## 15 When to Use Tuples

Use tuples when:

* returning multiple values
* grouping temporary data
* avoiding small one-off structs
* clarity > reuse

Avoid tuples when:

* fields have long-term meaning
* API stability matters
* documentation clarity is important

---

## **Summary**

* tuples are anonymous struct types
* `(T1, T2)` syntax
* optional named fields
* indexed or named access
* compile directly to structs
* no runtime overhead
* ideal for temporary grouping
