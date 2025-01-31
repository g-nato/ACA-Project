#ifndef KMEANS_H
#define KMEANS_H

#include <vector>
#include <string>
#include "point.h"
#include "centroid.h"

// Declaration of the loadSubset function
std::vector<Point> loadSubset(const std::string& filepath, int subset_size);

class KMeans {
public:
    int num_clusters;       // Number of clusters
    int max_iterations;     // Maximum number of iterations
    double epsilon;         // Convergence threshold

    KMeans(int k, int iterations, double convThreshold = 0.001);

    void initializeCentroids(std::vector<Centroid>& centroids, std::vector<Point>& points);
    void assignPointsToClusters(std::vector<Point>& points, std::vector<Centroid>& centroids);
    void calculateNewCentroids(const std::vector<Point>& points, std::vector<Centroid>& centroids);
    void run(std::vector<Point>& points, std::vector<Centroid>& centroids);
};

#endif
