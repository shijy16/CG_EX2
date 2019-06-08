#include "vector3.h"

Vector3::Vector3(Vector3* v){
	x = v->x;
	y = v->y;
	z = v->z;
}

Vector3::Vector3() {
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(double a, double b, double c){
	x = a;
	y = b;
	z = c;
}

double Vector3::getX(){
	return x;
}

double Vector3::getY(){
	return y;
}

double Vector3::getZ(){
	return z;
}


void Vector3::show(){
	printf("%f,%f,%f",x,y,z);
}

void Vector3::normalize(){
	double s = sqrt(x*x + y*y + z*z);
	x /= s;
	y /= s;
	z /= s;
}

double Vector3::getLength() {
	return sqrt(x*x + y*y + z*z);
}
