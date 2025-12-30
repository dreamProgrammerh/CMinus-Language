# **Identitie Encoding Specification**

**Version:** 1.0\
**Author:** Hossein Mojadami\
**Purpose:** Compact, identifier-safe base-64 encoding for 64-bit integers and hashes.

---

## Character Set

Identitie Encoding uses a custom 64-character alphabet designed to be:
- **Safe for C/C++ identifiers**
- **Compact** (6 bits per character)
- **Human-readable and reversible**

### Alphabet (Index -> Character)

| Index Range | Characters            | Description              |
|-------------|-----------------------|--------------------------|
| 0–9         | `0 1 2 3 4 5 6 7 8 9` | Decimal digits           |
| 10–35       | `a`–`z`               | Lowercase letters        |
| 36–61       | `A`–`Z`               | Uppercase letters        |
| 62          | `_`                   | Underscore               |
| 63          | `$`                   | Dollar sign              |

**Full Alphabet (in order):**  
`0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$`

---

## Encoding

### Rules:

- **Input**: A 64-bit unsigned integer (e.g. a hash)
- **Output**: An 11-character string using the Identitie alphabet
- **Encoding Base**: Base-64 (6 bits per character)
- **Length**: Always 11 characters (since 64 ÷ 6 = 10.666… -> ceil = 11)

### Steps
1. Treat the 64-bit number as a binary integer.
2. Divide it into 11 groups of 6 bits (pad the top with zeros if needed).
3. Convert each 6-bit group (0–63) to a character using the Identitie alphabet.
4. Concatenate the characters from most significant to least significant.

---

## Decoding

### Rules

To decode an Identitie string back to a 64-bit integer:
1. Map each character to its 6-bit index using the Identitie alphabet.
2. Concatenate the 11 values into a 66-bit binary number.
3. Discard the top 2 bits (only the lower 64 bits are valid).
4. Return the result as a 64-bit unsigned integer.

---

## Parsing Strategy

To extract an Identitie-encoded suffix from a string:
1. Assume the last 11 characters are the encoded value.
2. Validate that all characters are in the Identitie alphabet.
3. Use a lookup table to decode each character to its 6-bit value.
4. Reconstruct the 64-bit integer.

---

## Validity & Safety

- All Identitie strings are valid C/C++ identifiers when used as suffixes.
- The encoding avoids ambiguous characters (e.g. no `O`, `l`, or `+`).
- The fixed length ensures easy parsing and alignment in tools.

---

## Use Cases

Identitie Encoding is ideal for:
- **Compact symbol mangling** in compilers or interpreters
- **Shortened hash representations** in filenames or URLs
- **Identifier-safe encoding** of numeric IDs in source code
- **Obfuscation** of numeric values in user-facing strings
- **Custom base-64 encoding** where standard alphabets are unsafe

---
