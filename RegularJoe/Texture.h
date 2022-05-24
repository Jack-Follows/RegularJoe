#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <SDL_image.h>
#include <iostream>

//Texture wrapper class
class Texture
{
public:
	//Initializes variables
	Texture();

	//Deallocates memory
	~Texture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Deallocates texture
	void free();

	//Renders texture at given point
	void render(int x, int y);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* m_texture;
	SDL_Renderer* m_renderer; //The window renderer

	//Image dimensions
	int m_width;
	int m_height;
};


#endif // !_TEXTURE_H



