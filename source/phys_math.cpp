#include "phys_math.h"


int sgn(int num) {
	if (num > 0) return(1);
	else if (num < 0) return(-1);
	else return(0);
}

float getHeading(int x1, int y1, int x2, int y2) {

	float xDiff;
	float yDiff;

	float temp = 0;

	xDiff = x2 - x1;
	yDiff = y2 - y1;

	//this is a mod to swap the y axis, works this way in this case for my image format
	xDiff = x2 - x1;
	yDiff = y1 - y2;

	if (sgn(yDiff) > 0) {
		temp = atan(xDiff / yDiff);
	} else if (sgn(yDiff) < 0) {
		temp = atan(xDiff / yDiff) + (atan(1.0f) * 4);
	}
	if (temp < 0) { temp = temp + (atan(1.0f) * 8); }

	if (temp == 0) {
		if (x1 < x2) {
			temp = PI/2;
		} else if (x1 > x2) {
			temp = PI+(PI/2);
		} else {
			if (y1 < y2) {
				temp = PI;
			} else {
				temp = 0.00001;
			}
		}
	}

	return temp;
}

long getDistance(int x1, int y1, int x2, int y2) {


	float xDiff;
	float yDiff;
	float temp;
	float angle;

	xDiff = x2 - x1;
	yDiff = y2 - y1;

	temp = sqrt(xDiff*xDiff+yDiff*yDiff);

	angle = acos(yDiff / temp);

	return temp;

	return ((x1-x2)^2 + (y1-y2)^2)*((x1-x2)^2 + (y1-y2)^2);
}

float Vectorize(float vect, float adj) {

	float vec = vect+adj;
	if (vec < 0.0f) { vec = (atan(1.0f)*8) + vec; }
	if (vec > (atan(1.0f)*8)) { vec -= (atan(1.0f)*8); }
	
	return vec;

}

bool AccuracyCheck(float vec1, float vec2, float range) {
	if (vec1-vec2 < range && vec1-vec2 >= 0) {
		return true;
	}

	if (vec2-vec1 < range && vec2-vec1 >= 0) {
		return true;
	}

	return false;
}