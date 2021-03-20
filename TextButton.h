#ifndef M_BUTTON
#define M_BUTTON

#include <string>
#include <entt/fwd.hpp>
#include <entt/entity/entity.hpp>
#include "SDL.h"

namespace SDLEngine
{
    class RenderWindow;

    //Represent a drawable interface button
    class TextButton
    {
    public:

        TextButton(std::string text, int x, int y) :
            Text(text),
            PosX(x),
            PosY(y){}

        //Get the entity of the button texture
        entt::entity GetButtonEntity() const { return ButtonEntity; }

        //Get the entity of the button text
        entt::entity GetTextEntity() const { return TextEntity; }

        //Load the textures for this button
        void Load(entt::registry& registry, RenderWindow& window, int nStartLayer);

        //Unload the textures for this button
        void Unload(entt::registry& registry);

    private:

        static const SDL_Color DEFAULT_FONT_COLOR ;
        static int GetTextFontSize(const std::string& text);

        std::string Text;
        int PosX = -1;
        int PosY = -1;
        SDL_Color FontColor = DEFAULT_FONT_COLOR;
        entt::entity ButtonEntity = entt::null;
        entt::entity TextEntity   = entt::null;

    };
}


#endif // !M_BUTTON
