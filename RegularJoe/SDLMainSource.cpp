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
#include "Texture.h"

//Internal Function Declarations
bool Init(); // Starts up SDL and creates window
bool LoadMedia(); // Loads media
void Close(); //Frees media and shuts down SDL
SDL_Texture* LoadTexture(std::string path); //Loads individual image

//Global Variables
SDL_Window* g_window = NULL; //The window we'll be rendering to
SDL_Renderer* g_renderer = NULL; //The window renderer
SDL_Texture* g_texture = NULL; //Current displayed texture
Texture* g_fooTexture;
Texture* g_backgroundTexture;
SDL_Rect g_spriteClips[4];
Texture* g_spriteSheetTexture;

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

				//Clear screen
				SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(g_renderer);

				//Render top left sprite
				g_spriteSheetTexture->render(0, 0, &g_spriteClips[0]);

				//Render top right sprite
				g_spriteSheetTexture->render(SCREEN_WIDTH - g_spriteClips[1].w, 0, &g_spriteClips[1]);

				//Render bottom left sprite
				g_spriteSheetTexture->render(0, SCREEN_HEIGHT - g_spriteClips[2].h, &g_spriteClips[2]);

				//Render bottom right sprite
				g_spriteSheetTexture->render(SCREEN_WIDTH - g_spriteClips[3].w, SCREEN_HEIGHT - g_spriteClips[3].h, &g_spriteClips[3]);

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
	//Loading success flag
	bool success = true;

	g_spriteSheetTexture = new Texture(g_renderer);

	//Load sprite sheet texture
	if (!g_spriteSheetTexture->loadFromFile("Assets/Textures/Circle.png"))
	{
		printf("Failed to load sprite sheet texture!\n");
		success = false;
	}
	else
	{
		//Set top left sprite
		g_spriteClips[0].x = 0;
		g_spriteClips[0].y = 0;
		g_spriteClips[0].w = 100;
		g_spriteClips[0].h = 100;
						
		//Set top right sprite
		g_spriteClips[1].x = 100;
		g_spriteClips[1].y = 0;
		g_spriteClips[1].w = 100;
		g_spriteClips[1].h = 100;
						
		//Set bottom left sprite
		g_spriteClips[2].x = 0;
		g_spriteClips[2].y = 100;
		g_spriteClips[2].w = 100;
		g_spriteClips[2].h = 100;
						
		//Set bottom right sprite
		g_spriteClips[3].x = 100;
		g_spriteClips[3].y = 100;
		g_spriteClips[3].w = 100;
		g_spriteClips[3].h = 100;
	}

	return success;
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