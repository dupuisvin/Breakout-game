#ifndef M_LEVEL_GAME_STATE
#define M_LEVEL_GAME_STATE

#include "LevelBreakout.h"
#include "GameState.h"
#include "RenderSystem.h"
#include "MoveSystem.h"
#include "CollisionSystem.h"

namespace SDLEngine
{
    class RenderWindow;
    struct MouseButtonUpEvent;
    struct KeyDownEvent;
}

namespace Breakout
{
    //Represent a playable game level
    class LevelGameState : public SDLEngine::GameState
    {
    public:

        LevelGameState(std::string levelPath, entt::registry& reg, entt::dispatcher& dispatcher);

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

        void BrickDestroyed();
        void HandleKeyboardEvents(const SDLEngine::KeyDownEvent &event);

        std::string LevelAssetPath;
        LevelBreakout LevelEntities;
        SDLEngine::RenderSystem Renderer;
        MoveSystem MoveSys;
        CollisionSystem CollisionSys;
        size_t BrickCount = 0;

    };
}

#endif // !#define M_LEVEL_GAME_STATE
