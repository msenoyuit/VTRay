#include "sphere.hpp"

sphere::sphere()
{
}

sphere::~sphere()
{
}

const color sphere::intersect(const std::list<objects*> actors, const std::list<light*> lights, const ray traceRay)
{
	return color(0,0,0);
}

const vec sphere::getCenter()
{
	return vec(0,0,0);
}



