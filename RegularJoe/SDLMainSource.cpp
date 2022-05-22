/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//System Inlcudes
#include <SDL.h>
#include <stdio.h>
#include <iostream>

//Header Includes
#include "Constants.h"
#include "Commons.h"

//Internal Function Declarations
bool Init(); // Starts up SDL and creates window
bool LoadMedia(); // Loads media
void Close(); //Frees media and shuts down SDL
SDL_Surface* LoadSurface(std::string path); //Loads individual image

////Global Variables
//SDL_Window* g_window = NULL; //The window we'll be rendering to
//SDL_Surface* g_screenSurface = NULL; //The surface contained by the window
//SDL_Surface* g_helloWorld = NULL; //The image we will load and show on the screen

SDL_Window* g_window = NULL; //The window we'll be rendering to
SDL_Surface* g_screenSurface = NULL; //The surface contained by the window
SDL_Surface* g_keyPressSurfaces[KEY_PRESS_SURFACE_TOTAL]; //The images that correspond to a keypress
SDL_Surface* g_currentSurface = NULL; //Current displayed image

int main(int argc, char* args[])
{
	if (!Init()) //Start up SDL and create window
	{
		std::cerr << "Failed to initialize!\n";
	}
	else
	{
		if (!LoadMedia()) //Load media
		{
			std::cerr << "Failed to load media!\n";
		}
		else
		{
			bool l_quit = false; //Main loop flag

			SDL_Event e; //Event handler

			
			g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT]; //Set default current surface

			while (!l_quit) //While application is running
			{
				while (SDL_PollEvent(&e) != 0) //Handle events on queue
				{
					if (e.type == SDL_QUIT) //User requests quit
					{
						l_quit = true;
					}
					//User presses a key
					else if (e.type == SDL_KEYDOWN)
					{
						//Select surfaces based on key press
						switch (e.key.keysym.sym)
						{
						case SDLK_UP:
							g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_UP];
							break;

						case SDLK_DOWN:
							g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
							break;

						case SDLK_LEFT:
							g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
							break;

						case SDLK_RIGHT:
							g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
							break;

						default:
							g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
							break;
						}
					}
				}

				SDL_BlitSurface(g_currentSurface, NULL, g_screenSurface, NULL); //Apply the image

				SDL_UpdateWindowSurface(g_window); //Update the surface
			}
		}
	}

	Close(); //Free Resources and close SDL

	return 0;
}


bool Init()
{
	bool success = true; //Initialization check

	if (SDL_Init(SDL_INIT_VIDEO) < 0) //Initialize SDL
	{
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError();
		success = false;
	}
	else
	{
		g_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); //Create window
		if (g_window == NULL)
		{
			std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError();
			success = false;
		}
		else
		{
			g_screenSurface = SDL_GetWindowSurface(g_window); //Get window surface
		}
	}

	return success;
}

bool LoadMedia()
{
	//Loading success flag
	bool success = true;

	//Load default surface
	g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = LoadSurface("Assets/Textures/MenuScreen.bmp");
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		std::cerr << "Failed to load default image!\n";
		success = false;
	}

	//Load up surface
	g_keyPressSurfaces[KEY_PRESS_SURFACE_UP] = LoadSurface("Assets/Textures/Up.bmp");
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
	{
		std::cerr << "Failed to load up image!\n";
		success = false;
	}

	//Load down surface
	g_keyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = LoadSurface("Assets/Textures/Down.bmp");
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		std::cerr << "Failed to load down image!\n";
		success = false;
	}

	//Load left surface
	g_keyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = LoadSurface("Assets/Textures/Left.bmp");
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		std::cerr << "Failed to load left image!\n";
		success = false;
	}

	//Load right surface
	g_keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = LoadSurface("Assets/Textures/Right.bmp");
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
	std::cerr << "Failed to load right image!\n";
		success = false;
	}

	return success;
}

void Close()
{
	SDL_FreeSurface(g_currentSurface); //Deallocate surface
	g_currentSurface = NULL;

	SDL_DestroyWindow(g_window); //Destroy window
	g_window = NULL;

	SDL_Quit(); //Quit SDL subsystems
}

SDL_Surface* LoadSurface(std::string path)
{
	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL)
	{
		std::cerr << "Unable to load image SDL Error: " << path.c_str() << SDL_GetError();
	}

	return loadedSurface;
}