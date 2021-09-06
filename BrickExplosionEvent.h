#ifndef M_BRICK_EXPLOSION_EVENT
#define M_BRICK_EXPLOSION_EVENT

#include <entt/entity/entity.hpp>

namespace Breakout
{
    //Event called when a brick explodes
    struct BrickExplosionEvent
    { 
        BrickExplosionEvent(entt::entity e) :
            BrickEntity(e) {}
        entt::entity BrickEntity = entt::null;
    };
}

#endif // !M_BRICK_EXPLOSION_EVENT
