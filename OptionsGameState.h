#ifndef M_OPTIONS_GAME_STATE
#define M_OPTIONS_GAME_STATE

#include "GameState.h"
#include "RenderSystem.h"

namespace SDLEngine
{
    class RenderWindow;
}

namespace Breakout
{
    //Represents the option menu of the game
    class OptionsGameState : public SDLEngine::GameState
    {
    public:

        OptionsGameState(entt::registry& reg, entt::dispatcher& dispatcher);

        //Initialize the game state, loading all the textures, ...
        virtual void Init(SDLEngine::RenderWindow& window) override;

        //Render this game state to the specified window
        virtual void Render(SDLEngine::RenderWindow& window) override;

        //Uninitialize the game state, unloading all the textures, ...
        virtual void Uninit();

        //Update this game state with the specified time step (in seconds)
        virtual void Update(float nStep) override;

    private:
    };
}

#endif // !#define M_OPTIONS_GAME_STATE
