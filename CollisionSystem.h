#ifndef M_COLLISION_SYSTEM
#define M_COLLISION_SYSTEM

#include <entt/fwd.hpp>

namespace Breakout
{
    //System in charge of all the collisions
    class CollisionSystem
    {
    public:

        //Check for collisions between all the collision objects
        void Update(float nStep, entt::registry& reg, entt::dispatcher &dispatch);

    private:

    };
}

#endif // !M_COLLISION_SYSTEM

