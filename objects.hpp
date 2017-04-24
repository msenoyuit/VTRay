#ifndef OBJECTS_HEADER
#define OBJECTS_HEADER
#include <list>
#include <cmath>

struct color {
	int r;
	int g;
	int b;
	color(int rIn, int gIn, int bIn) {
		r = rIn;
		g = gIn;
		b = bIn;
	};

};

struct vec {
	double x;
	double y;
	double z;
	vec(double xIn, double yIn, double zIn) {
		x = xIn;
		y = yIn;
		z = zIn;
	};
	vec const normal() {
		double length = sqrt(x*x + y*y + z*z);
		return vec(x / length, y / length, z / length);
	}

	double const dotProduct(const vec & vecIn)
	{
		return x*vecIn.x + y*vecIn.y + z*vecIn.z;
	};

};

struct ray {
	vec origin;
	vec direction;
	ray(vec oIn, vec dirIn): origin(oIn), direction(dirIn){};
};

struct light
{
	vec location;
	double intensity;
	light(vec locIn, double intenIn): location(locIn) {
		intensity = intenIn;
	};

	light(double x, double y, double z, double intenIn): location(vec(x, y, z)) {
		intensity = intenIn;
	};
};


class objects
{
public:
	objects() {};
	~objects() {};
	virtual const color intersect(const std::list<objects*> actors, const std::list<light> lights, const ray traceRay){};
	virtual const vec getCenter(){};
	 
private:

};





#endif