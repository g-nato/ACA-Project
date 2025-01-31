#include "centroid.h"

Centroid::Centroid(double xCoord, double yCoord, int id)
    : x(xCoord), y(yCoord), previous_x(xCoord), previous_y(yCoord), id(id) {}

void Centroid::updateCoordinates(double newX, double newY) {
    previous_x = x;
    previous_y = y;
    x = newX;
    y = newY;
}



