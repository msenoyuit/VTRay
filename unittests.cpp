#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"
#include <cstdio>

#include "objects.hpp"

TEST_CASE( "Test Color struct ", "[objects], [color]" ) {

	color red = color(255, 0, 0);
	color green = color(0, 255, 0);
	color blue = color(0, 0, 255);
	

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

	REQUIRE(300.0 == vecA.dotProduct(vecB));
}

TEST_CASE("Test ray struct ", "[objects], [ray]") {

	vec vec1 = vec(1, 2, 3);
	vec vec2 = vec(.1, .2, .003);
	ray ray1 = ray(vec1, vec2);

	
	REQUIRE(ray1.origin.x == 1);
	REQUIRE(ray1.origin.y == 2);
	REQUIRE(ray1.origin.z == 3);

	REQUIRE(ray1.direction.x == .1);
	REQUIRE(ray1.direction.y == .2);
	REQUIRE(ray1.direction.z == .003);

}

TEST_CASE("Test light struct ", "[objects], [light]") {

	vec orgin = vec(1, 2, 3);
	double intens = 1.1;
	light light1 = light(orgin, intens);

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
