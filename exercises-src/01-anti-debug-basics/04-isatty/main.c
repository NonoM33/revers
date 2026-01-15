// Exercise: Terminal Detection (Anti-Debug Basics)
// Checks if stdin/stdout are connected to a real terminal
// Goal: Fool the terminal detection

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// FLAG{TTY_TRICKED_d4e6f8a2} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x03, 0x03, 0x0e,
    0x08, 0x03, 0x05, 0x1e, 0x14, 0x1c, 0x12, 0x13,
    0x08, 0x33, 0x63, 0x32, 0x61, 0x31, 0x6f, 0x36,
    0x65, 0x2a
};
static int flag_len = 26;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

int check_terminal() {
    // In a debugger, stdin might not be a real tty
    if (!isatty(STDIN_FILENO)) {
        return 1;  // Suspicious
    }
    if (!isatty(STDOUT_FILENO)) {
        return 1;  // Suspicious
    }
    return 0;
}

int main(int argc, char **argv) {
    printf("=== Terminal Detection ===\n\n");

    if (check_terminal()) {
        printf("This doesn't look like a real terminal!\n");
        printf("Hint: isatty() checks the file descriptor...\n");
        return 1;
    }

    printf("Running in a proper terminal.\n");

    char flag[64];
    decode_flag(flag);
    printf("Flag: %s\n", flag);

    return 0;
}
