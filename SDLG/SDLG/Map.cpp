#include "Map.h"
#include <fstream>

int lvl1[45][55];
Map::Map()
{
	
	CreateLevel();
	LoadTextures();
	rows = (sizeof(lvl1) / sizeof(lvl1[0]));
	columns = (sizeof(lvl1[0]) / sizeof(int));
	LoadMap(lvl1);

	src.x = src.y = 0;
	tileSizes = src.w = dest.w = 64;
	src.h = dest.h = 64;

	dest.x = dest.y = 0;
	loading = 0;
	number1 = 0;
	obstacleTiles = 0;
}

// Creates map based of text file.
void Map::CreateLevel()
{
	std::ifstream lm("Maps/LargeMap.txt");

	if (!lm)
		printf("File could not be opened");

	for (int row = 0; row < 45; row++)
	{
		for (int column = 0; column < 55; column++)
		{
			lm >> lvl1[row][column];
			if (!lm)
				printf("error reading file for element %d, %d\n", row, column);
		}
	}
}

// Load all Textures. All are located inside project folder.
void Map::LoadTextures()
{
	grass = Source::loadTexture("Assets/Landscape/Grass.png");
	grassyArea = Source::loadTexture("Assets/Landscape/GrassyArea.png");
	rockGrass = Source::loadTexture("Assets/Obstacles/RockGrass.png");
}

// Create tile map
void Map::LoadMap(int arr[45][55])
{

	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			map[row][column] = arr[row][column];
			
		}
	}
}

// Determines how many of the tiles are obstacles
void Map::countObstacleTiles()
{
	int type = 0;
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			type = map[row][column];


			switch (type)
			{
			case 2:
				obstacleTiles++;
				break;
			}
		}
	}
}

// Draws the map
void Map::DrawMap()
{

	int type = 0;
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			type = map[row][column];

			dest.x = column * tileSizes;
			dest.y = row * tileSizes;

			switch (type)
			{
			case 0:
				MainClass::Draw(grass, src, dest);
				break;
			case 1:
				MainClass::Draw(grassyArea, src, dest);
				break;
			case 2:
				MainClass::Draw(rockGrass, src, dest);

				if (loading <= obstacleTiles - 1) // number of scenery items
				{
					rockBody = obstaclePhysics.makeStaticReturn(dest.x, dest.y, 0,0);
					loading++;
				}

				// Sets position of rock
				//rockBody->SetTransform(b2Vec2((float)dest.x - 35, (float)dest.y - 40), rockBody->GetAngle());
				break;
			}
		}
	}

}

// Create physics body for rocks
void Map::createPhysics(SDL_Rect src, SDL_Rect dest)
{
	obstaclePhysics.makeStatic((float)dest.x, (float)dest.y, (float)src.w+ 100, (float)src.h);
}

// Deconstructor method
Map::~Map()
{
	SDL_DestroyTexture(grass);
	SDL_DestroyTexture(grassyArea);
}