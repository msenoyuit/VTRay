#include "objects.hpp"
#include "plane.hpp"
#include "sphere.hpp"
#include "sceneObj.hpp"
#include "threads.hpp"
#include "message_queue.h"
#include <vector>
#include <typeinfo>
#include <QImage>
#include <QString>
#include <QColor>
#include <ctime>


int main(int argc, char *argv[])
{
	std::vector<std::string>  arguments;
	std::string jsonFileName;
	std::string pngFileName;
	int threads = 1;
	for (int i = 0; i < argc; ++i)
	{
		arguments.push_back(argv[i]);
	}
	if (argc == 3)
	{
		jsonFileName = arguments[1];
		pngFileName = arguments[2];
	}
	else if (argc == 5 && arguments[1].compare("-t") == 0)
	{
		try{
			threads = std::stoi(arguments[2]);
		}
			catch (...)
		{
			fprintf(stderr, "Error:invalid number of threads\n");
			return EXIT_FAILURE;
		}
		jsonFileName = arguments[3];
		pngFileName = arguments[4];
	}
	else
	{
		fprintf(stderr, "Error:invalid arguments\n");
		return EXIT_FAILURE;
	}

	scene * stage;
	try
	{
		stage = new scene(jsonFileName);
	}
	catch (std::exception &ex)
	{
		fprintf(stderr, "%s", ex.what());
		fprintf(stderr, "%s", "\n");
		return EXIT_FAILURE;
	}
	int sizeX = stage->getCam()->sizeX;
	int sizeY = stage->getCam()->sizeY;
	std::vector< std::vector<ray*> > picture(sizeY);
	const camera * cam = stage->getCam();
	vec center = cam->center;
	std::list<objects*> actors = stage->getActors();
	std::list<light*> lights = stage->getLight();
	vec normal = cam->normal;
	colorStruct blank = colorStruct(0, 0, 0);
	plane screen = plane(&center, &normal, &blank, .5);
	QImage image = QImage(sizeX, sizeY, QImage::Format_RGB888);
	std::vector< std::vector<colorStruct*> > colors(sizeY);
	int maxValue = -1;
	colorStruct * colorFromActor;
	for (int j = 0; j < sizeY; j++)
	{
		for (int i = 0; i < sizeX; i++)
		{
			vec startPoint = stage->getCam()->focPoint;
			vec screenPoint = vec(center.x + (i - sizeX/2) * stage->getCam()->resX, center.y + (j - sizeY / 2) * stage->getCam()->resY, center.z);
			vec direct = vec(screenPoint.x - startPoint.x, screenPoint.y - startPoint.y, screenPoint.z - startPoint.z).normal();
			ray * castRay = new ray(&startPoint, &direct);
			//std::cout << castRay.direction.x << ' ' << castRay.direction.y << ' '<< castRay.direction.z << '\n';
			picture[j].push_back(castRay);
		}
	}
	MessageQueue inq, outq;
	ThreadPool P(threads, &inq, &outq);
	WorkBase * work;
	for (int j = 0; j < sizeY; j++)
	{
		work = new  WorkBase(&picture, &actors, &lights, &screen, j, sizeX);
		inq.push(work);
	}
	for (int j = 0; j < threads; j++)
	{
		inq.push(nullptr);
	}
	P.joinAll();
	WorkBase * job;
	while(!outq.empty())
	{
		outq.wait_and_pop(job);
		colors[job->getRow()] = *(job->getRowColors());
		if (job->getMaxColor() > maxValue)
		{
			maxValue = job->getMaxColor();
		}
	}
	int r, g, b;
	double expose = 255.0 / double(maxValue);
	for (int j = 0; j < sizeY; j++)
	{		
		for (int i = 0; i < sizeX; i++)
		{
			colorStruct * pixelCol = colors[j][i];
			QColor col = QColor(int(pixelCol->r * expose), int(pixelCol->g  * expose), int(pixelCol->b  * expose));			
			image.setPixel(i, j, col.rgb());		
			delete pixelCol;
			colors[j][i] = 0;
		}
	}
	image.save(QString::fromStdString(pngFileName), "PNG");
	delete stage;
	for (int j = 0; j < sizeY; j++)
	{
		for (int i = 0; i < sizeX; i++)
		{
			delete picture[j][i];
		}
	}
	return EXIT_SUCCESS;
}