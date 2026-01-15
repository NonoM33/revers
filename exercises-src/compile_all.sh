#!/bin/bash
# Compile all exercise binaries for ARM64 and x86_64

set -e

SRC_DIR="/Users/renaud.cosson-ext/hack_binary/cours/reverse-academy/exercises-src"
OUT_DIR="/Users/renaud.cosson-ext/hack_binary/cours/reverse-academy/static/binaries"

compile_exercise() {
    local module=$1
    local exercise=$2
    local src="$SRC_DIR/$module/$exercise/main.c"
    local out_base="$OUT_DIR/$module/$exercise/challenge"

    if [ -f "$src" ]; then
        echo "Compiling $module/$exercise..."
        clang -arch arm64 -o "${out_base}_arm64" "$src" 2>/dev/null || echo "  ARM64 failed"
        clang -arch x86_64 -o "${out_base}_x86_64" "$src" 2>/dev/null || echo "  x86_64 failed"
    fi
}

# Module 1: Anti-Debug Basics
for ex in 01-ptrace 02-sysctl 03-getppid 04-isatty 05-multi 06-sigtrap; do
    compile_exercise "01-anti-debug-basics" "$ex"
done

# Module 2: Distributed Checks
for ex in 01-three-steps 02-inverted 03-global 04-callbacks 05-herring 06-order 07-recursive; do
    compile_exercise "02-distributed-checks" "$ex"
done

# Module 3: Hash & Comparisons
for ex in 01-simple 02-xor 03-lookup 04-crc32 05-md5prefix 06-multi 07-timing; do
    compile_exercise "03-hash-comparisons" "$ex"
done

# Module 4: Light Obfuscation
for ex in 01-xorstring 02-caesar 03-base64 04-substitution 05-stackstrings 06-arithmetic 07-mixed; do
    compile_exercise "04-light-obfuscation" "$ex"
done

echo ""
echo "Done! Listing all compiled binaries:"
find "$OUT_DIR" -type f -name "challenge_*" | wc -l
echo "binaries compiled"
