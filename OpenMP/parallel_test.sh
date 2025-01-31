#!/bin/bash

DATASET_PATH="../dataset.csv"
PROGRAM="./KMeans_parallel"

# Verify that the program exists
if [ ! -f "$PROGRAM" ]; then
    echo "Error: the program $PROGRAM does not exist. Make sure KMeans_parallel has been compiled."
    exit 1
fi

# Array of subset sizes to test
SUBSET_SIZES=(1000 10000 100000 1000000 10000000)

# Array of number of cores to test
CORES=(2 4 8 16 24)

# Number of clusters and iterations for the KMeans algorithm
CLUSTERS=6
ITERATIONS=500

# Log file to save the results
LOG_FILE="results_parallel.log"

# Start the log
echo "=== KMeans Parallel Test Results ===" > "$LOG_FILE"
echo "Dataset: $DATASET_PATH" >> "$LOG_FILE"
echo "Number of clusters: $CLUSTERS, Maximum iterations: $ITERATIONS" >> "$LOG_FILE"
echo "----------------------------------------" >> "$LOG_FILE"

# Loop through each subset size
for SIZE in "${SUBSET_SIZES[@]}"
do
    echo "Running test on $SIZE points... ($(date))" | tee -a "$LOG_FILE"
    
    # Loop through each core configuration
    for CORE in "${CORES[@]}"
    do
        echo "Testing with $CORE cores... ($(date))" | tee -a "$LOG_FILE"
        
        export OMP_NUM_THREADS=$CORE  # Set the number of cores for OpenMP
        
        start_time=$(date +%s.%N)
        
        
        $PROGRAM "$DATASET_PATH" $CLUSTERS $ITERATIONS $SIZE >> "$LOG_FILE" 2>&1
        
        end_time=$(date +%s.%N)
        
        # time in seconds and decimals
        execution_time=$(echo "$end_time - $start_time" | bc)
        
        # Log the execution time
        echo "Test completed in $execution_time seconds with $CORE cores." | tee -a "$LOG_FILE"
        echo "----------------------------------------" >> "$LOG_FILE"
    done
done

echo "=== End of KMeans Parallel Tests ===" >> "$LOG_FILE"

