#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <SDL_image.h>
#include <iostream>

//Texture wrapper class
class Texture
{
public:
	
	Texture(SDL_Renderer* renderer);//Initializes variables
	~Texture(); //Deallocates memory

	
	bool LoadFromFile(std::string path); //Loads image at specified path
	void Free(); //Deallocates texture
	void Render(int x, int y, SDL_Rect* clip = NULL); //Renders texture at given point
	void SetColour(Uint8 red, Uint8 green, Uint8 blue); //Setting colours 
	void SetBlendMode(SDL_BlendMode blending); //Set blending
	void SetAlpha(Uint8 alpha); //Set alpha modulation

	//Gets image dimensions
	int GetWidth();
	int GetHeight();

private:
	//The actual hardware texture
	SDL_Texture* m_texture;
	SDL_Renderer* m_renderer; //The window renderer

	//Image dimensions
	int m_width;
	int m_height;
};


#endif // !_TEXTURE_H



