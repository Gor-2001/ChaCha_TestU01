#!/bin/bash
set -e

if [ $# -ne 2 ]; then
    echo "Usage: $0 <rng_name> <suite>"
    exit 1
fi

RNG="$1"
SUITE="$2"

mkdir -p results

echo "Running: $RNG / $SUITE"

./build/rng_bench "$RNG" "$SUITE" > "results/${RNG}_${SUITE}.csv"

echo "Done: results/${RNG}_${SUITE}.csv"


