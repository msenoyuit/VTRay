#ifndef SPHERE_HEADER
#define SPHERE_HEADER
#include <list>
#include "objects.hpp"


class sphere : public objects
{
public:
	sphere();
	~sphere();
	const color intersect(const std::list<objects*> actors, const std::list<light*> lights, const ray traceRay);
	const vec getCenter();

private:

};


#endif