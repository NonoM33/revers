// Exercise: Arithmetic Obf (Light Obfuscation)
// Password chars encoded as arithmetic operations
// Goal: Solve the arithmetic to get chars

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FLAG{MATH_ENCODED_a3b4c5d6} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x1a, 0x16, 0x03,
    0x1f, 0x08, 0x12, 0x19, 0x14, 0x18, 0x13, 0x12,
    0x13, 0x08, 0x36, 0x64, 0x35, 0x63, 0x34, 0x62,
    0x33, 0x61, 0x2a
};
static int flag_len = 27;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

// Password "CALC" encoded as arithmetic
// 'C' = 67 = 100 - 33
// 'A' = 65 = 130 / 2
// 'L' = 76 = 19 * 4
// 'C' = 67 = 201 / 3

int get_char_0() { return 100 - 33; }
int get_char_1() { return 130 / 2; }
int get_char_2() { return 19 * 4; }
int get_char_3() { return 201 / 3; }

int main(int argc, char **argv) {
    printf("=== Arithmetic Obfuscation ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: Characters are computed at runtime\n");
        return 1;
    }

    const char *input = argv[1];

    if (strlen(input) != 4) {
        printf("Password must be 4 characters.\n");
        return 1;
    }

    int ok = 1;
    if (input[0] != get_char_0()) ok = 0;
    if (input[1] != get_char_1()) ok = 0;
    if (input[2] != get_char_2()) ok = 0;
    if (input[3] != get_char_3()) ok = 0;

    if (ok) {
        printf("Password correct!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    printf("Wrong password.\n");
    return 1;
}
