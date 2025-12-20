<div align="center">
    <img width="1000em" src="res/CMinus-Banner.svg" alt="CMinus banner">
    <hr>
    <h1>CMinus 🎗️ Programming Language</h1>
    <p><strong>C without the limits</strong></p>
</div>

---

**CMinus (C-) is a source-to-source language that compiles to C.**  
You write modern, expressive, and less repetitive code, and CMinus generates **clean, readable, and optimizable C code** as output.

This project exists for a very simple reason:

> **I needed a better way to write large C projects without losing C’s power and portability.**

C is everywhere -- kernels, embedded systems, game engines, system libraries --  
but as projects grow, C becomes verbose, repetitive, and harder to scale.

CMinus does **not replace C**.  
It **embraces C** and builds on top of it.

---

## Important Notes (Please Read)

> **This project is in early design stage**\
> There is **no compiler yet**\
> The repository currently focuses on **language design and documentation**\
> Syntax, features, and rules may evolve

This is also **my first public GitHub repository**.  
If you notice anything odd, unclear, or poorly structured, please don’t mind --  
or better yet, **teach me how I can do it better**.

Feedback, corrections, and suggestions are very welcome.

---

## Why CMinus?

C already gives you:
- performance
- control
- portability

But it also comes with:
- boilerplate
- weak abstraction tools
- repetitive patterns
- fragile large-scale codebases

C++ tried to solve this by adding *everything*.  
CMinus takes a different path.

> **Make C easier to write, not harder to understand.**

---

## Philosophy

> **“All the power of C, minus the ceremony.”**

CMinus is built around a few core principles:

1. **Transparency**  
   You can always see the generated C code.

2. **Control**  
   Manual memory when you want it, safety tools when you need them.

3. **Simplicity**  
   One clear way to do things -- no hidden magic.

4. **Interop First**  
   C compatibility is a requirement, not an afterthought.

---

## Example (Conceptual)

```c
main [optimize: 3, libc: 1] {
    let name = "World";
    println("Hello, #{name}!");

    variant{int, string} value = 42;
    println("Number: #{value as int}") when value is int;

    let numbers = new List<int>(1, 2, 3, 4, 5);
    let doubled = numbers.map(\x -> x * 2);
    println("Doubled: #{doubled}");
}
```

This code is intended to compile into **clean, human-readable C** that any C compiler can further optimize.

---

## What This Project Is (and Is Not)

### ✅ What it is

* A **better way to write C**
* A **source-to-source compiler**
* A **personal need turned into a shared tool**
* A documentation-first language design

### ❎ What it is not

* Not hype-driven
* Not a C replacement
* Not a runtime-heavy language
* Not a finished product (yet)

---

## Who This Is For

* Systems programmers who love C but want better ergonomics
* Game developers working with large, performance-critical C codebases
* Embedded developers who need predictable, portable C output
* Library authors designing clean public C APIs
* Anyone who wants **modern structure without losing C**

---

## Project Status

* Language specification: **in progress**
* Design discussions: **open**
* Compiler: **not started**
* Contributions: **welcome (especially feedback)**

If you’re interested in language design, compiler construction, or just better C --
feel free to explore the docs, open issues, or share thoughts.

---

## Final Words

This project was not created for popularity or hype.
It exists because **I needed it**, and I believe I’m not alone.

If C is still your tool of choice,
CMinus aims to make it **less painful, more expressive, and easier to scale**.

> **CMinus -- write better C, get C back.**