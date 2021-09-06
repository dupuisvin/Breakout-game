#ifndef M_VELOCITY
#define M_VELOCITY

#include <glm/vec2.hpp>

using namespace glm;

namespace SDLEngine
{
    struct Velocity
    {
        Velocity() = default;
        Velocity(float dx, float dy) :
            Vel(dx, dy) {}
        vec2 Vel = vec2(0.0f, 0.0f);
    };
}

#endif