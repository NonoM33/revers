// Exercise: Timing Hash (Hash & Comparisons)
// Password check with timing vulnerability
// Goal: Exploit timing to extract password

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// FLAG{TIMING_ATTACK_a7b8c9d0} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x03, 0x1e, 0x1a,
    0x1e, 0x19, 0x10, 0x08, 0x16, 0x03, 0x03, 0x16,
    0x14, 0x1c, 0x08, 0x36, 0x60, 0x35, 0x6f, 0x34,
    0x6e, 0x33, 0x67, 0x2a
};
static int flag_len = 28;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

static const char secret[] = "TIME";

// Vulnerable comparison - delays on each correct character
int insecure_compare(const char *input) {
    if (strlen(input) != strlen(secret)) {
        return 0;
    }

    for (size_t i = 0; i < strlen(secret); i++) {
        // Delay before checking each character (timing leak!)
        usleep(100000);  // 100ms delay

        if (input[i] != secret[i]) {
            return 0;  // Early exit on mismatch
        }
    }

    return 1;
}

int main(int argc, char **argv) {
    printf("=== Timing Attack Challenge ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: The password check has a timing vulnerability\n");
        printf("Hint: Correct characters take longer to reject\n");
        return 1;
    }

    const char *input = argv[1];

    printf("Checking password");
    fflush(stdout);

    if (insecure_compare(input)) {
        printf("\n\nPassword correct!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    printf("\n\nPassword incorrect.\n");
    return 1;
}
