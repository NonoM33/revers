// Exercise: Substitution (Light Obfuscation)
// Full alphabet substitution cipher
// Goal: Reverse the substitution

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FLAG{SUBSTITUTION_e1f2a3b4} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x04, 0x02, 0x15,
    0x04, 0x03, 0x1e, 0x03, 0x02, 0x03, 0x1e, 0x18,
    0x19, 0x08, 0x32, 0x66, 0x31, 0x65, 0x36, 0x64,
    0x35, 0x63, 0x2a
};
static int flag_len = 27;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

// Substitution: ABCDEFGHIJKLMNOPQRSTUVWXYZ -> QWERTYUIOPASDFGHJKLZXCVBNM
static const char sub_from[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char sub_to[]   = "QWERTYUIOPASDFGHJKLZXCVBNM";

// Encrypted "CODE" = "EGRY"
static const char encrypted[] = "EGRY";

char substitute(char c, const char *from, const char *to) {
    for (int i = 0; i < 26; i++) {
        if (c == from[i]) return to[i];
        if (c == (from[i] + 32)) return to[i] + 32;  // lowercase
    }
    return c;
}

int check_password(const char *input) {
    if (strlen(input) != strlen(encrypted)) return 0;

    for (size_t i = 0; i < strlen(input); i++) {
        char c = input[i];
        if (c >= 'a' && c <= 'z') c -= 32;  // to upper

        char subst = substitute(c, sub_from, sub_to);
        if (subst != encrypted[i]) return 0;
    }
    return 1;
}

int main(int argc, char **argv) {
    printf("=== Substitution Cipher ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: Each letter maps to a different letter\n");
        printf("Encrypted: %s\n", encrypted);
        return 1;
    }

    if (check_password(argv[1])) {
        printf("Password correct!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    printf("Wrong password.\n");
    return 1;
}
