#include "sceneObj.hpp"


scene::scene(std::string fileLocation)
{
	QString fName = QString::fromStdString(fileLocation);
	QFile file(fName);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		throw std::invalid_argument("Error:File Failed to Open");
	}
	else
	{
		bool sucessfulParse;
		QByteArray jsonRaw = file.readAll();
		QJsonParseError *err = new QJsonParseError();
		QJsonDocument json = QJsonDocument::fromJson(jsonRaw, err);
		if (err->error != 0 || json.isNull())
		{
			throw std::invalid_argument("Error:Parse Failed");
		}
		else
		{
			jObj = json.object();
			addLight();
			addActors();
			addCamera();
		}


	}
	

}

scene::~scene()
{

	while (!lighting.empty()) delete lighting.front(), lighting.pop_front();
	while (!actorList.empty()) delete actorList.front(), actorList.pop_front();
	delete stage;
}

std::list<light*> scene::getLight()
{
	return lighting;
}

std::list<objects*> scene::getActors()
{
	return actorList;
}

const camera * scene::getCam()
{
	return stage;
}

void scene::addLight()
{
	QJsonArray lightArray = jObj["lights"].toArray();
	double x, y, z, intens;
	int numLights = lightArray.size();
	for (int i = 0; i < numLights; i++)
	{
		QJsonObject location = lightArray[i].toObject()["location"].toObject();
		intens = lightArray[i].toObject()["intensity"].toDouble();
		if (intens < 0 || !location["x"].isDouble() || !location["y"].isDouble() || !location["z"].isDouble())
		{
			throw std::invalid_argument("Error:Invalid light");
		}
		x = location["x"].toDouble();
		y = location["y"].toDouble();
		z = location["z"].toDouble();
		lighting.push_back(new light(x, y, z, intens));
	}

}

void scene::addActors()
{
	int r, g, b;
	double rD, gD, bD;
	double x, y, z;
	QJsonArray actorArray = jObj["objects"].toArray();
	int numActors = actorArray.size();
	for (int i = 0; i < numActors; i++) {
		QJsonObject actor = actorArray[i].toObject();
		QJsonObject colorStruct = actor["color"].toObject();
		QJsonObject center = actor["center"].toObject();
		double lambert = actor["lambert"].toDouble();
		rD = colorStruct["r"].toDouble();
		gD = colorStruct["g"].toDouble();
		bD = colorStruct["b"].toDouble();
		r = colorStruct["r"].toInt();
		g = colorStruct["g"].toInt();
		b = colorStruct["b"].toInt();
		if (r != rD || g != gD || b != bD || r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		{
			throw std::invalid_argument("Error:Invalid Object colorStruct");
		}
		if (!center["x"].isDouble() || !center["y"].isDouble() || !center["z"].isDouble())
		{
			throw std::invalid_argument("Error:Invalid Object center");
		}
		if (lambert > 1 || lambert < 0){
			throw std::invalid_argument("Error:Invalid Object lambert");
		}
		x = center["x"].toDouble();
		y = center["y"].toDouble();
		z = center["z"].toDouble();
		if (actor["type"].toString().compare("sphere") == 0)
		{
			double radius = actor["radius"].toDouble();
			if (radius <= 0){
				throw std::invalid_argument("Error:Invalid sphere radii");
			}
			vec cen = vec(x, y, z);
			::colorStruct col = ::colorStruct(r, g, b);
			sphere * obj = new sphere(&cen, radius, &col, lambert);
			actorList.push_back(obj);
		}
		else if (actor["type"].toString().compare("plane") == 0){
			QJsonObject normal = actor["center"].toObject();
			if (!normal["x"].isDouble() || !normal["y"].isDouble() || !normal["z"].isDouble()){
				throw std::invalid_argument("Error:Invalid surface normal");
			}
			double xN = normal["x"].toDouble();
			double yN = normal["y"].toDouble();
			double zN = normal["z"].toDouble();
			vec cen = vec(x, y, z);
			vec norm = vec(xN, yN, zN);
			::colorStruct col = ::colorStruct(r, g, b);
			plane * obj = new plane(&cen, &norm, &col, lambert);
			actorList.push_back(obj);
		}
		else{
			throw std::invalid_argument("Error:Invalid Object");
		}
	}
}

void scene::addCamera()
{

	QJsonObject camera = jObj["camera"].toObject();
	QJsonObject center = camera["center"].toObject();
	double focus = camera["focus"].toDouble();
	QJsonObject normal = camera["normal"].toObject();
	QJsonArray resolution = camera["resolution"].toArray();
	QJsonArray size = camera["size"].toArray();

	double camX, camY, camZ;
	double normX, normY, normZ;
	int sizeX, sizeY;
	double resX, resY;

	camX = center["x"].toDouble();
	camY = center["y"].toDouble();
	camZ = center["z"].toDouble();

	normX = normal["x"].toDouble();
	normY = normal["y"].toDouble();
	normZ = normal["z"].toDouble();

	sizeX = size[0].toInt();
	sizeY = size[1].toInt();
	
	resX = resolution[0].toDouble();
	resY = resolution[1].toDouble();
	if (resX < 0 || resY < 0 || sizeX < 0 || sizeY < 0)
	{
		throw std::invalid_argument("Error:Invalid Camera");
	}

	vec cent = vec(camX, camY, camZ);
	vec norm = vec(normX, normY, normZ);
	stage = new	::camera(&cent, &norm, focus, resX, resY, sizeX, sizeY);


}