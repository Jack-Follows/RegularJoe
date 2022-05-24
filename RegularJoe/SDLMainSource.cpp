/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//System Inlcudes
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

//Header Includes
#include "Constants.h"
#include "Commons.h"

//Internal Function Declarations
bool Init(); // Starts up SDL and creates window
bool LoadMedia(); // Loads media
void Close(); //Frees media and shuts down SDL
SDL_Surface* LoadSurface(std::string path); //Loads individual image

//Global Variables
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

			SDL_Event l_event; //Event handler

			
			g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT]; //Set default current surface

			while (!l_quit) //While application is running
			{
				while (SDL_PollEvent(&l_event) != 0) //Handle events on queue
				{
					if (l_event.type == SDL_QUIT) //User requests quit
					{
						l_quit = true;
					}
					
					else if (l_event.type == SDL_KEYDOWN) //User presses a key
					{
						
						switch (l_event.key.keysym.sym) //Select surfaces based on key press
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
	bool l_success = true; //Initialization check

	if (SDL_Init(SDL_INIT_VIDEO) < 0) //Initialize SDL
	{
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError();
		l_success = false;
	}
	else
	{
		g_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); //Create window
		if (g_window == NULL)
		{
			std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError();
			l_success = false;
		}
		else
		{
			//Initialize PNG loading
			int l_imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(l_imgFlags) & l_imgFlags))
			{
				std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError();
				l_success = false;
			}
			else
			{
				//Get window surface
				g_screenSurface = SDL_GetWindowSurface(g_window);
			}
		}
	}

	return l_success;
}

bool LoadMedia()
{
	bool l_success = true; //Loading success flag

	
	g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = LoadSurface("Assets/Textures/MenuScreen.bmp"); //Load default surface
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		std::cerr << "Failed to load default image!\n";
		l_success = false;
	}

	
	g_keyPressSurfaces[KEY_PRESS_SURFACE_UP] = LoadSurface("Assets/Textures/Up.bmp"); //Load up surface
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
	{
		std::cerr << "Failed to load up image!\n";
		l_success = false;
	}

	
	g_keyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = LoadSurface("Assets/Textures/Down.bmp"); //Load down surface
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		std::cerr << "Failed to load down image!\n";
		l_success = false;
	}

	
	g_keyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = LoadSurface("Assets/Textures/Left.bmp"); //Load left surface
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		std::cerr << "Failed to load left image!\n";
		l_success = false;
	}

	
	g_keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = LoadSurface("Assets/Textures/Right.bmp"); //Load right surface
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		std::cerr << "Failed to load right image!\n";
		l_success = false;
	}

	return l_success;
}

SDL_Surface* LoadSurface(std::string path)
{
	SDL_Surface* l_optimizedSurface = NULL; //The final optimized image
	SDL_Surface* l_loadedSurface = IMG_Load(path.c_str()); //Load image at specified path

	if (l_loadedSurface == NULL)
	{
		std::cerr << "Unable to load image SDL Error: " << path.c_str() << SDL_GetError();
	}
	else
	{
		l_optimizedSurface = SDL_ConvertSurface(l_loadedSurface, g_screenSurface->format, 0); //Convert surface to screen format
		
		if (l_optimizedSurface == NULL)
		{
			std::cerr << "Unable to optimize image SDL Error: " << path.c_str() << SDL_GetError();
		}

		SDL_FreeSurface(l_loadedSurface); //Get rid of old loaded surface
	}

	return l_optimizedSurface;
}

void Close()
{
	SDL_FreeSurface(g_currentSurface); //Deallocate surface
	g_currentSurface = NULL;

	SDL_DestroyWindow(g_window); //Destroy window
	g_window = NULL;

	SDL_Quit(); //Quit SDL subsystems
}