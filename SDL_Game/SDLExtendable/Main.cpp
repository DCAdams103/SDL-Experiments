#include <SDL.h>
#include <stdio.h>
#include "Main.h"
#include "Map.h"


using namespace std;
Map* map;

SDL_Rect MainClass::camera = { 0, 0, 1920, 1080};

// Screen Width = 1920
// Screen Height = 1080

SDL_Texture* MainClass::LoadTexture(string path)
{
	SDL_Texture* passTexture = Source::loadTexture(path);
	return passTexture;
}

SDL_Rect MainClass::createRect(float x, float y, int width, int height, bool offset)
{
	SDL_Rect rect;
	if (offset)
		rect = { static_cast<int>(x) - camera.x, static_cast<int>(y) - camera.y, width, height };
	else
		rect = {static_cast<int>(x), static_cast<int>(y), width, height };

	return rect;
}

void MainClass::render()
{
	// Load only once
	if (loadCounter == 0)
	{
		player1 = physics2.player.getTexture();

		loadCounter++;

		physics2.playerDynamic(Source::SCREEN_WIDTH / 2, Source::SCREEN_HEIGHT / 2, 64, 64);

		map = new Map();

		levelWidth = map->columns * map->tileSizes;
		levelHeight = map->rows * map->tileSizes;

		map->countObstacleTiles();
	}

	player1 = Player::getTexture();

	camera.x = (physics2.body->GetPosition().x) - (Source::SCREEN_WIDTH / 2);
	camera.y = (physics2.body->GetPosition().y) - (Source::SCREEN_HEIGHT / 2);

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;

	if (camera.x + camera.w >= levelWidth)
		camera.x = levelWidth - Source::SCREEN_WIDTH;
	if (camera.y + camera .h >= levelHeight ) 
		camera.y = levelHeight - Source::SCREEN_HEIGHT;

	physics2.moveBodies();

	SDL_Rect playerRect = createRect(physics2.posX, physics2.posY, 50, 50, false);
	SDL_SetRenderDrawColor(Source::gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	drawingRect = { (playerRect.x - camera.x), (playerRect.y - camera.y), playerRect.w, playerRect.h };

	SDL_RenderClear(Source::gRenderer);
	map->DrawMap();

	SDL_RenderCopy(Source::gRenderer, player1, NULL, &drawingRect);

	SDL_RenderPresent(Source::gRenderer);
}

void MainClass::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest) 
{
	SDL_Rect destOffset;
	destOffset.x = dest.x - camera.x;
	destOffset.y = dest.y - camera.y;
	destOffset.w = dest.w;
	destOffset.h = dest.h;

	SDL_RenderCopy(Source::gRenderer, tex, &src, &destOffset);
	
}