# C- (CMinus)

C without the limits;

**CMinus is a source-to-source compiler that generates highly optimized C code.** It is an open-source language designed to reduce C's verbosity and limitations while keeping its performance and control. You write cleaner, modern-style code in CMinus, and it outputs efficient, readable C.

**Why C-?**

C is everywhere, but writing it feels limited.
C++ added everything but the kitchen sink.
C- takes a different approach.

## C- Philosophy

"All the power of C, minus the ceremony."

C- believes in:

1. Transparency — You can always see the generated C code
2. Control — Manual memory when you want, safety when you need it
3. Simplicity — One obvious way to do things
4. Interop — First-class C compatibility, not an afterthought

**Quick Example**

```cm
// C- code
main [optimize: 3, libc: 1] {
    let name = "World";
    println("Hello, #{name}!");
    
    variant{int, string} value = 42;
    println("Number: #{value as int}") when value is int;
    
    let numbers = List<int>(1, 2, 3, 4, 5);
    let doubled = numbers.map(\x -> x * 2);
    println("Doubled: #{doubled}");
}
```

Compiles to clean, readable C that any compiler can optimize.

## Perfect For 

 - Systems programming where C is king but you want better ergonomics
 - Game development needing data-oriented design at the language level
 - Embedded systems that must compile to efficient C
 - Library authors wanting a better C for public APIs
 - Education — Learn systems concepts without  syntax

**Getting Started**

```bash
# Write C- code
echo 'main { println("C with less pain!"); }' > hello.cm

# Compile and run
cminus run hello.cm

# See generated C code  
cminus build --show-c hello.cm
```

## Final Word
---

C-: All the power, minus the pain.\
**NOT REPLACING C JUST LESS CODE**

---
