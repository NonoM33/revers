// Exercise: MD5 Prefix (Hash & Comparisons)
// Find input whose MD5 starts with specific bytes
// Goal: Partial hash collision

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CommonCrypto/CommonDigest.h>

// FLAG{MD5_PREFIX_e5f6a7b8} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x1a, 0x13, 0x62,
    0x08, 0x07, 0x05, 0x12, 0x11, 0x1e, 0x0f, 0x08,
    0x32, 0x62, 0x31, 0x61, 0x36, 0x60, 0x35, 0x6f,
    0x2a
};
static int flag_len = 25;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

void md5_hash(const char *input, unsigned char *output) {
    CC_MD5(input, (CC_LONG)strlen(input), output);
}

int main(int argc, char **argv) {
    printf("=== MD5 Prefix Match ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <input>\n", argv[0]);
        printf("Hint: Find input whose MD5 starts with 'dead'\n");
        return 1;
    }

    const char *input = argv[1];
    unsigned char hash[CC_MD5_DIGEST_LENGTH];

    md5_hash(input, hash);

    printf("MD5 of '%s': ", input);
    for (int i = 0; i < CC_MD5_DIGEST_LENGTH; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");

    // Check if MD5 starts with "dead" (0xde, 0xad)
    if (hash[0] == 0xde && hash[1] == 0xad) {
        printf("\nMD5 prefix matches 'dead'!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    printf("\nMD5 doesn't start with 'dead'.\n");
    printf("Hint: Try 'codez' or similar short strings\n");
    return 1;
}
