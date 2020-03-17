#ifndef _MAIN_H
#define _MAIN_H
#include "Source.h"
#include "Player.h"
#include <string>
#include <Box2D/Box2D.h>
#include "Physics.h"

using namespace std;

class MainClass
{
public:

	void render();
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);

	SDL_Rect createRect(float, float, float, float);
	SDL_Texture* LoadTexture(string);
	SDL_Texture* player1 = NULL;

	int loadCounter = 0;
	int levelWidth;
	int levelHeight;

	static SDL_Rect camera;
	SDL_Rect drawingRect;
	b2Vec2 position;

	Physics physics;
	Physics physics1;
	Physics physics2;
	

private:

};

#endif
