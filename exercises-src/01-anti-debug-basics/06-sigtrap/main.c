// Exercise: Exception Trap (Anti-Debug Basics)
// Uses SIGTRAP to detect if a debugger catches the signal
// Goal: Handle the trap correctly to get the flag

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

// FLAG{SIGTRAP_HANDLED_f6a8b2d4} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x04, 0x1e, 0x10,
    0x03, 0x05, 0x16, 0x07, 0x08, 0x1f, 0x16, 0x19,
    0x13, 0x1b, 0x12, 0x13, 0x08, 0x31, 0x61, 0x36,
    0x6f, 0x35, 0x65, 0x33, 0x63, 0x2a
};
static int flag_len = 30;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

static jmp_buf jump_buffer;
static volatile int trap_caught = 0;

void sigtrap_handler(int sig) {
    trap_caught = 1;
    longjmp(jump_buffer, 1);
}

int main(int argc, char **argv) {
    printf("=== SIGTRAP Detection ===\n\n");

    // Set up signal handler
    signal(SIGTRAP, sigtrap_handler);

    printf("Raising SIGTRAP...\n");

    if (setjmp(jump_buffer) == 0) {
        // First call - raise the trap
        raise(SIGTRAP);

        // If we get here, debugger caught the signal
        printf("The SIGTRAP was intercepted by something...\n");
        printf("Hint: Debuggers often catch SIGTRAP\n");
        return 1;
    }

    // We jumped here from the handler
    if (!trap_caught) {
        printf("Something went wrong.\n");
        return 1;
    }

    printf("SIGTRAP handled correctly!\n");

    char flag[64];
    decode_flag(flag);
    printf("Flag: %s\n", flag);

    return 0;
}
