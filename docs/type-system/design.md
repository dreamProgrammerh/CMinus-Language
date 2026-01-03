# Design Principles

## 1. No Hidden Cost

If an operation allocates memory, performs a copy, or introduces indirection,  
it should be **visible in the code** whenever possible.

> This principle is applied by design, but may be refined through discussion.

## 2. C Compatibility First

Every CMinus type must have:
- a clear C representation, or
- a clearly documented transformation into C

## 3. Safety Is Optional, Not Mandatory

CMinus provides **safety tools**, but never forces them.
Unsafe behavior must be explicit and local.
