#ifndef M_BRICK
#define M_BRICK

namespace Breakout
{
    struct Brick
    {
        enum class BrickType
        {
            Blue = 0,
            Green = 1,
            Grey = 2,
            Purple = 3,
            Red = 4,
            Yellow = 5
        };

        Brick(BrickType bType) :
            Type(bType) {}

        BrickType Type = BrickType::Blue;
    };
}

#endif // !M_BRICK
