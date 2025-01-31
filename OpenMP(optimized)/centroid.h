#ifndef CENTROID_H
#define CENTROID_H

struct Centroid {
    double x, y;            /
    double previous_x, previous_y;
    int id;
    
    Centroid(double xCoord, double yCoord, int id);
   
    void updateCoordinates(double newX, double newY);
};

#endif
