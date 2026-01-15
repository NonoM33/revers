// Exercise: Simple Hash (Hash & Comparisons)
// Uses a simple custom hash function
// Goal: Reverse the hash to find the password

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FLAG{HASH_CRACKED_a1b2c3d4} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x1f, 0x16, 0x04,
    0x1f, 0x08, 0x14, 0x05, 0x16, 0x14, 0x1c, 0x12,
    0x13, 0x08, 0x36, 0x66, 0x35, 0x65, 0x34, 0x64,
    0x33, 0x63, 0x2a
};
static int flag_len = 27;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

// Simple hash function - djb2 variant
unsigned int simple_hash(const char *str) {
    unsigned int hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }

    return hash;
}

int main(int argc, char **argv) {
    printf("=== Simple Hash Check ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: Find the input that produces the target hash\n");
        return 1;
    }

    const char *input = argv[1];

    // Target hash (for password "HASH")
    unsigned int target = 0x7c9e6865;

    unsigned int hash = simple_hash(input);

    printf("Your input hash: 0x%08x\n", hash);
    printf("Target hash:     0x%08x\n", target);

    if (hash == target) {
        printf("\nHash match!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    printf("\nHash mismatch.\n");
    return 1;
}
