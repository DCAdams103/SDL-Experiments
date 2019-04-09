#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "Source.h"
#include <string>

#include "Player.h"

using namespace std;
bool init();

SDL_DisplayMode DM;

MainClass mainClass;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Texture* image = NULL;
SDL_Renderer* Source::gRenderer = NULL;

int Source::SCREEN_HEIGHT = 640;
int Source::SCREEN_WIDTH = 800;

bool init()
{
	bool success = true;
	gWindow = NULL;
	gScreenSurface = NULL;

	

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Could not initialize SDL");
		success = false;
	}
	else
	{

		gWindow = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Source::SCREEN_WIDTH, Source::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
		SDL_GetCurrentDisplayMode(0, &DM);
		//SDL_RenderSetScale(Source::gRenderer, 100.0f, 0.0f);
		Source::SCREEN_HEIGHT = DM.h;
		Source::SCREEN_WIDTH = DM.w;

		if (gWindow == NULL)
		{
			printf("Window could not be created");
			success = false;
		}
		else
		{
			
			Source::gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (Source::gRenderer == NULL)
			{
				printf("Renderer could not be created");
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(Source::gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}

			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
	
	return success;

}

SDL_Texture* Source::loadTexture(string path)
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image");
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;

}

void Source::close()
{
	SDL_FreeSurface(gScreenSurface);
	gScreenSurface = NULL;

	SDL_DestroyTexture(image);
	image = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

int main(int argc, char* args[])
{

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	if (!init())
	{
		printf("Failed to intialize");
	}
	else
	{
		bool quit = false;

		SDL_Event e;
		Physics physics;
		while (!quit)
		{

			frameStart = SDL_GetTicks();

			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}

			}

			
			mainClass.render();
			
			const Uint8 *keystates = SDL_GetKeyboardState(NULL);

			if (keystates[SDL_SCANCODE_ESCAPE])
				quit = true;

			physics.move(keystates);

			frameTime = SDL_GetTicks() - frameStart;

			if (frameDelay > frameTime)
			{
				SDL_Delay(frameDelay - frameTime);
			}

		}
	}

	Source::close();

	return 0;
}

