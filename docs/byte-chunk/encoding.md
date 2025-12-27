# **ByteChunk Stream Encoding Specification**
**Version 1.0**

## **Executive Summary**

ByteChunk Stream (BCStream) is a variable-length byte encoding designed for streaming applications where error resilience and parsing simplicity are prioritized over random access. It uses a single continuation bit per byte to delimit logical data chunks.

## **1. Encoding Format**

### **1.1 Byte Structure**

Each byte is divided into two parts:
- **Bit 7 (MSB)**: Continuation flag
- **Bits 6-0**: Data payload (7 bits)

```
Byte Structure:
┌───────────────┬──────────────────────────────┐
│  Continuation │         Data Payload         │
│    Flag       │        (7 bits, 0-127)       │
│   (1 bit)     │                              │
└───────────────┴──────────────────────────────┘
```

### **1.2 Byte Types**

| Byte Type                | Bit Pattern | Hex Range   | Meaning                 |
|--------------------------|-------------|-------------|-------------------------|
| **Start Byte**           | `0xxxxxxx`  | `0x00-0x7F` | Begins a new data chunk |
| **Continuation Byte**    | `1xxxxxxx`  | `0x80-0xFF` | Continues current chunk |

### **1.3 Chunk Definition**

A **chunk** is a sequence of bytes where:
- The first byte is always a **Start Byte**
- Zero or more **Continuation Bytes** follow
- The chunk ends when another Start Byte appears or the stream ends

**Formal Grammar:**
```
chunk             = start-byte [continuation-byte]*
start-byte        = "0" bit bit bit bit bit bit bit
continuation-byte = "1" bit bit bit bit bit bit bit
bit               = "0" | "1"
```

## **2. Parsing Rules**

### **2.1 State Machine**

The parser maintains two states:
1. **Waiting for Start**: Seeking the beginning of a new chunk
2. **In Chunk**: Accumulating bytes for the current chunk

### **2.2 Parsing Algorithm**

```
For each incoming byte:
  If state is "Waiting for Start":
    If byte is Start Byte (bit7=0):
      - Begin new chunk with this byte
      - Change state to "In Chunk"
    Else (byte is Continuation Byte):
      - Discard byte (error recovery)
      - Remain in "Waiting for Start"
      
  Else if state is "In Chunk":
    If byte is Continuation Byte (bit7=1):
      - Append byte to current chunk
      - Remain in "In Chunk"
    Else (byte is Start Byte):
      - Complete current chunk
      - Begin new chunk with this byte
      - Remain in "In Chunk"
```

### **2.3 Error Recovery**

The encoding is **self-synchronizing**: a parser can always recover to a valid state within one byte.

**Recovery Guarantee:** No matter how corrupted the data stream, the next Start Byte (bit7=0) will always begin a valid chunk, and all subsequent bytes will be interpreted correctly.

## **3. Chunk Size Limits**

### **3.1 Implementation Limits**

| Limit Type        | Minimum    | Default      | Maximum                  |
|-------------------|------------|--------------|--------------------------|
| **Chunk Size**    | 1 byte     | 4,096 bytes  | Implementation-defined   |

### **3.2 Practical Considerations**

- **Minimum chunk**: A single Start Byte (7 bits of data)
- **Maximum chunk**: Unlimited in theory, limited by parser memory in practice
- **Empty chunks**: Not permitted (chunks must contain at least one byte)

## **4. Data Representation**

### **4.1 Encoding Arbitrary Data**

To encode arbitrary binary data:
1. Split data into logical chunks based on application needs
2. For each chunk:
   - Encode first byte as Start Byte (clear bit7)
   - Encode all subsequent bytes as Continuation Bytes (set bit7)

**Example:** Encoding 3-byte value `0x12 0x34 0x56`
```
Original:    00010010 00110100 01010110
Encoded:     00010010 10110100 11010110
             (0x12)   (0xB4)   (0xD6)
Type:        Start    Cont.    Cont.
```

### **4.2 Efficiency**

| Metric | Value | Comparison to UTF-8 |
|--------|-------|-------------------|
| **Overhead per byte** | 12.5% (1/8 bits) | Variable (0-300%) |
| **Effective payload** | 87.5% of raw bytes | 75-100% of raw bytes |
| **Error recovery** | 0-1 byte loss | 0-4 byte loss |

## **5. Comparison with Other Encodings**

### **5.1 Strengths**

1. **Streaming Performance**: No buffering required, processes bytes as they arrive
2. **Error Resilience**: Automatic recovery from corrupted data
3. **Parsing Simplicity**: Only one bit check per byte
4. **Uniform Handling**: Consistent behavior regardless of data type
5. **No Escape Sequences**: All byte values are valid

### **5.2 Limitations**

1. **No Random Access**: Cannot jump to arbitrary positions without scanning
2. **ASCII Incompatibility**: Not compatible with legacy ASCII-only systems
3. **Fixed Overhead**: Always uses 12.5% more space than raw data
4. **No Length Prefix**: Cannot determine chunk size without parsing

## **6. Use Cases**

### **6.1 Recommended Uses**

1. **Network Protocols**: Where packets may arrive corrupted or out-of-order
2. **Streaming Data**: Real-time sensor data, live feeds, audio/video streams
3. **Embedded Systems**: Limited memory and processing power
4. **Bytecode Distribution**: Streaming executable code with natural instruction boundaries
5. **Log Streaming**: Continuous log data where errors should not halt processing

### **6.2 Not Recommended For**

1. **Text Editing**: Where random access to characters is required
2. **Database Indexing**: Where direct position lookup is needed
3. **Legacy Systems**: Requiring ASCII compatibility
4. **Extremely Space-Constrained**: Where 12.5% overhead is unacceptable

## **7. Security Considerations**

### **7.1 Resource Management**

Implementations MUST:
1. Enforce maximum chunk size limits to prevent memory exhaustion
2. Provide timeout mechanisms for streams without Start Bytes
3. Validate chunk content before processing (application responsibility)

### **7.2 Error Handling**

Implementations SHOULD:
1. Provide configurable error recovery strategies
2. Log or report skipped bytes for debugging
3. Allow applications to define maximum skip limits

## **8. Implementation Guidelines**

### **8.1 Required Features**

A conforming implementation MUST:
1. Correctly parse valid BCStream data according to Section 2
2. Recover from corrupted data as described in Section 2.3
3. Enforce configurable maximum chunk sizes
4. Provide both streaming and batch parsing interfaces

### **8.2 Recommended Features**

A conforming implementation SHOULD:
1. Provide chunk callback mechanisms for event-driven architectures
2. Include utilities for common operations (chunk splitting, merging)
3. Support configurable memory allocation strategies
4. Include comprehensive test suites

## **9. Test Vectors**

### **9.1 Basic Parsing**

| Input (Hex)   | Expected Output                            |
|---------------|--------------------------------------------|
| `41`          | One chunk: `41`                            |
| `41 82 83`    | One chunk: `41 82 83`                      |
| `41 82 45 86` | Two chunks: `41 82` and `45 86`            |
| `80 41 82`    | One chunk: `41 82` (first byte skipped)    |

### **9.2 Error Recovery**

| Test Case            | Input                | Behavior                                  |
|----------------------|----------------------|-------------------------------------------|
| Missing Start        | `80 81 82`           | All bytes skipped, no chunks produced     |
| Interrupted Chunk    | `41 82 80 45`        | First chunk: `41 82`, second chunk: `45`  |
| Maximum Chunk        | `41` + 4095 × `82`   | Single 4096-byte chunk (if limit ≥4096)   |

## **10. Performance Characteristics**

### **10.1 Time Complexity**

| Operation         | Complexity | Notes                             |
|-------------------|------------|-----------------------------------|
| Parse byte        | O(1)       | Single bit mask operation         |
| Encode byte       | O(1)       | Single bit set/clear operation    |
| Find chunk start  | O(n)       | Must scan from beginning          |
| Error recovery    | O(1)       | Immediate on next byte            |

### **10.2 Space Efficiency**

| Data Type     | BCStream Size        | Equivalent UTF-8      | Comparison          |
|---------------|----------------------|-----------------------|---------------------|
| ASCII text    | ~100–112.5% of raw   | 100% of raw           | Similar or slightly larger |
| Mixed text    | 112.5% of raw        | 100-150% of raw       | Similar or better   |
| Binary data   | 112.5% of raw        | N/A                   | Baseline            |
| Dense data    | 112.5% of raw        | 300% of raw           | 62.5% smaller       |

## **11. Bytecode Application Notes**

### **11.1 Instruction Encoding**

For bytecode applications, each instruction naturally maps to a BCStream chunk:
- **Opcode**: Always encoded as Start Byte
- **Operands**: Encoded as Continuation Bytes
- **Instruction boundary**: Automatically detected by parser

This provides natural error recovery at instruction boundaries and enables streaming execution.

### **11.2 Streaming VM Integration**

A ByteChunk Stream Virtual Machine:
1. Receives BCStream-encoded bytecode
2. Parses chunks as they arrive
3. Executes each chunk as a complete instruction
4. Automatically recovers from transmission errors
5. Requires no buffering of future instructions

## **12. Formal Properties**

### **12.1 Guarantees**

1. **Self-Synchronization**: Any valid Start Byte begins a valid chunk
2. **Error Containment**: A corrupted byte affects at most one following byte
3. **Deterministic Parsing**: Same input always produces same chunks
4. **Bounded Recovery**: Maximum 1 byte lost per corruption point

### **12.2 Invariants**

1. Every chunk contains exactly one Start Byte
2. No two consecutive chunks can merge
3. The encoding of valid data is always valid BCStream
4. Empty streams produce no chunks

## **13. Conformance**

An implementation is **BCStream-compliant** if it:
1. Correctly parses all test vectors in Section 9
2. Adheres to the parsing rules in Section 2
3. Implements the error recovery guarantees in Section 2.3
4. Documents any deviations from this specification

## **14. Version History**

| Version | Date       | Changes                 |
|---------|------------|-------------------------|
| 1.0     | 2025-12-27 | Initial specification   |

## **15. Acknowledgments**

ByteChunk Stream encoding was designed with the following principles:
- Simplicity over complexity
- Streaming over random access
- Error recovery over strict validation
- Practical utility over theoretical perfection

---

**Copyright Notice**: This specification is provided as-is for implementation and discussion. Implementers are free to use, modify, and distribute implementations of this encoding.

**End of Specification**