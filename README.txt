Put Your design decisions here. What modules you defined and how they
interact to solve the ray tracing problem.

Overall
	use a thread pool to work on a picture line by line based on what you did in class
	It goes line by line because you said breaking it up smaller caused waiting time to go up.
	The json parse was split into actor, camera, and light funtions.


vtray.cpp
	This is the main file. It invokes the parsing, 
	sets and runs the threads, and builds the picture using qt.

unittests.cpp
uses catch to test the functions in the other modules.


plane.cpp/plane.hpp/sphere.cpp/sphere.hpp
	inherits from the object class and implements its functons
	intersectTrue 
		takes a ray and determins if the object(itself) is hit by the array. 
		If it is it returns the distance from the starting point to the hit point. 
		If it missed it returns -1
		I use the returned distance to determin which object was hit first if there are more than one hit. 
	
	intersect
		this takes a ray and a distace and uses it to find the point of impact on the object.
		then it traces to each light source, if it hits an object it is blocked, if it hits the light or the screen first
		then it is a hit. if its a hit you add that light to the total light coming off the point. and return that color.
		if blocked then check the other lights.
	
objects.hpp
	this contains the structs for the varous data types
	also contains the header for object class with virtual functions

sceneObj.cpp/sceneObj.hpp
	contains all the code for parsing a json using qt
	it also builds the set(objects, lights, camera) and has getters to acess them
	
message_queue.h
	its the file provided in class.
	
threads.hpp
	it is based on a file provided in class for a thread pool.
	only thing changed is that the work it takes in are rows of rays from the picture


	