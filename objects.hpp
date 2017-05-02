#ifndef OBJECTS_HEADER
#define OBJECTS_HEADER
#include <list>
#include <cmath>






struct vec {
	double x;
	double y;
	double z;
	vec(double xIn, double yIn, double zIn) {
		x = xIn;
		y = yIn;
		z = zIn;
	}
	const vec normal() const {
		double length = sqrt(x*x + y*y + z*z);
		return vec(x / length, y / length, z / length);
	}
	vec& operator=(const vec& src)
	{
		x = src.x;
		y = src.y;
		z = src.z;
		return *this;
	}

};

struct camera
{
	vec center = vec(0, 0, 0);
	vec normal = vec(0, 0, 0);
	vec focPoint = vec(0, 0, 0);
	double focus = 0;
	double resX = 0;
	double resY = 0;
	int sizeX = 0;
	int sizeY = 0;
	camera(vec * cenIn, vec * normIn, double focIn, double rXIn, double rYIn, int sXIn, int sYIn)
	{
		center = vec(cenIn->x, cenIn->y, cenIn->z);
		normal = vec(normIn->x, normIn->y, normIn->z);
		focus = focIn;
		resX = rXIn;
		resY = rYIn;
		sizeX = sXIn;
		sizeY = sYIn;
		vec norm = normal.normal();
		focPoint = vec(center.x - norm.x*focus, center.y - norm.y*focus, center.z - norm.z*focus);
	}

};

struct colorStruct {
	int r;
	int g;
	int b;
	colorStruct(int rIn, int gIn, int bIn) {
		r = rIn;
		g = gIn;
		b = bIn;
	};

};

struct ray {
	vec origin = vec(0, 0, 0);
	vec direction = vec(0, 0, 0);
	ray(vec * oIn, vec * dirIn) {
		origin = vec(oIn->x, oIn->y, oIn->z);
		direction = dirIn->normal();
	};
};

struct light
{
	vec location = vec(0,0,0);
	double intensity;
	light(vec * locIn, double intenIn) 
	{
		location = vec(locIn->x, locIn->y, locIn->z);
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
	virtual colorStruct * intersect(const std::list<objects*> actors, const std::list<light*> lights, const ray traceRay, double dist, objects * screen) = 0;
	virtual const double intersectTrue(const ray traceRay)  = 0;
	virtual vec* getCenter() = 0;
	const double dot(const vec * vecA, const vec * vecB) const
	{
		return (vecA->x*vecB->x + vecA->y*vecB->y + vecA->z*vecB->z);
	}

	const double distance(const vec * vecA, const vec * vecB) const
	{
		double x = vecA->x - vecB->x;
		double y = vecA->y - vecB->y;
		double z = vecA->z - vecB->z;
		return sqrt(x*x + y*y + z*z);
	}
	 
private:

};





#endif