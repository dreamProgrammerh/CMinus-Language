# **Class Types**

Classes represent **structured types with inheritance and behavior**, built on top of structs but extended with controlled polymorphism.
They remain explicit, predictable, and compile-time–driven, without hidden runtime mechanisms.

---

## 1 Defining a Class

```c
class Person {
    string name;
    uint age;
}
```

A class declaration defines:

* a named type
* a fixed set of fields
* optional inheritance
* optional methods

Classes are still *data-first* and map closely to structs.

---

## 2 Constructors

A class may define one or more constructors using `main`.

```c
class Person {
    string name;
    uint age;

    main(this.name, this.age);
}
```

Constructor rules:

* `main` defines how the class is initialized
* parameters are assigned explicitly
* no implicit default constructor unless declared
* overloading may be supported later

---

## 3 Instantiation

```c
Person p = Person("Alice", 20);
```

Allocation behavior depends on context:

```c
Person p1 = Person("A", 20);      // value
Person* p2 = new Person("B", 30); // heap allocation
Person p3{"C", 40};               // short-hand on stack 
```

The class itself does not imply heap allocation.

---

## 4 Inheritance

Classes may extend a single base class.

```c
class Student extends Person {
    string major;

    main(base.name, base.age, this.major);
}
```

Rules:

* single inheritance only
* base class fields are included first
* explicit constructor chaining required
* no implicit base initialization

---

## 5 Field Access and Shadowing

```c
student.name;
student.major;
```

* derived classes inherit all base fields
* name conflicts are disallowed unless explicitly resolved
* base fields accessed using `base.field`

---

## 6 Methods

Methods are functions associated with a class.

```c
class Counter {
    int value;

    void inc() {
        this.value += 1;
    }
}
```

Properties:

* receive `this` & `base` implicitly
* resolved at compile time
* no hidden virtual dispatch

---

## 7 Polymorphism Model

CMinus uses **compile-time polymorphism**, not runtime virtual dispatch.

* method calls are resolved statically
* no vtables
* no RTTI
* no implicit dynamic dispatch

This ensures:

* predictable performance
* zero runtime overhead

---

## 8 Overriding and Method Compatibility

Derived classes may define methods with the same name and signature.

Rules:

* method signatures must match exactly
* resolution is static
* no runtime override lookup

This behaves as *shadowing*, not virtual overriding.

---

## 9 Classes vs Structs

| Feature              | Struct            | Class              |
| -------------------- | ----------------- | -----------------  |
| Named type           | ✓                 | ✓                  |
| Fields               | ✓                 | ✓                  |
| Methods              | ✕ (functions ✓)   | ✓                  |
| Order                | ✓ preserved       | ✕ optimized        |
| Inheritance          | ✕                 | ✓                  |
| Layout control       | explicit          | inherited          |
| Runtime polymorphism | ✕                 | ✕                  |

---

## 10 Classes and Memory Layout

* class layout is deterministic
* base fields precede derived fields
* no hidden metadata
* compatible with C layout rules when inheritance is not used

---

## **Summary**

* classes extend structs with behavior
* inheritance is explicit and single
* no runtime polymorphism
* predictable layout and performance
* allocation controlled explicitly
