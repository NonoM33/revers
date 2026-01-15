// Exercise: Order Matters (Distributed Checks)
// Functions must be called in the correct order
// Goal: Figure out the right sequence

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FLAG{ORDER_MATTERS_f6a7b8c9} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x18, 0x05, 0x13,
    0x12, 0x05, 0x08, 0x1a, 0x16, 0x03, 0x03, 0x12,
    0x05, 0x04, 0x08, 0x31, 0x61, 0x36, 0x60, 0x35,
    0x6f, 0x34, 0x6e, 0x2a
};
static int flag_len = 28;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

static int sequence[4] = {0, 0, 0, 0};
static int seq_index = 0;

void func_alpha() {
    if (seq_index < 4) {
        sequence[seq_index++] = 1;
    }
}

void func_beta() {
    if (seq_index < 4) {
        sequence[seq_index++] = 2;
    }
}

void func_gamma() {
    if (seq_index < 4) {
        sequence[seq_index++] = 3;
    }
}

void func_delta() {
    if (seq_index < 4) {
        sequence[seq_index++] = 4;
    }
}

int check_sequence() {
    // Expected: gamma, alpha, delta, beta -> 3, 1, 4, 2
    return (sequence[0] == 3 &&
            sequence[1] == 1 &&
            sequence[2] == 4 &&
            sequence[3] == 2);
}

int main(int argc, char **argv) {
    printf("=== Order Matters ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <sequence>\n", argv[0]);
        printf("Hint: Enter a 4-letter sequence (A/B/G/D)\n");
        return 1;
    }

    const char *input = argv[1];

    if (strlen(input) != 4) {
        printf("Sequence must be exactly 4 characters\n");
        return 1;
    }

    printf("Executing sequence...\n");
    for (int i = 0; i < 4; i++) {
        switch (input[i]) {
            case 'A': case 'a':
                printf("  -> func_alpha()\n");
                func_alpha();
                break;
            case 'B': case 'b':
                printf("  -> func_beta()\n");
                func_beta();
                break;
            case 'G': case 'g':
                printf("  -> func_gamma()\n");
                func_gamma();
                break;
            case 'D': case 'd':
                printf("  -> func_delta()\n");
                func_delta();
                break;
            default:
                printf("  -> unknown ('%c')\n", input[i]);
        }
    }

    printf("\nChecking sequence...\n");
    if (check_sequence()) {
        printf("Correct order!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    printf("Wrong order. Try again.\n");
    return 1;
}
