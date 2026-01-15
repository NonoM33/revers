// Exercise: Lookup Table (Hash & Comparisons)
// Uses a substitution table for validation
// Goal: Reverse the lookup table

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FLAG{TABLE_LOOKUP_c3d4e5f6} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x03, 0x16, 0x15,
    0x1b, 0x12, 0x08, 0x1b, 0x18, 0x18, 0x1c, 0x02,
    0x07, 0x08, 0x34, 0x64, 0x33, 0x63, 0x32, 0x62,
    0x31, 0x61, 0x2a
};
static int flag_len = 27;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

// Lookup table for substitution
static unsigned char lookup[256] = {
    ['A'] = 'Q', ['B'] = 'W', ['C'] = 'E', ['D'] = 'R',
    ['E'] = 'T', ['F'] = 'Y', ['G'] = 'U', ['H'] = 'I',
    ['I'] = 'O', ['J'] = 'P', ['K'] = 'A', ['L'] = 'S',
    ['M'] = 'D', ['N'] = 'F', ['O'] = 'G', ['P'] = 'H',
    ['Q'] = 'J', ['R'] = 'K', ['S'] = 'L', ['T'] = 'Z',
    ['U'] = 'X', ['V'] = 'C', ['W'] = 'V', ['X'] = 'B',
    ['Y'] = 'N', ['Z'] = 'M',
    ['a'] = 'q', ['b'] = 'w', ['c'] = 'e', ['d'] = 'r',
    ['e'] = 't', ['f'] = 'y', ['g'] = 'u', ['h'] = 'i',
    ['i'] = 'o', ['j'] = 'p', ['k'] = 'a', ['l'] = 's',
    ['m'] = 'd', ['n'] = 'f', ['o'] = 'g', ['p'] = 'h',
    ['q'] = 'j', ['r'] = 'k', ['s'] = 'l', ['t'] = 'z',
    ['u'] = 'x', ['v'] = 'c', ['w'] = 'v', ['x'] = 'b',
    ['y'] = 'n', ['z'] = 'm'
};

// Expected output after lookup
static const char expected[] = "STRKT";

int check_password(const char *input) {
    if (strlen(input) != strlen(expected)) {
        return 0;
    }

    for (size_t i = 0; i < strlen(input); i++) {
        unsigned char c = (unsigned char)input[i];
        if (lookup[c] != expected[i]) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv) {
    printf("=== Lookup Table ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: The table transforms your input\n");
        return 1;
    }

    const char *input = argv[1];

    printf("Applying lookup table...\n");
    printf("Transformed: ");
    for (size_t i = 0; i < strlen(input); i++) {
        unsigned char c = lookup[(unsigned char)input[i]];
        printf("%c", c ? c : '?');
    }
    printf("\n");

    if (check_password(input)) {
        printf("\nPassword matches!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    printf("\nPassword incorrect.\n");
    return 1;
}
