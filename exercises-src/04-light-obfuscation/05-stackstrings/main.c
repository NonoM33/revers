// Exercise: Stack Strings (Light Obfuscation)
// Password built character by character on stack
// Goal: Reconstruct the string from stack operations

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FLAG{STACK_STRING_f2a3b4c5} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x04, 0x03, 0x16,
    0x14, 0x1c, 0x08, 0x04, 0x03, 0x05, 0x1e, 0x19,
    0x10, 0x08, 0x31, 0x65, 0x36, 0x64, 0x35, 0x63,
    0x34, 0x62, 0x2a
};
static int flag_len = 27;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

// Build password on stack (harder to see in strings)
void get_secret(char *buf) {
    volatile char c0 = 'B';
    volatile char c1 = 'U';
    volatile char c2 = 'I';
    volatile char c3 = 'L';
    volatile char c4 = 'D';

    buf[0] = c0;
    buf[1] = c1;
    buf[2] = c2;
    buf[3] = c3;
    buf[4] = c4;
    buf[5] = '\0';
}

int main(int argc, char **argv) {
    printf("=== Stack Strings ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: The password is built on the stack\n");
        return 1;
    }

    char secret[16];
    get_secret(secret);

    if (strcmp(argv[1], secret) == 0) {
        printf("Password correct!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    printf("Wrong password.\n");
    return 1;
}
