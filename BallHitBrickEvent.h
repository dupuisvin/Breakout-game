#ifndef M_BALL_HIT_BRICK_EVENT
#define M_BALL_HIT_BRICK_EVENT

#include <entt/entity/entity.hpp>

namespace Breakout
{
    //Event called when the bal hits a brick
    struct BallHitBrickEvent
    {
        BallHitBrickEvent(entt::entity brickEntity, entt::entity ballEntity) :
            BrickEntity(brickEntity),
            BallEntity(ballEntity){}
        entt::entity BrickEntity = entt::null;
        entt::entity BallEntity = entt::null;
    };
}

#endif // !M_BALL_HIT_BRICK_EVENT
