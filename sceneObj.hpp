#ifndef SCENE_HEADER
#define SCENE_HEADER

#include <QJsonObject>
#include <QFile>
#include <string>
#include <QTextStream>
#include <QJsonDocument>
#include <iostream>
#include <QJsonArray>
#include "objects.hpp"
#include <list>
#include "sphere.hpp"
#include "plane.hpp"



class scene
{
public:
	scene(std::string fileLocation);
	~scene();
	void debug();
	std::list<light*> getLight();
	std::list<objects*> getActors();
	const camera * getCam();

private:
	void addLight();
	void addActors();
	void addCamera();
	camera * stage = 0;
	QJsonObject jObj;
	std::list<light*> lighting;
	std::list<objects*> actorList;

};






#endif