#ifndef POINT_H
#define POINT_H

struct Point {
    double x, y;      
    int cluster_id;

    Point() : x(0.0), y(0.0), cluster_id(-1) {}
    Point(double xCoord, double yCoord);
};

#endif
