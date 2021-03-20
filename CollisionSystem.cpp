#include "CollisionSystem.h"

#include "Ball.h"
#include "Position.h"
#include "Sprite.h"
#include "Brick.h"
#include "Player.h"
#include "CollisionUtils.h"
#include "Active.h"
#include "BrickDestroyedEvent.h"
#include "PaddleCollisionObj.h"
#include <glm/geometric.hpp>

#include <entt/entt.hpp>

using namespace Breakout;
using namespace SDLEngine;

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
        ballPos.Pos.x += std::ceil(minContact.CollVec.x);
        ballPos.Pos.y += std::ceil(minContact.CollVec.y);

        if (minContact.CollVec.x != 0.0f)
            ball.Vel.x *= -1;
        else if (minContact.CollVec.y != 0.0f)
            ball.Vel.y *= -1;

        reg.remove<Active>(minEntity);
        dispatch.trigger<BrickDestroyedEvent>();
    }
}


template<class PaddleView>
void CollideWithPaddles(PaddleView &paddleView, entt::registry& reg, entt::dispatcher& dispatch, Ball& ball, Position& ballPos, CollisionCircle& ballColl)
{
    paddleView.each([&](auto& player, auto& playerPos, auto& playerColl)
        {
            auto& centerAABB = reg.get<CollisionAABB>(playerColl.CenterAABB);
            CollisionContact contactCenter = CollisionUtils::Collision(ballPos.Pos, ballColl, playerPos.Pos, centerAABB);
            if (contactCenter.Collision)
            {
                ballPos.Pos.x += std::ceil(contactCenter.CollVec.x);
                ballPos.Pos.y += std::ceil(contactCenter.CollVec.y);
                if (contactCenter.CollVec.x != 0.0f)
                    ball.Vel.x *= -1;
                else if (contactCenter.CollVec.y != 0.0f)
                    ball.Vel.y *= -1;
            }
            else
            {
                auto& leftCircle = reg.get<CollisionCircle>(playerColl.LeftCircle);
                CollisionContact contactLeft = CollisionUtils::Collision(ballPos.Pos, ballColl, playerPos.Pos, leftCircle);
                if (contactLeft.Collision)
                {
                    ballPos.Pos += contactLeft.CollVec;
                    ball.Vel = glm::normalize(contactLeft.CollVec) * glm::length(ball.Vel);;
                }
                else
                {
                    auto& rightCircle = reg.get<CollisionCircle>(playerColl.RightCircle);
                    CollisionContact contactRight = CollisionUtils::Collision(ballPos.Pos, ballColl, playerPos.Pos, rightCircle);
                    if (contactRight.Collision)
                    {
                        ballPos.Pos += contactRight.CollVec;
                        ball.Vel = glm::normalize(contactRight.CollVec) * glm::length(ball.Vel);
                    }
                }
            }
        });
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
        });
}