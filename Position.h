#ifndef M_POSITION
#define M_POSITION

#include <glm/vec2.hpp>

using namespace glm;

namespace SDLEngine
{
    struct Position
    {
        Position() = default;
        Position(float x, float y) :
            Pos(x, y) {}
        vec2 Pos = vec2(0.0f, 0.0f);
    };
}

#endif