#ifndef _SOURCE_H
#define _SOURCE_H
#include <string>
#include "Main.h"

using namespace std;

class Source
{
public:
	static SDL_Renderer * gRenderer;
	static SDL_Texture* loadTexture(string path);
	static void close();

	static int SCREEN_HEIGHT;
	static int SCREEN_WIDTH;

private:

};

#endif 

