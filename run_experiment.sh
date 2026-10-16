#!/bin/bash
set -e

N=10000000
RUNS=10

# Compile the C++ program
g++ -g -Wall -pthread -O2 mt-collatz.cpp -o mt-collatz.exe

# Write the CSV header
echo "problem_count,threads,seconds" > timings_raw.csv

# Test thread counts from 1 to 8, 10 times each
for threads in {1..8}; do
    echo "Testing $threads thread(s)..."

    for run in $(seq 1 "$RUNS"); do
        ./mt-collatz.exe "$N" "$threads" > /dev/null 2>> timings_raw.csv
        echo "  Run $run of $RUNS complete"
    done
done

echo "Experiment complete. Results saved in timings_raw.csv"