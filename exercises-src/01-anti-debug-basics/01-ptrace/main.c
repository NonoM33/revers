// Exercise: Detect ptrace (Anti-Debug Basics)
// The program uses ptrace(PT_DENY_ATTACH) to prevent debugging.
// Your goal: Bypass the ptrace protection to get the flag.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ptrace.h>

// FLAG{PTRACE_BYPASSED_a7f3b2c1} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x07, 0x03, 0x05,
    0x16, 0x14, 0x12, 0x08, 0x15, 0x0e, 0x07, 0x16,
    0x04, 0x04, 0x12, 0x13, 0x08, 0x36, 0x60, 0x31,
    0x64, 0x35, 0x65, 0x34, 0x66, 0x2a
};
static int flag_len = 30;

void decode_flag(char *out) {
    unsigned char key = 0x57;
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ key;
    }
    out[flag_len] = '\0';
}

// Anti-debug check using ptrace
int is_debugger_present() {
    // PT_DENY_ATTACH prevents a debugger from attaching
    // If we're already being debugged, this call fails
    if (ptrace(PT_DENY_ATTACH, 0, 0, 0) == -1) {
        return 1;  // Debugger detected
    }
    return 0;
}

int main(int argc, char **argv) {
    printf("=== Anti-Debug Challenge v1.0 ===\n\n");

    // Anti-debug protection
    if (is_debugger_present()) {
        printf("Nice try, but I don't like debuggers!\n");
        printf("Hint: Look at the ptrace call...\n");
        return 1;
    }

    // If we get here, ptrace was bypassed!
    printf("No debugger detected. Here's your reward:\n\n");

    char flag[64];
    decode_flag(flag);
    printf("Flag: %s\n", flag);

    return 0;
}
