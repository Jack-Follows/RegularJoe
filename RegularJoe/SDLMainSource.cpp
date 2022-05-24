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
SDL_Rect g_spriteClips[4];
Texture* g_modulatedTexture;

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

			//Modulation components
			Uint8 l_red = 255;
			Uint8 l_green = 255;
			Uint8 l_blue = 255;

			while (!l_quit) //While application is running
			{
				while (SDL_PollEvent(&l_event) != 0) //Handle events on queue
				{
					if (l_event.type == SDL_QUIT) //User requests quit
					{
						l_quit = true;
					}
					//On keypress change rgb values
					else if (l_event.type == SDL_KEYDOWN)
					{
						switch (l_event.key.keysym.sym)
						{
							//Increase red
						case SDLK_q:
							l_red += 32;
							break;

							//Increase green
						case SDLK_w:
							l_green += 32;
							break;

							//Increase blue
						case SDLK_e:
							l_blue += 32;
							break;

							//Decrease red
						case SDLK_a:
							l_red-= 32;
							break;

							//Decrease green
						case SDLK_s:
							l_green -= 32;
							break;

							//Decrease blue
						case SDLK_d:
							l_blue -= 32;
							break;
						}
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(g_renderer);

				//Modulate and render texture
				g_modulatedTexture->SetColour(l_red, l_green, l_blue);
				g_modulatedTexture->Render(0, 0);

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
	bool l_success = true;

	g_modulatedTexture = new Texture(g_renderer);

	//Load sprite sheet texture
	if (!g_modulatedTexture->LoadFromFile("Assets/Textures/full.png"))
	{
		std::cerr << "\nFailed to load sprite sheet texture!\n";
		l_success = false;
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