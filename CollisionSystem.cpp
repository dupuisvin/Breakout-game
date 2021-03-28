#include "CollisionSystem.h"

#include "Ball.h"
#include "Position.h"
#include "Sprite.h"
#include "Brick.h"
#include "Player.h"
#include "CollisionUtils.h"
#include "Active.h"
#include "RenderWindow.h"
#include "BrickDestroyedEvent.h"
#include "BallHitBrickEvent.h"
#include "BallHitPaddleEvent.h"
#include "BallDestroyedEvent.h"
#include "BallHitWallEvent.h"
#include "PaddleCollisionObj.h"
#include "Interpolations.h"
#include <glm/geometric.hpp>

#include <entt/entt.hpp>

using namespace Breakout;
using namespace SDLEngine;

static const  glm::vec2 PADDLE_MIN_OFFSET{ 0.0f, 0.0f };
static const  glm::vec2 RIGHT_PADDLE_OFFSET{ 0.75f, 0.0f };
static const  glm::vec2 LEFT_PADDLE_OFFSET{ -0.75f, 0.0f };

template<class BrickView>
static void CollideWithBricks(BrickView &brickView, entt::registry& reg, entt::dispatcher& dispatch, Ball& ball, Position& ballPos, CollisionCircle& ballColl)
{
    CollisionContact minContact;
    float minDist = std::numeric_limits<float>::max();
    entt::entity minEntity = entt::null;
    for (auto brickEntity : brickView)
    {
        auto& brickPos = brickView.get<Position>(brickEntity);
        auto& brickColl = brickView.get<CollisionAABB>(brickEntity);
        CollisionContact contact = CollisionUtils::Collision(ballPos.Pos, ballColl, brickPos.Pos, brickColl);
        if (contact.Collision)
        {
            float dist = CollisionUtils::DistanceSquared(
                ballPos.Pos.x + ballColl.Radius,
                ballPos.Pos.y + ballColl.Radius,
                brickPos.Pos.x + brickColl.Width / 2.0f,
                brickPos.Pos.y + brickColl.Height / 2.0f);
            if (dist < minDist)
            {
                minContact = contact;
                minDist = dist;
                minEntity = brickEntity;
            }
        }
    }

    if (minContact.Collision)
    {
        if (std::abs(minContact.CollVec.x) < std::abs(minContact.CollVec.y))
        {
            ball.Vel.x *= -1;
            ballPos.Pos.x += minContact.CollVec.x;
        }
        else if (std::abs(minContact.CollVec.y) < std::abs(minContact.CollVec.x))
        {
            ball.Vel.y *= -1;
            ballPos.Pos.y += minContact.CollVec.y;
        }
        else
        {
            ball.Vel *= -1;
            ballPos.Pos += minContact.CollVec;
        }

        reg.remove<Active>(minEntity);
        dispatch.trigger<BallHitBrickEvent>();
        dispatch.trigger<BrickDestroyedEvent>();
    }
}

//Check if c1 if the best collision but finding the minimal offset to separate the object that are colliding
static bool IsBestCollision(CollisionContact &c1, CollisionContact &c2, CollisionContact &c3)
{
    if (c2.Collision)
        return std::min(c1.CollVec.x, c1.CollVec.y) < std::min(c2.CollVec.x, c2.CollVec.y);
    else if (c3.Collision)
        return std::min(c1.CollVec.x, c1.CollVec.y) < std::min(c3.CollVec.x, c3.CollVec.y);
    else
        return true;
}

//Offset the position by x or y (whichever is the smallest) or both if they are equal
static void OffsetByMin(glm::vec2& pos, const glm::vec2 &contactOff)
{
    if (std::abs(contactOff.x) < std::abs(contactOff.y))
        pos.x += contactOff.x;
    else if (std::abs(contactOff.y) < std::abs(contactOff.x))
        pos.y += contactOff.y;
    else
        pos += contactOff;
}

template<class PaddleView>
void CollideWithPaddles(PaddleView &paddleView, entt::registry& reg, entt::dispatcher& dispatch, Ball& ball, Position& ballPos, CollisionCircle& ballColl)
{
    paddleView.each([&](auto& player, auto& playerPos, auto& playerColl)
        {
            auto& centerAABB = reg.get<CollisionAABB>(playerColl.CenterAABB);
            auto& leftCircle = reg.get<CollisionCircle>(playerColl.LeftCircle);
            auto& rightCircle = reg.get<CollisionCircle>(playerColl.RightCircle);
            CollisionContact contactCenter = CollisionUtils::Collision(ballPos.Pos, ballColl, playerPos.Pos, centerAABB);
            CollisionContact contactLeft = CollisionUtils::Collision(ballPos.Pos, ballColl, playerPos.Pos, leftCircle);
            CollisionContact contactRight = CollisionUtils::Collision(ballPos.Pos, ballColl, playerPos.Pos, rightCircle);

            if (contactCenter.Collision && IsBestCollision(contactCenter, contactLeft, contactRight))
            {
                glm::vec2 ballCenter = ballPos.Pos + ballColl.PosOffset;
                float paddleHalfWidth = centerAABB.HW();
                float paddleCenter = playerPos.Pos.x + paddleHalfWidth + leftCircle.Radius;
                float nDistanceToCenterZeroToOne = SmoothStart2(std::abs(ballCenter.x - paddleCenter) / paddleHalfWidth);
                float ballSpeed = glm::length(ball.Vel);

                glm::vec2 ballVector = glm::normalize(ball.Vel);

                if (ballCenter.x < paddleCenter)
                    ballVector += Interpolate(PADDLE_MIN_OFFSET, LEFT_PADDLE_OFFSET, nDistanceToCenterZeroToOne);
                else if (ballCenter.x > paddleCenter)
                    ballVector += Interpolate(PADDLE_MIN_OFFSET, RIGHT_PADDLE_OFFSET, nDistanceToCenterZeroToOne);
                
                ball.Vel = glm::normalize(ballVector) * ballSpeed;
                ball.Vel.y *= -1;

                OffsetByMin(ballPos.Pos, contactCenter.CollVec);
                dispatch.trigger<BallHitPaddleEvent>();
            }
            else
            {
                if (contactLeft.Collision)
                {
                    ballPos.Pos += contactLeft.CollVec;
                    ball.Vel = glm::normalize(contactLeft.CollVec) * glm::length(ball.Vel);
                    dispatch.trigger<BallHitPaddleEvent>();
                }
                else
                {
                    if (contactRight.Collision)
                    {
                        ballPos.Pos += contactRight.CollVec;
                        ball.Vel = glm::normalize(contactRight.CollVec) * glm::length(ball.Vel);
                        dispatch.trigger<BallHitPaddleEvent>();
                    }
                }
            }
        });
}

static void CollideWithWalls(entt::dispatcher& dispatch, Ball &ball, Position &pos, CollisionCircle& ballColl)
{
    if (pos.Pos.x <= 0.0f)
    {
        pos.Pos.x = 0.0f;
        ball.Vel.x *= -1.0f;
        dispatch.trigger<BallHitWallEvent>();
    }
    else if (pos.Pos.x >= RenderWindow::DEFAULT_SCREEN_WIDTH - (ballColl.Radius*2.0f))
    {
        pos.Pos.x = RenderWindow::DEFAULT_SCREEN_WIDTH - (ballColl.Radius * 2.0f);
        ball.Vel.x *= -1.0f;
        dispatch.trigger<BallHitWallEvent>();
    }
    else if (pos.Pos.y <= 0)
    {
        pos.Pos.y = 0.0f;
        ball.Vel.y *= -1.0f;
        dispatch.trigger<BallHitWallEvent>();
    }
    else if (pos.Pos.y >= RenderWindow::DEFAULT_SCREEN_HEIGHT - (ballColl.Radius * 2.0f))
    {
        ball.Vel.x = 0.0f;
        ball.Vel.y = 0.0f;
        dispatch.trigger<BallDestroyedEvent>();
    }
}

void CollisionSystem::Update(float nStep, entt::registry& reg, entt::dispatcher& dispatch)
{
    auto brickView  = reg.view<Brick, Position, CollisionAABB, Active>();
    auto ballView   = reg.view<Ball, Position, CollisionCircle, Active>();
    auto paddleView = reg.view<Player, Position, PaddleCollisionObj, Active>();

    ballView.each([&](auto& ball, auto& ballPos, auto& ballColl)
        {
            CollideWithBricks(brickView, reg, dispatch, ball, ballPos, ballColl);
            CollideWithPaddles(paddleView, reg, dispatch, ball, ballPos, ballColl);
            CollideWithWalls(dispatch, ball, ballPos, ballColl);
        });
}