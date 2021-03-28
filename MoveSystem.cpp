#include "MoveSystem.h"

#include "Position.h"
#include "Player.h"
#include "Sprite.h"
#include "Ball.h"
#include "RenderWindow.h"
#include "KeyboardEvents.h"

#include <entt/entt.hpp>

using namespace SDLEngine;
using namespace Breakout;

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
    case SDLK_SPACE:
        StartBall = true;
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

void MoveSystem::Update(float nStep, entt::registry& reg)
{
    auto playerView = reg.view<Player, Position, Sprite>();
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

    auto ballView = reg.view<Ball, Position, Sprite>();
    ballView.each([&](auto& ball, auto& pos, auto &sprite) 
        {
            if (StartBall && ball.Vel == glm::vec2())
            {
                ball.Vel = glm::vec2(Ball::DEFAULT_VELOCITY_X, Ball::DEFAULT_VELOCITY_Y);
                StartBall = false;
            }
            pos.Pos += glm::vec2(nStep, nStep) * ball.Vel;
        });
}