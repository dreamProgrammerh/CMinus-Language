# **Allocation Model and the `new` Keyword**

CMinus separates **value semantics** from **allocation semantics**.
Declaring or assigning a value does *not* imply where it is stored. Allocation behavior is explicit and controlled.

---

## 1 Value Semantics vs Allocation

By default, expressions produce **values**, not heap objects.

```c
let a = 10;
```

This does **not** imply heap allocation.

Where and how data lives depends on:

* the type
* the context
* whether `new` is used
* copy / move behavior defined by the type

---
`
## 2 The `new` Keyword

`new` explicitly allocates storage on the heap.

```c
let p = new Point(1, 2);
```

Properties:

* always allocates on the heap
* returns a value whose storage is heap-backed
* never implicit
* visible in syntax

---

## 3 Assignment and Allocation Are Separate Concepts
`
```c
let a = value;
```

means:

* evaluate `value`
* assign it to `a`

It does *not* imply allocation.

```c
let b = new value;
```

means:
`
* allocate storage
* construct value inside it

Assignment semantics depend on the type being assigned.

---

## 4 Copy vs Move Semantics

When assigning:

```c
a = b;
```

the behavior depends on the type:

* trivial types -> copied
* complex types -> may define move/copy behavior
* heap-backed values -> copy references or content depending on type rules

This behavior is explicit in type design, not implicit language magic.

---

## `5 Allocation and Strings

## `string`

```c
string s = "hello";
```

* always heap-allocated
* resizable
* owns its memory

Using `new` explicitly:

```c
string s = new "hello";
```

is `equivalent but emphasizes allocation intent.

---

## `str`

```c
str s = "hello";
```

* stack-allocated
* fixed size
* immutable length
* may reference heap data if assigned explicitly

```c
str s = new "hello"; // heap-backed string
```

---

## Pointer Access to Heap Data

```c
str* p = new "hello";
```

Stores the address of heap-allocated string data.

---

## 6 Allocation and Operator Overloading

Because CMinus supports operator overloading:

```c
a = b;
```

may internally:

* copy data
* move data
* allocate memory
* release memory

All such behavior is controlled by the type definition.

The language itself introduces **no implicit heap allocation** unless:
`
* `new` is used, or
* the type explicitly defines allocation behavior

---

## 7 Stack vs Heap Is Not a Type Property

Types do not inherently live on the stack or heap.

Instead:

* values live wherever their binding requires
* `new` forces heap allocation
* references and pointers may refer to either

---

## 8 Example

```c
struct Box {
    int value;
}
`
let a = Box{1};       // stack value
let b = new Box{2};   // heap allocation
```

Both have the same type but different storage behavior.

---

## **Summary**

* allocation is explicit
* ``new` always allocates on heap
* assignment does not imply allocation
* types define copy/move behavior
* stack vs heap is not part of the type
* allocation intent is visible in code
`