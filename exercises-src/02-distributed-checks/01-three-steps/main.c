// Exercise: Three Steps (Distributed Checks)
// Validation is split across three separate functions
// Goal: Pass all three validation steps

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FLAG{THREE_STEPS_OK_a1b2c3d4} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x03, 0x1f, 0x05,
    0x12, 0x12, 0x08, 0x04, 0x03, 0x12, 0x07, 0x04,
    0x08, 0x18, 0x1c, 0x08, 0x36, 0x66, 0x35, 0x65,
    0x34, 0x64, 0x33, 0x63, 0x2a
};
static int flag_len = 29;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

static int step1_passed = 0;
static int step2_passed = 0;
static int step3_passed = 0;

int validate_step1(const char *input) {
    // First 4 chars must be "STEP"
    if (strlen(input) < 4) return 0;
    if (input[0] != 'S') return 0;
    if (input[1] != 'T') return 0;
    if (input[2] != 'E') return 0;
    if (input[3] != 'P') return 0;
    step1_passed = 1;
    return 1;
}

int validate_step2(const char *input) {
    // Must contain "123"
    if (!strstr(input, "123")) return 0;
    step2_passed = 1;
    return 1;
}

int validate_step3(const char *input) {
    // Must end with "GO"
    size_t len = strlen(input);
    if (len < 2) return 0;
    if (input[len-2] != 'G' || input[len-1] != 'O') return 0;
    step3_passed = 1;
    return 1;
}

int main(int argc, char **argv) {
    printf("=== Three Steps Validation ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: The password must pass three validations\n");
        return 1;
    }

    const char *input = argv[1];

    printf("Step 1: Checking prefix...\n");
    if (!validate_step1(input)) {
        printf("Step 1 FAILED\n");
        return 1;
    }
    printf("Step 1 PASSED\n\n");

    printf("Step 2: Checking content...\n");
    if (!validate_step2(input)) {
        printf("Step 2 FAILED\n");
        return 1;
    }
    printf("Step 2 PASSED\n\n");

    printf("Step 3: Checking suffix...\n");
    if (!validate_step3(input)) {
        printf("Step 3 FAILED\n");
        return 1;
    }
    printf("Step 3 PASSED\n\n");

    // Final check
    if (step1_passed && step2_passed && step3_passed) {
        printf("All steps validated!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    printf("Validation incomplete.\n");
    return 1;
}
