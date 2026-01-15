#!/usr/bin/env python3
"""Helper script to generate XOR-encoded flags for exercises"""

import sys
import hashlib

def encode_flag(flag: str, key: int = 0x57) -> tuple:
    """Encode a flag with XOR and return C array"""
    encoded = [c ^ key for c in flag.encode()]
    hex_bytes = ', '.join(f'0x{b:02x}' for b in encoded)
    return len(flag), hex_bytes

def generate_flag_hash(flag: str) -> str:
    """Generate SHA256 hash for database"""
    return hashlib.sha256(flag.encode()).hexdigest()

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 encode_flag.py 'FLAG{...}'")
        sys.exit(1)

    flag = sys.argv[1]
    length, hex_bytes = encode_flag(flag)
    hash_val = generate_flag_hash(flag)

    print(f"Flag: {flag}")
    print(f"Length: {length}")
    print(f"SHA256: {hash_val}")
    print(f"\nC code:")
    print(f"static unsigned char encoded_flag[] = {{ {hex_bytes} }};")
    print(f"static int flag_len = {length};")
