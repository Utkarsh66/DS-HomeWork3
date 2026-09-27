#!/bin/bash

set -e

INPUT_FILE=${1:-A.txt}
B_FILE=${2:-B.txt}
OUTPUT_FILE=${3:-output.txt}

echo "Compiling programs..."

g++ -O2 -std=c++17 mapper.cpp -o mapper
g++ -O2 -std=c++17 combiner.cpp -o combiner
g++ -O2 -std=c++17 reducer.cpp -o reducer

echo "Compilation completed."

echo "Running MapReduce locally..."

./mapper "$B_FILE" < "$INPUT_FILE" | \
    sort -n -k1,1 | \
    ./combiner | \
    sort -n -k1,1 | \
    ./reducer > "$OUTPUT_FILE"

echo "MapReduce completed."
echo "Output saved to $OUTPUT_FILE"

echo ""
echo "Output:"
cat "$OUTPUT_FILE"