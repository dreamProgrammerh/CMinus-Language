# Goals of the CMinus Type System

The CMinus type system is designed with a small set of strict goals that guide all design decisions.
Anything not listed here is intentionally out of scope.

## 1. Predictable Mapping to C

Every construct in the type system must either:

* map directly to a C construct, or
* lower deterministically into C code

Generated C should be readable, debuggable, and understandable by humans (when show c enabled).
`
---

## 2. Explicit Cost Model

All operations that may:

* allocate memory
* copy data
* perform runtime checks
* introduce indirection

must be **visible in source code**.

Nothing expensive happens implicitly.

---

## 3. No Hidden Runtime Systems

The type system must not introduce:

* garbage collection
* reference counting
* hidden allocators
* hidden metadata
* implicit lifetime tracking

Any runtime behavior must be explicitly requested.

---

## 4. Strong but Honest Safety

CMinus supports safety mechanisms, but never enforces them silently.

* unsafe operations are allowed
* safe alternatives exist
* safety is opt-in and explicit
* unsafe behavior must be visible in syntax

The language favors **clarity over protection**.

---

## 5. Zero-Cost Abstractions

High-level constructs must compile away cleanly.

* abstractions should not cost more than hand-written C
* generics are monomorphized
* interfaces vanish after validation
* helpers exist only at compile time

If abstraction adds runtime cost, it must be explicit.

---

## 6. Deterministic Compilation

The same input must always generate the same output.

* no reflection
* no runtime type discovery
* no hidden dispatch
* no environment-dependent behavior

---

## 7. Explicit Ownership and Allocation

Memory ownership is never inferred.

* stack vs heap is explicit
* `new` is required for heap allocation
* lifetimes are programmer-managed
* references and pointers are distinct

---

## 8. Composability Over Magic

Features should compose cleanly instead of relying on special cases.

* types compose with generics
* options compose with tables
* interfaces compose with generics
* safety operators compose with all access forms

---

## 9. Practical Systems Focus

The type system is designed for:

* systems programming
* engines and tooling
* embedded development
* performance-critical software
* C interoperability

Not for scripting or dynamic environments.

---

## 10. Language as a Tool, Not a Policy Engine

CMinus does not attempt to:

* enforce programming paradigms
* prevent all errors
* impose architectural styles
* enforce ownership models

It provides tools — not ideology.


## Non-Goals of the CMinus Type System

The following are **explicitly not goals**, even if common in other modern languages.

---

### 1. Automatic Memory Management

CMinus will not provide:

* garbage collection
* automatic reference counting
* implicit lifetime inference

Memory management is explicit and visible.

---

### 2. Implicit Safety Guarantees

The language will not:

* auto-insert bounds checks
* auto-null-check pointers
* silently prevent undefined behavior
* rewrite unsafe logic

Safety must be requested using explicit syntax.

---

### 3. Runtime Polymorphism

CMinus does not provide:

* virtual dispatch
* vtables
* runtime interface objects
* dynamic method lookup

Polymorphism is resolved at compile time.

---

### 4. Reflection or Runtime Type Introspection

There is no:

* runtime `typeOf(x)`
* runtime RTTI
* reflection API
* dynamic inspection

All type reasoning happens at compile time.

---

### 5. Implicit Heap Allocation

The compiler will never:

* allocate memory automatically
* move values to heap silently
* convert stack values into heap objects

Heap allocation always requires `new`.

---

### 6. Hidden Control Flow

CMinus avoids:

* implicit exception throwing
* hidden panics
* silent control flow changes
* invisible error propagation

Control flow must be explicit in syntax.

---

### 7. Dynamic Typing

CMinus does not support:

* dynamically changing variable types
* runtime type mutation
* union-by-assignment semantics

All types are known at compile time.

---

### 8. Implicit Conversions

No automatic:

* numeric widening
* numeric narrowing
* pointer coercion
* boolean conversion
* enum-to-int coercion

All conversions must be explicit.

---

### 9. Language-Level Memory Safety Guarantees

CMinus does not attempt to prove:

* lifetime correctness
* alias safety
* race freedom
* thread safety

These may be built externally or via conventions.

---

### 10. Magical Optimizations

The compiler will not:

* assume programmer intent
* rewrite semantics
* change visible behavior
* “do what you probably meant”

Optimizations must preserve exact meaning.