// Exercise: Mixed Encoding (Light Obfuscation)
// Combines multiple encoding techniques
// Goal: Decode all layers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FLAG{MIXED_MASTER_b4c5d6e7} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x1a, 0x1e, 0x0f,
    0x12, 0x13, 0x08, 0x1a, 0x16, 0x04, 0x03, 0x12,
    0x05, 0x08, 0x35, 0x63, 0x34, 0x62, 0x33, 0x61,
    0x32, 0x60, 0x2a
};
static int flag_len = 27;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

// Password "MIX!" encoded with multiple methods:
// Char 0: 'M' XOR 0x20 = 0x6d ^ 0x20 = 0x4d... wait, let's use 0x77 ^ 0x3a = 0x4d = 'M'
// Char 1: 'I' = 73 = 8 * 9 + 1
// Char 2: 'X' = 88 = 'Y' - 1 (Caesar -1)
// Char 3: '!' = 33 = 0x21

// XOR encoded
static unsigned char char0_xor = 0x77;
static unsigned char char0_key = 0x3a;

// Arithmetic encoded
int get_char1() {
    return 8 * 9 + 1;  // 73 = 'I'
}

// Caesar shifted
char get_char2() {
    char c = 'Y';
    return c - 1;  // 'X'
}

// Direct but obfuscated
int get_char3() {
    int x = 0x100;
    int y = 0xdf;
    return x - y;  // 33 = '!'
}

int check_password(const char *input) {
    if (strlen(input) != 4) return 0;

    if (input[0] != (char)(char0_xor ^ char0_key)) return 0;
    if (input[1] != get_char1()) return 0;
    if (input[2] != get_char2()) return 0;
    if (input[3] != get_char3()) return 0;

    return 1;
}

int main(int argc, char **argv) {
    printf("=== Mixed Encoding ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: Multiple encoding methods combined\n");
        return 1;
    }

    if (check_password(argv[1])) {
        printf("Password correct!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    printf("Wrong password.\n");
    return 1;
}
