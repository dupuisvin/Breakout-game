#ifndef M_GAME_OVER_GAME_STATE
#define M_GAME_OVER_GAME_STATE

#include "GameState.h"
#include "RenderSystem.h"
#include "SoundSystem.h"
#include <entt/entity/entity.hpp>

namespace SDLEngine
{
    class RenderWindow;
}

namespace Breakout
{
    //Represents the game over screen
    class GameOverGameState : public SDLEngine::GameState
    {
    public:

        GameOverGameState(entt::dispatcher& dispatcher);

        //Connect all the events to this state
        virtual void ConnectEvents() override;

        //Disconnect all the events to this state
        virtual void DisconnectEvents() override;

        //Initialize the game state, loading all the textures, ...
        virtual void Init(SDLEngine::RenderWindow& window) override;

        //Render this game state to the specified window
        virtual void Render(SDLEngine::RenderWindow& window) override;

        //Uninitialize the game state, unloading all the textures, ...
        virtual void Uninit();

        //Update this game state with the specified time step (in seconds)
        virtual void Update(float nStep) override;

    private:

        void BuildGameoverText(SDLEngine::RenderWindow& window);

        SDLEngine::RenderSystem Renderer;
        SDLEngine::SoundSystem SoundSys;
        entt::entity GameoverText = entt::null;
        entt::entity GameoverSound = entt::null;
        static constexpr float ScreenDuration = 3; //3 seconds
        float ScreenActiveTime = 0.0f;
    };
}

#endif // !#define M_GAME_OVER_GAME_STATE
