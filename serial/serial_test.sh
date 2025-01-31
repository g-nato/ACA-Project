#!/bin/bash

DATASET_PATH="/home/giuseppe/aca_project/big_dataset_project.csv"
PROGRAM="./KMeans_serial"

# Check if the program exists
if [ ! -f "$PROGRAM" ]; then
    echo "Error: The program $PROGRAM does not exist. Make sure you have compiled KMeans_serial."
    exit 1
fi

# Array of subset sizes to test
SUBSET_SIZES=(1000 10000 100000 1000000 10000000)

# Number of clusters and iterations for the KMeans algorithm
CLUSTERS=6
ITERATIONS=500

# Log file to save results
LOG_FILE="results_serial.log"

# Start the log
echo "=== KMeans Serial Test Results ===" > "$LOG_FILE"
echo "Dataset: $DATASET_PATH" >> "$LOG_FILE"
echo "Number of clusters: $CLUSTERS, Maximum iterations: $ITERATIONS" >> "$LOG_FILE"
echo "----------------------------------------" >> "$LOG_FILE"

# Loop through each subset size
for SIZE in "${SUBSET_SIZES[@]}"
do
    echo "Running test on $SIZE points... ($(date))" | tee -a "$LOG_FILE"
    
    start_time=$(date +%s.%N)
    
    # Redirect the output to the log
    $PROGRAM "$DATASET_PATH" $CLUSTERS $ITERATIONS $SIZE >> "$LOG_FILE" 2>&1
    
    end_time=$(date +%s.%N)
    
    # Execution time in seconds with decimals
    execution_time=$(echo "$end_time - $start_time" | bc)
    
    # Log the execution time
    echo "Test completed in $execution_time seconds." | tee -a "$LOG_FILE"
    echo "----------------------------------------" >> "$LOG_FILE"
done

echo "=== End of KMeans Serial Tests ===" >> "$LOG_FILE"

