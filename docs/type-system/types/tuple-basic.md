## **Tuples (Inline Struct Types)**

Tuples in CMinus are **anonymous, inline struct types**.
They exist to group values without defining a named `struct`.

A tuple is a **value type** with a fixed layout determined at compile time.

---

## 1 Basic Tuple Syntax

```c
(int, string)
```

This is equivalent to the anonymous struct:

```c
struct {
    int $0;
    string $1;
}
```

* fields are stored in order
* layout is predictable
* no hidden padding beyond ABI requirements

---

## 2 Tuple Values

```c
let t = (1, "ok");
```

Type inferred as:

```c
(int, string)
```

Field access uses positional names:

```c
t.$0;   // int
t.$1;   // string
```

---

## 3 Named Tuples

Tuple fields may be explicitly named.

```c
(int code, string state)
```

Lowers to:

```c
struct {
    int code;
    string state;
}
```

Rules:

* if **any** field is named, **all fields must be named**
* partial naming is a compile-time error

```c
(int code, string)   // error
```

---

## 4 Tuple Construction

```c
(int, int) p = (10, 20);
```

Order and type must match exactly.

```c
(int, int) p = (20, 10);     // OK
(int, int) p = (10, 20, 30); // error
```

---

## 5 Tuple Assignment and Copying

Tuples follow normal value semantics:

```c
let a = (1, "x");
let b = a;    // value copy
```

Copy behavior depends on the contained types.

---

## 6 Tuples in Function Returns

Tuples are commonly used for multiple return values.

```c
(int, string) parse();
```

Usage:

```c
let result = parse();
println(result.$0);
println(result.$1);
```

Named tuple returns improve readability:

```c
(int code, string message) parse();
```

---

## 7 Tuples vs Structs

| Tuples                  | Structs             |
| ----------------------- | ------------------- |
| anonymous               | named               |
| inline                  | reusable            |
| positional by default   | named fields        |
| no separate declaration | explicit definition |

Tuples do not support:

* methods
* inheritance
* layout directives

---

## **Summary**

* tuples are inline, anonymous structs
* field order defines layout
* naming is all-or-nothing
* no runtime overhead
* ideal for grouping temporary or return values
