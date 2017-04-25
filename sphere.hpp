#ifndef SPHERE_HEADER
#define SPHERE_HEADER
#include <list>
#include <cstdio>
#include <iostream>
#include "objects.hpp"


class sphere : public objects
{
public:
	sphere(vec * cenIn, double radIn, colorStruct * colIn, double lamIn);
	~sphere();
	colorStruct * intersect(const std::list<objects*> actors, const std::list<light*> lights, const ray traceRay, double dist);
	const double intersectTrue(const ray traceRay);
	vec * getCenter();

private:
	colorStruct * col = 0; 
	double lambert;
	vec * center = 0;
	double radius;

};


#endif