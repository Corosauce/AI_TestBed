#ifndef PHYS_MATH_H
#define PHYS_MATH_H

#include "math.h"

#define PI 3.1415926535897932384626433832795f

struct coord {
	long x;
	long y;
};

int sgn(int num);
float getHeading(int x1, int y1, int x2, int y2);
long getDistance(int x1, int y1, int x2, int y2);
float Vectorize(float vec, float adj);
bool AccuracyCheck(float vec1, float vec2, float range);


#endif