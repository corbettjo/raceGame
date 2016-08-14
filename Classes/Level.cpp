#include "Level.h"
#include "globals.h"

void Level::loadMap(const char* mapname)
{
	levelMap = TMXTiledMap::create(mapname);
	levelMap->retain();
}

TMXTiledMap * Level::getMap()
{
	return levelMap;
}

Point Level::tileCoordinateToPosition(Point point) //CONVERTS A TILE COORDINATE TO A POINT
{
	float x = floor(point.x * levelMap->getTileSize().width * 2.0f);
	float y = floor(point.y * levelMap->getTileSize().height * 2.0f);

	return Point(x, y);
}

Point Level::positionToTileCoordinate(Point point) //CONVERTS A POSITION TO A TILE COORDINATE
{
	float x = floor(point.x / (levelMap->getTileSize().width * 2.0f)); //Rounds down to nearest tile x coordinate
	float y = floor(point.y / (levelMap->getTileSize().height * 2.0f)); //Rounds down to nearest tile y coordinate

	return Point((int)x, (int)y);
}

vector<Rect> Level::getCollisionTiles() //OBTAINS ALL TILES ON THE MAP LAYER 
{
	vector<Rect> fullList; //Declares vector for rectangle objects

	TMXLayer *walls = levelMap->getLayer("walls"); //makes TMXLayer pointer to be the walls layer of the .tmx file

	int mapheight = (int)levelMap->getMapSize().height - 1; //999
	int mapwidth = (int)levelMap->getMapSize().width - 1; //74

	//DOUBLE FOR LOOP TO OBTAIN TILES
	for (int x = 0; x < mapwidth; x++)
	{
		for (int y = 0; y < mapheight; y++)
		{
			Sprite *tile = walls->getTileAt(Point(x, y)); //Sets sprite "tile" to be the current tile coordinates being checked

			if (tile != NULL) //If the tile is not empty...
			{
				Rect tileRect = Rect();

				Point temp = walls->positionAt(Point(x, y)); //Set temp point to be the point being checked

				tileRect.setRect(temp.x * 2.0f, temp.y * 2.0f, levelMap->getTileSize().width * 2.0f, levelMap->getTileSize().height * 2.0f); //Set rect "tileRect" to be the scaled point

				fullList.push_back(tileRect); //Add that tile to the list
			}
		}
	}
	return fullList; //Return the full list of tiles 
}

Level::Level(void)
{
}

Level::~Level(void)
{
	levelMap->release();
}