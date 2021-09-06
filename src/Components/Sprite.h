#ifndef M_SPRITE
#define M_SPRITE

#include <SDL.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_image.h>
#include <memory>

namespace SDLEngine
{
    struct Sprite
    {
    private:
        
        //Deleter to remove the texture properly
        static void DeleteTexture(SDL_Texture* p)
        {
            SDL_DestroyTexture(p);
        }

    public:

        Sprite() = default;
        Sprite(SDL_Texture* pTexture, SDL_Rect r, uint32_t layer) :
            Texture(pTexture, DeleteTexture),
            Rect(r),
            Layer(layer){}
        std::shared_ptr<SDL_Texture> Texture = nullptr;
        SDL_Rect Rect{};
        uint32_t Layer = 0;

        //Checks if the texture is valid (was loaded)
        bool IsValid() const { return static_cast<bool>(Texture); }
    };
}

#endif // !M_SPRITE

