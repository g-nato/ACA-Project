#include "kmeans.h"
#include <cstdlib>
#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <omp.h>

// Function to load a subset of the dataset
std::vector<Point> loadSubset(const std::string& filepath, int subset_size) {
    std::vector<Point> points;
    points.reserve(subset_size);  // Avoid reallocations

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filepath << std::endl;
        return points;
    }

    std::string line;
    int count = 0;
    int line_number = 0;

    // Read and skip the header line
    if (std::getline(file, line)) {
        line_number++;
    }

    while (std::getline(file, line) && count < subset_size) {
        line_number++;
        double x, y;
        std::size_t pos1 = line.find(',');
        if (pos1 != std::string::npos) {
            std::size_t pos2 = line.find(',', pos1 + 1);
            if (pos2 != std::string::npos) {
                std::string x_str = line.substr(0, pos1);
                std::string y_str = line.substr(pos1 + 1, pos2 - pos1 - 1);
                try {
                    x = std::stod(x_str);
                    y = std::stod(y_str);
                    points.emplace_back(x, y);
                    ++count;
                } catch (const std::exception& e) {
                    std::cerr << "Parsing error at line " << line_number << ": " << line << std::endl;
                    std::cerr << "Exception: " << e.what() << std::endl;
                }
            }
        }
    }

    file.close();
    return points;
}

KMeans::KMeans(int k, int iterations, double convThreshold)
    : num_clusters(k), max_iterations(iterations), epsilon(convThreshold) {}

// Function to initialize centroids randomly
void KMeans::initializeCentroids(std::vector<Centroid>& centroids, std::vector<Point>& points) {
    for (int i = 0; i < num_clusters; ++i) {
        int random_index = rand() % points.size();
        centroids.emplace_back(points[random_index].x, points[random_index].y, i);
    }
}

// Parallelize function to assign points to nearest centroids
void KMeans::assignPointsToClusters(std::vector<Point>& points, std::vector<Centroid>& centroids) {
    #pragma omp parallel for
    for (size_t i = 0; i < points.size(); ++i) {
        double min_distance_sq = std::numeric_limits<double>::max();
        int closest_cluster = -1;

        for (const auto& centroid : centroids) {
            double dx = points[i].x - centroid.x;
            double dy = points[i].y - centroid.y;
            double dist_sq = dx * dx + dy * dy;
            if (dist_sq < min_distance_sq) {
                min_distance_sq = dist_sq;
                closest_cluster = centroid.id;
            }
        }
        points[i].cluster_id = closest_cluster;
    }
}

// Parallelization of the calculation of new centroids
void KMeans::calculateNewCentroids(const std::vector<Point>& points, std::vector<Centroid>& centroids) {
    int K = num_clusters;
    std::vector<double> sumX(K, 0.0);
    std::vector<double> sumY(K, 0.0);
    std::vector<int> counts(K, 0);

    // Local arrays for each thread
    #pragma omp parallel
    {
        std::vector<double> local_sumX(K, 0.0);
        std::vector<double> local_sumY(K, 0.0);
        std::vector<int> local_counts(K, 0);

        // Parallelization of the for loop
        #pragma omp for
        for (int i = 0; i < points.size(); ++i) {
            int cluster_id = points[i].cluster_id;
            local_sumX[cluster_id] += points[i].x;
            local_sumY[cluster_id] += points[i].y;
            local_counts[cluster_id] += 1;
        }

        // Manual reduction of local results in global results
        #pragma omp critical
        {
            for (int j = 0; j < K; ++j) {
                sumX[j] += local_sumX[j];
                sumY[j] += local_sumY[j];
                counts[j] += local_counts[j];
            }
        }
    }

    //  Update coordinates of centroids
    for (int j = 0; j < K; ++j) {
        if (counts[j] > 0) {
            centroids[j].updateCoordinates(sumX[j] / counts[j], sumY[j] / counts[j]);
        } else {
            // Management of the case where the cluster has no points
        }
    }
}


// Function to run KMeans algorithm with iterations and convergence checking
void KMeans::run(std::vector<Point>& points, std::vector<Centroid>& centroids) {
    initializeCentroids(centroids, points);

    bool converged = false;

    for (int i = 0; i < max_iterations && !converged; ++i) {
        converged = true;

        assignPointsToClusters(points, centroids);

        std::vector<Centroid> old_centroids = centroids;

        calculateNewCentroids(points, centroids);

        // Checking convergence using squared distances
        #pragma omp parallel for
        for (int j = 0; j < num_clusters; ++j) {
            double dx = centroids[j].x - old_centroids[j].x;
            double dy = centroids[j].y - old_centroids[j].y;
            double movement_sq = dx * dx + dy * dy;

            if (movement_sq > epsilon * epsilon) {
                converged = false;
            }
        }

        if (converged) {
            std::cout << "Convergence achieved after " << i + 1 << " iterations." << std::endl;
            break;
        }
    }

    if (!converged) {
        std::cout << "Reached the maximum number of iterations without convergence." << std::endl;
    }
}
