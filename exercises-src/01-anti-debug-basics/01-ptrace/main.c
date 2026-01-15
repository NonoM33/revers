// Exercise: Detect ptrace (Anti-Debug Basics)
// The program uses ptrace(PT_DENY_ATTACH) to prevent debugging.
// Your goal: Bypass the protection and find the hidden flag.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ptrace.h>

#ifndef FLAG
#define FLAG "FLAG{PTRACE_BYPASSED_a7f3b2c1}"
#endif

// Anti-debug check using ptrace
int is_debugger_present() {
    // PT_DENY_ATTACH prevents a debugger from attaching
    // If we're already being debugged, this call fails
    if (ptrace(PT_DENY_ATTACH, 0, 0, 0) == -1) {
        return 1;  // Debugger detected
    }
    return 0;
}

int check_password(const char *pass) {
    // Simple password check (not the main challenge)
    return strcmp(pass, "reverse2024") == 0;
}

int main(int argc, char **argv) {
    printf("=== License Check v1.0 ===\n\n");

    // Anti-debug protection
    if (is_debugger_present()) {
        printf("Nice try, but I don't like debuggers!\n");
        printf("Hint: Look at the ptrace call...\n");
        return 1;
    }

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        return 1;
    }

    printf("Checking password...\n");

    if (check_password(argv[1])) {
        printf("\nAccess granted!\n");
        printf("Here's your flag: %s\n", FLAG);
        return 0;
    }

    printf("Access denied.\n");
    return 1;
}
