# **Interfaces (Compile-Time Prototypes)**

An **interface** in CMinus is a *compile-time contract* used to describe the required **shape** of a type.
It does **not** generate runtime data, vtables, or metadata.

Interfaces exist only to:

* validate structure at compile time
* constrain generics
* define required fields and functions
* provide optional default implementations

They are closer to **struct/class blueprints** than to runtime polymorphism.

---

## 1 Purpose of Interfaces

Interfaces are used to:

* enforce that a type provides certain fields or methods
* describe behavior without defining storage
* constrain generic parameters
* enable static polymorphism
* share default logic safely

They do **not**:

* allocate memory
* introduce runtime dispatch
* generate hidden metadata
* affect layout directly

---

## 2 Basic Interface Declaration

```c
interface Drawable {
    void draw();
}
```

This defines a contract requiring a `draw()` method.

---

## 3 Implementing an Interface

Interfaces must be implemented **explicitly**.

```c
struct Sprite : Drawable {
    int x;
    int y;

    void draw() {
        // drawing logic
    }
}
```

Rules:

* implementation must be explicit (`: Interface`)
* all required members must exist
* names and signatures must match
* missing members are compile-time errors

---

## 4 Interfaces With Fields

Interfaces may define **required fields** in addition to methods.

```c
interface Entity {
    int id;
    string name;
}
```

A conforming type must define those fields:

```c
struct Player : Entity {
    int id;
    string name;
    int score;
}
```

Field names and types must match exactly.

---

## 5 Default Fields and Methods

Interfaces may define defaults using the `default` keyword.

```c
interface Drawable {
    default void draw() {
        // do nothing
    }
}
```

Default fields are also allowed:

```c
interface Colored {
    default Color color = black;
}
```

Rules:

* defaults are optional to override
* if not overridden, the default is used
* defaults do not force memory layout
* defaults do not allocate automatically

Defaults are injected at compile time.

---

## 6 Constructor Requirements

Interfaces may declare constructor requirements.

```c
interface Named {
    main(string name);
}
```

Implementing types must provide a compatible constructor.

---

## 7 Interface Inheritance

Interfaces may extend other interfaces:

```c
interface Movable {
    void move(int dx, int dy);
}

interface DrawableMovable : Movable {
    void draw();
}
```

Implementers must satisfy **all inherited requirements**.

---

## 8 Multiple Interface Implementation

A type may implement multiple interfaces:

```c
struct Button : Clickable, Drawable, Focusable {
    ...
}
```

There is no ambiguity because:

* no runtime dispatch exists
* method resolution is static
* names must be unique or explicitly resolved

---

## 9 Interfaces and Generics

Interfaces are commonly used as **generic constraints**.

```c
void render<T: Drawable>(T obj) {
    obj.draw();
}
```

Rules:

* `T` must implement `Drawable`
* verification is done at compile time
* no runtime checking

---

### Multiple Constraints

```c
result process<T: Drawable | Serializable>(T obj) {
    ...
}
```

Meaning:

* `T` must satisfy both interfaces

---

## 10 Predefined Primitive Type Interfaces
- `Numeric` for all kinds of numbers
- `Integer` for all kinds of signed integers
- `Unteger` for all kinds of unsigned integers
- `Floating` for all kinds of floating point numbers
- `Character` for all kinds of characters
- `Comparable` for all kinds of comparable types
- `Iterable` for all kinds of iterable types

| Interface       | Primitives & Types                    | Description                    |
|-----------------|---------------------------------------|--------------------------------|
| `Integer`       | `i8, i16, i32, i64`                   | all kinds of signed integers   |
| `Unteger`       | `u8, u16, u32, u64`                   | all kinds of unsigned integers |
| `Floating`      | `f32, f64`                            | all kinds of floating numbers  |
| `Numeric`       | `i8-i64, u8-u64, f32-f64`             | all kinds of numbers           | 
| `Character`     | `c8, c16, c32`                        | all kinds of characters        |
| `Logical`       | `bool, trnry, void`                   | all logic kind type            |
| `Comparable`    | `Numeric, trnry`                      | support `< > == != <= >=` etc. |
| `Checkable`     | `Numeric, Character, bool, trnry`     | support `== !=`                |


---

## 11 Interfaces Do Not Affect Layout

Interfaces:

* do not insert fields
* do not add padding
* do not generate vtables
* do not change memory layout

They only validate structure and signatures.

---

## 12 Interfaces vs Classes vs Structs

| Feature          | Interface          | Struct | Class |
| ---------------- | ------------------ | ------ | ----- |
| Allocates memory | No                 | Yes    | Yes   |
| Fields           | Yes (requirements) | Yes    | Yes   |
| Methods          | Yes                | Yes    | Yes   |
| Defaults         | Yes                | Yes    | Yes   |
| Inheritance      | Yes                | No     | Yes   |
| Runtime dispatch | No                 | No     | No    |
| Code generation  | No                 | Yes    | Yes   |

---

## 13 Interfaces Are Compile-Time Only

Interfaces are erased after type checking.

They:

* do not exist in generated C
* cannot be inspected at runtime
* cannot be stored as values
* cannot be referenced dynamically

They exist solely to improve correctness and structure.

---

## 14 Why Interfaces Exist in CMinus

Interfaces solve problems such as:

* enforcing APIs without inheritance
* validating generic usage
* documenting expectations
* enabling safe composition
* avoiding runtime polymorphism

They provide structure **without cost**.

---

### **Summary**

* interfaces define required fields and methods
* they are compile-time only
* no runtime overhead
* explicit implementation required
* support defaults
* support inheritance
* usable as generic constraints
* never affect memory layout
