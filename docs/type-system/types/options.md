# **`options` — Scoped Named Constants (Inline Enums)**

`options` define a **set of named constant values bound to a base type**.
They behave like *compile-time enums*, but are more flexible, more expressive, and fully zero-cost.

They exist only at compile time and disappear after lowering.

---

## 1 Basic Syntax

```c
options<int>{up, down, left, right}
```

This declares four named constants associated with the base type `int`.

By default, values are assigned incrementally starting from `0`:

```text
up    = 0
down  = 1
left  = 2
right = 3
```

---

## 2 Using `options` in Variables

```c
options<int>{up, down, left, right} dir = up;
```

Outside this declaration scope, the names `up`, `down`, etc. **do not exist**.

Only the resulting value remains.

Lowered to:

```c
int dir = 0;
```

---

## 3 Explicit Values

You may explicitly assign values:

```c
options<i8>{less=-1, equal=0, more=1}
```

Mixing explicit and implicit values is allowed:

```c
options<u8>{a, b, c=10, d}
```

Results in:

```text
a = 0
b = 1
c = 10
d = 11
```

---

## 4 Counter Rules

Automatic numbering follows these rules:

* starts from `0`
* increments by `1`
* applies only to numeric base types
* cannot decrement
* must not overflow
* produces a compile-time error on overflow

Example (invalid):

```c
options<u8>{a=254, b, c}
```

`c` would overflow -> compile-time error.

---

## 5 Supported Base Types

`options<T>` requires `T` to be a **compile-time literal-compatible type**, such as:

* integers
* unsigned integers
* characters
* enums / numeric aliases
* variants
* strings (with explicit values)

Unsupported types cause a compile-time error.

---

## 6 String-Based Options

```c
options<string>{
    red   = "red",
    green = "green",
    blue  = "blue"
}
```

Compiles to string constants.

Used when semantic clarity matters more than numeric identity.

---

## 7 Variant-Based Options

Options can wrap a variant:

```c
options<variant{int, string}>{
    id = 1,
    name = "user"
}
```

Each option expands to a valid value of the variant.

---

## 8 Options Inside Functions

Options may appear inside function scope:

```c
options<int>{less=-1, equal=0, more=1}
compare(int a, int b) {

    if (a < b) return less;
    if (a > b) return more;
    return equal;
}
```

Scope rules:

* names exist only inside the block
* values lower directly to constants
* no runtime structure is created

---

## 9 Options With Type Aliases

Options work cleanly with `typedef`:

```c
typedef Order = i8;

options<Order>{buy=1, sell=2}
```

---

## 10 Options With Functions

Options are often used as symbolic return values:

```c
typedef result = options<i8>{ok=0, error=1};

result process() {
    if (fail()) return error;
    return ok;
}
```

---

## 11 Options Do Not Create Types

Important distinction:

```c
options<int>{a, b}
```

does **not** define a new type.

It only defines named constants of type `int`.

This means:

```c
int x = a;   // valid
```

There is no type-checking barrier like enums in some languages.

---
Below is a **complete, precise version of section 18.12**, written in the same tone and rigor as your other sections.
It explains intent, rules, precedence, and edge cases without over-engineering.

---

## 12 Option Name Resolution and Scope Priority

When using `options`, names defined inside the option set may collide with variables or symbols from the surrounding scope.

CMinus resolves this ambiguity using **scope priority rules**.

---

#### Default Name Resolution Rule

When an identifier used inside an `options` expression matches both:

* a local variable name, and
* an option constant name

**the local variable always wins**.

Example:

```c
int x = 2;

options<int>{x, y} a = x;
```

Interpretation:

* `x` refers to the variable `x`
* value assigned is `2`

The option constant `x` is ignored in this context.

This rule ensures that normal lexical scoping always has priority and avoids accidental shadowing.

---

#### Accessing Option Constants Explicitly (`:` operator)

To explicitly refer to an option constant, use the **option access unary operator** `:`.

```c
options<int>{x, y} a = :x;
```

This forces lookup inside the option set.

Result:

* `:x` refers to the option constant
* assigned value is the option’s numeric value (`0` in this case)

---

#### Meaning of the `:` Operator

The `:` operator:

* bypasses normal name resolution
* explicitly selects an option constant
* ignores variables, parameters, and fields with the same name
* is only valid inside option-related expressions

It always refers to the **current option scope**.

> we may change it to `:.` if become complicated.

---

## 13 Options vs Enums (Conceptually)

| Feature           | options | traditional enum |
| ----------------- | ------- | ---------------- |
| Runtime presence  | ✕ none  | usually yes      |
| Scoped            | ✓       | sometimes        |
| Base type control | ✓       | limited          |
| String support    | ✓       | rare             |
| Variant support   | ✓       | no               |
| Zero cost         | ✓       | not always       |
| Compile-time only | ✓       | often no         |

---

## 14 Options and Readability

Options exist to:

* name magic numbers
* express intent
* avoid raw constants
* document logic
* keep code readable
* avoid enum overhead

They are especially useful for:

* return codes
* comparison results
* flags
* symbolic states
* temporary scoped constants

---

## **Summary**

* `options<T>` define named constants
* scoped and compile-time only
* zero runtime cost
* support auto-increment
* support explicit values
* support strings and variants
* disappear entirely after compilation
* ideal replacement for lightweight enums
