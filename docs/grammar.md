# **C- Runtime Grammar**

---

## 1. Program Structure

```
Program        ::= { TopLevelDecl }
```

---

## 2. Top-Level Declarations

```
TopLevelDecl   ::=
      MainDecl
    | FunctionDecl
    | StructDecl
    | UnionDecl
    | VariantDecl
    | EnumDecl
    | ClassDecl
    | InterfaceDecl
    | DatatypeDecl
    | ModuleDecl
    | ImportDecl
    | GlobalVarDecl
```

---

## 3. Entry Point (`main`)

```
MainDecl ::= 
    "main"
    [ Identifier ]
    [ MainConfig ]
    Block

MainConfig ::= 
    "[" { ConfigEntry } "]"

ConfigEntry ::= 
    Identifier ":" Expression
```

---

## 4. Imports & Modules

```
ImportDecl ::= 
    "import" String [ "as" Identifier ] ";"

ModuleDecl ::= 
    "alias" Identifier ";"
```

---

## 5. Types

```
Type ::=
      PrimitiveType
    | Identifier
    | PointerType
    | ArrayType
    | SliceType
    | FunctionType
    | TupleType
    | VariantType
```

### Primitive Types

```
PrimitiveType ::=
      "i8" | "i16" | "i32" | "i64"
    | "u8" | "u16" | "u32" | "u64"
    | "f32" | "f64"
    | "int" | "uint" | "long" | "ulong"
    | "float" | "double"
    | "bool"
    | "char" | "rune" | "string"
```

---

### Pointers & Arrays

```
PointerType ::= Type "*"

ArrayType ::= 
    Type "[" Expression "]"

SliceType ::= 
    Type "[" "]"
```

---

### Function Types

```
FunctionType ::= 
    Type "(" [ ParameterTypes ] ")"

ParameterTypes ::= 
    Type { "," Type }
```

---

### Tuple & Variant Types

```
TupleType ::= 
    "(" Type { "," Type } ")"

VariantType ::= 
    "variant" "{" Type { "," Type } "}"
```

---

## 6. Variables & Constants

```
GlobalVarDecl ::= 
      VarDecl ";"
    | ConstDecl ";"

VarDecl ::= 
    [ StorageClass ] Type Identifier [ "=" Expression ]

ConstDecl ::= 
    "const" Type Identifier "=" Expression

StorageClass ::= 
    "static" | "volatile" | "register"
```

---

## 7. Functions

```
FunctionDecl ::= 
    [ FunctionQualifier ]
    Type Identifier
    "(" [ Parameters ] ")"
    FunctionBody
```

```
FunctionQualifier ::= 
    "inline" | "flat" | "static" | "const"
```

```
Parameters ::= 
    Parameter { "," Parameter }

Parameter ::= 
    Type Identifier [ "=" Expression ]
```

```
FunctionBody ::= 
      Block
    | "->" Expression ";"
```

---

## 8. Statements

```
Statement ::=
      Block
    | VarDecl ";"
    | Assignment ";"
    | IfStmt
    | SwitchStmt
    | ForStmt
    | WhileStmt
    | ReturnStmt ";"
    | BreakStmt ";"
    | ContinueStmt ";"
    | ExpressionStmt ";"
```

---

## 9. Blocks

```
Block ::= 
    "{" { Statement } "}"
```

---

## 10. Control Flow

### If

```
IfStmt ::= 
    "if" Expression Block
    { "else" "if" Expression Block }
    [ "else" Block ]
```

---

### Switch

```
SwitchStmt ::= 
    "switch" [ ":" Identifier ] "(" Expression ")" "{"
        { CaseStmt }
        [ DefaultStmt ]
    "}"
```

```
CaseStmt ::= 
    "case" CaseValue "->" SwitchBody

CaseValue ::= 
      Expression
    | Expression ".." Expression
```

```
SwitchBody ::= 
      Statement
    | Block
```

---

### Loops

```
ForStmt ::=
    "for" [ ":" Identifier ]
    "(" ForHeader ")"
    Block
    [ "never" Block ]
```

```
ForHeader ::=
      Identifier "in" Range
    | Identifier "of" Expression
```

```
Range ::= 
    Expression ".." Expression
    [ "by" Expression ]
```

---

```
WhileStmt ::=
    "while" [ "not" ] Expression Block
    [ "never" Block ]
```

---

## 11. Jump Statements

```
ReturnStmt ::= 
    "return" [ Expression ]

BreakStmt ::= 
    "break" [ Identifier ]

ContinueStmt ::= 
    "continue" [ Identifier ] [ "when" Expression ]
```

---

## 12. Expressions

```
Expression ::= 
      AssignmentExpr
```

```
AssignmentExpr ::= 
      LogicalOrExpr
    | UnaryExpr AssignmentOp AssignmentExpr
```

```
AssignmentOp ::= 
    "=" | "+=" | "-=" | "*=" | "/="
```

---

### Operator Precedence

```
LogicalOrExpr ::= 
    LogicalAndExpr { "||" LogicalAndExpr }

LogicalAndExpr ::= 
    EqualityExpr { "&&" EqualityExpr }

EqualityExpr ::= 
    RelationalExpr { ("==" | "!=") RelationalExpr }

RelationalExpr ::= 
    AdditiveExpr { ("<" | ">" | "<=" | ">=") AdditiveExpr }

AdditiveExpr ::= 
    MultiplicativeExpr { ("+" | "-") MultiplicativeExpr }

MultiplicativeExpr ::= 
    UnaryExpr { ("*" | "/" | "%") UnaryExpr }
```

---

### Unary & Postfix

```
UnaryExpr ::= 
      ("!" | "-" | "&" | "*") UnaryExpr
    | PostfixExpr
```

```
PostfixExpr ::= 
    PrimaryExpr
    { PostfixOp }
```

```
PostfixOp ::= 
      "(" [ Arguments ] ")"
    | "[" Expression "]"
    | "." Identifier
```

---

### Primary Expressions

```
PrimaryExpr ::=
      Identifier
    | Literal
    | "(" Expression ")"
```

---

## 13. Literals

```
Literal ::=
      IntegerLiteral
    | FloatLiteral
    | StringLiteral
    | CharLiteral
    | "true"
    | "false"
    | "null"
```

---

## 14. Arguments

```
Arguments ::= 
    Expression { "," Expression }
```

---

## 15. Structs, Enums, Classes (High-Level)

```
StructDecl ::= 
    "struct" Identifier
    [ "~" ]
    Block
```

```
EnumDecl ::= 
    "enum" [ ":" Type ] Identifier
    "{" EnumEntry { "," EnumEntry } "}"
```

```
ClassDecl ::= 
    "class" Identifier
    [ "extends" Identifier ]
    Block
```

```
InterfaceDecl ::= 
    "interface" Identifier
    Block
```

---

## 16. Notes (Semantic Rules)

* Preprocessor is **fully resolved before parsing**
* Types are resolved after parsing
* Expressions are **C-like but safer**
* `variant`, `options`, `guard/boil` expand into IR-level constructs
* Grammar is **LL-friendly** with minor refactoring

---

*Version 1.0*