# **Struct Types**

Structs define **named aggregate types** with a fixed layout and explicit fields.
They are the primary mechanism for modeling structured data in CMinus.

Structs are value types and map directly to C `struct`.

---

## 1 Defining a Struct

```c
struct Point {
    int x;
    int y;
}
```

This defines a new type `Point` with two fields.

Equivalent C representation:

```c
struct Point {
    int x;
    int y;
};
```

---

## 2 Field Access

```c
Point p;
p.x = 10;
p.y = 20;
```

Field access is direct and has no runtime overhead.

---

## 3 Default Values & Constant

```c
struct Point {
    int x = 0;
    int y = 10;
}

Point p; // x = 0, y = 10

struct Point3D {
    const int x;
    const int y = 10;
    int z = 20;
}

Point3D p3d{ x: 10, y: 20, z: 30 };
p3d.x = 100; // error: x is const
p3d.y = 200; // error: y is const
p3d.z = 300; // ok
```

---

## 3 Initialization

Structs may be initialized using positional or named initialization.

```c
Point p1 = Point{ 10, 20 };
Point p2 = Point{ x: 10, y: 20 };
Point p2{ x: 10, y: 20 };   // short-hand
```

Rules:

* positional initialization follows field order
* named initialization may appear in any order
* initialization cannot be mixed is either positional or named
* missing fields are an error unless defaults exist
* constants cannot change after initialization

---

## 4 Value Semantics

Structs are value types.

```c
Point a = Point{1, 2};
Point b = a;   // full copy
```

Copying copies all fields recursively.

No implicit heap allocation occurs.

---

## 5 Layout and Padding

Struct layout is:

* deterministic
* field order preserved
* padding inserted only when required by ABI or alignment rules

The compiler may apply layout optimizations **only if explicitly enabled** with tag `#[optimize(layout)]`.

---

## 6 Structs and Assignment

Assignment replaces the entire value:

```c
p = Point{3, 4};
```

Partial assignment is not allowed unless explicitly supported via field access.

---

## 7 Structs as Parameters and Returns

```c
Point make() {
    return Point{1, 2};
}
```

Passing and returning structs follows value semantics unless optimized by the compiler.

---

## 8 Nested Structs

```c
struct Rect {
    Point min;
    Point max;
}
```

Nested structs are laid out inline.

---

## 9 Structs vs Tuples

| Struct               | Tuple                           |
| -------------------- | ------------------------------- |
| named type           | anonymous type                  |
| reusable             | inline only                     |
| explicit field names | positional by default           |
| suitable for APIs    | suitable for temporary grouping |

---

## **Summary**

* structs define named, reusable aggregate types
* layout is predictable and C-compatible
* copied by value
* no hidden allocation
* foundation for higher-level abstractions
