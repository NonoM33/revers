// Exercise: XOR Compare (Hash & Comparisons)
// Password is XOR-compared byte by byte
// Goal: Extract the XOR key and password

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FLAG{XOR_MASTER_b2c3d4e5} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x0f, 0x18, 0x05,
    0x08, 0x1a, 0x16, 0x04, 0x03, 0x12, 0x05, 0x08,
    0x35, 0x65, 0x34, 0x64, 0x33, 0x63, 0x32, 0x62,
    0x2a
};
static int flag_len = 25;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

// Expected password XORed with key
static unsigned char xored_pass[] = {
    0x1b, 0x18, 0x0e, 0x2c, 0x18, 0x1e, 0x04
};
static int pass_len = 7;
static unsigned char xor_key = 0x42;

int check_password(const char *input) {
    if (strlen(input) != (size_t)pass_len) {
        return 0;
    }

    for (int i = 0; i < pass_len; i++) {
        if ((input[i] ^ xor_key) != xored_pass[i]) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv) {
    printf("=== XOR Compare ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: XOR works both ways...\n");
        return 1;
    }

    const char *input = argv[1];

    printf("Checking password...\n");

    if (check_password(input)) {
        printf("Password correct!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    printf("Wrong password.\n");
    return 1;
}
