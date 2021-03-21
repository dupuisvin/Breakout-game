#ifndef M_PLAYER
#define M_PLAYER

namespace Breakout
{
    struct Player
    {
        static constexpr float ACCELERATION = 0.15f;

        enum class Direction
        {
            Idle,
            Left,
            Right
        };

        Direction Dir = Direction::Idle;
        int Lives = 3;
    };
}

#endif // !M_PLAYER
