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
Texture* g_backgroundTexture;

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
			Uint8 l_alpha = 225;

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
						//Increase alpha on w
						if (l_event.key.keysym.sym == SDLK_w)
						{
							//Cap if over 255
							if (l_alpha + 32 > 255)
							{
								l_alpha = 255;
							}
							//Increment otherwise
							else
							{
								l_alpha += 32;
							}
						}
						//Decrease alpha on s
						else if (l_event.key.keysym.sym == SDLK_s)
						{
							//Cap if below 0
							if (l_alpha - 32 < 0)
							{
								l_alpha = 0;
							}
							//Decrement otherwise
							else
							{
								l_alpha -= 32;
							}
						}
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(g_renderer);

				//Render background
				g_backgroundTexture->Render(0, 0);

				//Render front blended
				g_modulatedTexture->SetAlpha(l_alpha);
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

	//Load front alpha texture
	if (!g_modulatedTexture->LoadFromFile("Assets/Textures/alpha100.png"))
	{
		std::cerr << "\nFailed to load front texture!\n";
		l_success = false;
	}
	else
	{
		//Set standard alpha blending
		g_modulatedTexture->SetBlendMode(SDL_BLENDMODE_BLEND);
	}
	
	g_backgroundTexture = new Texture(g_renderer);

	//Load background texture
	if (!g_backgroundTexture->LoadFromFile("Assets/Textures/alpha000.png"))
	{
		std::cerr << "Failed to load background texture!\n";
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
	//Destroy window	
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	g_window = NULL;
	g_renderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}