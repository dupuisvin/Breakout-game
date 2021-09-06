#ifndef M_MOVE_SYSTEM
#define M_MOVE_SYSTEM

#include <entt/fwd.hpp>
#include "Player.h"

namespace SDLEngine
{
    struct KeyDownEvent;
    struct KeyUpEvent;
}

namespace Breakout
{
    //System in charge of all the movements of the dynamic entities and the player inputs
    class MoveSystem
    {
    public:

        MoveSystem(entt::registry& reg) :
            Registry(reg){}

        //Launch the ball
        void LaunchBall();

        //Called when a keybord key is pressed down
        void OnKeyDown(const SDLEngine::KeyDownEvent& event);

        //Called when a keybord key is released
        void OnKeyUp(const SDLEngine::KeyUpEvent& event);

        //Update the position of all the dynamic entities
        void Update(float nStep);

    private:

        Player::Direction PlayerDir = Player::Direction::Idle;
        entt::registry &Registry;
    };
}

#endif // !M_MOVE_SYSTEM
