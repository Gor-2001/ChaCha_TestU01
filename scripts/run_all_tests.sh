#!/bin/bash
set -e

RNGS=("pcg" "lcg" "chacha20" "chacha12" "chacha8" "chacha4" "chacha4_feedback")
SUITES=("smallcrush" "crush" "bigcrush")

mkdir -p results

for rng in "${RNGS[@]}"; do
    for suite in "${SUITES[@]}"; do
        echo "===== Running $rng / $suite ====="
        ./build/rng_bench "$rng" "$suite" > "results/${rng}_${suite}.csv"
    done
done
