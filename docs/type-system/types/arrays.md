# **Arrays**

CMinus provides two primary array forms:

1. **Fixed-size arrays**
2. **Length-prefixed arrays**

Both are value types with predictable memory layout and no hidden allocation.

---

## 1 Fixed-Size Arrays

### Declaration

```c
int[10] values;
```

This declares an array of 10 integers.

Properties:

* size known at compile time
* stored contiguously
* layout identical to C arrays
* no runtime bounds metadata

Equivalent C:

```c
int values[10];
```

---

## 2 Initialization

```c
int[3] a = [1, 2, 3];
```

Partial initialization is allowed only if explicitly defined by the language later; otherwise, missing values are an error.

---

## 3 Access

```c
a[0] = 10;
int x = a[1];
```

Access has no bounds checking by default.

---

## 4 Fixed Arrays as Values

Fixed arrays:

* are copied by value
* can be passed to functions
* can be embedded inside structs

```c
struct S {
    int[4] data;
}
```

---

## 5 Length-Prefixed Arrays

Length-prefixed arrays store their length alongside their elements.

```c
int[~10] arr;
```

Meaning:

* up to 10 elements
* first field stores length
* remaining memory stores elements

Logical layout:

```c
struct {
    usize length;
    int data[10];
}
```

---

## 6 Length Access

Length is available in O(1):

```c
arr.length
```

---

## 7 Initialization

```c
int[~10] arr = [1, 2, 3];
```

The stored length becomes `3`.

> if think it should be `10` we will discuss it further.

---

## 8 Access

```c
arr[0]    // no bounds check
arr:[1]   // always no bounds check
arr![2]   // panic when out of bounds
arr?[3]   // return optional, none when out of bound
```

---

## 9 Safety Characteristics

* bounds known at compile time
* length tracked explicitly
* no automatic bounds checks unless requested
* safer than raw C arrays

---

## 10 Comparison to Fixed Arrays

| Feature           | Fixed Array | Length-Prefixed |
| ----------------- | ----------- | --------------- |
| Compile-time size | ✓           | ✓ (capacity)    |
| Runtime length    | ✕           | ✓               |
| Safer             | ✕           | ✓               |
| Extra storage     | ✕           | ✓ (length)      |

---

## **Summary**

* fixed arrays match C exactly
* length-prefixed arrays track size
* both are value types
* no implicit allocation
* no automatic bounds checks
