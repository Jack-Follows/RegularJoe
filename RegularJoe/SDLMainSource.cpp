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

				SDL_RenderClear(g_renderer); //Clears screen
<<<<<<< Updated upstream
				SDL_RenderCopy(g_renderer, g_texture, NULL, NULL); //Render texture to screen
				SDL_RenderPresent(g_renderer); //Updates the screen
=======

				//Top left corner viewport
				SDL_Rect topLeftViewport;
				topLeftViewport.x = 0;
				topLeftViewport.y = 0;
				topLeftViewport.w = SCREEN_WIDTH / 2;
				topLeftViewport.h = SCREEN_HEIGHT / 2;
				SDL_RenderSetViewport(g_renderer, &topLeftViewport);

				//Render texture to screen
				SDL_RenderCopy(g_renderer, g_texture, NULL, NULL);


				//Top right viewport
				SDL_Rect topRightViewport;
				topRightViewport.x = SCREEN_WIDTH / 2;
				topRightViewport.y = 0;
				topRightViewport.w = SCREEN_WIDTH / 2;
				topRightViewport.h = SCREEN_HEIGHT / 2;
				SDL_RenderSetViewport(g_renderer, &topRightViewport);

				//Render texture to screen
				SDL_RenderCopy(g_renderer, g_texture, NULL, NULL);


				//Bottom viewport
				SDL_Rect bottomViewport;
				bottomViewport.x = 0;
				bottomViewport.y = SCREEN_HEIGHT / 2;
				bottomViewport.w = SCREEN_WIDTH;
				bottomViewport.h = SCREEN_HEIGHT / 2;
				SDL_RenderSetViewport(g_renderer, &bottomViewport);


				//Render texture to screen
				SDL_RenderCopy(g_renderer, g_texture, NULL, NULL);


				//Update screen
				SDL_RenderPresent(g_renderer);
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
	g_texture = LoadTexture("Assets/Textures/MenuScreen.bmp");
=======
	//Load texture
	g_texture = LoadTexture("Assets/Textures/Test.gif");
>>>>>>> Stashed changes
	if (g_texture == NULL)
	{
		std::cerr << "Failed to load texture image!\n";
		l_success = false;
	}

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
	SDL_DestroyTexture(g_texture);//Free loaded image
	g_texture = NULL;

	SDL_DestroyRenderer(g_renderer); //Destroy window    
	SDL_DestroyWindow(g_window);
	g_window = NULL;
	g_renderer = NULL;
	
	IMG_Quit(); //Quit SDL subsystems
	SDL_Quit();
}