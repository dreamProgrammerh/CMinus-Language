# **Data-Oriented Types (`data`)**

`data` types describe **logical entities whose physical layout is optimized automatically** for cache efficiency and iteration performance.
They are designed for data-oriented programming while remaining usable like normal structs.

A `data` declaration describes *what fields exist*, not how they are physically arranged.

---

## 1 Basic Declaration

```c
data Player {
    hot vec3<float> position;
    hot vec3<float> velocity;
    warm i64 score;
    cold string name;
}
```

This declares a logical type `Player`, but its storage is **not a single struct**.

---

## 2 Logical vs Physical Representation

A `data` type is conceptually one object, but physically it is split into multiple internal structures based on access frequency.

Conceptual view:

```c
Player {
    position;
    velocity;
    score;
    name;
}
```

Physical layout (conceptual example):

```c
struct Player_hot {
    vec3<float> position;
    vec3<float> velocity;
}

struct Player_warm {
    i64 score;
}

struct Player_cold {
    string name;
}

struct Player_data {
    Key key;
}

List<Player_hot>  player_group_hot;
List<Player_warm> player_group_warm;
List<Player_cold> player_group_cold;
```

The compiler automatically generates and manages these layouts.

---

## 3 Access Semantics

Despite the internal split, access looks like a normal struct:

```c
Player p;
p.position = ...;
p.name = "Alice";
```

The compiler rewrites accesses to the correct internal storage.

---

## 4 Access Frequency Qualifiers

### `hot`

* accessed very frequently
* optimized for cache locality
* grouped together

### `warm`

* moderate access frequency

### `cold`

* rarely accessed
* separated to reduce cache pollution

These qualifiers affect **layout only**, not semantics.

---

## 5 Performance Motivation

`data` exists to:

* reduce cache misses
* improve memory locality
* support data-oriented design
* avoid manual struct-of-arrays rewriting

Especially useful in:

* game engines
* simulations
* ECS-style systems
* large collections

---

## 6 Iteration Behavior

When iterating over collections of `data` objects, the compiler may transform loops into structure-of-arrays (SoA) access patterns automatically.

Example (conceptual):

```c
for (Player p of players) {
    update(p.position);
}
```

May compile into iteration over `Player_hot.position[]`.

---

## 7 Grouping Note
Sure! Here's a clearer and more readable version of your explanation:

---

### Data Grouping Guidelines

- **`hot`**:  
  Use this group for data that is accessed **very frequently** and requires **fast iteration**.

- **`warm`**:  
  Suitable for data that is **accessed occasionally**, but still benefits from **efficient iteration**.

- **`cold`**:  
  Best for data that is **infrequently accessed**, yet still needs **reasonable iteration performance**.

- **No group (default)**:  
  If you don't assign a group, the data is stored directly in the main structure alongside the key.
  This setup is optimized to **speed up iteration for grouped data**, but may not be ideal for general use.
  Only use this option if you **understand the implications**--we do **not recommend** leaving data ungrouped unless you have a specific reason and know how it behaves.

---

## 8 Semantics and Guarantees

* field access is transparent
* no observable semantic difference from structs
* layout may change between compilations
* ABI stability is not guaranteed
* not suitable for FFI boundaries

---

## 9 Interaction With Other Types

* can contain structs, primitives, strings
* can be used in arrays and slices
* can be passed to functions
* cannot rely on memory layout
* cannot take raw pointers to internal fields safely

---

## 10 Restrictions

* cannot be used in C ABI directly
* address of internal fields may not be stable
* layout must not be assumed
* unsuitable for serialization without explicit control

---

## **Summary**

* `data` describes *what data exists*, not how it is laid out
* compiler splits storage automatically
* optimized for cache and iteration
* accessed like normal structs
* layout intentionally abstracted
