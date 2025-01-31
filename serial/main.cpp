#include "point.h"
#include "kmeans.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Use: " << argv[0] << " <dataset_path> <num_clusters> <iterations> <subset_size>" << std::endl;
        return 1;
    }

    std::string dataset_path = argv[1];
    int num_clusters = std::stoi(argv[2]);
    int max_iterations = std::stoi(argv[3]);
    int subset_size = std::stoi(argv[4]);

    // seed for random number generator
    
    std::srand(42);

    // Start timer for loading data
    auto load_start = std::chrono::high_resolution_clock::now();

    std::vector<Point> points = loadSubset(dataset_path, subset_size);

    // End timer for loading data
    auto load_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> load_duration = load_end - load_start;
    std::cout << "Data loading time: " << load_duration.count() << " seconds." << std::endl;

    if (points.empty()) {
        std::cerr << "The dataset was not loaded correctly or the requested subset is too large." << std::endl;
        return 1;
    }

    std::vector<Centroid> centroids;
    KMeans kmeans(num_clusters, max_iterations);

    // Start timer for computation
    auto compute_start = std::chrono::high_resolution_clock::now();

    kmeans.run(points, centroids);

    // End timer for computation
    auto compute_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> compute_duration = compute_end - compute_start;
    std::cout << "Computation time: " << compute_duration.count() << " seconds." << std::endl;


    return 0;
}
