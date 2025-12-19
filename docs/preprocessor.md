# **CMinus Preprocessor**

Think of the CMinus Preprocessor as:

> **A small programming language that runs *before* compilation and generates code for you.**

---

## 0. What the Preprocessor Is (Big Picture)

- Runs **before compilation**
- Reads `.cm` files
- Can **define values**, **generate code**, **read files**, **loop**, **branch**, and **validate the build**
- Output is **pure source code**, then passed to the compiler

Unlike C’s preprocessor, this one is:
- Type‑aware
- Expression‑aware
- File‑system aware
- Much closer to a scripting language

---

## 1. Defining & Removing Constants

### `#def` — Define a compile‑time constant

```c
#def MAX_SIZE 100
#def PI 3.14159
#def NAME "Hossein"
```

Usage:

```c
int x = MAX_SIZE;
double y = PI;
```

Once defined, a value **cannot be changed**.

---

### `#udef` — Remove a definition

```c
#udef MAX_SIZE
```

After this, `MAX_SIZE` no longer exists.

### `#set` — Define a compile‑time variable

```c
#set X 2
#set Y 5

#set X 4
#set Z @calc(X * Y)     // X=4, Y=5, Z=20
```

Mutable variable, the value change at compile-time.


### `#uset` — Remove a variable

```c
#uset X
```

X is no longer available..


---

### Safe versions (`?`)

| Directive | Meaning                                 |
| --------- | --------------------------------------- |
| `#def?`   | Define **only if not already defined**  |
| `#udef?`  | Remove **only if defined constant**     |
| `#set?`   | Set **only if not defined constant**    |
| `#uset?`  | Remove **only if defined variable**     |


`#def` = constant (fixed)\
`#set` = variable (mutable)

```c
#def? VERSION "1.0"
#udef? TEMP
#set? Name "Ali"
#uset? AGE
```

These prevent errors in shared or merged files.

---

### Multi‑line definition (raw source)

```c
#def CODE `` `
    int x = 10;
    int y = 20;
`` `
```

This stores **raw source code**.

---

## 2. Blocks `{( )}` vs Raw Source ` ``` `

This is **very important**.

### `{( )}` — Evaluated block (runs once)

- Executed **when defined**
- Result is stored
- Faster

```c
#def VALUE {(
    #if: $Debug
        -> 100
    #else:
        -> 50
)}
```

Later use:

```c
int x = VALUE;  // already resolved
```

---

### ` ``` ` — Raw source (runs every time)

- Stores code, not result
- Evaluated **each time used**
- Used for templates

```c
#def INIT `` `
    setup();
    start();
`` `
```

Each usage reinserts and re‑evaluates the code.

---

### Summary

| Feature    | `{( )}`   | ` ``` `         |
| ---------- | --------- | --------------- |
| Evaluation | Once      | Every use       |
| Stores     | Result    | Code            |
| Speed      | Fast      | Slower          |
| Use case   | Constants | Code generation |

---

## 3. Templates (`#temp` & `#eval`)

Templates generate **new code** based on parameters.

```c
#temp Vec(T) `` `
    struct Vector_#T {
        #T* data;
        int size;
    }
`` `
```

Instantiate:

```c
#eval Vec(int)
#eval Vec(float)
```

Result:

```c
struct Vector_int { ... }
struct Vector_float { ... }
```

---

## 4. Conditions (`#if`, `#else`)

### Single line

```c
#if: MAX_SIZE > 50 -> println("Big");
```

### Multi‑line

```c
#if: $OS_Windows
    -> println("Windows");
#else if: $OS_Linux
    -> println("Linux");
#else:
    -> println("Unknown");
```

### Negation

```c
#if not: $Arch_x86_32
```

---

## 5. Loops (`#for`)

### Basic range

```c
#for(I in 0..5):
    -> println(#I);
```

### With step

```c
#for(I in 10..0 by -1):
    -> println(#I);
```

### Nested

```c
#for(X in 0..2): {(
    #for(Y in 0..2):
        -> println(X, Y);
)}
```

---

## 6. Functions (`#func`)

### Simple

```c
#func SUM(a, b) a + b
```

### Typed

```c
#func ADD(int a, int b) a + b
```

### Multiple accepted types

```c
#func toString({int, float, string} v) "#v"
```

### Multi‑line

```c
#func INIT() `` `
    load();
    start();
`` `
```

### Returning values

```c
#ret value
```

---

## 7. Switch (`#switch`)

```c
#switch: $OS -> {
    case $$WindowsOS: -> #def PLATFORM_WINDOWS
    case $$LinuxOS:   -> #def PLATFORM_LINUX
    default:          -> @error("Unsupported OS")
}
```

---

## 8. File Inclusion

### `#include` — raw text insertion

- Any file type
- No preprocessing

```c
#include <file.h>
#include <:src/cli.c>
```

---

### `#merge` — preprocess then insert

- Only `.cm` files
- Safe and structured

```c
#merge <utils>
#merge <:settings/config>
```

---

## 9. `#once`

Prevents multiple inclusion.

```c
#once
```

Equivalent to header guards.

---

## 10. Assertions (`#assert`)

```c
#assert: $Arch_x86_64 -> "x86_64 required"
#assert not: $Debug -> "No debug builds allowed"
```

Stops compilation if false.

---

## 11. Built‑in Constants

### Environment

| Name         | Example               | Meaning                               |
| ------------ | --------------------- | ------------------------------------- |
| `$Source`    | \<source code\>       | Current file source                   |
| `$Path`      | "./src/main.cm"       | Current file path                     |
| `$File`      | "main.cm"             | Current file name                     |
| `$Line`      | 24                    | Current line                          |
| `$OS`        | "windows"             | Operating system                      |
| `$Arch`      | "x86_64"              | CPU architecture                      |
| `$Mode`      | "release"             | debug / test / release                |
| `$Version`   | 0x010000              | Current CMinus version                |
| `Standard`   | 0x260100              | Current CMinus standard               |
| `$Std`       | "cm126"               | Current CMinus standard               |

---

### OS Values

```c
$$WindowsOS     // "windows"
$$LinuxOS       // "linux"
$$MacOS         // "mac"
```

Boolean shortcuts:

```c
$OS_Windows
$OS_Linux
$OS_Mac
```

---

### Mode Values

```c
$$Debug         // "debug"
$$Test          // "test"
$$Release       // "release"
```

Boolean shortcuts:

```c
$Debug
$Test
$Release
```

---

### Architecture Values

```c
$$x86_64        // "x86_64"
$$x86_32        // "x86_32"
$$ARMv2         // "armv2"
$$ARMv3         // "armv3"
$$ARMv4T        // "armv4t"
$$ARMv5         // "armv5"
$$ARMv6         // "armv6"
$$ARMv6T2       // "armv6t2"
$$ARMv7         // "armv7"
$$ARMv7A        // "armv7a"
$$ARMv7R        // "armv7r"
$$ARMv7M        // "armv7m"
$$ARMv7S        // "armv7s"
$$ARMv64        // "armv64"
$$MIPS          // "misp"
$$SUPERH        // "superh"
$$POWERPC       // "powerpc"
$$POWERPC64     // "powerpc64"
$$SPARC         // "sparc"
$$M68K          // "m68k"
```

Boolean shortcuts:

```c
$Arch_x86_64
$Arch_x86_32
$Arch_ARMv2
$Arch_ARMv3
$Arch_ARMv4T
$Arch_ARMv5
$Arch_ARMv6
$Arch_ARMv6T2
$Arch_ARMv7
$Arch_ARMv7A
$Arch_ARMv7R
$Arch_ARMv7M
$Arch_ARMv7S
$Arch_ARMv64
$Arch_MIPS
$Arch_SUPERH
$Arch_POWERPC
$Arch_POWERPC64
$Arch_SPARC
$Arch_M68K
```

⚠️ **Note:**
Not all real‑world architectures exist here (e.g. RISC‑V is missing). Absence means `$Arch_*` will simply be false.

---

## 12. Built‑in Functions

### Math & Logic
`@calc(expression)` | `@compare(expression)`

Evaluate a **compile-time** expression and return its result as source.

Rules:
- Expression is evaluated immediately
- Only simple operators are allowed
- No function calls inside expressions
- Result is pasted as literal source

Supported operators:

```
+, -, *, /
==, <=, >=
&&, ||
! (unary)
```

Example:

```c
#def A 10
#def B 5
#def C @calc(A * B + 2)   // -> 52

int x = @calc(4 + 6);       // expands to: int x = 10;
bool y = @compare(A > B);   // expands to: bool y = true;
```

### String / Source Functions
These functions operate on SOURCE, not runtime strings. They return new source, which is pasted at the call site.

```c
@join(separator, source1, source2, ...)
// Join multiple sources using a separator.

@concat(source1, source2, ...)
// Concatenate sources directly.

@replace(pattern, replacement, source)
// Replace all matches of pattern in source.

@reverse(source)
// Reverse the source text.

@repeat(source, count)
// Repeat source N times.

@upper(source)
@lower(source)
// Change letter case.

@length(source)
// Return length as NUMBER source.

@trim(source)
// Trim whitespace.
```

Example:

```c
@join(`, `, `a`, `b`, `c`)              // -> a, b, c
@concat(`user_`, 5)                     // -> user_5
@replace(`world`, `cm`, `hello world`)  // -> hello cm
@reverse(`hello`)                       // -> olleh
@repeat(`*`, 5)                         // -> *****
@upper(`hi`)                            // -> HI
@lower(`HI`)                            // -> hi
@length(`abcd`)                         // -> 4
@trim(`  hello  `)                      // -> hello
```

### Source Conversion

```c
@stringify(source)
// Convert source into STRING literal.

@escape(source)
// Escape special characters.

@unescape(source)
// Unescape escaped characters.
```

Example:

```c
@stringify(x + y)     // -> "x + y"
@escape(`a
b`)                   // -> a\nb
@unescape(e`a\nb`)    /* -> a
b
*/
```

### Counting

```c
@count(pattern, source)
// Count how many times pattern appears.

@lines(source)
// Count number of lines.
```

Example:

```c
@count(`a`, `abcabc`)  // -> 2
@lines(e`a\nb\nc`)     // -> 3
```

### Matching

```c
@match(pattern, source)
// true if pattern matches source.

@contain(pattern, source)
// true if pattern is contained in source.
```

Example:

```c
@match(*`^a..`, `abc`)     // -> true
@contain(%`__c%`, `abcde`)   // -> true
```

### Output

```c
@echo(string)
// Print message during compilation.

@warning(string)
// Print warning (does not stop build).

@error(string)
// Print error and stop compilation.
```

Example:

```c
@echo(`Compiling main.cm`)
@warning(`Deprecated API used`)
@error(`Invalid configuration`)
```

---

## 13. File Functions

### Safe (read‑only)

```c
@file_exists(path)
@file_size(path)
@file_read(path)
```

### Dangerous (write during compile)

```c
@file_write(path, content)
@file_append(path, content)
@file_delete(path)
@file_move(oldpath, newpath)
@file_copy(source, destination)
@file_rename(oldpath, newpath)
```

⚠️ These run **every compilation**.

---

## 14. Concatenation Rules

| Operator | Meaning          |
| -------- | ---------------- |
| default  | `(a) + (b)`      |
| `#`      | direct insertion |
| `##`     | token merge      |

```c
#func ID(x) var_#x
```

---

## 15. Clarifications & Precise Rules (Important)

This section fixes **exact misunderstandings** and documents the **real rules**.

---

### A. `#merge <utils>` — Folder or file?

```c
#merge <utils>
```

**Meaning:**
- `utils` is treated as a **module name**
- The preprocessor **automatically adds `.cm`**
- It resolves to:

```text
utils.cm
```

So:

```c
#merge <utils>      // → utils.cm
#merge <utils.cm>   // same thing
```

If the file does not exist → error.

---

### B. `#merge` Paths (No Wildcards)

The preprocessor **does NOT support pattern matching** for `#merge`.

Only **explicit paths** are allowed.

---

### Relative path (from current file)

```c
#merge <utils>                // utils.cm (auto-added)
#merge <utils/string>         // utils/string.cm
#merge <../common/helpers>    // ../common/helpers.cm
```

Rules:
- `.cm` is added automatically
- Must resolve to a **single file**
- File must exist

---

### Path from main file (`<: >`)

```c
#merge <:src/utils/string>
```

Meaning:
- Path is resolved **from the main compilation unit**
- Equivalent to absolute import inside the project

---

### `#include` from main path

```c
#include <:src/config/cfg.json>
```

Same path rules, but:
- Raw text paste
- No preprocessing

---

## 16. **Exact Rules for Blocks vs Source**

This is the most important correction.

### `{( )}` — BLOCK (evaluated)

Used by:
- `#def`
- `#if`
- `#for`
- `#switch`
- `#func`

Rules:
- Executed when expanded
- Can contain control flow
- Can use `#ret`
- **Result is source**

```c
#func F() {(
    #ret 5
)}
```

> Valid ✅

---

### `->` — ONE-LINE BLOCK

```c
#if: X > 0 -> println(X);
```

- Same as `{( )}` but single expression
- Allowed anywhere a block is allowed

---

### ` ``` ` — RAW SOURCE (template)

Used by:
- `#temp`
- `#def` (source literal)
- `#set` 

Rules:
- NOT evaluated immediately
- Pasted **as-is**
- Evaluated **after paste**
> ❎ `#ret` NOT allowed

```c
#def SRC `` `
    println("hi");
`` `
```

This is **literal source**, not logic.

---

### Summary Table

| Construct | Evaluated | Allows `#ret` | Reevaluated         |
| --------- | --------- | ------------- | ------------------- |
| `{( )}`   | Yes       | Yes           | Yes (`#func` only)  |
| `->`      | Yes       | Yes           | Yes (`#func` only)  |
| `` ` ``   | No        | ❌             | Yes (after paste)   |

---

## 17. `#func` — Why It Acts Differently

Key rule:

> **Functions are always re-evaluated when called**

Even if implemented using `{( )}`.

```c
#func ADD(a, b) {(
    #ret a + b
)}
```

Every call re-runs the block.

Source functions:

```c
#func CODE() `` `
    println("Hello");
`` `
```

Rules:
- No `#ret`
- Acts like literal paste
- Reevaluated each time
- No `#eval` needed

---

## 18. `#temp` — Allowed Forms

Templates accept **ONLY SOURCE**:

- single-line source
- multi-line ` ``` ` source

> ❎ `{( )}` blocks

```c
#temp T(x) `` `
    int #x;
`` `
```

---

## 19. Function Parameter Types (FULL LIST)

```text
i8, i16, i32, i64
u8, u16, u32, u64
f32, f64

char, rune, string
int, long, float, double
uint, ulong

ident        // identifier
regex        // *`pattern`
like         // %`pattern`
expr         // default (expression)
```

Example:

```c
#func F({int, float} x) x * 2
```

---

## 20. Source Passing Forms

```c
@function(
    direct,
    `quoted source`,
    `` `
        multi
        line
        source
    `` `
)
```

Meaning:
- `direct` → expression
- `` `x` `` → literal token
- multi-line → pasted source

---

## 21. String Expansion & Placeholders

### Direct variable

```c
"#var"
```

### Expression placeholder

```c
"#{@calc(x * 10)}"
```

### Include inside string

```c
"##include<file.txt>##"
```

### Conditional inside string

```c
"##if: $OS_Windows -> `windows` #else: -> `other`##"
```

All are expanded **before compilation**.


---

## 22. Patterns Explained

| Syntax         | Meaning            |
| -------------- | ------------------ |
| `` *`regex` `` | Regular expression |
| `` %`like` ``  | SQL LIKE           |
| `` `string` `` | Exact literal      |

---

## 23. Expressions — REAL LIMITS

Supported:

```text
+, -, *, /
==, <=, >=
&&, ||
! (unary)
```

No function calls, no ternary, no complex AST.

Compiler understands **literals only**, not runtime variables.

---

## 24. `#include` vs `#merge` (FINAL RULE)

### `#include`

- Raw text paste
- No processing
- Any file type
- C-style

### `#merge`

- `.cm` only
- Preprocessed
- Supports `#once`
- Compile-time import\

> ✅ Both expand **in place**\
> ✅ Both can be used inside strings\
> ✅ Both use `<path>` — NOT quoted strings

```c
"##include<file.txt>##"
```

---

## `#include` vs `#merge` (Unified Explanation)

### `#include`

- Comes from C
- Raw text inclusion
- No preprocessing
- Any file type
- No guards

```c
#include <file.h>
#include <:src/config/cfg.json>
```

---

### `#merge`

- CMinus feature
- `.cm` files only
- Preprocessed before insertion
- Supports `#once`
- Acts like **compile-time import**

```c
#merge <utils>
#merge <:src/utils/string>
```

---

### Shared behavior

- Expand **in place**
- Can be expanded **inside strings**
- Expect paths inside `< >` (not quoted strings)

```c
"##include<file.txt>##"
```

---

## Final Mental Model (Unified)

- Preprocessor = compile-time language
- Blocks `{( )}` / `->` = logic
- Source ` ``` ` = templates
- Functions always re-evaluate
- `#merge` = module system
- Compiler only sees final expanded source


**C-PP: All C preprocessor magic, minus the limitations.**

---

*Documentation version: 1.0 - Based on agreed preprocessor system as our discussions*\
*Author: Hossein Mojadami (dreamProgrammer)*\
*Date: 2025-12-19*\
\
**Contributors**:
- `Hossein Mojadami`  —  [Github](https://github.com/dreamProgrammerh)
- `Mehdi Amiri`  —  [Github]()