#!/usr/bin/env python3
"""Fix all XOR encodings in exercise source files"""

import os
import re

BASE_DIR = "/Users/renaud.cosson-ext/hack_binary/cours/reverse-academy/exercises-src"

def encode_flag(flag, key=0x57):
    encoded = [c ^ key for c in flag.encode()]
    # Format as C array with 8 bytes per line
    lines = []
    for i in range(0, len(encoded), 8):
        chunk = encoded[i:i+8]
        line = ', '.join(f'0x{b:02x}' for b in chunk)
        lines.append(line)
    hex_str = ',\n    '.join(lines)
    return len(flag), hex_str

# All correct flags
FLAGS = {
    "01-anti-debug-basics/02-sysctl": "FLAG{SYSCTL_DEFEATED_b8e4f2a1}",
    "01-anti-debug-basics/03-getppid": "FLAG{PPID_SPOOFED_c3d5a7b9}",
    "01-anti-debug-basics/04-isatty": "FLAG{TTY_TRICKED_d4e6f8a2}",
    "01-anti-debug-basics/05-multi": "FLAG{MULTI_BYPASS_e5f7a9c3}",
    "01-anti-debug-basics/06-sigtrap": "FLAG{SIGTRAP_HANDLED_f6a8b2d4}",
    "02-distributed-checks/01-three-steps": "FLAG{THREE_STEPS_OK_a1b2c3d4}",
    "02-distributed-checks/02-inverted": "FLAG{LOGIC_INVERTED_b2c3d4e5}",
    "02-distributed-checks/03-global": "FLAG{STATE_MACHINE_c3d4e5f6}",
    "02-distributed-checks/04-callbacks": "FLAG{CALLBACK_CHAIN_d4e5f6a7}",
    "02-distributed-checks/05-herring": "FLAG{REAL_NOT_FAKE_e5f6a7b8}",
    "02-distributed-checks/06-order": "FLAG{ORDER_MATTERS_f6a7b8c9}",
    "02-distributed-checks/07-recursive": "FLAG{RECURSIVE_WIN_a7b8c9d0}",
    "03-hash-comparisons/01-simple": "FLAG{HASH_CRACKED_a1b2c3d4}",
    "03-hash-comparisons/02-xor": "FLAG{XOR_MASTER_b2c3d4e5}",
    "03-hash-comparisons/03-lookup": "FLAG{TABLE_LOOKUP_c3d4e5f6}",
    "03-hash-comparisons/04-crc32": "FLAG{CRC32_BROKEN_d4e5f6a7}",
    "03-hash-comparisons/05-md5prefix": "FLAG{MD5_PREFIX_e5f6a7b8}",
    "03-hash-comparisons/06-multi": "FLAG{MULTI_HASH_f6a7b8c9}",
    "03-hash-comparisons/07-timing": "FLAG{TIMING_ATTACK_a7b8c9d0}",
    "04-light-obfuscation/01-xorstring": "FLAG{XOR_DECODED_b8c9d0e1}",
    "04-light-obfuscation/02-caesar": "FLAG{CAESAR_SHIFT_c9d0e1f2}",
    "04-light-obfuscation/03-base64": "FLAG{BASE64_LAYERS_d0e1f2a3}",
    "04-light-obfuscation/04-substitution": "FLAG{SUBSTITUTION_e1f2a3b4}",
    "04-light-obfuscation/05-stackstrings": "FLAG{STACK_STRING_f2a3b4c5}",
    "04-light-obfuscation/06-arithmetic": "FLAG{MATH_ENCODED_a3b4c5d6}",
    "04-light-obfuscation/07-mixed": "FLAG{MIXED_MASTER_b4c5d6e7}",
}

def fix_file(path, flag):
    """Fix the encoded_flag array in a source file"""
    src_file = os.path.join(BASE_DIR, path, "main.c")
    if not os.path.exists(src_file):
        print(f"  File not found: {src_file}")
        return False

    with open(src_file, 'r') as f:
        content = f.read()

    flag_len, hex_bytes = encode_flag(flag)

    # Pattern to match the encoded_flag array and flag_len
    pattern = r'static unsigned char encoded_flag\[\] = \{[^}]+\};\s*static int flag_len = \d+;'
    replacement = f'static unsigned char encoded_flag[] = {{\n    {hex_bytes}\n}};\nstatic int flag_len = {flag_len};'

    new_content = re.sub(pattern, replacement, content)

    if new_content != content:
        with open(src_file, 'w') as f:
            f.write(new_content)
        return True
    return False

# Fix all files
print("Fixing XOR encodings...")
for path, flag in FLAGS.items():
    print(f"  {path}...", end=" ")
    if fix_file(path, flag):
        print("FIXED")
    else:
        print("unchanged")

print("\nDone!")
