#ifndef _MAP_H
#define _MAP_H
#include <SDL.h>
#include "Main.h"

class Map
{
public:
	
	Map();
	~Map();
	
	int rows;
	int columns;
	void CreateLevel();
	void LoadTextures();
	void LoadMap(int arr[45][55]);
	void DrawMap();
	int tileSizes;
	int loading;
	int number1;
	int obstacleTiles;
	bool test;
	void countObstacleTiles();
	void loadObstacles(int number);

	SDL_Texture *grass;
	SDL_Texture *grassyArea;
	SDL_Texture *rockGrass;

	Physics obstaclePhysics;

	b2Body* rockBody;

private:
	SDL_Rect src, dest;
	void createPhysics(SDL_Rect src, SDL_Rect dest);
	int map[45][55];

};

#endif