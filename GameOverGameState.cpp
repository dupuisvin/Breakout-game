#include "GameOverGameState.h"

#include "TextureLoader.h"
#include "RenderWindow.h"
#include "Active.h"
#include "Sprite.h"
#include "Position.h"
#include "Sound.h"
#include "GameStatesEvents.h"
#include <entt/entt.hpp>
#include <SDL_mixer.h>

using namespace SDLEngine;
using namespace Breakout;

static constexpr char DEFAULT_FONT[] = "Assets/Fonts/Kenney_Rocket.ttf";
static constexpr char GAME_OVER_SOUND[] = "Assets/Sounds/gameover2.ogg";

GameOverGameState::GameOverGameState(entt::dispatcher& dispatcher) :
    GameState(dispatcher),
    SoundSys(Registry, dispatcher)
{
}

void GameOverGameState::BuildGameoverText(RenderWindow& window)
{
    GameoverText = Registry.create();
    auto backgroundTextSprite = TextureLoader::LoadFromText(
        window.GetRenderer(),
        "Game Over",
        DEFAULT_FONT,
        40,
        { 200, 25, 25 },
        1);
    Registry.emplace<Position>(
        GameoverText,
        RenderWindow::DEFAULT_SCREEN_WIDTH / 2.0f - backgroundTextSprite.Rect.w / 2.0f,
        RenderWindow::DEFAULT_SCREEN_HEIGHT / 4.0f);
    Registry.emplace<Sprite>(GameoverText, std::move(backgroundTextSprite));
    Registry.emplace<Active>(GameoverText);
}

void GameOverGameState::ConnectEvents()
{
}

void GameOverGameState::DisconnectEvents()
{
}

void GameOverGameState::Init(RenderWindow& window)
{
    ScreenActiveTime = 0.0f;

    BuildGameoverText(window);
    GameoverSound = Registry.create();
    Registry.emplace<Sound>(GameoverSound, Mix_LoadWAV(GAME_OVER_SOUND));
    
    Renderer.Init(window);

    SoundSys.PlaySound(GameoverSound);
}

void GameOverGameState::Render(RenderWindow& window)
{
    Renderer.Render(Registry, window);
}

void GameOverGameState::Uninit()
{
    Registry.destroy(GameoverText);
}

void GameOverGameState::Update(float nStep)
{
    ScreenActiveTime += nStep;
    if (ScreenActiveTime > ScreenDuration)
        Dispatcher.trigger<ReturnToMenuEvent>();
}