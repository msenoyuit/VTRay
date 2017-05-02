#include "plane.hpp"

plane::plane(vec * cenIn, vec * normIn, colorStruct * colIn, double lamIn)
{
	center = new vec(cenIn->x, cenIn->y, cenIn->z);
	normal = new vec(normIn->x, normIn->y, normIn->z); 
	color = new colorStruct(colIn->r, colIn->g, colIn->b);
	lambert = lamIn;
}

plane::~plane()
{
	delete center;
	delete normal;
	delete color;
}

colorStruct * plane::intersect(const std::list<objects*> actors, const std::list<light*> lights, const ray traceRay, double dist, objects * screen)
{
	double objDist;
	colorStruct * pointColor = new colorStruct(0, 0, 0);
	vec pointIntersect = vec(traceRay.origin.x + traceRay.direction.x * dist,
		traceRay.origin.y + traceRay.direction.y * dist,
		traceRay.origin.z + traceRay.direction.z * dist);
	bool clearPath = true;
	vec surfaceNormal = normal->normal();
	for (auto const& i : lights)
	{
		double distLight = distance(&pointIntersect, &(i->location));
		vec shadDir = vec(i->location.x - pointIntersect.x, i->location.y - pointIntersect.y, i->location.z - pointIntersect.z);
		const ray shadRay = ray(&pointIntersect, &shadDir);
		double screenDist = screen->intersectTrue(shadRay);
		clearPath = true;
		for (auto const& j : actors) {
			if (distance(center, j->getCenter()) > 1e-3)
			{
				objDist = j->intersectTrue(shadRay);
				if (objDist > 1e-3 && objDist < distLight && objDist < screenDist)
				{
					clearPath = false;
					break;
				}
			}
		}
		if (clearPath)
		{
			
			double scale = -1 * dot(&surfaceNormal, &(shadRay.direction)) * lambert;
			//std::cout << "intersectPlane " << scale << '\n';
			if (scale < 0)
			{
				scale = 0;
			}
			pointColor->r = pointColor->r + scale*i->intensity*color->r;
			pointColor->g = pointColor->g + scale*i->intensity*color->g;
			pointColor->b = pointColor->b + scale*i->intensity*color->b;
		}
	}

	return pointColor;
}

const double plane::intersectTrue(const ray traceRay)
{
	const vec direcNorm = traceRay.direction.normal();
	double ln = dot(&direcNorm, normal);
	if (ln < 1e-3 && ln > -1e-3)
	{
		return -1.0;
	}
	const vec polo = vec(center->x - traceRay.origin.x, center->y - traceRay.origin.y, center->z - traceRay.origin.z);
	double distance = dot(&polo, normal)/ln;
	return distance;
}

vec * plane::getCenter()
{
	return center;
}
