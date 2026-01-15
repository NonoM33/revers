// Exercise: Sysctl Check (Anti-Debug Basics)
// Uses sysctl() to query process info and detect debugging
// Goal: Bypass the sysctl-based debugger detection

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <unistd.h>

// FLAG{SYSCTL_DEFEATED_b8e4f2a1} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x04, 0x0e, 0x04,
    0x14, 0x03, 0x1b, 0x08, 0x13, 0x12, 0x11, 0x12,
    0x16, 0x03, 0x12, 0x13, 0x08, 0x35, 0x6f, 0x32,
    0x63, 0x31, 0x65, 0x36, 0x66, 0x2a
};
static int flag_len = 30;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

int is_debugger_attached() {
    int mib[4];
    struct kinfo_proc info;
    size_t size = sizeof(info);

    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PID;
    mib[3] = getpid();

    info.kp_proc.p_flag = 0;

    if (sysctl(mib, 4, &info, &size, NULL, 0) == -1) {
        return 0;
    }

    // P_TRACED flag indicates debugging
    return (info.kp_proc.p_flag & P_TRACED) != 0;
}

int main(int argc, char **argv) {
    printf("=== Sysctl Debugger Check ===\n\n");

    if (is_debugger_attached()) {
        printf("Debugger detected via sysctl!\n");
        printf("Hint: Check the P_TRACED flag...\n");
        return 1;
    }

    printf("No debugger detected.\n");

    char flag[64];
    decode_flag(flag);
    printf("Flag: %s\n", flag);

    return 0;
}
