#include "LevelGameState.h"
#include "LevelLoader.h"
#include "RenderWindow.h"
#include "Position.h"
#include "Sprite.h"
#include "KeyboardEvents.h"
#include "MouseEvents.h"
#include "CollisionUtils.h"
#include "GameStatesEvents.h"
#include "Brick.h"
#include "BrickDestroyedEvent.h"
#include "Active.h"

#include <entt/entt.hpp>

using namespace SDLEngine;
using namespace Breakout;

//@temp
#include <stdexcept>

LevelGameState::LevelGameState(std::string levelPath, entt::registry& reg, entt::dispatcher& dispatcher) :
    GameState(reg, dispatcher),
    LevelAssetPath(levelPath)
{
}

void LevelGameState::BrickDestroyed()
{
    --BrickCount;
    if (BrickCount == 0)
        Dispatcher.trigger<LevelDoneEvent>();
}

void LevelGameState::ConnectEvents()
{
    Dispatcher.sink<KeyDownEvent>().connect<&MoveSystem::OnKeyDown>(&MoveSys);
    Dispatcher.sink<KeyUpEvent>().connect<&MoveSystem::OnKeyUp>(&MoveSys);
    Dispatcher.sink<KeyDownEvent>().connect<&LevelGameState::HandleKeyboardEvents>(this);
    Dispatcher.sink<BrickDestroyedEvent>().connect<&LevelGameState::BrickDestroyed>(this);
}

void LevelGameState::DisconnectEvents()
{
    Dispatcher.sink<KeyDownEvent>().disconnect<&MoveSystem::OnKeyDown>(&MoveSys);
    Dispatcher.sink<KeyUpEvent>().disconnect<&MoveSystem::OnKeyUp>(&MoveSys);
    Dispatcher.sink<KeyDownEvent>().disconnect<&LevelGameState::HandleKeyboardEvents>(this);
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
            if (!brickView.empty())
            {
                Registry.remove<Active>(brickView.front());
                Dispatcher.trigger<BrickDestroyedEvent>();
            }
        } 
    }
}

void LevelGameState::Init(RenderWindow& window)
{
    LevelEntities = LevelLoader::LoadLevel(LevelAssetPath, Registry, window.GetRenderer());
    BrickCount = LevelEntities.BricksEntities.size();

    Renderer.Init(window);

    //Set paddle position to starting position
    auto [paddlePos, paddleSprite] = Registry.get<Position, Sprite>(LevelEntities.PaddleEntity);
    paddlePos.Pos = { (window.GetWidth() - paddleSprite.Rect.w) / 2, window.GetHeight() - paddleSprite.Rect.h * 2 };

    //Set ball position to starting position
    auto [ballPos, ballSprite] = Registry.get<Position, Sprite>(LevelEntities.BallEntity);
    ballPos.Pos = { (window.GetWidth() - ballSprite.Rect.w) / 2, paddlePos.Pos.y - ballSprite.Rect.h };

    ConnectEvents();
}

void LevelGameState::Render(RenderWindow& window)
{
    Renderer.Render(Registry, window);
}

void LevelGameState::Uninit()
{
    LevelEntities.Unload(Registry);
    DisconnectEvents();
}

void LevelGameState::Update(float nStep)
{
    MoveSys.Update(nStep, Registry);
    CollisionSys.Update(nStep, Registry, Dispatcher);
}