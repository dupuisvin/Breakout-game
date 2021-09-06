#include "CollisionSystem.h"

#include "Ball.h"
#include "Position.h"
#include "Sprite.h"
#include "Brick.h"
#include "Player.h"
#include "Velocity.h"
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
static void CollideBallWithBricks(BrickView &brickView, entt::dispatcher& dispatch, entt::entity ballEntity, const Ball &ball, Position& ballPos, Velocity &ballVel, CollisionCircle& ballColl)
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
        if (ball.CanCollideWithBricks())
        {
            //Update ball velocity reset position before collision
            if (std::abs(minContact.CollVec.x) < std::abs(minContact.CollVec.y))
            {
                ballVel.Vel.x *= -1;
                ballPos.Pos.x += minContact.CollVec.x;
            }
            else if (std::abs(minContact.CollVec.y) < std::abs(minContact.CollVec.x))
            {
                ballVel.Vel.y *= -1;
                ballPos.Pos.y += minContact.CollVec.y;
            }
            else
            {
                ballVel.Vel *= -1;
                ballPos.Pos += minContact.CollVec;
            }
        }

        dispatch.trigger<BallHitBrickEvent>(minEntity, ballEntity);
    }
}


template<class BrickView>
static void CollideBrickWithBricks(BrickView& brickView, entt::entity currentBrick, entt::dispatcher& dispatch, Position& brickPos, Velocity& brickVel, CollisionAABB& brickColl)
{
    if (brickVel.Vel != glm::vec2(0.0f, 0.0f))
    {
        entt::entity collEntity = entt::null;
        for (auto brickEntity : brickView)
        {
            if (brickEntity != currentBrick)
            {
                auto& pos = brickView.get<Position>(brickEntity);
                auto& coll = brickView.get<CollisionAABB>(brickEntity);
                if (CollisionUtils::IsCollision(brickPos.Pos, brickColl, pos.Pos, coll))
                {
                    collEntity = brickEntity;
                    break;
                }
            }
        }

        if (collEntity != entt::null)
        {
            dispatch.trigger<BrickDestroyedEvent>(currentBrick);
            dispatch.trigger<BrickDestroyedEvent>(collEntity);
        }
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
void CollideBallWithPaddles(PaddleView &paddleView, entt::registry& reg, entt::dispatcher& dispatch, Position& ballPos, Velocity& ballVel, CollisionCircle& ballColl)
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
                float ballSpeed = glm::length(ballVel.Vel);

                glm::vec2 ballVector = glm::normalize(ballVel.Vel);

                if (ballCenter.x < paddleCenter)
                    ballVector += Interpolate(PADDLE_MIN_OFFSET, LEFT_PADDLE_OFFSET, nDistanceToCenterZeroToOne);
                else if (ballCenter.x > paddleCenter)
                    ballVector += Interpolate(PADDLE_MIN_OFFSET, RIGHT_PADDLE_OFFSET, nDistanceToCenterZeroToOne);
                
                ballVel.Vel = glm::normalize(ballVector) * ballSpeed;
                ballVel.Vel.y *= -1;

                OffsetByMin(ballPos.Pos, contactCenter.CollVec);
                dispatch.trigger<BallHitPaddleEvent>();
            }
            else
            {
                if (contactLeft.Collision)
                {
                    ballPos.Pos += contactLeft.CollVec;
                    ballVel.Vel = glm::normalize(contactLeft.CollVec) * glm::length(ballVel.Vel);
                    dispatch.trigger<BallHitPaddleEvent>();
                }
                else
                {
                    if (contactRight.Collision)
                    {
                        ballPos.Pos += contactRight.CollVec;
                        ballVel.Vel = glm::normalize(contactRight.CollVec) * glm::length(ballVel.Vel);
                        dispatch.trigger<BallHitPaddleEvent>();
                    }
                }
            }
        });
}

static void CollideWithWalls(entt::dispatcher& dispatch, Position &pos, Velocity& vel, CollisionCircle& ballColl)
{
    if (pos.Pos.x <= 0.0f)
    {
        pos.Pos.x = 0.0f;
        vel.Vel.x *= -1.0f;
        dispatch.trigger<BallHitWallEvent>();
    }
    else if (pos.Pos.x >= RenderWindow::DEFAULT_SCREEN_WIDTH - (ballColl.Radius*2.0f))
    {
        pos.Pos.x = RenderWindow::DEFAULT_SCREEN_WIDTH - (ballColl.Radius * 2.0f);
        vel.Vel.x *= -1.0f;
        dispatch.trigger<BallHitWallEvent>();
    }
    else if (pos.Pos.y <= 0)
    {
        pos.Pos.y = 0.0f;
        vel.Vel.y *= -1.0f;
        dispatch.trigger<BallHitWallEvent>();
    }
    else if (pos.Pos.y >= RenderWindow::DEFAULT_SCREEN_HEIGHT - (ballColl.Radius * 2.0f))
    {
        vel.Vel.x = 0.0f;
        vel.Vel.y = 0.0f;
        dispatch.trigger<BallDestroyedEvent>();
    }
}

static void CollideBrickWithWalls(entt::entity currentBrick, entt::dispatcher& dispatch, Position& brickPos, CollisionAABB& brickColl)
{
    if (brickPos.Pos.x <= 0.0f ||
        brickPos.Pos.x >= RenderWindow::DEFAULT_SCREEN_WIDTH - (brickColl.Width) ||
        brickPos.Pos.y <= 0 ||
        brickPos.Pos.y >= RenderWindow::DEFAULT_SCREEN_HEIGHT - (brickColl.Height))
    {
        dispatch.trigger<BrickDestroyedEvent>(currentBrick);
    }
}

template<class PaddleView, class BallView>
static void CollideBrickWithPaddle(entt::registry& reg, PaddleView &paddleView, BallView &ballView, entt::dispatcher &dispatch,const Position& brickPos, CollisionAABB& brickColl)
{
    paddleView.each([&](auto& player, auto& playerPos, auto& playerColl)
        {
            auto& centerAABB = reg.get<CollisionAABB>(playerColl.CenterAABB);
            auto& leftCircle = reg.get<CollisionCircle>(playerColl.LeftCircle);
            auto& rightCircle = reg.get<CollisionCircle>(playerColl.RightCircle);
            
            CollisionContact contactLeft  = CollisionUtils::Collision(playerPos.Pos, leftCircle , brickPos.Pos, brickColl);
            CollisionContact contactRight = CollisionUtils::Collision(playerPos.Pos, rightCircle, brickPos.Pos, brickColl);

            if (CollisionUtils::IsCollision(playerPos.Pos, centerAABB, brickPos.Pos, brickColl) ||
                contactLeft.Collision ||
                contactRight.Collision)
            {
                ballView.each([&](auto& ball, auto& ballPos, auto& ballVel, auto& ballColl)
                    {
                        ballVel.Vel = glm::vec2(0.0f, 0.0f);
                    });
                dispatch.trigger<BallDestroyedEvent>();
            }
        });
}

void CollisionSystem::Update(float nStep, entt::registry& reg, entt::dispatcher& dispatch)
{
    auto brickView  = reg.view<Brick, Position, CollisionAABB, Active>();
    auto ballView   = reg.view<Ball, Position, Velocity, CollisionCircle, Active>();
    auto paddleView = reg.view<Player, Position, PaddleCollisionObj, Active>();

    for (auto ballEntity : ballView)
    {
        auto& ball = reg.get<Ball>(ballEntity);
        auto& ballPos = reg.get<Position>(ballEntity);
        auto& ballVel = reg.get<Velocity>(ballEntity);
        auto& ballColl = reg.get<CollisionCircle>(ballEntity);

        CollideBallWithPaddles(paddleView, reg, dispatch, ballPos, ballVel, ballColl);
        CollideWithWalls(dispatch, ballPos, ballVel, ballColl);
        CollideBallWithBricks(brickView, dispatch, ballEntity, ball, ballPos, ballVel, ballColl);
    }

    auto movingBrickView = reg.view<Brick, Position, Velocity, CollisionAABB, Active>();
    for (auto brickEntity : movingBrickView)
    {
        auto& pos = reg.get<Position>(brickEntity);
        auto& vel = reg.get<Velocity>(brickEntity);
        auto& coll = reg.get<CollisionAABB>(brickEntity);
        CollideBrickWithBricks(brickView, brickEntity, dispatch, pos, vel, coll);
        CollideBrickWithWalls(brickEntity, dispatch, pos, coll);
        CollideBrickWithPaddle(reg, paddleView, ballView, dispatch, pos, coll);
    }
}