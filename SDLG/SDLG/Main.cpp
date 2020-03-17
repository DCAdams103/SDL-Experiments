#include <SDL.h>
#include <stdio.h>
#include "Main.h"
#include "Map.h"


using namespace std;
Map* map;

// Creates the camera rectangle.
SDL_Rect MainClass::camera = { 0, 0, 1920, 1080};

// Screen Width = 1920
// Screen Height = 1080

// Loades texture in from path
SDL_Texture* MainClass::LoadTexture(string path)
{
	SDL_Texture* passTexture = Source::loadTexture(path);
	return passTexture;
}

// Creates a rect based off given variables. 
SDL_Rect MainClass::createRect(float x, float y, float width, float height)
{

	// Creates rect to return
	SDL_Rect rect;
	rect = {static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height) };

	return rect;
}

void MainClass::render()
{
	// Load only once
	if (loadCounter == 0)
	{
		player1 = physics2.player.getTexture();

		// Sets loadCounter to more than 0 to prevent running more than once
		loadCounter++;

		// Creates the player physics object. (x, y, width, height)
		physics2.playerDynamic(Source::SCREEN_WIDTH / 2.0f, Source::SCREEN_HEIGHT / 2.0f, 64.0f, 64.0f);

		// Creates Map object
		map = new Map();

		// Sets map width and height based on their columns/rows * the tilesizes.
		levelWidth = map->columns * map->tileSizes;
		levelHeight = map->rows * map->tileSizes;

		// Determines how many rocks need to be rendered
		map->countObstacleTiles();
	}

	// Gets player texture
	player1 = Player::getTexture();

	// Sets camera to center of center
	camera.x = ((int)physics2.body->GetPosition().x );
	camera.y = ((int)physics2.body->GetPosition().y);

	// Creates bounds and camera
	if (camera.x < 0)
		camera.x = 0;
	if (camera.y  < 0)
		camera.y = 0;

	

	if (camera.x >= (Source::SCREEN_WIDTH / 2) / physics2.MTP)
		camera.x = (int)(physics2.body->GetPosition().x / physics2.PTM) - 1000;

	if (camera.y >= (Source::SCREEN_HEIGHT / 2) / physics2.MTP)
		camera.y = (int)(physics2.body->GetPosition().y / physics2.PTM) - 600;

	if (camera.x + camera.w >= levelWidth)
		camera.x = (levelWidth - Source::SCREEN_WIDTH);
	if (camera.y + camera.h >= levelHeight )
		camera.y = (levelHeight - Source::SCREEN_HEIGHT);

	// Run this to allow the character to be moved
	physics2.moveBodies();

	// Creates player rect based on updated position
	SDL_Rect playerRect = createRect(physics2.posX, physics2.posY, 64, 64);
	SDL_SetRenderDrawColor(Source::gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// This rect focuses the camera on the player. 
	drawingRect = { ((playerRect.x) - camera.x), ((playerRect.y) - camera.y), (playerRect.w), playerRect.h };

	// Clears the current rendering target with specified drawing color (set above)
	SDL_RenderClear(Source::gRenderer);
	// Draws map
	map->DrawMap();

	// Use this to copy the texture to current rendering target
	SDL_RenderCopy(Source::gRenderer, player1, NULL, &drawingRect);

	// Updates the screen
	SDL_RenderPresent(Source::gRenderer);
}

// Renders given texture
void MainClass::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest) 
{
	SDL_Rect destOffset;
	destOffset.x = dest.x - camera.x;
	destOffset.y = dest.y - camera.y;
	destOffset.w = dest.w;
	destOffset.h = dest.h;

	SDL_RenderCopy(Source::gRenderer, tex, &src, &destOffset);
	
}