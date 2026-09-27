#!/bin/bash

#SBATCH --job-name=matrix_mapreduce
#SBATCH --output=matrix_mapreduce_%j.out
#SBATCH --error=matrix_mapreduce_%j.err

#SBATCH --nodes=4
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --time=00:15:00


# ============================================================
# Distributed Matrix Multiplication using MapReduce
# Row-Row Method
# ============================================================


# ------------------------------------------------------------
# Move to directory from which job was submitted
# ------------------------------------------------------------

if [ -n "$SLURM_SUBMIT_DIR" ]; then
    SCRIPT_DIR="$SLURM_SUBMIT_DIR"
else
    SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
fi

cd "$SCRIPT_DIR"


# ------------------------------------------------------------
# Input / Output files
# ------------------------------------------------------------

INPUT_FILE="$SCRIPT_DIR/A.txt"
B_FILE="$SCRIPT_DIR/B.txt"

OUTPUT_FILE="$SCRIPT_DIR/output.txt"


# ------------------------------------------------------------
# Check input files
# ------------------------------------------------------------

if [ ! -f "$INPUT_FILE" ]; then
    echo "ERROR: A.txt not found."
    exit 1
fi

if [ ! -f "$B_FILE" ]; then
    echo "ERROR: B.txt not found."
    exit 1
fi


echo "============================================"
echo "Distributed Matrix Multiplication"
echo "============================================"

echo "Input A: $INPUT_FILE"
echo "Input B: $B_FILE"
echo "Output:  $OUTPUT_FILE"
echo "Nodes:   $SLURM_JOB_NODELIST"
echo "Tasks:   $SLURM_NTASKS"
echo "============================================"


# ------------------------------------------------------------
# Compile programs
# ------------------------------------------------------------

echo ""
echo "Compiling programs..."

g++ -O2 -std=c++17 mapper.cpp -o mapper
g++ -O2 -std=c++17 combiner.cpp -o combiner
g++ -O2 -std=c++17 reducer.cpp -o reducer

echo "Compilation completed."


# ------------------------------------------------------------
# Determine number of mapper tasks
# ------------------------------------------------------------

if [ -z "$SLURM_NTASKS" ]; then
    SLURM_NTASKS=4
fi


# ------------------------------------------------------------
# Replicate B to every allocated node
# ------------------------------------------------------------

echo ""
echo "Broadcasting B.txt to all Mapper nodes..."

sbcast "$B_FILE" /tmp/B.txt

if [ $? -ne 0 ]; then
    echo "ERROR: Failed to broadcast B.txt"
    exit 1
fi

echo "B.txt successfully broadcast."


# ------------------------------------------------------------
# Split A among mapper tasks
# ------------------------------------------------------------

echo ""
echo "Splitting A.txt across $SLURM_NTASKS mapper tasks..."

rm -f chunk_*

split -d -a 2 -n l/$SLURM_NTASKS "$INPUT_FILE" chunk_


# ------------------------------------------------------------
# MAP STAGE
# ------------------------------------------------------------

echo ""
echo "Running Mapper..."

srun --ntasks=$SLURM_NTASKS bash -c '
    TID=$(printf "%02d" $SLURM_PROCID)

    echo "Mapper $SLURM_PROCID running on $(hostname)" >&2

    ./mapper /tmp/B.txt \
        < "chunk_${TID}" \
        > "map_${TID}.out"
'


if [ $? -ne 0 ]; then
    echo "ERROR: Mapper stage failed."
    exit 1
fi

echo "Mapper stage completed."


# ------------------------------------------------------------
# SHUFFLE 1
# ------------------------------------------------------------

echo ""
echo "Running first shuffle/sort..."

sort -n -k1,1 map_*.out > shuffle1.out

echo "First shuffle completed."


# ------------------------------------------------------------
# COMBINER
# ------------------------------------------------------------

echo ""
echo "Running Combiner..."

./combiner < shuffle1.out > combined.out

if [ $? -ne 0 ]; then
    echo "ERROR: Combiner stage failed."
    exit 1
fi

echo "Combiner stage completed."


# ------------------------------------------------------------
# SHUFFLE 2
# ------------------------------------------------------------

echo ""
echo "Running second shuffle/sort..."

sort -n -k1,1 combined.out > shuffle2.out

echo "Second shuffle completed."


# ------------------------------------------------------------
# REDUCER
# ------------------------------------------------------------

echo ""
echo "Running Reducer..."

./reducer < shuffle2.out > "$OUTPUT_FILE"

if [ $? -ne 0 ]; then
    echo "ERROR: Reducer stage failed."
    exit 1
fi

echo "Reducer stage completed."


# ------------------------------------------------------------
# Display result
# ------------------------------------------------------------

echo ""
echo "============================================"
echo "MapReduce completed successfully!"
echo "============================================"

echo ""
echo "Final output:"
cat "$OUTPUT_FILE"


# ------------------------------------------------------------
# Cleanup temporary files
# ------------------------------------------------------------

rm -f chunk_*
rm -f map_*.out
rm -f shuffle1.out
rm -f combined.out
rm -f shuffle2.out

echo ""
echo "Temporary files cleaned up."