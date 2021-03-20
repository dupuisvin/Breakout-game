#ifndef M_MAIN_MENU_GAME_STATE
#define M_MAIN_MENU_GAME_STATE

#include "GameState.h"
#include "RenderSystem.h"
#include "TextButton.h"
#include "MenuSoundSystem.h"

namespace SDLEngine
{
    class RenderWindow;
    struct MouseMoveEvent;
    struct MouseButtonUpEvent;
}


namespace Breakout
{
    //Represents the main menu of the game
    class MainMenuGameState : public SDLEngine::GameState
    {
    public:

        MainMenuGameState(entt::registry& reg, entt::dispatcher& dispatcher);

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

        void BuildMenuBackground(SDLEngine::RenderWindow& window);
        void BuildGameLogo(SDLEngine::RenderWindow& window);
        void HandleMouseButtonUp(const SDLEngine::MouseButtonUpEvent& event);

        SDLEngine::RenderSystem Renderer;
        MenuSoundSystem SoundSys;
        SDLEngine::TextButton StartButton;
        SDLEngine::TextButton OptionsButton;
        SDLEngine::TextButton ExitButton;
        entt::entity Background;
        entt::entity BackgroundText;
    };
}

#endif // !#define M_MAIN_MENU_GAME_STATE
