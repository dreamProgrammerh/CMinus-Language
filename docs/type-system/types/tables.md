# **`table` — Static Compile-Time Lookup Tables**

A `table` defines a **compile-time–constructed lookup structure** that lowers into a static array.
All keys are resolved at compile time and mapped to numeric indices.

A table has **no runtime hashing**, no dynamic lookup, and no allocation.

---

## 1 Basic Syntax

```c
table<string> Table {
    "red"    = "color",
    'A'      = "capital char",
    10       = "integer",
    0x00FF00 = "green hex",
};
```

Syntax form:

```c
table<T> Name {
    <constant-key> = <value>,
    ...
}
```

Where:

* `T` is the **value type**
* keys must be **compile-time constants**
* values must be assignable to `T`

---

## 2 What a Table Compiles To

A table is lowered into:

```c
static T Name_table[];
```

and a compile-time–generated mapping:

```
key -> index
```

Access is always transformed into **index-based access**.

There is no hash table, map, or runtime structure.

---

## 3 Table Value Type

The type inside `<>` defines the stored array type:

```c
table<string> Names;
```

Low-level representation:

```c
string[];
```

This means:

* all values must be convertible to `string`
* values are stored contiguously
* normal indexing rules apply internally

### Using variant values

```c
table<variant{int, string}> Mixed {
    "a" = 1,
    "b" = "hello"
}
```

---

## 4 Access Table Length

to access length of table at compile-time use `.length`

```c
Names.length;
```

length of table is determined by the compiler,
zero-cost table length access.


---

## 5 Keys — General Rules

Keys may be **any compile-time constant**, including:

* string literals
* character literals
* integers (decimal, hex, binary, octal)
* `true`, `false`, `null`
* `final` constants
* literals used in `options`
* compile-time expressions

Each key is converted into a **canonical internal string representation**.

---

## 6 Key Canonicalization Rules

Keys are normalized at compile time into internal identifiers:

### Strings

```
"red" -> "$$red"
```

### Characters

```
'A' -> "$A"
```

### Integers

```
10     -> "10"
0xFF   -> "255"
-1     -> "_1"
```

### Booleans

```
true  -> "Ztrue"
false -> "Zfalse"
```

### Null

```
null -> "Znull"
```

This representation is internal only and never exposed to users.

---

## 7 Compile-Time Key Validation

All keys must be:

* compile-time known
* unique after normalization
* valid for the table

Duplicate or conflicting keys produce a **compile-time error**.

---

## 8 Access Forms

#### **1. Compile-Time Key Access**

```c
Colors["red"];
```

Rules:

* key must be compile-time constant
* lookup resolved at compile time
* lowered to array indexing

Equivalent to:

```c
Colors_table[INDEX];
```

---

#### **2. Compile-Time Index Access**

```c
Colors::"red";
```

Returns the index of the key.

Useful when you want to store or pass the index explicitly.

```c
int idx = Colors::"red";
```

---

## 8 Runtime Index Access (Unsafe)

```c
Colors:[3]
```

* access index at runtime
* no bounds check
* can cause undefined behavior if index is invalid

Use it when you fully sure the index gonna be valid.

---

## 9 Runtime Index Access (Safe)

```c
Colors![3];
```

* runtime index
* performs bounds check
* panics on invalid index

Used when index comes from user input or computation.

---

## 10 Runtime Optional Index Access (Safe)

```c
Colors?[3];
```

Returns an optional value:

```c
string? value = Colors?[i];
```

* returns `none` if index is out of range
* never panics
* always runtime

---

## 11 Using `final` Keys

Keys may be stored in `final` variables and reused:

```c
final key = "red";

Colors[key];     // valid
```

Rules:

* key must be `final`
* must resolve to a valid table key
* otherwise compile-time error

Non-final variables are rejected:

```c
let k = "red";
Colors[k]; // error
```

---

## 12 Why Runtime Keys Are Disallowed

Tables are **not hash maps**.

Allowing runtime string keys would imply:

* hashing
* comparison loops
* runtime memory access
* unpredictable cost

To preserve zero-cost semantics, tables only accept:

* compile-time keys
* runtime *indices*

---

## 13 Relationship to Arrays

A table behaves like a named, indexed static array:

```c
table<int> T {
    "a" = 10,
    "b" = 20
}
```

Is equivalent to:

```c
static int T_data[] = { 10, 20 };
```

Plus a compile-time mapping:

```
"a" -> 0
"b" -> 1
```

---

## 14 Why Tables Exist

Tables are designed to:

* replace hardcoded switch logic
* replace magic constants
* avoid runtime maps
* provide readable symbolic access
* allow deterministic codegen
* keep lookup O(1)
* stay fully static

---

## 15 Typical Use Cases

### Named configuration values

```c
table<int> Errors {
    "ok" = 0,
    "not_found" = 404,
    "denied" = 403,
}
```

---

### Mapping symbolic names to resources

```c
table<string> Shaders {
    "basic" = "basic.glsl",
    "ui"    = "ui.glsl"
}
```

---

### Runtime-safe indexed access

```c
int i = getIndex();
string? s = Shaders?[i];
```

---

## **Summary**

* `table<T>` creates a static indexed array
* keys are compile-time constants only
* all lookups lower to index access
* no hashing, no allocation
* safe and unsafe runtime access available
* supports any value type, including variants
* ideal for symbolic lookup tables
