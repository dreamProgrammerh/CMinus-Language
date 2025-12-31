# Function Signature Specification

## 1. Overview
Function signatures uniquely identify functions in the language. They are used for overload resolution, mangling, interop, and compile-time function reference generation. Signatures are based on required parameters only; defaults and syntactic sugar are metadata.

---

## 2. Canonical Signature
- A function signature consists of the function name and its required parameter types.
- Example:
  ```c
  void foo(int a, int b);
  ```
  Canonical signature: `foo(int,int)`

---

## 3. Defaults
- Defaults are **not part of the signature hash**.
- Defaults must be trailing parameters.
- Defaults must be compile-time constants or folded expressions (math, bitwise).
- No runtime calls, variables, or static state.
- Example:
  ```c
  void foo(int a, int b = 0, int c = 1);
  ```
  Canonical signature: `foo(int)`
  Metadata: defaults `[b=0, c=1]`

### Call Expansion
- At call sites, the compiler expands missing arguments using defaults.
- Example:
  ```c
  foo(10);       // -> foo(10,0,1)
  foo(10,2);     // -> foo(10,2,1)
  foo(10,2,3);   // -> foo(10,2,3)
  ```

---

## 4. Variadics
- Variadics are written as `T...` .
- Canonicalized to `T[~]` (length-prefixed array).
- Example:
  ```c
  f64 sum(f64... numbers);
  ```
  Canonical signature: `sum(f64[~])`

- Variadics and defaults cannot be combined (open for future discussion).

---

## 5. Generics
- Generics expand at compile time into concrete functions.
- Each instantiation produces its own canonical signature.
- Defaults are **not allowed** on generic parameters.
- Variadics are allowed.
- Example:
  ```c
  void f<T>(T... xs);
  ```
  Instantiation: `f<int>(int[~])`

---

## 6. Interop
- At ABI level, functions are emitted with full parameter lists.
- Defaults vanish; only canonical full signatures are exported.
- Compiler inserts comments showing defaults and hashed signatures.
- Example:
  ```c
  // foo:int,int,int  (defaults: b=0, c=1)
  void _CMfoo_A1b9xYz(int a, int b, int c);
  ```

---

## 7. Resolution Rules
- No implicit conversions: arguments must match parameter types exactly, or be explicitly cast.
- Overload resolution:
  1. Exact match -> select.
  2. If fewer arguments provided, check if defaults exist to fill.
  3. If no match, error: *no such function exists*.

---

## 8. Named Arguments (Future)
- Named arguments may be added later, Dart-style `{}` syntax.
- Defaults integrate naturally: omitted named args filled from defaults.

---

## 9. Diagnostics
- Error messages are minimal: *no such function exists*.
- Compiler may suggest closest matches.

---

## 10. Compile-Time Function Signature Generators
The language provides a `func` keyword to access a function reference or pointer at **compile time** based on its signature. This allows safe, type-checked function references without runtime lookup.

### Reference Value
```c
ReturnType(ParamTypes...) var = func FunctionName(ParamTypes...);
```
- Produces a **reference value** to the function.
- Example:
  ```c
  int(int,int) f = func add(int,int);
  f(5,9); // -> 14
  ```

### Pointer Value
```c
ReturnType(ParamTypes...)* var = func& FunctionName(ParamTypes...);
```
- Produces a **pointer value** to the function.
- Example:
  ```c
  int(int,int)* f = func& add(int,int);
  (*f)(5,9); // -> 14
  ```

### Rules
- The signature must match an existing function exactly.
- No implicit conversions: argument types must match.
- Generics are expanded at compile time; `func` works on each instantiation.
- Defaults are ignored in signature matching.
  ```c
  void foo(int a, int b = 0);
  func foo(int); // valid, expands to foo(int,int)
  ```

---

## 11. Examples
```c
void bar(int a, int b = 0);
void bar(int a, double b);

bar(5);       // -> bar(int) with default b=0
bar(5,10);    // -> bar(int) with default b=0 (override with 10)
bar(5,1.0);   // -> bar(int,double)

// Function reference
int add(int a, int b) { return a + b; }
int(int,int) ref = func add(int,int);
int result1 = ref(3,4); // -> 7

// Function pointer
int(int,int)* ptr = func& add(int,int);
int result2 = (*ptr)(5,9); // -> 14
```
