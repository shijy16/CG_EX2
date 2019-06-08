#ifndef Vector3_H
#define Vector3_H

#include <stdlib.h>
#include <iostream>
#include <math.h>

class Vector3{
	public:
		double x,y,z;
	public:
		double getX();
		double getY();
		double getZ();
		Vector3(double, double, double);
		Vector3(Vector3*);
		Vector3();
		void show();
		void normalize();
		double getLength();
		
		static Vector3 cross(Vector3 v1, Vector3 v2){ 
			return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x); 
		}
		static Vector3 mul(Vector3 v1,Vector3 v2){
			return Vector3(v2.x*v1.x,v1.y*v2.y,v1.z*v2.z);
		}
		static Vector3 add(Vector3 v1,Vector3 v2){
			return Vector3(v1.x+v2.x,v1.y+v2.y,v1.z+v2.z);
		}
		static Vector3 sub(Vector3 v1,Vector3 v2){
			return Vector3(v1.x-v2.x,v1.y-v2.y,v1.z-v2.z);
		}
		static Vector3 div(Vector3 v1,Vector3 v2){
			return Vector3(v1.x/v2.x,v1.y/v2.y,v1.z/v2.z);
		}
		static double dot(Vector3 v1, Vector3 v2) {
			return v2.x*v1.x + v1.y*v2.y + v1.z*v2.z;
		}

		void operator += (Vector3 v){
			x += v.x;
			y += v.y;
			z += v.z;
		}
		void operator -= (Vector3 v){
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}
		void operator /= (Vector3 v){
			x /= v.x;
			y /= v.y;
			z /= v.z;
		}
		void operator *= (Vector3 v){
			x *= v.x;
			y *= v.y;
			z *= v.z;
		}
		
		friend Vector3 operator + (Vector3 v1,Vector3 v2){
			return Vector3::add(v1,v2);
		}	
		friend Vector3 operator - (Vector3 v1,Vector3 v2){
			return Vector3::sub(v1,v2);
		}	
		friend Vector3 operator * (Vector3 v1,Vector3 v2){
			return Vector3::mul(v1, v2);
		}	
		friend Vector3 operator * (Vector3 v1,double f){
			return Vector3::mul(v1,Vector3(f,f,f));
		}
		friend Vector3 operator * (double f,Vector3 v1) {
			return Vector3::mul(v1, Vector3(f, f, f));
		}
		friend Vector3 operator / (Vector3 v1,Vector3 v2){
			return Vector3::sub(v1,v2);
		}	
		friend Vector3 operator / (Vector3 v1, double f) {
			return Vector3(v1.x /= f,v1.y /= f,v1.z /= f);
		}


};

#endif
