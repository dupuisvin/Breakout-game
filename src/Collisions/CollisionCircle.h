#ifndef M_COLLISION_CIRCLE
#define M_COLLISION_CIRCLE

#include <glm/vec2.hpp>

namespace SDLEngine
{
    struct CollisionCircle
    {
        CollisionCircle() = default;
        CollisionCircle(float r, glm::vec2 off) : Radius(r), PosOffset(off) {}
        float Radius = 0.0;
        glm::vec2 PosOffset;
    };
}

#endif // !M_COLLISION_CIRCLE
