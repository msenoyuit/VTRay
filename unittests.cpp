#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"
#include <cstdio>

#include "objects.hpp"
#include "sphere.hpp"
#include "sceneObj.hpp"
#include "plane.hpp"


TEST_CASE( "Test colorStruct struct ", "[objects], [colorStruct]" ) {

	colorStruct red = colorStruct(255, 0, 0);
	colorStruct green = colorStruct(0, 255, 0);
	colorStruct blue = colorStruct(0, 0, 255);
	

	REQUIRE(1 == 1);
	REQUIRE(red.r == 255);
	
	REQUIRE(red.g == 0);
	REQUIRE(red.b == 0);

	REQUIRE(green.r == 0);
	REQUIRE(green.g == 255);
	REQUIRE(green.b == 0);

	REQUIRE(blue.r == 0);
	REQUIRE(blue.g == 0);
	REQUIRE(blue.b == 255);
}

TEST_CASE("Test colorStruct camera ", "[objects], [cam]") {

	vec center = vec(0, 0, 0);
	vec normal = vec(1, 0, 0);
	vec normal2 = vec(3, 4, 0);
	vec focPoint = vec(-10, 0, 0);
	vec focPoint2 = vec(-3, -4, 0);
	double focus = 10;
	double focus2 = 5;
	double resX = 1.0;
	double resY = 2.0;
	int sizeX = 3;
	int sizeY = 4;

	camera testCam(&center, &normal, focus, resX, resY, sizeX, sizeY);
	camera testCam2(&center, &normal2, focus2, resX, resY, sizeX, sizeY);


	REQUIRE(testCam.focPoint.x == focPoint.x);
	REQUIRE(testCam.focPoint.y == focPoint.y);
	REQUIRE(testCam.focPoint.z == focPoint.z);


	REQUIRE(testCam2.focPoint.x == focPoint2.x);
	REQUIRE(testCam2.focPoint.y == focPoint2.y);
	REQUIRE(testCam2.focPoint.z == focPoint2.z);


}

TEST_CASE("Test vec struct ", "[objects], [vec]") {

	vec vec1 = vec(10, 0, 0);
	vec vecA = vec(10, 10, 10);
	vec vecB = vec(10, 10, 10);
	vec normVec = vec1.normal();

	REQUIRE(vec1.x == 10);
	REQUIRE(vec1.y == 0);
	REQUIRE(vec1.z == 0);

	REQUIRE(normVec.x == 1);
	REQUIRE(normVec.y == 0);
	REQUIRE(normVec.z == 0);

}

TEST_CASE("Test ray struct ", "[objects], [ray]") {

	vec vec1 = vec(1, 2, 3);
	vec vec2 = vec(.1, .2, .003);
	ray ray1 = ray(&vec1, &vec2);
	vec normal = vec2.normal();

	
	REQUIRE(ray1.origin.x == 1);
	REQUIRE(ray1.origin.y == 2);
	REQUIRE(ray1.origin.z == 3);

	REQUIRE(ray1.direction.x == normal.x);
	REQUIRE(ray1.direction.y == normal.y);
	REQUIRE(ray1.direction.z == normal.z);

}

TEST_CASE("Test light struct ", "[objects], [light]") {

	vec orgin = vec(1, 2, 3);
	double intens = 1.1;
	light light1 = light(&orgin, intens);

	light light2 = light(.1, .2, 3.3, 4.4);


	REQUIRE(light1.location.x == 1);
	REQUIRE(light1.location.y == 2);
	REQUIRE(light1.location.z == 3);

	REQUIRE(light1.intensity == 1.1);

	REQUIRE(light2.location.x == .1);
	REQUIRE(light2.location.y == .2);
	REQUIRE(light2.location.z == 3.3);

	REQUIRE(light2.intensity == 4.4);

}

TEST_CASE("Test sphere class intersectTrue", "[sphere], [intersectTrue]") {

	vec center = vec(1, 2, 3);
	vec center2 = vec(0, 1, 0);
	vec center3 = vec(2, 2, 2);
	vec startRay = vec(2, 1, 0);
	vec startRay2 = vec(0, 0, 0);
	vec directRay = vec(-1, 0, 0);
	vec directRay2 = vec(1, 1, 1).normal();
	ray castRay = ray(&startRay, &directRay);
	ray castRay2 = ray(&startRay2, &directRay2);

	double radius = 1.0;
	colorStruct col = colorStruct(100, 0, 255);
	double lambert = .9;

	sphere ball = sphere(&center, radius, &col, lambert);
	sphere ball2 = sphere(&center2, radius, &col, lambert);
	sphere ball3 = sphere(&center3, radius, &col, lambert);

	double distance = ball2.intersectTrue(castRay);
	double distance2 = ball3.intersectTrue(castRay);
	double distance3 = ball3.intersectTrue(castRay2);

	REQUIRE(ball.getCenter()->x == center.x);
	REQUIRE(ball.getCenter()->y == center.y);
	REQUIRE(ball.getCenter()->z == center.z);
	REQUIRE(distance == 1);
	REQUIRE(distance2 == -1.0);
	REQUIRE(distance3 > 2.46);
	REQUIRE(distance3 < 2.47);


}


TEST_CASE("Test sceneObj class ", "[scene]") {

	REQUIRE_THROWS(scene("/vagrant/tests/sce.json"));
	REQUIRE_THROWS(scene("/vagrant/tests/scene3.json"));
	REQUIRE_THROWS(scene("/vagrant/tests/scene4.json"));
	REQUIRE_THROWS(scene("/vagrant/sceneNegLight.json"));
	scene sceneObj1 = scene("/vagrant/tests/scene1.json");

	std::list<light*> lighting = sceneObj1.getLight();
	std::list<objects*> actors = sceneObj1.getActors();
	const camera * cam = sceneObj1.getCam();


	REQUIRE(lighting.front()->intensity == 1);
	REQUIRE(lighting.front()->location.x == 5);
	REQUIRE(lighting.front()->location.y == -5);
	REQUIRE(lighting.front()->location.z == 0);

	REQUIRE(actors.front()->getCenter()->x == 0);
	REQUIRE(actors.front()->getCenter()->y == 0);
	REQUIRE(actors.front()->getCenter()->z == 5);

	REQUIRE(actors.back()->getCenter()->x == 0);
	REQUIRE(actors.back()->getCenter()->y == 5);
	REQUIRE(actors.back()->getCenter()->z == 0);

	REQUIRE(cam->center.x == 0);
	REQUIRE(cam->focus == 10);
	REQUIRE(cam->normal.z == 1);
}

TEST_CASE("Test plane class intersectTrue", "[plane], [intersectTrue]") {

	vec center = vec(0, 0, 0);
	vec normal = vec(1, 0, 0);
	vec startRay = vec(2, 1, 0);
	vec startRay2 = vec(1, 1, 1);
	vec startRay3 = vec(3, 0, 0);
	vec directRay = vec(-1, 0, 0);
	vec directRay3 = vec(-3, -4, 0);
	vec directRay4 = vec(3, 4, 0);
	colorStruct col = colorStruct(100, 0, 255);
	double lambert = .9;
	plane plane1 = plane(&center, &normal, &col, lambert);
	ray castRay = ray(&startRay, &directRay);
	ray castRay2 = ray(&startRay2, &directRay);
	ray castRay3 = ray(&startRay3, &directRay3);
	ray castRay4 = ray(&startRay3, &directRay4);

	double dist = plane1.intersectTrue(castRay);
	double dist2 = plane1.intersectTrue(castRay2);
	double dist3 = plane1.intersectTrue(castRay3);
	double dist4 = plane1.intersectTrue(castRay4);
	REQUIRE(dist == 2);
	REQUIRE(dist2 == 1);
	REQUIRE(dist3 == 5);
	REQUIRE(dist4 < 0);
}

TEST_CASE("Test plane class intersect", "[plane], [intersect]") {

	// intersect(const std::list<objects*> actors, const std::list<light*> lights, const ray traceRay, double dist)
	vec center1 = vec(0, 0, 0);
	vec normal1 = vec(1, 0, 0);

	vec center2 = vec(2, 0, 0);
	vec normal2 = vec(1, 0, 0);

	vec center3 = vec(100, 0, 0);
	vec normal3 = vec(1, 0, 0);

	vec startRay1 = vec(1, 0, 0);
	vec startRay2 = vec(3, 0, 0);

	vec directRay1 = vec(-1, 0, 0);
	vec directRay2 = vec(-1, 0, 0);

	colorStruct col = colorStruct(100, 0, 255);
	double lambert = .5;
	plane plane1 = plane(&center1, &normal1, &col, lambert);
	plane plane2 = plane(&center2, &normal2, &col, lambert);
	plane plane3 = plane(&center3, &normal3, &col, lambert);

	ray castRay = ray(&startRay1, &directRay2);
	ray castRay2 = ray(&startRay2, &directRay2);

	light l1 = light(5, 0, 0, 1);
	std::list<light*> lights;
	lights.push_back(&l1);

	std::list<objects*> actors;
	actors.push_back(&plane1);
	actors.push_back(&plane2);




	colorStruct * colorA = plane1.intersect(actors, lights, castRay, plane1.intersectTrue(castRay), &plane3);
	colorStruct * colorB = plane2.intersect(actors, lights, castRay2, plane1.intersectTrue(castRay2), &plane3);
	REQUIRE(colorA->r == 0);
	REQUIRE(colorB->r == 0);

	REQUIRE(colorA->g == 0);
	REQUIRE(colorB->g == 0);

	REQUIRE(colorA->b == 0);
	REQUIRE(colorB->b == 0);
	delete colorA;
	delete colorB;
	
}

TEST_CASE("Test sphere class intersect", "[sphere], [intersect]") {

	// intersect(const std::list<objects*> actors, const std::list<light*> lights, const ray traceRay, double dist)
	vec center1 = vec(0, 0, 0);
	double raid = .1;

	vec center2 = vec(2, 0, 0);

	vec startRay1 = vec(1, 0, 0);
	vec startRay2 = vec(3, 0, 0);

	vec directRay1 = vec(-1, 0, 0);
	vec directRay2 = vec(-1, 0, 0);

	vec center3 = vec(100, 0, 0);
	vec normal3 = vec(1, 0, 0);


	colorStruct col = colorStruct(100, 0, 255);
	double lambert = .5;
	sphere sphere1 = sphere(&center1, raid, &col, lambert);
	sphere sphere2 = sphere(&center2, raid, &col, lambert);
	plane plane3 = plane(&center3, &normal3, &col, lambert);

	ray castRay = ray(&startRay1, &directRay2);
	ray castRay2 = ray(&startRay2, &directRay2);

	light l1 = light(5, 0, 0, 1);
	std::list<light*> lights;
	lights.push_back(&l1);

	std::list<objects*> actors;
	actors.push_back(&sphere1);
	actors.push_back(&sphere2);




	colorStruct * colorA = sphere1.intersect(actors, lights, castRay, sphere1.intersectTrue(castRay), &plane3);
	colorStruct * colorB = sphere2.intersect(actors, lights, castRay2, sphere2.intersectTrue(castRay2), &plane3);
	REQUIRE(colorA->r == 0);
	REQUIRE(colorB->r == 50);

	REQUIRE(colorA->g == 0);
	REQUIRE(colorB->g == 0);

	REQUIRE(colorA->b == 0);
	REQUIRE(colorB->b == int(255 * lambert));
	delete colorA;
	delete colorB;


}