#ifndef M_LEVEL_GAME_STATE
#define M_LEVEL_GAME_STATE

#include "LevelBreakout.h"
#include "GameState.h"
#include "RenderSystem.h"
#include "MoveSystem.h"
#include "CollisionSystem.h"
#include "LevelSoundSystem.h"
#include <entt/entity/entity.hpp>

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

        LevelGameState(std::string levelPath, entt::dispatcher& dispatcher);

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

        static std::string GetLivesString(int lives)
        {
            return std::string(" : ") + std::to_string(lives);
        }

        //Called when the ball is destroyed
        void BallDestroyed();

        //Called when a brick from the level is destroyed
        void BrickDestroyed();

        //Build the player lives display
        void BuildPlayerLives(SDLEngine::RenderWindow& w);

        //Reset the ball and the paddle to their starting position
        void ResetBallPaddle();

        //Handle the keyboard events
        void HandleKeyboardEvents(const SDLEngine::KeyDownEvent &event);

        void UpdateLives(SDLEngine::RenderWindow& w);

        std::string LevelAssetPath;
        LevelBreakout LevelEntities;
        SDLEngine::RenderSystem Renderer;
        MoveSystem MoveSys;
        CollisionSystem CollisionSys;
        LevelSoundSystem SoundSys;
        size_t BrickCount = 0;
        SDLEngine::RenderWindow* Window = nullptr;
        entt::entity PlayerLivesIcon = entt::null;
        entt::entity PlayerLivesText = entt::null;

    };
}

#endif // !#define M_LEVEL_GAME_STATE
