#include "LevelGameState.h"
#include "LevelLoader.h"
#include "RenderWindow.h"
#include "Position.h"
#include "Sprite.h"
#include "TextureLoader.h"
#include "KeyboardEvents.h"
#include "MouseEvents.h"
#include "CollisionUtils.h"
#include "GameStatesEvents.h"
#include "Brick.h"
#include "BrickDestroyedEvent.h"
#include "BallDestroyedEvent.h"
#include "SpriteTransform.h"
#include "Active.h"

#include <iostream>//@temp

#include <entt/entt.hpp>

using namespace SDLEngine;
using namespace Breakout;

static constexpr char DEFAULT_FONT[] = "Assets/Fonts/Kenney Future Square.ttf";
static constexpr char LIVE_ICON[] = "Assets/Graphics/Paddle/hud_heartFull.png";
static const SDL_Color LIVE_TEXT_COLOR = {25, 25, 25};

LevelGameState::LevelGameState(std::string levelPath, entt::dispatcher& dispatcher) :
    GameState(dispatcher),
    LevelAssetPath(levelPath),
    SoundSys(Registry, dispatcher)
{
}

void LevelGameState::BrickDestroyed()
{
    --BrickCount;
    if (BrickCount == 0)
        Dispatcher.trigger<LevelDoneEvent>();
}

void LevelGameState::BallDestroyed()
{
    auto& player = Registry.get<Player>(LevelEntities.PaddleEntity);

    --player.Lives;

    std::cout << "Player Lives is " << player.Lives << "\n";
    if(Window)
        UpdateLives(*Window);

    if (player.Lives <= 0)
        Dispatcher.trigger<GameOverEvent>();
    else
        ResetBallPaddle();
}

void LevelGameState::BuildPlayerLives(RenderWindow &w)
{
    PlayerLivesIcon = Registry.create();
    Registry.emplace<Sprite>(PlayerLivesIcon, TextureLoader::LoadFromFile(w.GetRenderer(), LIVE_ICON, 1));
    Registry.emplace<Position>(PlayerLivesIcon, 5.0f, 5.0f);
    Registry.emplace<SpriteTransform>(PlayerLivesIcon, SpriteTransform::BuildFromScale(0.5f, 0.5f));
    Registry.emplace<Active>(PlayerLivesIcon);

    UpdateLives(w);
}

void LevelGameState::UpdateLives(RenderWindow& w)
{
    auto& player = Registry.get<Player>(LevelEntities.PaddleEntity);

    if (PlayerLivesText == entt::null)
    {
        PlayerLivesText = Registry.create();
        Registry.emplace<Position>(PlayerLivesText, 30.0f, 5.0f);
        Registry.emplace<Active>(PlayerLivesText);
        Registry.emplace<Sprite>(PlayerLivesText, TextureLoader::LoadFromText(w.GetRenderer(), GetLivesString(player.Lives), DEFAULT_FONT, 20, LIVE_TEXT_COLOR, 1));
    }
    else 
        Registry.replace<Sprite>(PlayerLivesText, TextureLoader::LoadFromText(w.GetRenderer(), GetLivesString(player.Lives), DEFAULT_FONT, 20, LIVE_TEXT_COLOR, 1));
    
}

void LevelGameState::ConnectEvents()
{
    Dispatcher.sink<KeyDownEvent>().connect<&MoveSystem::OnKeyDown>(&MoveSys);
    Dispatcher.sink<KeyUpEvent>().connect<&MoveSystem::OnKeyUp>(&MoveSys);
    Dispatcher.sink<KeyDownEvent>().connect<&LevelGameState::HandleKeyboardEvents>(this);
    Dispatcher.sink<BrickDestroyedEvent>().connect<&LevelGameState::BrickDestroyed>(this);
    Dispatcher.sink<BallDestroyedEvent>().connect<&LevelGameState::BallDestroyed>(this);
}

void LevelGameState::DisconnectEvents()
{
    Dispatcher.sink<KeyDownEvent>().disconnect<&MoveSystem::OnKeyDown>(&MoveSys);
    Dispatcher.sink<KeyUpEvent>().disconnect<&MoveSystem::OnKeyUp>(&MoveSys);
    Dispatcher.sink<KeyDownEvent>().disconnect<&LevelGameState::HandleKeyboardEvents>(this);
    Dispatcher.sink<BrickDestroyedEvent>().disconnect<&LevelGameState::BrickDestroyed>(this);
    Dispatcher.sink<BallDestroyedEvent>().disconnect<&LevelGameState::BallDestroyed>(this);
}

static bool IsButtonClicked(entt::registry& reg, entt::entity buttonEntity, const MouseButtonUpEvent& event)
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

void LevelGameState::HandleKeyboardEvents(const KeyDownEvent& event)
{
    switch (event.KeyCode)
    {
    case SDLK_ESCAPE:
        Dispatcher.trigger<PauseEvent>();
        break;
    case SDLK_p:
        {
            auto brickView = Registry.view<Brick, Active>();
            if (BrickCount > 0)
            {
                Registry.remove<Active>(brickView.front());
                Dispatcher.trigger<BrickDestroyedEvent>();
            }
        } 
    }
}

void LevelGameState::Init(RenderWindow& window)
{
    Window = &window;
    LevelEntities = LevelLoader::LoadLevel(LevelAssetPath, Registry, window.GetRenderer());
    BrickCount = LevelEntities.BricksEntities.size();

    Renderer.Init(window);
    SoundSys.Init(LevelEntities.LevelMusicPath);

    BuildPlayerLives(window);

    ResetBallPaddle();

    ConnectEvents();
}

void LevelGameState::Render(RenderWindow& window)
{
    Renderer.Render(Registry, window);
}

void LevelGameState::ResetBallPaddle()
{
    //Set paddle position to starting position
    auto [paddlePos, paddleSprite] = Registry.get<Position, Sprite>(LevelEntities.PaddleEntity);
    paddlePos.Pos = { (RenderWindow::DEFAULT_SCREEN_WIDTH - paddleSprite.Rect.w) / 2, RenderWindow::DEFAULT_SCREEN_HEIGHT - paddleSprite.Rect.h * 2 };

    //Set ball position to starting position
    auto [ballPos, ballSprite] = Registry.get<Position, Sprite>(LevelEntities.BallEntity);
    ballPos.Pos = { (RenderWindow::DEFAULT_SCREEN_WIDTH - ballSprite.Rect.w) / 2, paddlePos.Pos.y - (ballSprite.Rect.h+2) };
}

void LevelGameState::Uninit()
{
    DisconnectEvents();
    SoundSys.Uninit();
    Registry.clear();
    PlayerLivesText = entt::null;
    PlayerLivesIcon = entt::null;
}

void LevelGameState::Update(float nStep)
{
    MoveSys.Update(nStep, Registry);
    CollisionSys.Update(nStep, Registry, Dispatcher);
}