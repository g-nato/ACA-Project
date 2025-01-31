#ifndef POINT_H
#define POINT_H

struct Point {
    double x, y;      // Coordinates of the point
    int cluster_id;   // Cluster to which the point belongs

    Point() : x(0.0), y(0.0), cluster_id(-1) {}
    Point(double xCoord, double yCoord);
};

#endif
