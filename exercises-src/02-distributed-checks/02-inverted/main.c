// Exercise: Inverted Logic (Distributed Checks)
// The validation logic is inverted - success looks like failure
// Goal: Understand the inverted logic and find the right input

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FLAG{LOGIC_INVERTED_b2c3d4e5} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x1b, 0x18, 0x10,
    0x1e, 0x14, 0x08, 0x1e, 0x19, 0x01, 0x12, 0x05,
    0x03, 0x12, 0x13, 0x08, 0x35, 0x65, 0x34, 0x64,
    0x33, 0x63, 0x32, 0x62, 0x2a
};
static int flag_len = 29;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

// This function returns 1 for "failure" and 0 for "success" (inverted!)
int check_password(const char *pass) {
    if (strlen(pass) != 8) return 1;

    // Check each char - but logic is inverted
    if (pass[0] == 'F') return 1;  // Actually wants 'F'
    if (pass[1] == 'L') return 1;
    if (pass[2] == 'I') return 1;
    if (pass[3] == 'P') return 1;
    if (pass[4] == 'P') return 1;
    if (pass[5] == 'E') return 1;
    if (pass[6] == 'D') return 1;
    if (pass[7] == '!') return 1;

    return 0;  // "Success" in inverted logic
}

int main(int argc, char **argv) {
    printf("=== Inverted Logic Check ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: Things are not what they seem...\n");
        return 1;
    }

    int result = check_password(argv[1]);

    // The result interpretation is also inverted
    if (result == 0) {
        printf("Access denied.\n");
        return 1;
    }

    // result != 0 means success in this twisted world
    printf("Access granted!\n");
    char flag[64];
    decode_flag(flag);
    printf("Flag: %s\n", flag);

    return 0;
}
