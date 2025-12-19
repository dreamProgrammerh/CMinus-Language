# CMinus Preprocessor Grammar

## 1. Lexical Elements

```
Identifier   ::= Letter { Letter | Digit | "_" }
Number       ::= Digit { Digit } [ "." Digit { Digit } ]
String       ::= '"' { Character } '"'
RawSource    ::= "```" { AnyChar } "```"
Regex        ::= "*`" { AnyChar } "`"
Like         ::= "%`" { AnyChar } "`"
Literal      ::= "`" { AnyChar } "`"

Path         ::= "<" PathBody ">"
PathBody     ::= [ ":" ] { AnyCharExcept(">") }

Letter       ::= "A"…"Z" | "a"…"z"
Digit        ::= "0"…"9"
````

---

## 2. Program Structure

```
PreprocessorFile ::= { Statement }
```

---

## 3. Statements

```
Statement ::=
      Definition
    | Undefinition
    | Assignment
    | Unassignment
    | Conditional
    | Loop
    | FunctionDecl
    | TemplateDecl
    | Switch
    | Include
    | Merge
    | Once
    | Assertion
    | ExpressionStmt
```

---

## 4. Definitions & Variables

```
Definition ::=
      "#def"  Identifier Value
    | "#def?" Identifier Value

Undefinition ::=
      "#udef"  Identifier
    | "#udef?" Identifier

Assignment ::=
      "#set"  Identifier Value
    | "#set?" Identifier Value

Unassignment ::=
      "#uset"  Identifier
    | "#uset?" Identifier
```

---

## 5. Values

```
Value ::=
      Expression
    | Block
    | RawSource
```

---

## 6. Blocks

```
Block ::= "{(" { Statement } ")}"

OneLineBlock ::= "->" Expression
```

---

## 7. Conditionals

```
Conditional ::=
    "#if" ":" Condition Block
    { "#else" "if" ":" Condition Block }
    [ "#else" ":" Block ]

Condition ::=
      Expression
    | "not" ":" Expression
```

---

## 8. Loops

```
Loop ::=
    "#for" "(" Identifier "in" Range ")" ":" LoopBody

LoopBody ::= Block | OneLineBlock

Range ::=
    Expression ".." Expression
    [ "by" Expression ]
```

---

## 9. Functions

```
FunctionDecl ::=
    "#func" Identifier "(" [ Parameters ] ")" FunctionBody

FunctionBody ::=
      Expression
    | Block
    | RawSource

Parameters ::=
    Parameter { "," Parameter }

Parameter ::=
      TypedParameter
    | Identifier

TypedParameter ::=
    Type Identifier

Type ::=
      PrimitiveType
    | "{" Type { "," Type } "}"

PrimitiveType ::=
    "int" | "float" | "string"
  | "i8" | "i16" | "i32" | "i64"
  | "u8" | "u16" | "u32" | "u64"
  | "f32" | "f64"
  | "ident" | "regex" | "like" | "expr"
```

---

## 10. Return Statement

```
Return ::= "#ret" Expression
```

(Only valid inside evaluated blocks)

---

## 11. Templates

```
TemplateDecl ::=
    "#temp" Identifier "(" [ Parameters ] ")" RawSource

TemplateEval ::=
    "#eval" Identifier "(" [ Arguments ] ")"
```

---

## 12. Switch

```
Switch ::=
    "#switch" ":" Expression "->" "{"
        { Case }
        [ DefaultCase ]
    "}"

Case ::=
    "case" Expression ":" SwitchBody

DefaultCase ::=
    "default" ":" SwitchBody

SwitchBody ::= OneLineBlock | Block
```

---

## 13. Includes & Merges

```
Include ::= "#include" Path
Merge   ::= "#merge"   Path
Once    ::= "#once"
```

---

## 14. Assertions

```
Assertion ::=
      "#assert" ":" Condition "->" String
    | "#assert" "not" ":" Condition "->" String
```

---

## 15. Expressions

```
Expression ::=
      Literal
    | String
    | Number
    | Identifier
    | FunctionCall
    | UnaryExpr
    | BinaryExpr
```

### Function Call

```
FunctionCall ::=
    "@" Identifier "(" [ Arguments ] ")"
```

```
Arguments ::=
    Expression { "," Expression }
```

---

### Unary & Binary Expressions

```
UnaryExpr ::= "!" Expression

BinaryExpr ::=
    Expression BinaryOp Expression

BinaryOp ::=
      "+"
    | "-"
    | "*"
    | "/"
    | "=="
    | "<="
    | ">="
    | "&&"
    | "||"
```

---

## 16. Expression Statements

```
ExpressionStmt ::= Expression
```

---

## 17. Special Patterns

```
Pattern ::=
      Regex
    | Like
    | Literal
```

---

## 18. Built-in Functions (Grammar View)

```
BuiltinFunction ::=
      "@calc" "(" Expression ")"
    | "@compare" "(" Expression ")"
    | "@join" "(" Arguments ")"
    | "@concat" "(" Arguments ")"
    | "@replace" "(" Pattern "," Expression "," Expression ")"
    | "@stringify" "(" Expression ")"
    | "@escape" "(" Expression ")"
    | "@unescape" "(" Expression ")"
```

---

## 19. Notes (Important Semantics)

- `{( )}` blocks are **evaluated**
- ` ``` ``` ` blocks are **raw source**
- `#func` bodies **re-evaluate on every call**
- `#temp` only accepts **raw source**
- `#ret` is **illegal in raw source**
- `#merge` always resolves to `.cm`
- `#merge` is **recursive**
- `#once` ensures **single inclusion**
- `#assert` checks **conditions at compile time**
- `#func` and `#temp` are **dynamic**
- `#ret` is **used for returning values**

---

*Version 1.0*