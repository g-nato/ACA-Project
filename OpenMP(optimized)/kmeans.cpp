#include "kmeans.h"
#include <cstdlib>
#include <limits>
#include <cmath>
#include <iostream>
#include <omp.h>


KMeans::KMeans(int k, int iterations, double convThreshold)
    : num_clusters(k), max_iterations(iterations), epsilon(convThreshold) {}

// Function to initialize centroids randomly
void KMeans::initializeCentroids(std::vector<Centroid>& centroids, const std::vector<Point>& points) {
    centroids.reserve(num_clusters);
    for (int i = 0; i < num_clusters; ++i) {
        int random_index = rand() % points.size();
        centroids.emplace_back(points[random_index].x, points[random_index].y, i);
    }
}

// Assigns points to the nearest centroids
void KMeans::assignPointsToClusters(std::vector<Point>& points, const std::vector<Centroid>& centroids) {
    #pragma omp parallel for schedule(static) default(none) shared(points, centroids)
    for (size_t i = 0; i < points.size(); ++i) {
        double min_distance_sq = std::numeric_limits<double>::max();
        int closest_cluster = -1;

        for (int c = 0; c < num_clusters; ++c) {
            double dx = points[i].x - centroids[c].x;
            double dy = points[i].y - centroids[c].y;
            double dist_sq = dx * dx + dy * dy;
            if (dist_sq < min_distance_sq) {
                min_distance_sq = dist_sq;
                closest_cluster = centroids[c].id;
            }
        }
        points[i].cluster_id = closest_cluster;
    }
}

// Calculates new centroids by optimizing the reduction
void KMeans::calculateNewCentroids(const std::vector<Point>& points, std::vector<Centroid>& centroids) {
    int K = num_clusters;

    // Initializes arrays for manual reduction
    double sumX[K];
    double sumY[K];
    int counts[K];

    // Initialize values to 0
    for (int i = 0; i < K; ++i) {
        sumX[i] = 0.0;
        sumY[i] = 0.0;
        counts[i] = 0;
    }

    #pragma omp parallel
    {
        // Local variables for reduction
        double local_sumX[K] = {0.0};
        double local_sumY[K] = {0.0};
        int local_counts[K] = {0};

        // Parallel cycle with manual reduction
        #pragma omp for schedule(static)
        for (size_t i = 0; i < points.size(); ++i) {
            int cluster_id = points[i].cluster_id;
            local_sumX[cluster_id] += points[i].x;
            local_sumY[cluster_id] += points[i].y;
            local_counts[cluster_id] += 1;
        }

        // Manual reduction out of parallel cycle
        #pragma omp critical
        {
            for (int j = 0; j < K; ++j) {
                sumX[j] += local_sumX[j];
                sumY[j] += local_sumY[j];
                counts[j] += local_counts[j];
            }
        }
    }

    // Update coordinates of centroids
    for (int j = 0; j < K; ++j) {
        if (counts[j] > 0) {
            centroids[j].updateCoordinates(sumX[j] / counts[j], sumY[j] / counts[j]);
        } else {
            // If a cluster has no points, reassign a random centroid
            int random_index = rand() % points.size();
            centroids[j].updateCoordinates(points[random_index].x, points[random_index].y);
        }
    }
}


void KMeans::run(std::vector<Point>& points, std::vector<Centroid>& centroids) {
    initializeCentroids(centroids, points);

    bool converged = false;
    int iteration = 0;

    while (iteration < max_iterations && !converged) {
        assignPointsToClusters(points, centroids);
        calculateNewCentroids(points, centroids);

        // Convergence control
        converged = true;
        #pragma omp parallel for schedule(static) default(none) shared(converged, centroids)
        for (int c = 0; c < num_clusters; ++c) {
            double dx = centroids[c].x - centroids[c].previous_x;
            double dy = centroids[c].y - centroids[c].previous_y;
            double movement_sq = dx * dx + dy * dy;

            if (movement_sq > epsilon * epsilon) {
                #pragma omp atomic write
                converged = false;
            }
        }
        iteration++;
    }

    if (converged) {
        std::cout << "Convergence achieved after " << iteration << " iterations." << std::endl;
    } else {
        std::cout << "Reached the maximum number of iterations without convergence." << std::endl;
    }
}
