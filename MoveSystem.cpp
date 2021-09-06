#include "MoveSystem.h"

#include "Position.h"
#include "Player.h"
#include "Velocity.h"
#include "Sprite.h"
#include "Ball.h"
#include "RenderWindow.h"
#include "KeyboardEvents.h"

#include <entt/entt.hpp>

using namespace SDLEngine;
using namespace Breakout;

void MoveSystem::LaunchBall()
{
    
    auto ballView = Registry.view<Ball, Velocity>();
    ballView.each([](auto &ball, auto &vel) 
        {
            if(vel.Vel == glm::vec2(0.0f, 0.0f))
                vel.Vel = glm::vec2(Ball::DEFAULT_VELOCITY_X, Ball::DEFAULT_VELOCITY_Y);
        });   
}

void MoveSystem::OnKeyDown(const KeyDownEvent& event)
{
    switch (event.KeyCode)
    {
    case SDLK_LEFT:
        PlayerDir = Player::Direction::Left;
        break;
    case SDLK_RIGHT:
        PlayerDir = Player::Direction::Right;
        break;
    }
}

void MoveSystem::OnKeyUp(const KeyUpEvent& event)
{
    switch (event.KeyCode)
    {
    case SDLK_LEFT:
    case SDLK_RIGHT:
        PlayerDir = Player::Direction::Idle;
        break;
    }
}

void MoveSystem::Update(float nStep)
{
    auto playerView = Registry.view<Player, Position, Sprite>();
    playerView.each([&](auto& player, auto& pos, auto &sprite)
        {
            switch (PlayerDir)
            {
            case Player::Direction::Left:
                pos.Pos.x += -640.0f * nStep;
                break;

            case Player::Direction::Right:
                pos.Pos.x += 640.0f * nStep;
                break;
            }

            if (pos.Pos.x < 0.0f)
                pos.Pos.x = 0.0f;
            if (pos.Pos.x >= RenderWindow::DEFAULT_SCREEN_WIDTH - (float)sprite.Rect.w)
                pos.Pos.x = RenderWindow::DEFAULT_SCREEN_WIDTH - (float)sprite.Rect.w;
        });

    auto movingView = Registry.view<Position, Velocity>();
    movingView.each([&](auto& pos, auto& vel) 
        {
            pos.Pos += glm::vec2(nStep, nStep) * vel.Vel;
        });
}