#ifndef M_PADDLE_COLLISION_OBJ
#define M_PADDLE_COLLISION_OBJ

#include <entt/entity/entity.hpp>

namespace Breakout
{
    struct PaddleCollisionObj
    {
        PaddleCollisionObj() = default;
        PaddleCollisionObj(entt::entity center, entt::entity left, entt::entity right) :
            CenterAABB(center),
            LeftCircle(left),
            RightCircle(right) {}
        entt::entity CenterAABB  = entt::null;
        entt::entity LeftCircle  = entt::null;
        entt::entity RightCircle = entt::null;
    };
}

#endif // !M_PADDLE_COLLISION_OBJ
