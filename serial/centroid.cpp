#include "centroid.h"

Centroid::Centroid(double xCoord, double yCoord, int id) : x(xCoord), y(yCoord), id(id) {}

void Centroid::updateCoordinates(double newX, double newY) {
    x = newX;
    y = newY;
}
