# **Slices**

Slices represent a **non-owning view over a contiguous sequence of elements**.
They consist of a pointer plus a length and do not allocate or own memory.

Slices are designed for safe, explicit access to array-like data without copying.

---

## 1 Slice Type Syntax

```c
int[] slice;
// may change to `int[*] slice;`
```

A slice always has two components:

```c
struct {
    int* data;
    usize length;
}
```

This representation is conceptual; the exact layout is ABI-compatible.

---

## 2 Creating Slices

#### From a length-prefixed array

```c
int[~10] arr = [1, 2, 3];
int[] s = arr[..];
```

The slice references the array’s data and length.

---

#### From a fixed array

```c
int[4] a = [1, 2, 3, 4];
int[] s = {*a, 4};
```

Explicit construction is required because fixed arrays do not store length.

---

#### From a subrange

```c
int[] sub1 = arr.subarr(1, 3);
int[] sub2 = arr[1..3];
```

Creates a slice referencing elements `[1..3)`.

---

## 3 Slice Ownership

Slices:

* do **not** own memory
* never allocate
* do not free
* are invalid if the backing storage becomes invalid

The programmer is responsible for ensuring the underlying storage lives long enough.

---

## 4 Accessing Slice Elements

```c
int x = s[0];
```

By default:

* indexing is unchecked
* out-of-bounds access is undefined behavior

Safe access is available via safety operators (covered later).

---

## 5 Slice Length

```c
usize n = s.length;
```

Length access is O(1).

---

## 6 Copying Slices

```c
int[] a = s;
```

Copies only the pointer and length, not the data.

---

## 7 Mutability

Mutability depends on the element type:

```c
const int[] s;   // elements are read-only
int[] s;         // elements are mutable
```

The slice itself may still be reassigned unless declared `const`.

---

## 8 Slices vs Arrays

| Feature         | Slice    | Fixed Array | Length-Prefixed |
| --------------- | -------- | ----------- | --------------- |
| Owns data       | ✕        | ✓           | ✓               |
| Stores length   | ✓        | ✕           | ✓               |
| Heap allocation | ✕        | ✕           | ✕               |
| Copy cost       | O(1)     | O(n)        | O(n)            |
| Bounds safety   | optional | none        | partial         |

---

## 9 Common Use Cases

* passing arrays to functions
* working with subranges
* zero-copy views
* API boundaries

---

## **Summary**

* slices are `(pointer, length)`
* non-owning and lightweight
* no implicit allocation
* explicit safety
* ideal for APIs
