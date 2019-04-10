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
	// Used to determine if the window was created successfully
	bool success = true;
	gWindow = NULL;
	gScreenSurface = NULL;

	// Checks to see if it can Initialize
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Could not initialize SDL");
		success = false;
	}
	else
	{
		// Creates window with given name, width and height
		gWindow = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Source::SCREEN_WIDTH, Source::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		// Sets the window to fullscreen
		SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
		// Grabs size of monitor it is displaying to
		SDL_GetCurrentDisplayMode(0, &DM);
		// Sets window width and height to said size
		Source::SCREEN_HEIGHT = DM.h;
		Source::SCREEN_WIDTH = DM.w;

		if (gWindow == NULL)
		{
			printf("Window could not be created");
			success = false;
		}
		else
		{
			// Creates the renderer
			Source::gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (Source::gRenderer == NULL)
			{
				printf("Renderer could not be created");
				success = false;
			}
			else
			{
				// Sets the color. White in this case.
				SDL_SetRenderDrawColor(Source::gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}

			// Sets the surface to the windows surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
	
	return success;

}

// Call this to load a picture into a texture.
SDL_Texture* Source::loadTexture(string path)
{
	SDL_Texture* newTexture = NULL;

	// Calls IMG_Load to find picture
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image");
	}
	else
	{
		// Creates texture and stores it in newTexture
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		// Frees loadedSurface from memory
		SDL_FreeSurface(loadedSurface);
	}

	// Returns texture
	return newTexture;

}

// Calls when the window closes. It will free the surfaces, textures and window.
void Source::close()
{
	SDL_FreeSurface(gScreenSurface);
	gScreenSurface = NULL;

	SDL_DestroyTexture(image);
	image = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	// Stops running SDL
	SDL_Quit();
}

// Main method, called first
int main(int argc, char* args[])
{
	// Set fps and frame delay
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
		// flag to determine when to quit SDL
		bool quit = false;

		// Creates event for user input and physics from Box2D.
		SDL_Event e;
		Physics physics;
		while (!quit)
		{
			// SDL_GetTicks() will return the number of milliseconds since SDL intialized
			frameStart = SDL_GetTicks();

			// Quits only once the window is closed.
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}

			}
			
			// Renders everything onto screen
			mainClass.render();
			
			// Stores which key is being pressed
			const Uint8 *keystates = SDL_GetKeyboardState(NULL);

			// Closes the window and SDL if the Escape key is pressed. Easier to exit when in fullscreen
			if (keystates[SDL_SCANCODE_ESCAPE])
				quit = true;

			// Checks for certain key presses and allows for character movement.
			physics.move(keystates);
			
			// Sets how fast the game should update based on when it last updated.
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

