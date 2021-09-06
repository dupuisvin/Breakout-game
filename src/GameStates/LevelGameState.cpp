#include "LevelGameState.h"
#include "LevelLoader.h"
#include "RenderWindow.h"
#include "Position.h"
#include "Velocity.h"
#include "Sprite.h"
#include "TextureLoader.h"
#include "KeyboardEvents.h"
#include "MouseEvents.h"
#include "CollisionUtils.h"
#include "GameStatesEvents.h"
#include "Ball.h"
#include "Brick.h"
#include "BrickDestroyedEvent.h"
#include "BrickExplosionEvent.h"
#include "BallHitBrickEvent.h"
#include "BallDestroyedEvent.h"
#include "LaunchBallEvent.h"
#include "SpriteTransform.h"
#include "Active.h"

#include <entt/entt.hpp>

using namespace SDLEngine;
using namespace Breakout;

static constexpr char DEFAULT_FONT[] = "Assets/Fonts/Kenney Future Square.ttf";
static constexpr char LIVE_ICON[] = "Assets/Graphics/Paddle/hud_heartFull.png";
static constexpr char POWER_UP_ICON[] = "Assets/Graphics/Paddle/powerup_1.png";
static constexpr char GOAL_ICON[] = "Assets/Graphics/Paddle/goal_token.png";
static const SDL_Color LIVE_TEXT_COLOR = {25, 25, 25};

LevelGameState::LevelGameState(std::string levelPath, entt::dispatcher& dispatcher) :
    GameState(dispatcher),
    LevelAssetPath(levelPath),
    SoundSys(Registry, dispatcher),
    MoveSys(Registry)
{
}

static void MoveAllBricksDown(entt::registry &reg)
{
    auto brickView = reg.view<Brick, Position, Active>();
    brickView.each([](auto &brick, auto &pos)
        {
            pos.Pos.y += 32;
        });
}

void LevelGameState::BrickWasHit(const BallHitBrickEvent& event)
{
    auto &brick = Registry.get<Brick>(event.BrickEntity);
    switch (brick.Type)
    {
    case Brick::BrickType::Purple: //All bricks move down
        MoveAllBricksDown(Registry);
        Dispatcher.trigger<BrickDestroyedEvent>(event.BrickEntity);
        break;
    case Brick::BrickType::Green: //Changes ball color
    {
        auto& ball = Registry.get<Ball>(event.BallEntity);
        PowerUpBall(event.BallEntity);
        Dispatcher.trigger<BrickDestroyedEvent>(event.BrickEntity);
        break;
    }
    case Brick::BrickType::Red: // Explosion
        Explosion(event.BrickEntity);
        Dispatcher.trigger<BrickDestroyedEvent>(event.BrickEntity);
        break;
    case Brick::BrickType::Blue: //Normal brick
        Dispatcher.trigger<BrickDestroyedEvent>(event.BrickEntity);
        break;
    case Brick::BrickType::Yellow: //Brick falls down
        if(!Registry.has<Velocity>(event.BrickEntity))
            Registry.emplace<Velocity>(event.BrickEntity, 0.0f, -Ball::DEFAULT_VELOCITY_Y);
        break;
    case Brick::BrickType::Grey: //Indestructible brick
        break;
    }
}

void LevelGameState::BrickDestroyed(const BrickDestroyedEvent& event)
{
    auto& brick = Registry.get<Brick>(event.BrickEntity);
    switch (brick.Type)
    {
    case Brick::BrickType::Blue: //Normal brick
        --LevelEntities.BrickCount;
        break;
    case Brick::BrickType::Red: // Explosion
        [[fallthrough]];
    case Brick::BrickType::Green: //Changes ball color
        [[fallthrough]];
    case Brick::BrickType::Purple: //All bricks move down
        [[fallthrough]];
    case Brick::BrickType::Yellow: //Brick falls down
        [[fallthrough]];
    case Brick::BrickType::Grey: //Indestructible brick
        break;
    }
    Registry.remove<Active>(event.BrickEntity);

    if (LevelEntities.BrickCount == 0)
        Dispatcher.trigger<LevelDoneEvent>();
}

void LevelGameState::BallDestroyed()
{
    auto& player = Registry.get<Player>(LevelEntities.PaddleEntity);

    --player.Lives;

    if(Window)
        UpdateLives(*Window);

    if (player.Lives <= 0)
        Dispatcher.trigger<GameOverEvent>();
    else
        ResetBallPaddle();
}

void LevelGameState::BuildGoal(RenderWindow& w)
{
    GoalIcon = Registry.create();
    Registry.emplace<Sprite>(GoalIcon, TextureLoader::LoadFromFile(w.GetRenderer(), GOAL_ICON, 1));
    Registry.emplace<Position>(GoalIcon, RenderWindow::DEFAULT_SCREEN_WIDTH - 90.0f, 5.0f);
    Registry.emplace<SpriteTransform>(GoalIcon, SpriteTransform::BuildFromScale(0.4f, 0.4f));
    Registry.emplace<Active>(GoalIcon);

    GoalText = Registry.create();
    Registry.emplace<Position>(GoalText, RenderWindow::DEFAULT_SCREEN_WIDTH - 60.0f, 5.0f);
    Registry.emplace<Sprite>(GoalText, TextureLoader::LoadFromText(Window->GetRenderer(), GetLivesString(LevelEntities.BrickCount), DEFAULT_FONT, 20, LIVE_TEXT_COLOR, 1));
    Registry.emplace<Active>(GoalText);
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

void LevelGameState::BuildPowerUp(RenderWindow& w)
{
    PowerUpIcon = Registry.create();
    Registry.emplace<Sprite>(PowerUpIcon, TextureLoader::LoadFromFile(w.GetRenderer(), POWER_UP_ICON, 1));
    Registry.emplace<Position>(PowerUpIcon, 5.0f, 40.0f);
    Registry.emplace<SpriteTransform>(PowerUpIcon, SpriteTransform::BuildFromScale(0.18f, 0.18f));

    PowerUpText = Registry.create();
    Registry.emplace<Position>(PowerUpText, 30.0f, 40.0f);
    Registry.emplace<Sprite>(PowerUpText, TextureLoader::LoadFromText(Window->GetRenderer(), GetLivesString(5), DEFAULT_FONT, 20, LIVE_TEXT_COLOR, 1));
}

void LevelGameState::Explosion(entt::entity source)
{
    static constexpr float EXPLOSION_RADIUS = 100.0f;
    
    const auto &sourcePos = Registry.get<Position>(source);
    auto brickView = Registry.view<Brick, Active, Position>();
    for (auto entity : brickView)
    {
        const auto& brickPos = Registry.get<Position>(entity);
        if (glm::distance(brickPos.Pos, sourcePos.Pos) <= EXPLOSION_RADIUS && source != entity)
        {
            Dispatcher.trigger<BrickExplosionEvent>(BrickExplosionEvent(entity));
            Dispatcher.trigger<BrickDestroyedEvent>(BrickDestroyedEvent(entity));
        }
    }
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
    Dispatcher.sink<LaunchBallEvent>().connect<&MoveSystem::LaunchBall>(&MoveSys);
    Dispatcher.sink<KeyDownEvent>().connect<&LevelGameState::HandleKeyboardEvents>(this);
    Dispatcher.sink<BallHitBrickEvent>().connect<&LevelGameState::BrickWasHit>(this);
    Dispatcher.sink<BallDestroyedEvent>().connect<&LevelGameState::BallDestroyed>(this);
    Dispatcher.sink<BrickDestroyedEvent>().connect<&LevelGameState::BrickDestroyed>(this);
}

void LevelGameState::DisconnectEvents()
{
    Dispatcher.sink<KeyDownEvent>().disconnect<&MoveSystem::OnKeyDown>(&MoveSys);
    Dispatcher.sink<KeyUpEvent>().disconnect<&MoveSystem::OnKeyUp>(&MoveSys);
    Dispatcher.sink<LaunchBallEvent>().disconnect<&MoveSystem::LaunchBall>(&MoveSys);
    Dispatcher.sink<KeyDownEvent>().disconnect<&LevelGameState::HandleKeyboardEvents>(this);
    Dispatcher.sink<BallHitBrickEvent>().disconnect<&LevelGameState::BrickWasHit>(this);
    Dispatcher.sink<BallDestroyedEvent>().disconnect<&LevelGameState::BallDestroyed>(this);
    Dispatcher.sink<BrickDestroyedEvent>().disconnect<&LevelGameState::BrickDestroyed>(this);
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
    case SDLK_SPACE:
        Dispatcher.trigger<LaunchBallEvent>();
        break;
    case SDLK_p:
        {
            if (LevelEntities.BrickCount > 0)
            {
                auto brickView = Registry.view<Brick, Active>();
                for (auto entity : brickView)
                {
                    const auto& brick = Registry.get<Brick>(entity);
                    if (brick.Type == Brick::BrickType::Blue)
                    {
                        Dispatcher.trigger<BrickDestroyedEvent>(entity);
                        break;
                    }
                }

            }
        } 
    }
}

void LevelGameState::Init(RenderWindow& window)
{
    Window = &window;
    LevelEntities = LevelLoader::LoadLevel(LevelAssetPath, Registry, window.GetRenderer());

    Renderer.Init(window);
    SoundSys.Init(LevelEntities.LevelMusicPath);

    BuildGoal(window);
    BuildPlayerLives(window);
    BuildPowerUp(window);

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

void LevelGameState::PowerUpBall(entt::entity ballEntity)
{
    auto& ball = Registry.get<Ball>(ballEntity);

    ball.Mode = Ball::BallMode::Passtrough_Blue;
    Registry.replace<SDLEngine::Sprite>(ballEntity, LevelEntities.BallBlueSprite);
    Registry.replace<Sprite>(PowerUpText, TextureLoader::LoadFromText(Window->GetRenderer(), GetLivesString(static_cast<int>(std::ceil(5.0f))), DEFAULT_FONT, 20, LIVE_TEXT_COLOR, 1));
    BallTimer.Start(5.0f);
    if(!Registry.any<Active>(PowerUpIcon))
        Registry.emplace<Active>(PowerUpIcon);
    if(!Registry.any<Active>(PowerUpText))
        Registry.emplace<Active>(PowerUpText);
}

void LevelGameState::RemovePowerUp(entt::entity ballEntity)
{
    auto& ball = Registry.get<Ball>(ballEntity);
    ball.Mode = Ball::BallMode::Normal;
    Registry.replace<SDLEngine::Sprite>(ballEntity, LevelEntities.BallGraySprite);
    if(Registry.any<Active>(PowerUpIcon))
        Registry.remove<Active>(PowerUpIcon);
    if(Registry.any<Active>(PowerUpText))
        Registry.remove<Active>(PowerUpText);
}

void LevelGameState::Uninit()
{
    DisconnectEvents();
    SoundSys.Uninit();
    Registry.clear();
    PlayerLivesText = entt::null;
    PlayerLivesIcon = entt::null;
    PowerUpIcon     = entt::null;
    PowerUpText     = entt::null;
    GoalIcon        = entt::null;
    GoalText        = entt::null;
}

void LevelGameState::Update(float nStep)
{
    MoveSys.Update(nStep);
    CollisionSys.Update(nStep, Registry, Dispatcher);

    auto ballView = Registry.view<Ball, SDLEngine::Active>();
    for (auto ballEntity : ballView)
    {
        auto& ball = Registry.get<Ball>(ballEntity);
        if (ball.Mode == Ball::BallMode::Passtrough_Blue)
        {
            float nRemainingTime = BallTimer.GetLimit() - BallTimer.Tick();
            Registry.replace<Sprite>(PowerUpText, TextureLoader::LoadFromText(Window->GetRenderer(), GetLivesString(static_cast<int>(std::ceil(nRemainingTime))), DEFAULT_FONT, 20, LIVE_TEXT_COLOR, 1));
            
            if (nRemainingTime < 0.0f)
            {
                RemovePowerUp(ballEntity);
                BallTimer.Stop();
            }
        }
    }

    if(GoalText != entt::null)
        Registry.replace<Sprite>(GoalText, TextureLoader::LoadFromText(Window->GetRenderer(), GetLivesString(LevelEntities.BrickCount), DEFAULT_FONT, 20, LIVE_TEXT_COLOR, 1));
}