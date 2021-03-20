#ifndef M_COLLISION_AABB
#define M_COLLISION_AABB

#include <glm/vec2.hpp>

namespace SDLEngine
{
    struct CollisionAABB
    {
        CollisionAABB() = default;
        CollisionAABB(float width, float height, glm::vec2 off) :
            Width(width),
            Height(height),
            PosOffset(off) {}
        float Width  = 0.0f;
        float Height = 0.0f;
        glm::vec2 PosOffset;

        /// Get the center x coordinate
        constexpr float CX(float posX) const { return posX+ PosOffset.x + HW(); }

        /// Get the center y coordinate
        constexpr float CY(float posY) const { return posY + PosOffset.y + HH(); }

        /// Get the half width
        constexpr float HW() const { return Width / 2.0f; }

        /// Get the half height
        constexpr float HH() const { return Height / 2.0f; }
    };
}

#endif // !M_COLLISION_AABB
