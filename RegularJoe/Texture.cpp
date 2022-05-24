#include "Texture.h"

Texture::Texture(SDL_Renderer* renderer)
{
	//Initialize
	m_renderer = renderer;
	m_texture = NULL;
	m_width = 0;
	m_height = 0;
}

Texture::~Texture()
{
	//Deallocate
	Free();
}

bool Texture::LoadFromFile(std::string path)
{
	
	Free(); //Get rid of preexisting texture

	SDL_Texture* l_newTexture = NULL; //The final texture
	SDL_Surface* l_loadedSurface = IMG_Load(path.c_str()); //Load image at specified path
	
	if (l_loadedSurface == NULL)
	{
		std::cerr << "Unable to load image! SDL_image Error: \n" << path.c_str() << IMG_GetError();
	}
	else
	{
		//Color key image
		SDL_SetColorKey(l_loadedSurface, SDL_TRUE, SDL_MapRGB(l_loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		l_newTexture = SDL_CreateTextureFromSurface(m_renderer, l_loadedSurface);
		if (l_newTexture == NULL)
		{
			std::cerr << "Unable to create texture! SDL Error: \n" << path.c_str() << SDL_GetError();
		}
		else
		{
			//Get image dimensions
			m_width = l_loadedSurface->w;
			m_height = l_loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(l_loadedSurface);
	}

	//Return success
	m_texture = l_newTexture;
	return m_texture != NULL;
}

void Texture::Free()
{
	//Free texture if it exists
	if (m_texture != NULL)
	{
		SDL_DestroyTexture(m_texture);
		m_texture = NULL;
		m_width = 0;
		m_height = 0;
	}
}

void Texture::Render(int x, int y, SDL_Rect* clip)
{
	
	SDL_Rect renderQuad = { x, y, m_width, m_height }; //Set rendering space and render to screen

	if (clip != NULL) //Set clip rendering dimensions
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopy(m_renderer, m_texture, clip, &renderQuad); //Render to screen
}

void Texture::SetColour(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(m_texture, red, green, blue); //Modulate texture
}

void Texture::SetBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(m_texture, blending); //Set blending function
}

void Texture::SetAlpha(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(m_texture, alpha); //Modulate texture alpha
}

int Texture::GetWidth()
{
	return m_width;
}

int Texture::GetHeight()
{
	return m_height;
}
