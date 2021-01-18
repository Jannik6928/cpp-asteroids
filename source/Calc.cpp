#include <cmath>
#include "Calc.h"

double d2r(double deg) { return deg / 180 * 3.141592f; }
double distance(double x1, double x2, double y1, double y2) { return pow(pow((x1 - x2), 2) + pow((y1 - y2), 2), 0.5f); }
