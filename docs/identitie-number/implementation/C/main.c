#include <stdint.h>
#include <stdio.h>

#include "idte.c"

int main() {
    uint64_t original = 0xFE21B3A4D9C8E712ULL;
    char encoded[12];
    uint64_t decoded;
    
    int len = IDTE_encode(original, encoded);

    printf("Original: 0x%llX\n", original);
    printf("Encoded:  %s\n", encoded);

    if (IDTE_decode(encoded, len, &decoded) == 0) {
        printf("Decoded:  0x%llX\n", decoded);

    } else {
        printf("Invalid identitie string!\n");
    }

    return 0;
}