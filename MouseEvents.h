#ifndef M_MOUSE_EVENTS
#define M_MOUSE_EVENTS

namespace SDLEngine
{
    //Event called when the mouse is moved
    struct MouseMoveEvent
    {
        MouseMoveEvent(float x, float y) :
            PosX(x),
            PosY(y) {}

        float PosX = 0.0f;
        float PosY = 0.0f;
    };

    //Event called when a mouse button is pressed
    struct MouseButtonDownEvent
    {
        MouseButtonDownEvent(float x, float y, int button) :
            PosX(x),
            PosY(y),
            Button(button){}

        float PosX = 0.0f;
        float PosY = 0.0f;
        int Button = -1;
    };

    //Event called when a mouse button is released
    struct MouseButtonUpEvent
    {
        MouseButtonUpEvent(float x, float y, int button) :
            PosX(x),
            PosY(y),
            Button(button) {}

        float PosX = 0.0f;
        float PosY = 0.0f;
        int Button = -1;
    };
}


#endif