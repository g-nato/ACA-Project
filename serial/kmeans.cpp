#include "kmeans.h"
#include <cstdlib>
#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

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
        //print header line for confirmation
        // std::cout << "Intestazione: " << line << std::endl;
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
                // Ignore the third column (original cluster)
                try {
                    x = std::stod(x_str);
                    y = std::stod(y_str);
                    points.emplace_back(x, y);
                    ++count;
                } catch (const std::exception& e) {
                    std::cerr << "Parsing error at line " << line_number << ": " << line << std::endl;
                    std::cerr << "Exception: " << e.what() << std::endl;
                }
            } else {
                std::cerr << "Invalid format at the line " << line_number << ": " << line << std::endl;
            }
        } else {
            std::cerr << "Invalid format at the line" << line_number << ": " << line << std::endl;
        }
    }

    file.close();
    return points;
}

// Implementation of the KMeans constructor
KMeans::KMeans(int k, int iterations, double convThreshold)
    : num_clusters(k), max_iterations(iterations), epsilon(convThreshold) {}

// Implementation of KMeans constructor Function to initialize centroids randomly
void KMeans::initializeCentroids(std::vector<Centroid>& centroids, std::vector<Point>& points) {
    for (int i = 0; i < num_clusters; ++i) {
        int random_index = rand() % points.size();
        centroids.emplace_back(points[random_index].x, points[random_index].y, i);
    }
}

// Assigns points to the nearest centroids
void KMeans::assignPointsToClusters(std::vector<Point>& points, std::vector<Centroid>& centroids) {
    for (auto& point : points) {
        double min_distance_sq = std::numeric_limits<double>::max();
        int closest_cluster = -1;

        for (const auto& centroid : centroids) {
            double dx = point.x - centroid.x;
            double dy = point.y - centroid.y;
            double dist_sq = dx * dx + dy * dy;  // Distance squared
            if (dist_sq < min_distance_sq) {
                min_distance_sq = dist_sq;
                closest_cluster = centroid.id;
            }
        }
        point.cluster_id = closest_cluster;
    }
}

// Calculates new centroids based on points assigned to clusters
void KMeans::calculateNewCentroids(const std::vector<Point>& points, std::vector<Centroid>& centroids) {
    int K = num_clusters;
    std::vector<double> sumX(K, 0.0);
    std::vector<double> sumY(K, 0.0);
    std::vector<int> counts(K, 0);

    for (const auto& point : points) {
        int cluster_id = point.cluster_id;
        sumX[cluster_id] += point.x;
        sumY[cluster_id] += point.y;
        counts[cluster_id] += 1;
    }

    for (int j = 0; j < K; ++j) {
        if (counts[j] > 0) {
            centroids[j].updateCoordinates(sumX[j] / counts[j], sumY[j] / counts[j]);
        } else {
            // Handling the case where the cluster has no points
            
        }
    }
}

// Function to run KMeans algorithm with iterations and convergence check
void KMeans::run(std::vector<Point>& points, std::vector<Centroid>& centroids) {
    initializeCentroids(centroids, points);

    bool converged = false;

    for (int i = 0; i < max_iterations && !converged; ++i) {
        converged = true;

        assignPointsToClusters(points, centroids);

        std::vector<Centroid> old_centroids = centroids;

        calculateNewCentroids(points, centroids);

        // Checking convergence using squared distances.
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
