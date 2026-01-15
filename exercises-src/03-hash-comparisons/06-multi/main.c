// Exercise: Multi Hash (Hash & Comparisons)
// Multiple hash checks combined
// Goal: Pass all hash verifications

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// FLAG{MULTI_HASH_f6a7b8c9} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x1a, 0x02, 0x1b,
    0x03, 0x1e, 0x08, 0x1f, 0x16, 0x04, 0x1f, 0x08,
    0x31, 0x61, 0x36, 0x60, 0x35, 0x6f, 0x34, 0x6e,
    0x2a
};
static int flag_len = 25;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

// Hash 1: Sum of bytes
uint32_t hash_sum(const char *s) {
    uint32_t sum = 0;
    while (*s) sum += (unsigned char)*s++;
    return sum;
}

// Hash 2: XOR of bytes
uint32_t hash_xor(const char *s) {
    uint32_t h = 0;
    while (*s) h ^= (unsigned char)*s++;
    return h;
}

// Hash 3: Rotating hash
uint32_t hash_rotate(const char *s) {
    uint32_t h = 0;
    while (*s) {
        h = (h << 4) | (h >> 28);
        h ^= (unsigned char)*s++;
    }
    return h;
}

int main(int argc, char **argv) {
    printf("=== Multi Hash ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: Password must pass 3 hash checks\n");
        return 1;
    }

    const char *input = argv[1];

    // Expected values (for "MIX")
    uint32_t target_sum = 0x108;     // 'M' + 'I' + 'X' = 77 + 73 + 88 = 264
    uint32_t target_xor = 0x14;      // 'M' ^ 'I' ^ 'X'
    uint32_t target_rot = 0x00004d89;

    uint32_t h1 = hash_sum(input);
    uint32_t h2 = hash_xor(input);
    uint32_t h3 = hash_rotate(input);

    printf("Hash Sum:    0x%08x (target: 0x%08x) %s\n",
           h1, target_sum, h1 == target_sum ? "OK" : "FAIL");
    printf("Hash XOR:    0x%08x (target: 0x%08x) %s\n",
           h2, target_xor, h2 == target_xor ? "OK" : "FAIL");
    printf("Hash Rotate: 0x%08x (target: 0x%08x) %s\n",
           h3, target_rot, h3 == target_rot ? "OK" : "FAIL");

    if (h1 == target_sum && h2 == target_xor && h3 == target_rot) {
        printf("\nAll hashes match!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    printf("\nNot all hashes match.\n");
    return 1;
}
