#ifndef M_BALL
#define M_BALL

#include <glm/vec2.hpp>

namespace Breakout
{
    struct Ball
    {
        static constexpr float DEFAULT_VELOCITY_X = 320;
        static constexpr float DEFAULT_VELOCITY_Y = -320;
        glm::vec2 Vel;
    };
}

#endif // !M_BALL
