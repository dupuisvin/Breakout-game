#include "PauseMenuGameState.h"

#include "MouseEvents.h"
#include "ButtonClickedEvent.h"
#include "KeyboardEvents.h"
#include "RenderWindow.h"
#include "TextureLoader.h"
#include "CollisionUtils.h"
#include "Position.h"
#include "Sprite.h"
#include "QuitEvent.h"
#include "GameStatesEvents.h"
#include "Active.h"
#include <entt/entt.hpp>

using namespace SDLEngine;
using namespace Breakout;

static constexpr char DEFAULT_BACKGROUND_TEXTURE[] = "Assets/Graphics/Background/bg_grasslands.png";
static constexpr char DEFAULT_FONT[] = "Assets/Fonts/Kenney_Rocket.ttf";

PauseMenuGameState::PauseMenuGameState(entt::registry& reg, entt::dispatcher& dispatcher) :
    GameState(reg, dispatcher),
    SoundSys(reg, dispatcher),
    ExitToMenuButton("Exit to menu", SDLEngine::RenderWindow::DEFAULT_SCREEN_WIDTH / 2, SDLEngine::RenderWindow::DEFAULT_SCREEN_HEIGHT / 2)
{
}


void PauseMenuGameState::BuildPauseText(RenderWindow& window)
{
    auto backgroundTextSprite = TextureLoader::LoadFromText(
        window.GetRenderer(),
        "Pause",
        DEFAULT_FONT,
        40,
        {227, 252, 3},
        4);
    Registry.emplace<Position>(
        PauseText,
        RenderWindow::DEFAULT_SCREEN_WIDTH / 2.0f - backgroundTextSprite.Rect.w / 2.0f,
        RenderWindow::DEFAULT_SCREEN_HEIGHT / 4.0f);
    Registry.emplace<Sprite>(PauseText, std::move(backgroundTextSprite));
    Registry.emplace<Active>(PauseText);
}

void PauseMenuGameState::ConnectEvents()
{
    Dispatcher.sink<MouseButtonUpEvent>().connect<&PauseMenuGameState::HandleMouseButtonUp>(*this);
    Dispatcher.sink<KeyDownEvent>().connect<&PauseMenuGameState::HandleKeyboardEvent>(*this);
}

void PauseMenuGameState::DisconnectEvents()
{
    Dispatcher.sink<MouseButtonUpEvent>().disconnect<&PauseMenuGameState::HandleMouseButtonUp>(*this);
    Dispatcher.sink<KeyDownEvent>().disconnect<&PauseMenuGameState::HandleKeyboardEvent>(*this);
}

static bool IsButtonClicked(entt::registry &reg, entt::entity buttonEntity, const MouseButtonUpEvent& event)
{
    const auto [pos, sprite] = reg.get<Position, Sprite>(buttonEntity);
    return CollisionUtils::IsInsideRect(
        static_cast<float>(event.PosX),
        static_cast<float>(event.PosY),
        pos.Pos.x,
        pos.Pos.y,
        static_cast<float>(sprite.Rect.w),
        static_cast<float>(sprite.Rect.h));
}


void PauseMenuGameState::HandleMouseButtonUp(const MouseButtonUpEvent& event)
{
    switch (event.Button)
    {
    case SDL_BUTTON_LEFT:
        {
            if (IsButtonClicked(Registry, ExitToMenuButton.GetButtonEntity(), event))
            {
                Dispatcher.trigger<ButtonClickedEvent>();
                Dispatcher.trigger<ReturnToMenuEvent>();
            }
        }
        break;
    }
}

void PauseMenuGameState::HandleKeyboardEvent(const KeyDownEvent& event)
{
    switch (event.KeyCode)
    {
    case SDLK_ESCAPE:
        Dispatcher.trigger<UnpauseEvent>();
        break;
    }
}

void PauseMenuGameState::Init(RenderWindow& window)
{
    PauseText = Registry.create();

    BuildPauseText(window);
    ExitToMenuButton.Load(Registry, window, 3);

    Renderer.Init(window);
    SoundSys.Init("");
    ConnectEvents();
}

void PauseMenuGameState::Render(RenderWindow& window)
{
    Renderer.Render(Registry, window);
}

void PauseMenuGameState::Uninit()
{
    Registry.destroy(PauseText);
    ExitToMenuButton.Unload(Registry);
    DisconnectEvents();
    SoundSys.Uninit();
}