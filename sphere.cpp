#include "sphere.hpp"

sphere::sphere(vec * cenIn, double radIn, colorStruct * colIn, double lamIn)
{
	radius = radIn;
	lambert = lamIn;
	col = new colorStruct(colIn->r, colIn->g, colIn->b);
	center = new vec(cenIn->x, cenIn->y, cenIn->z);
}

sphere::~sphere()
{
	delete col;
	delete center;
}

colorStruct * sphere::intersect(const std::list<objects*> actors, const std::list<light*> lights, const ray traceRay, double dist)
{
	double objDist;
	colorStruct * pointColor = new colorStruct(0, 0, 0);
	vec pointIntersect = vec(traceRay.origin.x + traceRay.direction.x * dist, 
							 traceRay.origin.y + traceRay.direction.y * dist, 
							 traceRay.origin.z + traceRay.direction.z * dist);
	bool clearPath = true;
	vec surfaceNormal = vec(pointIntersect.x - center->x, pointIntersect.y - center->y, pointIntersect.z - center->z).normal();
	for (auto const& i :lights)
	{
		double distLight = distance(&pointIntersect, &(i->location));
		vec shadDir = vec(i->location.x - pointIntersect.x, i->location.y - pointIntersect.y, i->location.z - pointIntersect.z);
		const ray shadRay = ray(&pointIntersect, &shadDir);
		clearPath = true;
		for (auto const& j : actors) {
			if (distance(center, j->getCenter()) > 1e-3)
			{
				objDist = j->intersectTrue(shadRay);
				if (objDist > 1e-3 && objDist < distLight)
				{
					clearPath = false;
					break;
				}
			}
		}
		if (clearPath)
		{
			double scale = dot(&surfaceNormal, &(shadRay.direction)) * lambert;
			if (scale < 0)
			{
				scale = 0;
			}
			pointColor->r = pointColor->r + scale*i->intensity*col->r;
			pointColor->g = pointColor->g + scale*i->intensity*col->g;
			pointColor->b = pointColor->b + scale*i->intensity*col->b;
		}
	}

	return pointColor;
}

const double sphere::intersectTrue(const ray traceRay)
{
	double distance = -1.0;
	const vec direction = vec(traceRay.direction.x, traceRay.direction.y, traceRay.direction.z);
	const vec dif = vec(traceRay.origin.x - center->x, traceRay.origin.y - center->y, traceRay.origin.z - center->z);
	//double b = 2 * (traceRay.direction.x*dif.x + traceRay.direction.y*dif.y + traceRay.direction.z*dif.z);
	double b = 2.0 * dot(&direction, &dif);
	double c = dot(&dif, &dif) - radius*radius;

	//std::cout << "dif " << dif.x << " " << dif.y << " " << dif.z << '\n';
	//std::cout << "direction " << direction.x << " " << direction.y << " " << direction.z << '\n';
	//std::cout << "A " << dot(&direction, &direction) << '\n';
	//std::cout << "B " << b << '\n';
	//std::cout << "C " << c << '\n';
	//std::cout << "radius " << radius << '\n';

	double root = (b)*(b) - 4*c;
	//std::cout << "Root " << root << '\n';
	if (root < 0)
	{
		return distance;
	}
	root = sqrt(root);
	//std::cout << "Root 2 " << root << '\n';
	distance = (-b - root)/2;
	//std::cout << "distance " << distance << '\n';
	double distance2 = (-b + root)/2;
	//std::cout << "distance2 " << distance2 << '\n';
	if (distance > distance2)
	{
		return distance2;
	}
	return distance;
}

 vec* sphere::getCenter()
{
	return center;
}





