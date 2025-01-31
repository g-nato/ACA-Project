#include "kmeans.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <omp.h>

// Function to load a subset of the dataset
std::vector<Point> loadSubset(const std::string& filepath, int subset_size) {
    std::vector<Point> points;
    points.reserve(subset_size);

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filepath << std::endl;
        return points;
    }

    std::string line;
    int count = 0;

    // Skip the header
    if (std::getline(file, line)) {}

    std::vector<std::string> lines;
    lines.reserve(subset_size);

    while (std::getline(file, line) && count < subset_size) {
        lines.emplace_back(line);
        ++count;
    }

    file.close();
    points.resize(count);

    // Parallelize line parsing
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < count; ++i) {
        const char* line_cstr = lines[i].c_str();
        char* end_ptr;

        // Parse of the X coordinate
        double x = std::strtod(line_cstr, &end_ptr);
        if (end_ptr == line_cstr || *end_ptr != ',') {
            // if invalid line assigns 0
            points[i] = Point(0.0, 0.0);
            continue;
        }

        // Parse of the Y coordinate
        const char* y_cstr = end_ptr + 1;
        double y = std::strtod(y_cstr, &end_ptr);
        if (end_ptr == y_cstr) {
            // if invalid line assigns 0
            points[i] = Point(0.0, 0.0);
            continue;
        }

        points[i] = Point(x, y);
    }

    return points;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <dataset_path> <num_clusters> <iterations> <subset_size>" << std::endl;
        return 1;
    }

    std::string dataset_path = argv[1];
    int num_clusters = std::stoi(argv[2]);
    int max_iterations = std::stoi(argv[3]);
    int subset_size = std::stoi(argv[4]);

    std::srand(42);  // Seed per la generazione casuale

    // Start timer for loading data
    auto load_start = std::chrono::high_resolution_clock::now();
    std::vector<Point> points = loadSubset(dataset_path, subset_size);
    auto load_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> load_duration = load_end - load_start;
    std::cout << "Data loading time: " << load_duration.count() << " seconds." << std::endl;

    if (points.empty()) {
        std::cerr << "Errore nel caricamento del dataset." << std::endl;
        return 1;
    }

    std::vector<Centroid> centroids;
    KMeans kmeans(num_clusters, max_iterations);

    // Start timer for computation
    auto compute_start = std::chrono::high_resolution_clock::now();
    kmeans.run(points, centroids);
    auto compute_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> compute_duration = compute_end - compute_start;
    std::cout << "Computation time: " << compute_duration.count() << " seconds." << std::endl;

    return 0;
}
