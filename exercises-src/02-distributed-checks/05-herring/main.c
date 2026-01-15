// Exercise: Red Herring (Distributed Checks)
// Contains fake validation code to mislead reversers
// Goal: Find the real validation among the decoys

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FLAG{REAL_NOT_FAKE_e5f6a7b8} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x05, 0x12, 0x16,
    0x1b, 0x08, 0x19, 0x18, 0x03, 0x08, 0x11, 0x16,
    0x1c, 0x12, 0x08, 0x32, 0x62, 0x31, 0x61, 0x36,
    0x60, 0x35, 0x6f, 0x2a
};
static int flag_len = 28;

// FAKE flag to mislead
static unsigned char fake_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x11, 0x16, 0x1a,
    0x12, 0x08, 0x11, 0x1b, 0x16, 0x10, 0x2a
};

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

void decode_fake(char *out) {
    for (int i = 0; i < 15; i++) {
        out[i] = fake_flag[i] ^ 0x57;
    }
    out[15] = '\0';
}

// FAKE validator - never actually called on success path
int fake_validate(const char *pass) {
    if (strcmp(pass, "FAKE_PASSWORD") == 0) {
        return 1;
    }
    if (strcmp(pass, "admin123") == 0) {
        return 1;
    }
    return 0;
}

// REAL validator - hidden among the noise
int real_validate(const char *pass) {
    if (strlen(pass) != 6) return 0;
    if (pass[0] != 'H') return 0;
    if (pass[1] != 'I') return 0;
    if (pass[2] != 'D') return 0;
    if (pass[3] != 'D') return 0;
    if (pass[4] != 'E') return 0;
    if (pass[5] != 'N') return 0;
    return 1;
}

// Another FAKE function
int decoy_check(const char *pass) {
    // Looks important but does nothing
    volatile int x = 0;
    for (size_t i = 0; i < strlen(pass); i++) {
        x += pass[i] * 31;
    }
    return x == 0xDEADBEEF;  // Never true
}

int main(int argc, char **argv) {
    printf("=== Red Herring Challenge ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: Not everything is what it seems...\n");
        return 1;
    }

    const char *input = argv[1];

    // Fake paths that lead nowhere
    if (fake_validate(input)) {
        printf("Hmm, that looked right but...\n");
        char fake[32];
        decode_fake(fake);
        printf("Flag: %s\n", fake);
        printf("(That's not the real flag!)\n");
        return 1;
    }

    // More decoy
    if (decoy_check(input)) {
        printf("Impossible path reached!\n");
        return 1;
    }

    // Real validation
    if (real_validate(input)) {
        printf("You found the real path!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    printf("Access denied.\n");
    return 1;
}
