#include "objects.hpp"
#include "plane.hpp"
#include "sphere.hpp"
#include "sceneObj.hpp"
#include <vector>
#include <typeinfo>
#include <QImage>
#include <QString>
#include <QColor>
#include <ctime>


int main(int argc, char *argv[])
{
	std::clock_t start;
	double duration;
	start = std::clock();

	std::vector<std::string>  arguments;
	std::string jsonFileName;
	std::string pngFileName;
	int threads = 1;
	for (int i = 0; i < argc; ++i)
	{
		arguments.push_back(argv[i]);
		//std::cout << i << " - " << arguments[i] << " - " << typeid(arguments[i]).name() << " " << argc << '\n';
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

	std::cout << "T " << threads << '\n';
	std::cout << "jF " << jsonFileName << '\n';
	std::cout << "pF " << pngFileName << '\n';

	const int sizeX = stage->getCam()->sizeX;
	const int sizeY = stage->getCam()->sizeY;
	const double resX = stage->getCam()->resX;
	const double resY = stage->getCam()->resY;
	//std::cout << resX << '\n';
	const camera * cam = stage->getCam();
	vec center = cam->center;
	std::list<objects*> actors = stage->getActors();
	std::list<light*> lights = stage->getLight();
	vec normal = cam->normal;
	colorStruct blank = colorStruct(0, 0, 0);
	plane screen = plane(&center, &normal, &blank, .5);
	QImage image = QImage(sizeX, sizeY, QImage::Format_RGB888);
	std::list<colorStruct *> colors;
	int maxValue = -1;
	double dist = -1;
	double minDist = -1;
	colorStruct * colorFromActor;
	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			vec startPoint = stage->getCam()->focPoint;
			vec screenPoint = vec(center.x + (i - sizeX/2) * resX, center.y + (j - sizeY / 2) * resY, center.z);
			vec direct = vec(screenPoint.x - startPoint.x, screenPoint.y - startPoint.y, screenPoint.z - startPoint.z).normal();
			ray castRay = ray(&startPoint, &direct);
			//std::cout << castRay.direction.x << ' ' << castRay.direction.y << ' '<< castRay.direction.z << '\n';
			objects * actor = 0;
			minDist = -1;
			for (auto const& obj : actors) {
				dist = obj->intersectTrue(castRay);
				//std::cout << dist << ' ' << minDist << '\n';
				if (dist < 0 || (minDist > 0 && minDist < dist))
				{
					continue;
				}
				else
				{
					actor = obj;
					minDist = dist;
				}
			}
			if (minDist > 0)
			{
				//std::cout << "after108\n"; 
				//std::cout << actor->getCenter()->x << '\n';
				//std::cout << i << " " << j << '\n';
				
				colorFromActor = actor->intersect(actors, lights, castRay, minDist, &screen);
				//std::cout << "after110\n";
				if (colorFromActor->r > maxValue)
				{
					maxValue = colorFromActor->r;
				}
				if (colorFromActor->g > maxValue)
				{
					maxValue = colorFromActor->g;
				}
				if (colorFromActor->b > maxValue)
				{
					maxValue = colorFromActor->b;
				}
				colors.push_back(colorFromActor);
			}
			else
			{
				colors.push_back(new colorStruct(0,0,0));
			}

		}
	}
	int r;
	int g;
	int b;
	double expose = 255.0 / double(maxValue);
	//std::cout << maxValue << '\n';
	//std::cout << expose << '\n';
	//std::cout << 255.0/183.0 << '\n';
	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			
			QColor col = QColor(int(colors.front()->r * expose), int(colors.front()->g  * expose), int(colors.front()->b  * expose));
			image.setPixel(i, j, col.rgb());
			colors.pop_front();
		}
	}

	image.save(QString::fromStdString(pngFileName), "PNG");
	delete stage;
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "printf: " << duration << '\n';
	return EXIT_SUCCESS;
}