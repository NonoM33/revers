// Exercise: Caesar Cipher (Light Obfuscation)
// Password encrypted with Caesar cipher
// Goal: Find the shift and decode

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FLAG{CAESAR_SHIFT_c9d0e1f2} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x14, 0x16, 0x12,
    0x04, 0x16, 0x05, 0x08, 0x04, 0x1f, 0x1e, 0x11,
    0x03, 0x08, 0x34, 0x6e, 0x33, 0x67, 0x32, 0x66,
    0x31, 0x65, 0x2a
};
static int flag_len = 27;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

// "ROME" shifted by 13 (ROT13) = "EBZR"
static const char encrypted[] = "EBZR";
static int shift = 13;

char decrypt_char(char c) {
    if (c >= 'A' && c <= 'Z') {
        return 'A' + (c - 'A' + 26 - shift) % 26;
    }
    if (c >= 'a' && c <= 'z') {
        return 'a' + (c - 'a' + 26 - shift) % 26;
    }
    return c;
}

int check_password(const char *input) {
    if (strlen(input) != strlen(encrypted)) return 0;

    for (size_t i = 0; i < strlen(input); i++) {
        char shifted = 0;
        if (input[i] >= 'A' && input[i] <= 'Z') {
            shifted = 'A' + (input[i] - 'A' + shift) % 26;
        } else if (input[i] >= 'a' && input[i] <= 'z') {
            shifted = 'A' + (input[i] - 'a' + shift) % 26;  // Convert to upper
        } else {
            return 0;
        }

        if (shifted != encrypted[i]) return 0;
    }
    return 1;
}

int main(int argc, char **argv) {
    printf("=== Caesar Cipher ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: Julius Caesar used this cipher\n");
        return 1;
    }

    printf("Encrypted password hint: %s\n\n", encrypted);

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
