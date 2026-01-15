// Exercise: Base64 Layers (Light Obfuscation)
// Password encoded multiple times with Base64
// Goal: Decode the layers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FLAG{BASE64_LAYERS_d0e1f2a3} XOR 0x57
static unsigned char encoded_flag[] = {
    0x11, 0x1b, 0x16, 0x10, 0x2c, 0x15, 0x16, 0x04,
    0x12, 0x61, 0x63, 0x08, 0x1b, 0x16, 0x0e, 0x12,
    0x05, 0x04, 0x08, 0x33, 0x67, 0x32, 0x66, 0x31,
    0x65, 0x36, 0x64, 0x2a
};
static int flag_len = 28;

void decode_flag(char *out) {
    for (int i = 0; i < flag_len; i++) {
        out[i] = encoded_flag[i] ^ 0x57;
    }
    out[flag_len] = '\0';
}

static const char b64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int b64_decode_char(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

int base64_decode(const char *in, char *out) {
    int len = (int)strlen(in);
    int out_len = 0;

    for (int i = 0; i < len; i += 4) {
        int n = b64_decode_char(in[i]) << 18;
        n |= b64_decode_char(in[i+1]) << 12;
        n |= (in[i+2] != '=' ? b64_decode_char(in[i+2]) << 6 : 0);
        n |= (in[i+3] != '=' ? b64_decode_char(in[i+3]) : 0);

        out[out_len++] = (n >> 16) & 0xFF;
        if (in[i+2] != '=') out[out_len++] = (n >> 8) & 0xFF;
        if (in[i+3] != '=') out[out_len++] = n & 0xFF;
    }
    out[out_len] = '\0';
    return out_len;
}

// "KEY" -> base64 -> "S0VZ" -> base64 -> "UzBWWg=="
static const char double_encoded[] = "UzBWWg==";

int main(int argc, char **argv) {
    printf("=== Base64 Layers ===\n\n");

    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        printf("Hint: The password was encoded twice\n");
        printf("Encoded value: %s\n", double_encoded);
        return 1;
    }

    // Decode user input twice and compare
    char temp1[64], temp2[64];

    // First decode
    base64_decode(double_encoded, temp1);
    // Second decode
    base64_decode(temp1, temp2);

    if (strcmp(argv[1], temp2) == 0) {
        printf("Password correct!\n");
        char flag[64];
        decode_flag(flag);
        printf("Flag: %s\n", flag);
        return 0;
    }

    printf("Wrong password.\n");
    printf("Hint: Decode '%s' twice\n", double_encoded);
    return 1;
}
