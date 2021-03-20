#ifndef M_TEXTURE_LOADER
#define M_TEXTURE_LOADER

#include "Sprite.h"

#include <SDL.h>
#include <string>

namespace SDLEngine
{
    class TextureLoader
    {
    public:

        //Load a texture from the specified file
        static Sprite LoadFromFile(SDL_Renderer* renderer, const std::string& path, uint32_t layer);

        //Load a string into a texture with the specified font, size, color
        static Sprite LoadFromText(SDL_Renderer* renderer, const std::string& text, const std::string& fontPath, int textSize, SDL_Color color, uint32_t layer);
    };
}

#endif