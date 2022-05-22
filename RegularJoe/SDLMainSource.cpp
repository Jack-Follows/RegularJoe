/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Starts up SDL and creates window
bool Init();

// Loads media
bool LoadMedia();

//Frees media and shuts down SDL
void Close();

/* !v!v! Global Variables !v!v! */

//The window we'll be rendering to
SDL_Window* g_window = NULL;

//The surface contained by the window
SDL_Surface* g_screenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* g_helloWorld = NULL;

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!Init())
	{
		std::cerr << "Failed to initialize!\n";
	}
	else
	{
		//Load media
		if (!LoadMedia())
		{
			std::cerr << "Failed to load media!\n";
		}
		else
		{
			//Main loop flag
			bool l_quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while (!l_quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						l_quit = true;
					}
				}

				//Apply the image
				SDL_BlitSurface(g_helloWorld, NULL, g_screenSurface, NULL);

				//Update the surface
				SDL_UpdateWindowSurface(g_window);
			}
		}
	}

	//Free Resources and close SDL
	Close();

	return 0;
}


bool Init()
{
	//Initialization check
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError();
		success = false;
	}
	else
	{
		//Create window
		g_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (g_window == NULL)
		{
			std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError();
			success = false;
		}
		else
		{
			//Get window surface
			g_screenSurface = SDL_GetWindowSurface(g_window);
		}
	}

	return success;
}

bool LoadMedia()
{
	//Loading success flag
	bool success = true;

	//Load splash image
	g_helloWorld = SDL_LoadBMP("Assets/Textures/MenuScreen.bmp");
	if (g_helloWorld == NULL)
	{
		std::cerr << "Unable to load image SDL Error: " << "Assets/Textures/MenuScreen.bmp" << SDL_GetError();
		success = false;
	}

	return success;
}

void Close()
{
	//Deallocate surface
	SDL_FreeSurface(g_helloWorld);
	g_helloWorld = NULL;

	//Destroy window
	SDL_DestroyWindow(g_window);
	g_window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}