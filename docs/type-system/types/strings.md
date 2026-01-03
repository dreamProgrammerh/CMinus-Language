# **String Types**

CMinus provides multiple string representations with explicit ownership, mutability, and allocation semantics.
There is no single “magic string” type; each form exists for a specific purpose.

---

## 1 Overview of String Forms

| Type     | Allocation      | Mutable         | Owns memory | Notes                  |
| -------- | --------------- | --------------- | ----------- | ---------------------- |
| `string` | heap            | yes             | yes         | growable, dynamic      |
| `str`    | stack (default) | no (size-fixed) | yes         | fixed-capacity         |
| `str*`   | heap or stack   | depends         | no          | pointer to string      |
| `char*`  | external        | mutable         | no          | C-style string         |
| `char[]` | inline          | mutable         | yes         | null-terminated buffer |

---

## 2 `string` — Dynamic Heap String

```c
string s = "hello";
```

Properties:

* always heap allocated
* dynamically resizable
* owns its memory
* mutable
* length and capacity tracked
* dynamic encoding (UTF-8 16 32, ByteChuck, User Defined)

Equivalent conceptual layout:

```c
struct string {
    char* data;
    usize length;
    usize capacity;
}
```

#### Behavior

```c
s += " world";
```

May reallocate internally.

Access:

```c
s[0];
s.length;
```

---

## 3 Explicit Heap Allocation with `new`

```c
string s = new "hello";
```

This explicitly allocates the string on the heap.

Semantically equivalent to the implicit heap allocation that `string` already performs, but makes allocation intent explicit.

---

## 4 `str` — Fixed-Size String

```c
str name = "John";
```

Properties:

* fixed size
* stack-allocated by default
* immutable length
* owns its memory
* encoding-aware

The capacity is inferred from the initializer unless specified explicitly:

```c
str<16> buf = "hello";
string<256> str = "builder";
```

---

## 5 Heap-Backed `str`

`str` can also be backed by heap storage:

```c
str s = new "hello";
```

In this case:

* memory lives on heap
* capacity fixed
* still immutable in size

---

## 6 String Pointers

```c
str* p = new "hello";
```

* pointer to a string object
* does not own the string
* may point to stack or heap string

---

## 7 C-Style Strings

#### `char*`

```c
char* s = "hello\0";
```

* null-terminated
* mutable
* no length tracking
* interoperable with C

#### `char[]`

```c
char buf[32] = "hello";
```

* inline storage
* mutable
* null-terminated
* fixed size

---

## 8 Length and Character Access

| Type     | Length Access | Character Access      |
| -------- | ------------- | --------------------- |
| `string` | O(1)          | O(1) byte / O(n) char |
| `str`    | O(1)          | O(1) byte / O(n) char |
| `char*`  | O(n)          | O(1)                  |
| `char[]` | O(n)          | O(1)                  |

---

## 9 Encoding Notes

* `char` represents a UTF-8 byte or ByteChuck
* `dchar` represents UTF-16 units
* `qchar` represents UTF-32 code points
* no implicit transcoding occurs

---

## 10 String Conversion

Conversions must be explicit.

```c
string a = "hi";
str b = a;
char* c = a;
```

Each conversion may:

* allocate
* copy
* or reinterpret
  depending on the target type.

---

## **Summary**

* multiple string types exist by design
* ownership and allocation are explicit
* `string` is dynamic and heap-backed
* `str` is fixed-size and value-oriented
* C strings are supported for interop
* no implicit conversions or hidden allocation
