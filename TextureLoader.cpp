#include "TextureLoader.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdexcept>

using namespace SDLEngine;

Sprite TextureLoader::LoadFromFile(SDL_Renderer* renderer, const std::string &path, uint32_t layer)
{
	Sprite sprite;
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface)
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture)
		{
			sprite.Rect.w = loadedSurface->w;
			sprite.Rect.h = loadedSurface->h;
			sprite.Texture.reset(newTexture);
			sprite.Layer = layer;
		}
		else
			throw std::runtime_error(SDL_GetError());

		SDL_FreeSurface(loadedSurface);
	}
	else
		throw std::runtime_error(SDL_GetError());

	
	return sprite;
}


Sprite TextureLoader::LoadFromText(SDL_Renderer* renderer, const std::string& text, const std::string &fontPath, int textSize, SDL_Color color, uint32_t layer)
{
	Sprite sprite;
	SDL_Texture* newTexture = NULL;

	//Open the font
	auto textFont = TTF_OpenFont(fontPath.c_str(), textSize);
	if (textFont)
	{
		//Render text surface
		SDL_Surface* textSurface = TTF_RenderText_Solid(textFont, text.c_str(), color);
		if (textSurface)
		{
			//Create texture from surface pixels
			newTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
			if (newTexture)
			{
				//Get image dimensions
				sprite.Rect.w = textSurface->w;
				sprite.Rect.h = textSurface->h;
				sprite.Texture.reset(newTexture);
				sprite.Layer = layer;
			}
			else
				throw std::runtime_error(SDL_GetError());

			//Get rid of old surface
			SDL_FreeSurface(textSurface);
		}
		else
			throw std::runtime_error(SDL_GetError());
		TTF_CloseFont(textFont);
	}
	else
		throw std::runtime_error(SDL_GetError());

	return sprite;
}