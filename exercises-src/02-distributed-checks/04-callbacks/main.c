// Exercise: Callback Maze (Distributed Checks)
// Validation through function pointer callbacks
// Goal: Trace through the callback chain

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FLAG{CALLBACK_CHAIN_d4e5f6a7} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x14, 0x16, 0x1b,
    0x1b, 0x15, 0x16, 0x14, 0x1c, 0x08, 0x14, 0x1f,
    0x16, 0x1e, 0x19, 0x08, 0x33, 0x63, 0x32, 0x62,
    0x31, 0x61, 0x36, 0x60, 0x2a
};
static int flag_len = 29;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

typedef int (*validator_fn)(const char *, int);

static int validation_score = 0;

int validator_length(const char *s, int expected) {
    return strlen(s) >= (size_t)expected;
}

int validator_char(const char *s, int index) {
    // Check specific character positions
    if (index == 0 && s[0] == 'C') return 1;
    if (index == 1 && s[1] == 'A') return 1;
    if (index == 2 && s[2] == 'L') return 1;
    if (index == 3 && s[3] == 'L') return 1;
    return 0;
}

int validator_sum(const char *s, int target) {
    int sum = 0;
    for (size_t i = 0; i < strlen(s); i++) {
        sum += s[i];
    }
    return (sum % 100) == target;
}

// Array of validators
static validator_fn validators[] = {
    validator_length,
    validator_char,
    validator_char,
    validator_char,
    validator_char,
    validator_sum
};

static int validator_args[] = {
    4,   // length >= 4
    0,   // char at index 0
    1,   // char at index 1
    2,   // char at index 2
    3,   // char at index 3
    56   // sum % 100 == 56
};

int main(int argc, char **argv) {
    printf("=== Callback Maze ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: Multiple validators will check your input\n");
        return 1;
    }

    const char *input = argv[1];
    int num_validators = sizeof(validators) / sizeof(validators[0]);

    printf("Running %d validators...\n\n", num_validators);

    for (int i = 0; i < num_validators; i++) {
        printf("Validator %d: ", i + 1);
        if (validators[i](input, validator_args[i])) {
            printf("PASSED\n");
            validation_score++;
        } else {
            printf("FAILED\n");
        }
    }

    printf("\nScore: %d/%d\n", validation_score, num_validators);

    if (validation_score == num_validators) {
        printf("\nAll validators passed!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    return 1;
}
