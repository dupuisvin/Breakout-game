#include "MainMenuGameState.h"

#include "MouseEvents.h"
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

MainMenuGameState::MainMenuGameState(entt::registry& reg, entt::dispatcher& dispatcher) :
    GameState(reg, dispatcher),
    StartButton("Start", SDLEngine::RenderWindow::DEFAULT_SCREEN_WIDTH / 2, SDLEngine::RenderWindow::DEFAULT_SCREEN_HEIGHT / 2),
    OptionsButton("Options", SDLEngine::RenderWindow::DEFAULT_SCREEN_WIDTH / 2, SDLEngine::RenderWindow::DEFAULT_SCREEN_HEIGHT / 2 + 60),
    ExitButton("Exit", SDLEngine::RenderWindow::DEFAULT_SCREEN_WIDTH / 2, SDLEngine::RenderWindow::DEFAULT_SCREEN_HEIGHT / 2 + 120)
{
}

void MainMenuGameState::BuildMenuBackground(RenderWindow& window)
{
    Registry.emplace<Position>(Background, 0.0f, 0.0f);
    Registry.emplace<Sprite>(Background, TextureLoader::LoadFromFile(window.GetRenderer(), DEFAULT_BACKGROUND_TEXTURE, 0));
    Registry.emplace<Active>(Background);
}

void MainMenuGameState::BuildGameLogo(RenderWindow& window)
{
    auto backgroundTextSprite = TextureLoader::LoadFromText(
        window.GetRenderer(),
        "Breakout",
        DEFAULT_FONT,
        40,
        {25, 25, 25},
        1);
    Registry.emplace<Position>(
        BackgroundText,
        RenderWindow::DEFAULT_SCREEN_WIDTH / 2.0f - backgroundTextSprite.Rect.w / 2.0f,
        RenderWindow::DEFAULT_SCREEN_HEIGHT / 4.0f);
    Registry.emplace<Sprite>(BackgroundText, std::move(backgroundTextSprite));
    Registry.emplace<Active>(BackgroundText);
}

void MainMenuGameState::ConnectEvents()
{
    Dispatcher.sink<MouseButtonUpEvent>().connect<&MainMenuGameState::HandleMouseButtonUp>(*this);
}

void MainMenuGameState::DisconnectEvents()
{
    Dispatcher.sink<MouseButtonUpEvent>().disconnect<&MainMenuGameState::HandleMouseButtonUp>(*this);
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


void MainMenuGameState::HandleMouseButtonUp(const MouseButtonUpEvent& event)
{
    switch (event.Button)
    {
    case SDL_BUTTON_LEFT:
        {
            if(IsButtonClicked(Registry, StartButton.GetButtonEntity(), event))
            {
                printf("Start Clicked\n");
                Dispatcher.trigger<StartGameEvent>();
            }
            else if (IsButtonClicked(Registry, OptionsButton.GetButtonEntity(), event))
            {
                printf("Options clicked\n");
            }
            else if (IsButtonClicked(Registry, ExitButton.GetButtonEntity(), event))
            {
                printf("Exit clicked\n");
                Dispatcher.trigger<QuitEvent>();
            }
        }
        break;
    }
}

void MainMenuGameState::Init(RenderWindow& window)
{
    Background     = Registry.create();
    BackgroundText = Registry.create();

    BuildMenuBackground(window);
    BuildGameLogo(window);
    StartButton.Load(Registry, window, 1);
    OptionsButton.Load(Registry, window, 1);
    ExitButton.Load(Registry, window, 1);

    Renderer.Init(window);

    ConnectEvents();
}

void MainMenuGameState::Render(RenderWindow& window)
{
    Renderer.Render(Registry, window);
}

void MainMenuGameState::Uninit()
{
    Registry.destroy(Background);
    Registry.destroy(BackgroundText);

    StartButton.Unload(Registry);
    OptionsButton.Unload(Registry);
    ExitButton.Unload(Registry);

    DisconnectEvents();
}

void MainMenuGameState::Update(float nStep)
{
}