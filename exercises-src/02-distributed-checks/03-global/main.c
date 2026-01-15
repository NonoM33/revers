// Exercise: Global State (Distributed Checks)
// Uses global state machine for validation
// Goal: Navigate the state machine correctly

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FLAG{STATE_MACHINE_c3d4e5f6} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x04, 0x03, 0x16,
    0x03, 0x12, 0x08, 0x1a, 0x16, 0x14, 0x1f, 0x1e,
    0x19, 0x12, 0x08, 0x34, 0x64, 0x33, 0x63, 0x32,
    0x62, 0x31, 0x61, 0x2a
};
static int flag_len = 28;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

typedef enum {
    STATE_INIT = 0,
    STATE_A = 1,
    STATE_B = 2,
    STATE_C = 3,
    STATE_SUCCESS = 4,
    STATE_ERROR = 5
} State;

static State current_state = STATE_INIT;

void process_char(char c) {
    switch (current_state) {
        case STATE_INIT:
            if (c == 'G') current_state = STATE_A;
            else current_state = STATE_ERROR;
            break;
        case STATE_A:
            if (c == 'O') current_state = STATE_B;
            else current_state = STATE_ERROR;
            break;
        case STATE_B:
            if (c == 'O') current_state = STATE_C;
            else current_state = STATE_ERROR;
            break;
        case STATE_C:
            if (c == 'D') current_state = STATE_SUCCESS;
            else current_state = STATE_ERROR;
            break;
        default:
            current_state = STATE_ERROR;
    }
}

int main(int argc, char **argv) {
    printf("=== Global State Machine ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <input>\n", argv[0]);
        printf("Hint: The state machine needs the right sequence\n");
        return 1;
    }

    const char *input = argv[1];

    printf("Processing input through state machine...\n");
    for (size_t i = 0; i < strlen(input); i++) {
        printf("State %d -> ", current_state);
        process_char(input[i]);
        printf("State %d (char: '%c')\n", current_state, input[i]);

        if (current_state == STATE_ERROR) {
            printf("\nState machine entered ERROR state!\n");
            return 1;
        }
    }

    if (current_state == STATE_SUCCESS) {
        printf("\nState machine reached SUCCESS!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    printf("\nState machine did not reach success state.\n");
    return 1;
}
