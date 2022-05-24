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
SDL_Texture* LoadTexture(std::string path); //Loads individual image

//Global Variables
SDL_Window* g_window = NULL; //The window we'll be rendering to
SDL_Renderer* g_renderer = NULL; //The window renderer
SDL_Texture* g_texture = NULL; //Current displayed texture

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

			while (!l_quit) //While application is running
			{
				while (SDL_PollEvent(&l_event) != 0) //Handle events on queue
				{
					if (l_event.type == SDL_QUIT) //User requests quit
					{
						l_quit = true;
					}
				}

				SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(g_renderer); //Clears screen

				//Render red filled quad
				SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
				SDL_SetRenderDrawColor(g_renderer, 0xFF, 0x00, 0x00, 0xFF);
				SDL_RenderFillRect(g_renderer, &fillRect);

				//Render green outlined quad
				SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
				SDL_SetRenderDrawColor(g_renderer, 0x00, 0xFF, 0x00, 0xFF);
				SDL_RenderDrawRect(g_renderer, &outlineRect);

				//Draw blue horizontal line
				SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0xFF, 0xFF);
				SDL_RenderDrawLine(g_renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

				//Draw vertical line of yellow dots
				SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0x00, 0xFF);
				for (int i = 0; i < SCREEN_HEIGHT; i += 4)
				{
					SDL_RenderDrawPoint(g_renderer, SCREEN_WIDTH / 2, i);
				}

				//Update screen
				SDL_RenderPresent(g_renderer);
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
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) //Set texture filtering to linear
		{
			std::cerr << "Warning: Linear texture filtering not enabled!\n";
		}

		g_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); //Create window
		if (g_window == NULL)
		{
			std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError();
			l_success = false;
		}
		else
		{
			g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
			if (g_renderer == NULL)
			{
				std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError();
				l_success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xff, 0xFF); //Initialise Render Colour

				int l_imgFlags = IMG_INIT_PNG; //Initialize PNG loading
				if (!(IMG_Init(l_imgFlags) & l_imgFlags))
				{
					std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError();
					l_success = false;
				}
			}
		}
	}

	return l_success;
}

bool LoadMedia()
{
	bool l_success = true; //Loading success flag

	return l_success;
}

SDL_Texture* LoadTexture(std::string path)
{
	SDL_Texture* l_newTexture = NULL; //The Final Texture
	SDL_Surface* l_loadedSurface = IMG_Load(path.c_str());

	if (l_loadedSurface == NULL)
	{
		std::cerr << "Unable to load image SDL Error: " << path.c_str() << SDL_GetError();
	}
	else
	{
		//Create texture from surface pixels
		l_newTexture = SDL_CreateTextureFromSurface(g_renderer, l_loadedSurface);
		if (l_newTexture == NULL)
		{
			std::cerr << "Unable to optimize image SDL Error: " << path.c_str() << SDL_GetError();
		}

		SDL_FreeSurface(l_loadedSurface); //Get rid of old loaded surface
	}

	return l_newTexture;
}

void Close()
{
	//Destroy window	
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	g_window = NULL;
	g_renderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}