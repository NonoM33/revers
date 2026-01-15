// Exercise: XOR String (Light Obfuscation)
// Password is stored XOR-encoded
// Goal: Decode the XOR string to find password

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FLAG{XOR_DECODED_b8c9d0e1} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x0f, 0x18, 0x05,
    0x08, 0x13, 0x12, 0x14, 0x18, 0x13, 0x12, 0x13,
    0x08, 0x35, 0x6f, 0x34, 0x6e, 0x33, 0x67, 0x32,
    0x66, 0x2a
};
static int flag_len = 26;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

// Password XOR-encoded with key 0x33
static unsigned char secret_xor[] = {
    0x43, 0x52, 0x58, 0x40, 0x55  // "proxy" ^ 0x33
};
static int secret_len = 5;
static unsigned char xor_key = 0x33;

int check_password(const char *input) {
    if (strlen(input) != (size_t)secret_len) return 0;

    for (int i = 0; i < secret_len; i++) {
        // Decode and compare
        char expected = secret_xor[i] ^ xor_key;
        if (input[i] != expected) return 0;
    }
    return 1;
}

int main(int argc, char **argv) {
    printf("=== XOR String ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: The password is hidden with XOR\n");
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
