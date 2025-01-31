

# README - K-Means Clustering Project with OpenMP

## Project Structure

The project is organized into three main folders, included within the codes folder , each of which contains different versions of the K-means clustering code, along with test scripts for running implementations. The folders are:

1. **serial**: 
   - Contains the K-means code in its serial version. This is the original, non-parallelized code, useful as a reference for comparing performance against the parallel versions.

2. **OpenMP**: 
   - This folder contains the base parallelized code, parallelized using OpenMP. The code in this folder represents the first parallel implementation of the K-means clustering algorithm.
   
3. **OpenMP(optimized)**: 
   - Contains the optimized parallel code. In this version, additional improvements have been made to optimize resource usage and the distribution of workload across threads.

## Main Files

The following files are present in all folders, with adaptations for each version:

- **centroid.cpp**: Contains the definition of functions that manage the centroids of the clusters. In particular, it implements the operations for updating the position of the centroids.
  
- **centroid.h**: Declaration of the `Centroid` class and associated functions.
  
- **kmeans.cpp**: Implements the K-means algorithm, including the assignment of points to clusters and recalibration of centroids at each iteration.
  
- **kmeans.h**: Declaration of the `KMeans` class and the main functions.
  
- **main.cpp**: The entry point of the program. It handles the initialization of the dataset and calls the functions to execute the K-means algorithm.
  
- **point.cpp**: Defines the operations on points in space (coordinates). Each point is represented as an object with associated functions to calculate distances from the centroids.
  
- **point.h**: Declaration of the `Point` class and its related functions.
  
- **parallel_test.sh**: This Bash script allows automatic testing of the K-means code with various dataset sizes and thread counts (in the case of parallel code). The script performs tests as reported during project development (and documented in the report) and logs execution times for performance analysis.

## How to Run Tests

### Serial Version
To run the test on the serial version, enter the `serial` folder and execute the test script:
```bash
cd serial
./serial_test.sh
```

### Parallel Version
To run the test on the parallel version, enter the `OpenMP` or `OpenMP(optimized)` folder and execute the corresponding test script:
```bash
cd OpenMP
./parallel_test.sh
```
or
```bash
cd OpenMP(optimized)
./parallel_test.sh
```

The script will run the K-means algorithm on datasets of various sizes, using a variable number of threads to evaluate the scalability and performance of the parallel implementation.

---







