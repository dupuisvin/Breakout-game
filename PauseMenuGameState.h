#ifndef M_PAUSE_MENU_GAME_STATE
#define M_PAUSE_MENU_GAME_STATE

#include "GameState.h"
#include "RenderSystem.h"
#include "MenuSoundSystem.h"
#include "TextButton.h"

namespace SDLEngine
{
    class RenderWindow;
    struct MouseButtonUpEvent;
    struct KeyDownEvent;
}


namespace Breakout
{
    //Represents the main menu of the game
    class PauseMenuGameState : public SDLEngine::GameState
    {
    public:

        PauseMenuGameState(entt::registry& reg, entt::dispatcher& dispatcher);

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

        void BuildPauseText(SDLEngine::RenderWindow& window);
        void HandleMouseButtonUp(const SDLEngine::MouseButtonUpEvent& event);
        void HandleKeyboardEvent(const SDLEngine::KeyDownEvent& event);

        SDLEngine::RenderSystem Renderer;
        MenuSoundSystem SoundSys;
        SDLEngine::TextButton ExitToMenuButton;
        entt::entity PauseText = entt::null;
    };
}

#endif // !#define M_PAUSE_MENU_GAME_STATE
