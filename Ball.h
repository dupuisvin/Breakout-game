#ifndef M_BALL
#define M_BALL

#include <glm/vec2.hpp>

namespace Breakout
{
    struct Ball
    {
        enum class BallMode
        {
            Normal,
            Passtrough_Blue
        };

        static constexpr float DEFAULT_VELOCITY_X = 320;
        static constexpr float DEFAULT_VELOCITY_Y = -320;
        
        BallMode Mode = BallMode::Normal;
        
        //Return true if the ball can collide with brick, false otherwise
        bool CanCollideWithBricks() const
        {
            return Mode == BallMode::Normal;
        }
    };
}

#endif // !M_BALL
