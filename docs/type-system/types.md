# Type System

This document defines the **CMinus (C-) type system**.


The type system is designed to:
 - stay close to C’s mental model
 - avoid hidden runtime costs
 - preserve performance predictability
 - provide a rich set of primitive and composite types

This is a **design document**, not a compiler reference.

---

## Goals

The CMinus type system aims to:
 - map cleanly to **C types**
 - be **explicit** not implicit
 - make unsafe operations visible
 - allow zero-cost abstractions

 ---

## Design Principles


### 1. No Hidden Cost

If an operation allocates memory, performs a copy, or introduces indirection,  
it must be **visible in the code**.

> This must be **applied** whenever possible.


### 2. C Compatibility First

Every CMinus type must have:
- a clear C representation, or
- a clearly documented transformation into C


### 3. Safety Is Optional, Not Mandatory

CMinus provides **safety tools**, but never forces them.
Unsafe behavior must be explicit.


## Overview of Types

CMinus provides the following categories of types:

- Primitive types
- Pointer types
- Length-Prefixed types
- Array and slice types
- String and substring types
- Struct and union types
- Variant (tagged union) types
- Class and data types
- Function types


## Primitive Types
## Composite Types
## Type Safety Model
## Performance Model
## Use Cases
## Non-Goals
