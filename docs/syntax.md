# **C- Language: Complete Documentation**

## **1. Language Philosophy**

**C- (C Minus)** is C without limits, C++ without complexity. It's a superset of C that eliminates boilerplate while maintaining full compatibility. Write modern, expressive code that compiles to clean, efficient C.

**Core Principles:**
- **Transparency**: You can always see generated C code
- **Control**: Manual memory when you want, safety when you need
- **Simplicity**: One obvious way to do things
- **Interop**: First-class C compatibility

## **2. Program Structure**

### **Entry Points**
```c
// Single entry point
main {
    println("Hello C-");
}

// Multiple entry points with config
main server [port: 8080, threads: 4] {
    start_server();
}

// External config
main configed [config: "config.json"] {
    run_program();
}

main client [debug: 1] {
    connect();
}

// Config access at runtime
main [optimize: 3] {
    if (main::optimize is int level) {
        println("Optimization level: #{level}");
    }
}

// Rules:
//   1. you can have multiple entry points with different names.
//   2. only one anonymous entry point is allowed.
//   3. the compiler will ask which entry point you want to compile your program.
//   4. anonymous entry points are the default one to enter if no other entry point is specified.
//   5. entry points can have inline configuration or external from json file.
//   6. inline configuration can override external configuration. 
```
 
### **Configuration Syntax**
```c
main [
    // Build settings
    mode: "release",           // debug, release, test
    libc: 1,                   // 0 = no libc, 1 = full libc
    optimize: 3,               // 0-3
    singlefile: 0,             // 1 = single C file output
    
    // Output
    executable: "app",
    output: "./bin/",
    
    // Target
    target: "x86_64-linux",
    
    // Features
    warnings: 1,
    sanitize: 0,
    
    // Dependencies (soon)
    links: "m; pthread",
    defines: "DEBUG=1",
    
    // Import prefixes
    prefixes: "std::/usr/lib/cminus;lib::./lib"
] {
    // Program code
}
```

## **3. Type System**

### **Primitive Types**
```c
// Signed integers
i8, i16, i32, i64

// Unsigned integers
u8, u16, u32, u64

// Platform-dependent
isize, usize      // Pointer-sized integers

// Floating point
f32, f64

// Named signed integers
byte        // i8
short       // i16
int         // i32
long        // i64

// Named unsigned integers
ubyte       // u8
ushort      // u16
uint        // u32
ulong       // u64

// Named floating point
float       // f32
double      // f64

// Boolean
bool             // true/false (1 byte)

// Character types
char             // UTF-8 code unit (u8)
rune             // Unicode code point (i32)
wchar            // UTF-16 (i16)
bchar            // ByteChunk encoding (u8) (new)
```

### **Type Aliases**
```c
let x = 42;        // Inferred as i32 (default int)
int count = 100;   // i32 alias
float pi = 3.14;   // f64 alias
string buf = "";   // Mutable string alias
```

### **Pointer Types**
```c
int* ptr = null;              // Pointer to int
const int* cptr = null;       // Pointer to const
int* const const_ptr = null;  // Const pointer
int** ptr_to_ptr = null;      // Pointer to pointer
```

### **Array Types**
```c
// Fixed arrays (stack)
int[10] arr;                  // 10 integers
int[5][5] matrix;             // 5x5 2D array

// Length-prefixed arrays
int[~10] dyn = [1, 2, 3];     // First 32bit = length
int len = dyn.len;            // O(1) length

// Slices (pointer + length)
int[] slice = dyn;            // Slice of dyn
int[] sub = slice[2..5];      // Sub-slice

// C-style arrays (interop)
int* c_array = alloc 100 of int;  // No length
```


### **String Types**
```c
// Immutable strings
str greeting = "Hello";            // UTF-8, read-only
str<100> fixed_str = "Hello";      // Max 100 chars

// Mutable strings  
string buffer = "Edit me";         // UTF-8, resizable
string<256> buf = "Buffer";        // Max 256 chars, mutable

// String literals
u8"UTF-8"               // UTF-8 (default)
u16"UTF-16"             // UTF-16
u32"UTF-32/Runes"       // UTF-32
r"Rune string"          // Short for UTF-32
bc"ByteChunk"           // ByteChunk format
w"Wide string"          // UTF-16 (Windows)
x"48656C6C6F"           // Hex bytes
b"\x48\x65\x6C\x6C\x6F" // Byte escapes
```

### **Struct Types**
```c
// Basic struct
struct Point {
    int x;
    int y;
    
    // Method
    float distance() {
        return sqrt(x*x + y*y);
    }
}

// Packed struct (no padding)
struct Pixel ~{
    u8 r, g, b, a;
}

// Default values
struct Config {
    int timeout = 30;
    bool enabled = true;
}
```

### **Union Types**
```c
// C-style union
union Value {
    int i;
    float f;
    char* s;
};

// Tagged union (variant)
variant{int, float, string} val = 3.14;

// Type checking
if (val is float) {
    float f = val as float;
}

// Pattern matching (may change)
match val {
    int x => println("int: #{x}"),
    float f => println("float: #{f}"),
    string s => println("string: #{s}"),
}
```

### **Tuple Types**
```c
// Basic tuple
(int, string) result = (200, "OK");

// Access
int code = result.$0;
string msg = result.$1;

// Named tuple
typedef (int code, string message) HttpResult;
HttpResult res = (404, "Not Found");

// Destructuring
(int x, string y) = parse_input();
```

### **Enum Types**
```c
// Basic enum
enum Direction { North, East, South, West }

// With explicit type
enum: u16 HttpStatus {
    Ok = 200,
    NotFound = 404,
    ServerError = 500,
}

// Bit flags
enum: u8 Permissions {
    None = 0,
    Read = 1 << 0,
    Write = 1 << 1,
    Execute = 1 << 2,
    All = Read | Write | Execute,
}
```

### **Options (Scoped Named Constants)**
```c
// Inline constants with scope
options<int>{small=0, medium, large} size = medium;
// medium = 1, only visible during assignment

// Mixed types
options<variant{float, int, string}>{
    scale=1.8f, count=10, name="test"
} mixed = "#test: #{count + scale}";
// none of them visible here

// Typedef for reuse
typedef options<char>{up='w', down='s'} DirKeys;
DirKeys move = up;  // up visible here, becomes 'w'
```

### **Function Types**
```c
// Function pointer
int(int, int) add_func = null;

// Higher-order function
void apply(int[] arr, int(int) fn) {
    for (&item of arr) {
        item = fn(item);
    }
}
```

## **4. Variables & Constants**

### **Declaration**
```c
// Explicit type
int age = 25;
string name = "Alice";

// Type inference
let count = 42;        // i32
let pi = 3.14159;      // f64
let list = [1, 2, 3];  // i32[~3]

// Multiple variables
int x = 1, y = 2, z = 3;

// Pointers variables
int* a = &x, b = &y, c = &z;

// Constants
const MAX_SIZE = 1024;
const string GREETING = "Hello";

// Compile-time constants
final TIMEOUT = 30;
final AREA = final circle_area(5.0);
```

### **Storage Classes**
```c
static int counter = 0;      // Module-level
thread_local int tid = 0;    // Thread-local (future)
register int fast = 0;       // Hint (may be ignored)
volatile int* reg = hardware_addr;
```

### **Special Values**
```c
null          // Null pointer
none          // Optional none
undefined     // Uninitialized (compile-time tracking)
NaN/Infinite  // Special float
true/false    // Boolean values
```

## **5. Control Flow**

### **If Statement**
```c
if (x > 0) {
    println("positive");
} else if (x < 0) {
    println("negative");
} else {
    println("zero");
}

// When (postfix condition)
println("bad") when error else println("good");
return when done;

// Not keyword
if not (file.exists()) {
    create_file();
}
```

### **Switch Statement**
```c
// Basic switch
switch (value) {
    case 1:
        println("One");
        break;
    case 2..5:
        println("Two to five");
        break;
    default:
        println("Other");
}

// Enum shorthand
enum Color { Red, Green, Blue, Yellow }
Color c = Color::Red;

switch: Color (c) {
    case Red    -> println("Red");
    case Green  -> println("Green");
    case Blue   -> println("Blue");
    // arrow `->` used for one liner block
    // use {} after arrow for multi-line block
    case Yellow -> {
        println("Yellow");
        println("Yellow");
    }
}
```

### **For Loop**
```c
// Range loop
for (i in 0..10) { }          // 0 to 10
for (i in 0..20 by 2) { }     // Step by 2
for (i in 10..0 by -1) { }    // Reverse

// For-each
for (item of collection) { }      // By value
for (&item of collection) { }     // By reference
for ({i, item} in array) { }      // With index

// Map iteration
for ({key, value} of map) { }

// Named loops
for: outer (i in 0..10) {
    for: inner (j in 0..10) {
        break outer;                // Break specific loop
        continue inner when j == 5; // Conditional continue
    }
} never {
    println("Loop never entered");
}
```

### **While Loop**
```c
while (condition) { }
while not (empty) { }
do { } while (condition);

// Named while
while not: loading (done) {
    break loading when error;
} never {
    log("Loading never started");
}
```

### **Scope Control**
```c
scope: validation {
    if (input.empty()) {
        error = "Empty input";
        break validation;  // Skip to end of scope
    }
    // Only reached if no breaks
    valid = true;
}
```

### **Goto & Labels**
```c
if (error) {
    goto cleanup;
}
// ...
cleanup:
    free(resources);
    return;
```

## **6. Functions**

### **Function Declaration**
```c
// Basic function
int add(int a, int b) {
    return a + b;
}

// No return value
void greet(string name) {
    println("Hello, #{name}!");
}

// One Liner
int square(int x) -> x * x;

// Multiple returns
(int, string) get_result() {
    return (200, "OK");
}

// Default parameters
void log(string msg, bool error = false) { ... }

// Variadic functions
void printf(string fmt, void... args);      // Untyped variadic
float sum(float... nums);                   // Typed variadic
T max<T>(T... values);                      // Generic variadic
```

### **Function Qualifiers**
```c
inline  int fast_add(int a, int b) { ... }      // Inline hint
flat    void no_stack() { ... }                 // No stack variables (fast)
static  void counter() { ... }                  // Static function
const   float pi() { return 3.14; }             // Readonly return
```

### **Lambda Functions**
```c
// Basic
\(a, b) -> a + b

// Capture
int x = 10, y = 20;
\[x](a) -> a + x            // By value
\[&y](a) -> a + y           // By reference
\[x, &y]() -> x + y         // Mixed

// With types
\(int a, int b): int -> a * b

// Block body
\x -> {
    println("Processing #{x}");
    return x * 2;
}
```

### **Higher-Order Functions**
```c
// Function parameter
void apply(int[] arr, int(int) fn) {
    for (&item of arr) {
        item = fn(item);
    }
}

// Function returning function
int(int) make_adder(int n) {
    return \(int x) -> x + n;
}
```

## **7. Error Handling (Guard/Boil System)**

### **Error Type Definitions**
```c
// Basic error type
errtype Crash(bool fatal);

// Structured error
errtype MathError(string msg) {
    DivisionByZero().("Cannot divide by zero"),
    WrongSign(bool negative).(negative ?
        "Negative not supported" :
        "Positive not supported"),
};

// Type extending
errtype MathError(string msg);
errtype DivisionByZero() -> MathError("Cannot divide by zero");
```

### **Boiling Functions**
```c
float divide(int x, int y) boils MathError {
    boil WrongSign(true) when x < 0;
    boil DivisionByZero() when y == 0;
    return x / y;
}

File open(string path) boils IOError, PermissionError {
    boil FileNotFound() when !exists(path);
    boil PermissionDenied() when !can_read(path);
    return File(path);
}
```

### **The 5 Calling Conventions**
```c
// 1. func() - Errors propagate
float a = divide(5, 2);        // Boils propagate to caller

// 2. func?() - Warning, returns default
float b = divide?(5, 0);       // Warning, returns 0.0

// 3. func!() - Use buckets or crash
float c = divide!(5, 0);       // Looks for MathError bucket

// 4. func!!() - Crash immediately
float d = divide!!(5, 0);      // Immediate crash

// 5. func??() - Returns Result<T, Error>
Result<float, MathError> e = divide??(5, 0);  // Err(DivisionByZero)
```

### **Flow Control**
```c
// Basic flow
float x = divide?(5, 0) flow 0.0;

// Pattern matching flow
float y = factorial?(-1) flow {
    case WrongSign s -> -1.0;
    case DivisionByZero d -> 0.0;
    case MathError e -> {
        println("Error: #{e.msg}");
        yield 0.0;
        // retry;  // Retry operation
    };
};
```

### **Bucket Handlers**
```c
// Specific error bucket
bucket MathError e {
    println("Math error: #{e.msg}");
    yield 0.0;      // Provide alternative
    // continue;    // Continue execution
    // retry;       // Retry operation
}

// Generic bucket
bucket<t> any {
    println("Some error occurred");
    exit 1: "Unhandled error";
}

// Crash bucket
bucket Crash c {
    exit errid(Crash): "Program Crashed";
}
```

### **Guard Blocks**
```c
// Guard single expression
bool online = g! network.checkStatus;

// Guard array access
int value = g? arr[100] flow 0;

// Guard block with cleanup
g? {
    divide(5, 0);
    factorial(-1);
} flow {
    case MathError e -> yield 0.0;
    case IOError e -> yield -1.0;
} finally {
    cleanup();  // Always runs
};
```

## **8. Classes & Objects**

### **Class Declaration**
```c
class Person {
    // Fields
    string name;
    int age;
    
    // Constructor
    main(string name, int age) {
        this.name = name;
        this.age = age;
    }
    
    // Named constructor
    main.create(string n) {
        this.name = n;
        this.age = 0;
    }
    
    // Method
    void greet() {
        println("Hello, I'm #{name}, age #{age}");
    }
    
    // Static method
    static Person empty() {
        return Person("Nobody", -1);
    }
    
    // Destructor
    ~main() {
        println("Person #{name} destroyed");
    }
}
```

### **Inheritance**
```c
class Animal {
    string name;
    void make_sound() { println("Some sound"); }
}

class Dog extends Animal {          // Inheritance
    string breed;
    void make_sound() { println("Woof!"); }  // Override
    void fetch() { println("#{name} fetches"); }
}
```

### **Interface**
```c
interface Shape {
    float area();
    float perimeter();
    
    default float volume() { return 0; }
}

class Circle : Shape {
    float radius;
    float area() { return PI * radius * radius; }
    float perimeter() { return 2 * PI * radius; }
}

interface Drawable {
    int x, y;
}

class Point3D : Drawable {
    int x, y;
    int z;
    
    default void draw() { println("Drawing #{x}, #{y}"); }
}
```

### **Datatype Sugar**
```c
datatype Person(string name, int age);
// Expands to struct with constructor
```

### **Access Control**
```c
class BankAccount {
    private float balance;      // Only within class
    protected int acc_number;   // Class + derived
    public string owner;        // Anywhere
    
    public fn deposit(float amount) { ... }
    private fn validate() { ... }
}
```

### **Properties**
```c
class Rectangle {
    private float _width, _height;
    
    float get width -> _width;
    float get height -> _height;
    float get area -> _width * _height;
    
    float set width(float value) -> _width = value;
    float set height(float value) -> _height = value;
}
```

## **9. Memory Management**

### **Allocation**
```c
// Heap allocation
int* ptr = alloc 100 of int;        // 100 integers
byte* buffer = alloc 1024;          // 1024 bytes
Person* person = alloc of Person;      // One object

// Zero-initialized heap
int* zeroed = alloc* 100 of int;    // All bytes zero

// Stack allocation
int* stack_buf = alloc~ 256;        // 256 bytes on stack
int* stack_arr = alloc~ 50 of float; // 50 floats on stack

// Zero-initialized stack
int* zero_stack = alloc*~ 100 of int; // Zeroed stack array

// Object creation
Person* p1 = new Person("Alice", 30);  // Calls constructor
Person* p2 = new* Person();            // Zero-initialized
```

### **Reallocation**
```c
int* arr = alloc 10 of int;
// ... use arr ...
alloc on arr 20 of int;    // Resize to 20 elements
// arr may have new address
```

### **Deallocation**
```c
free ptr;        // Free heap memory
delete person;   // Call destructor then free
del x;           // Destroy variable (compile-time)
```

### **Array Safety**
```c
int[~10] arr = [1, 2, 3];

arr[5] = 42;        // Unsafe (no bounds check)
arr![5] = 42;       // Safe (panics if out of bounds)
int? val = arr?[5]; // Optional (none if out of bounds)

// Negative indices (only with safe accessors)
arr![-1]           // Last element
arr?[-2]           // Second to last
```

## **10. Collections (Standard Library)**

### **Lists**
```c
List<int> numbers = List.new();
numbers.append(1);
numbers.append(2);
int first = numbers[0];
numbers[0] = 42;
int len = numbers.len;
```

### **Maps**
```c
Map<string, int> ages;
ages["Alice"] = 30;
ages["Bob"] = 25;
int? alice_age = ages?["Alice"];  // Optional access
```

### **Linked Tables (Ordered)**
```c
Linked<string, int> scores;  // Insertion order preserved
scores["Alice"] = 100;
scores["Bob"] = 200;

// Iteration in insertion order
for ({name, score} of scores) {
    println("#{name}: #{score}");
}
```

### **Static Tables**
```c
table<string> Colors {
    "red"    = "red color",
    0x00FF00 = "green hex",
    10       = "special",
    'A'      = "capital",
};

// Access
Colors["red"];           // "red color" (compile-time key)
Colors::"red";           // 0 Index (compile-time)
Colors![3];              // "special" By index (panics)
Colors?[99];             // none Optional (none if invalid)
```

## **11. Modules & Imports**

### **Module Structure**
```c
// force to have alias Math by default on import 
alias Math;
// File: math.cm (becomes module "math")
const PI = 3.1415926535;

float to_radians(float deg) {
    return deg * PI / 180.0;
}

float sin(float angle) {
    return c_sin(to_radians(angle));
}

// Export list
public PI, sin(float);
// to_radians is private
// no export mean all public
```

### **Import Statements**
```c
// Basic import
import "std:math";
// uses default alias if exist

// Import with custom alias
import "std:math" as math;

// Relative imports
import "./utils";
import "../lib/network";

// Automatic Extension
import "utils";
import "utils.cm";
// both are the same

// Explicit Extension
import "utils.c";
import "utils.h";

// Standard library
import "std:io";
import "std:list";

// Custom prefixes (configured)
import "glfw::window";
import "render::shaders";

// Multiple imports
import "math", "utils" as utils;
```

### **Usage**
```c
float s = math::sin(90.0);
float c = math::cos(180.0);

// Bring into scope
using math;
float x = sin(45.0);
```

## **12. C Interoperability**

### **C Header Import**
```c
clang {
    #include <stdio.h>
    #include <math.h>
    
    // Optional declarations
    int printf(const char* format, ...);
    double sin(double x);
}

// Usage
printf("Hello from C\n");
double x = sin(3.14);
```

### **Assembly**
```c
// Direct assembly
asm {
    mov eax, 1
    mov ebx, 0
    int 0x80
}

// With inputs/outputs (need better syntax)
asm {
    add %[result], %[a], %[b]
    : [result] "=r" (result)
    : [a] "r" (a), [b] "r" (b)
}
```

### **C Types Compatibility**
```c
// C types available in standard
size_t, ssize_t
ptrdiff_t
intptr_t, uintptr_t

// Struct compatibility
#[repr(C)]
struct Point {
    int x, y;
}
```

## **13. Compile-Time Features**

### **Compile-Time Constants**
```c
final int MAX_SIZE = 1024;
final string PATH = "/usr/local";

// Compile-time assertions
ensure(MAX_SIZE > 0);        // Compile-time assert
assert(x > 0);               // Runtime assert
```

### **Type Utilities**
```c
sizeof(int)           // Size in bytes
typeof(x)             // Type object
typeid(int)           // Type identifier
alignof(int)          // Alignment requirement
maxof(u8)             // 255
minof(i8)             // -128
```

### **Attributes**
```c
#[constructor]
void init() { }       // Called before main

#[destructor]
void cleanup() { }    // Called after main

#[inline(always)]
void fast() { }

#[no_return]
void panic() { exit(1); }
```

## **14. Standard Library Preview**

### **I/O Operations**
```c
print("Hello");
println("World");
printf("Value: %d", 42);
eprint("Error");
eprintln("Message");

// Standard streams
io::stdin.readline();
io::stdout.write("Hello");
io::stderr.write("Error");
```

### **Memory Operations**
```c
mem::set(ptr, 0, 100);
mem::copy(dest, src, size);
int cmp = mem::compare(a, b, size);
```

### **Math Functions**
```c
math::min(a, b)
math::max(a, b)
math::abs(x)
math::sqrt(x)
math::pow(x, y)
math::sin(x), math::cos(x), math::tan(x)

### **String Operations**
```c
str::cmp(a, b)
str::cat(dest, src)
str::format(fmt, ...args)
...
```

## **15. Build System**

### **Compiler Commands**
```bash
# Basic compilation
c- compile program.cm

# Output options
c- compile main.cm --show-c
c- compile main.cm --show-asm
c- compile main.cm --out bin/app.out

# Compile & Run
c- run main.cm

# Override config
c- compile main.cm -mode release -optimize 2 -links "m; pthread"
c- compile main.cm -mode debug --out out.exe

# Shorthands
c-              # minmal help
c- help         # help
c- compile      # look for main.cm with deep of 3 to compile
c- run          # look for main.cm with deep of 3 to run

# CLI tools
c-              # compiler
cmin            # testing tool
                #       - network
                #       - send signals
                #       - setup project
                #       - simulate keyboard & mouse
                #       - save/load execute shortcut
                #       etc...
cminus          # documentations
```

### **Configuration File**
```json
// cminus.json
{
    "mode": "release",
    "optimize": 3,
    "target": "x86_64-linux",
    "output": "./dist/",
    "prefixes": {
        "std": "/usr/lib/cminus/std",
        "lib": "./lib"
    },
    "defines": {
        "DEBUG": "0",
        "VERSION": "\"1.0.0\""
    },
    "links": ["m", "pthread", "dl"]
}
```

---

## **Language Summary**

**C- gives you:**
- C's performance with modern syntax
- Zero-cost abstractions
- Compile-time safety checks
- First-class C interop
- Innovative error handling (Guard/Boil)
- Clean, predictable memory management
- Progressive feature adoption

**Perfect for:**
- Systems programming
- Game development
- Embedded systems
- High-performance applications
- Library development
- Education

**C-: All C's power, minus the ceremony.**

---

*Documentation version: 1.0 - Based on agreed syntax as of our design discussions*\
*Author: Hossein Mojadami (dreamProgrammer)*\
*Date: 2025-12-15*\
\
*License: MIT*\
**Contributors**:
* [Hossein Mojadami](dreamProgrammer8691@hotmail.com)

---

C- is a community-driven reimagining of C. 
Your ideas fuel its evolution!

Open an issue to propose features, submit a PR to implement improvements, 
or join design discussions about language syntax.

If you can dream of better C, together we can build it.

---

"C- is built by dreamers who see C's potential beyond its limits."
 - See a better way to write C? Let's make it happen!
 - Have a vision for better systems programming? Share it!
