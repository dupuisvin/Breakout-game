#ifndef M_LEVEL_EDITOR_GAME_STATE
#define M_LEVEL_EDITOR_GAME_STATE

#include "GameState.h"
#include "RenderSystem.h"
#include "SoundSystem.h"
#include "LevelLoader.h"
#include "CollisionAABB.h"
#include <entt/entity/entity.hpp>

namespace SDLEngine
{
    class RenderWindow;
    struct MouseButtonUpEvent;
    struct MouseMoveEvent;
    struct KeyDownEvent;
}

namespace Breakout
{
    //Represents the game over screen
    class LevelEditorGameState : public SDLEngine::GameState
    {
    public:

        LevelEditorGameState(entt::dispatcher& dispatcher);

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
        virtual void Update(float nStep) override {}

    private:

        void AddRemoveBrick(glm::vec2 mousePos, bool bAdd);
        void HandleMouseButtonUp(const SDLEngine::MouseButtonUpEvent& event);
        void HandleKeyboardEvents(const SDLEngine::KeyDownEvent& event);
        void HandleMouseMoveEvent(const SDLEngine::MouseMoveEvent& event);
        void LoadBackgrounds(SDLEngine::RenderWindow& window);
        void SaveLevel();

        SDLEngine::RenderSystem Renderer;
        SDLEngine::SoundSystem SoundSys;
        glm::vec2 BrickZonePos{ 0.0f, 0.0f };
        SDLEngine::CollisionAABB BrickZone;
        Brick::BrickType CurrentBrickType = Brick::BrickType::Blue;
        entt::entity CurrentBrick = entt::null;
        entt::entity CurrentBackground = entt::null;
        LevelBreakout Level;
        int CurrentBackgroundIdx = 0;
        std::vector<std::pair<SDLEngine::Sprite, std::string>> Backgrounds;
    };
}

#endif // !#define M_LEVEL_EDITOR_GAME_STATE
