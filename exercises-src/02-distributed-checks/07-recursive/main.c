// Exercise: Recursive Check (Distributed Checks)
// Validation through recursive function calls
// Goal: Understand the recursive validation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FLAG{RECURSIVE_WIN_a7b8c9d0} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x05, 0x12, 0x14,
    0x02, 0x05, 0x04, 0x1e, 0x01, 0x12, 0x08, 0x00,
    0x1e, 0x19, 0x08, 0x36, 0x60, 0x35, 0x6f, 0x34,
    0x6e, 0x33, 0x67, 0x2a
};
static int flag_len = 28;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

// Expected password built character by character
static const char expected[] = "RECURSE";

int validate_recursive(const char *input, int index, int depth) {
    // Base case: reached end of expected
    if (index >= (int)strlen(expected)) {
        return (depth == strlen(expected));  // Must have correct depth
    }

    // Check current character
    if (input[index] != expected[index]) {
        return 0;
    }

    // Recursive call
    return validate_recursive(input, index + 1, depth + 1);
}

int main(int argc, char **argv) {
    printf("=== Recursive Validation ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: Follow the recursive calls\n");
        return 1;
    }

    const char *input = argv[1];

    if (strlen(input) != strlen(expected)) {
        printf("Password length incorrect.\n");
        return 1;
    }

    printf("Starting recursive validation...\n");
    printf("Depth 0: checking character 0\n");

    if (validate_recursive(input, 0, 0)) {
        printf("\nRecursive validation successful!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    printf("\nRecursive validation failed.\n");
    return 1;
}
