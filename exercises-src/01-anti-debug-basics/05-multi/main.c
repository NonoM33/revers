// Exercise: Multi-Check (Anti-Debug Basics)
// Combines multiple anti-debug techniques
// Goal: Bypass ALL checks to get the flag

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/ptrace.h>

// FLAG{MULTI_BYPASS_e5f7a9c3} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x1a, 0x02, 0x1b,
    0x03, 0x1e, 0x08, 0x15, 0x0e, 0x07, 0x16, 0x04,
    0x04, 0x08, 0x32, 0x62, 0x31, 0x60, 0x36, 0x6e,
    0x34, 0x64, 0x2a
};
static int flag_len = 27;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

volatile int check_count = 0;

int check_ptrace() {
    if (ptrace(PT_DENY_ATTACH, 0, 0, 0) == -1) {
        return 1;
    }
    check_count++;
    return 0;
}

int check_sysctl() {
    int mib[4];
    struct kinfo_proc info;
    size_t size = sizeof(info);

    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PID;
    mib[3] = getpid();

    info.kp_proc.p_flag = 0;
    sysctl(mib, 4, &info, &size, NULL, 0);

    if ((info.kp_proc.p_flag & P_TRACED) != 0) {
        return 1;
    }
    check_count++;
    return 0;
}

int check_timing() {
    // Simple timing check - debuggers slow things down
    volatile int sum = 0;
    for (int i = 0; i < 100000; i++) {
        sum += i;
    }
    check_count++;
    return 0;  // Simplified for this exercise
}

int main(int argc, char **argv) {
    printf("=== Multi Anti-Debug Check ===\n\n");

    printf("Running check 1/3 (ptrace)...\n");
    if (check_ptrace()) {
        printf("Check 1 FAILED: ptrace detected debugger\n");
        return 1;
    }
    printf("Check 1 PASSED\n\n");

    printf("Running check 2/3 (sysctl)...\n");
    if (check_sysctl()) {
        printf("Check 2 FAILED: sysctl detected debugger\n");
        return 1;
    }
    printf("Check 2 PASSED\n\n");

    printf("Running check 3/3 (timing)...\n");
    if (check_timing()) {
        printf("Check 3 FAILED: timing anomaly\n");
        return 1;
    }
    printf("Check 3 PASSED\n\n");

    if (check_count != 3) {
        printf("Something is wrong with the checks...\n");
        return 1;
    }

    printf("All checks passed!\n");

    char flag[64];
    decode_flag(flag);
    printf("Flag: %s\n", flag);

    return 0;
}
