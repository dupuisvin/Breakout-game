#ifndef M_MOUSE_EVENTS
#define M_MOUSE_EVENTS

namespace SDLEngine
{
    //Event called when the mouse is moved
    struct MouseMoveEvent
    {
        MouseMoveEvent(int x, int y) :
            PosX(x),
            PosY(y) {}

        int PosX = -1;
        int PosY = -1;
    };

    //Event called when a mouse button is pressed
    struct MouseButtonDownEvent
    {
        MouseButtonDownEvent(int x, int y, int button) :
            PosX(x),
            PosY(y),
            Button(button){}

        int PosX = -1;
        int PosY = -1;
        int Button = -1;
    };

    //Event called when a mouse button is released
    struct MouseButtonUpEvent
    {
        MouseButtonUpEvent(int x, int y, int button) :
            PosX(x),
            PosY(y),
            Button(button) {}

        int PosX = -1;
        int PosY = -1;
        int Button = -1;
    };
}


#endif