// Exercise: CRC32 Check (Hash & Comparisons)
// Uses CRC32 for password validation
// Goal: Find input with matching CRC32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// FLAG{CRC32_BROKEN_d4e5f6a7} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x14, 0x05, 0x14,
    0x64, 0x65, 0x08, 0x15, 0x05, 0x18, 0x1c, 0x12,
    0x19, 0x08, 0x33, 0x63, 0x32, 0x62, 0x31, 0x61,
    0x36, 0x60, 0x2a
};
static int flag_len = 27;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

// CRC32 lookup table
static uint32_t crc_table[256];
static int table_computed = 0;

void make_crc_table() {
    for (int n = 0; n < 256; n++) {
        uint32_t c = (uint32_t)n;
        for (int k = 0; k < 8; k++) {
            if (c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }
        crc_table[n] = c;
    }
    table_computed = 1;
}

uint32_t crc32(const unsigned char *buf, size_t len) {
    if (!table_computed) make_crc_table();

    uint32_t c = 0xffffffffL;
    for (size_t n = 0; n < len; n++) {
        c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
    }
    return c ^ 0xffffffffL;
}

int main(int argc, char **argv) {
    printf("=== CRC32 Check ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: CRC32 of 'CRC!' is the target\n");
        return 1;
    }

    const char *input = argv[1];

    // Target CRC32 (for "CRC!")
    uint32_t target = 0x4eb9a3bc;

    uint32_t hash = crc32((const unsigned char *)input, strlen(input));

    printf("Your CRC32:   0x%08x\n", hash);
    printf("Target CRC32: 0x%08x\n", target);

    if (hash == target) {
        printf("\nCRC32 match!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    printf("\nCRC32 mismatch.\n");
    return 1;
}
