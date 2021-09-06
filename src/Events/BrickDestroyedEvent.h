#ifndef M_BRICK_DESTROYED_EVENT
#define M_BRICK_DESTROYED_EVENT

#include <entt/entity/entity.hpp>

namespace Breakout
{
    //Event called when a brick is destroyed
    struct BrickDestroyedEvent
    { 
        BrickDestroyedEvent(entt::entity e) :
            BrickEntity(e) {}
        entt::entity BrickEntity = entt::null;
    };
}

#endif // !M_BRICK_DESTROYED_EVENT
