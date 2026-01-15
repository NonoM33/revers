// Exercise: Parent Process Check (Anti-Debug Basics)
// Checks if parent process is a known debugger
// Goal: Make the program think it has a normal parent

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libproc.h>

// FLAG{PPID_SPOOFED_c3d5a7b9} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x07, 0x07, 0x1e,
    0x13, 0x08, 0x04, 0x07, 0x18, 0x18, 0x11, 0x12,
    0x13, 0x08, 0x34, 0x64, 0x33, 0x62, 0x36, 0x60,
    0x35, 0x6e, 0x2a
};
static int flag_len = 27;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

int is_parent_debugger() {
    pid_t ppid = getppid();
    char pathbuf[PROC_PIDPATHINFO_MAXSIZE];

    if (proc_pidpath(ppid, pathbuf, sizeof(pathbuf)) > 0) {
        // Check for common debuggers
        if (strstr(pathbuf, "lldb") != NULL ||
            strstr(pathbuf, "gdb") != NULL ||
            strstr(pathbuf, "debugserver") != NULL) {
            return 1;
        }
    }

    return 0;
}

int main(int argc, char **argv) {
    printf("=== Parent Process Check ===\n\n");

    if (is_parent_debugger()) {
        printf("Your parent process looks suspicious!\n");
        printf("Hint: What is your parent's name?\n");
        return 1;
    }

    printf("Parent process looks clean.\n");

    char flag[64];
    decode_flag(flag);
    printf("Flag: %s\n", flag);

    return 0;
}
