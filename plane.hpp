#ifndef PLANE_HEADER
#define PLANE_HEADER

#include "objects.hpp"
#include <iostream>


class plane : public objects
{
public:
	plane(vec * cenIn, vec * normIn, colorStruct * colIn, double lamIn);
	~plane();
	colorStruct * intersect(const std::list<objects*> actors, const std::list<light*> lights, const ray traceRay, double dist, objects * screen);
	const double intersectTrue(const ray traceRay);
	vec* getCenter();

private:
	vec * center = 0;
	vec * normal = 0;
	colorStruct * color = 0;
	double lambert;

};



#endif