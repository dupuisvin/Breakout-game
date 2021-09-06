#ifndef M_KEYBOARD_EVENTS
#define M_KEYBOARD_EVENTS

namespace SDLEngine
{
    //Event called when a keyboard button is pressed down
    struct KeyDownEvent
    {
        KeyDownEvent(int key) :
            KeyCode(key) {}
        int KeyCode = -1;
    };

    //Event called when a keyboard button is released
    struct KeyUpEvent
    {
        KeyUpEvent(int key) :
            KeyCode(key) {}
        int KeyCode = -1;
    };
}

#endif // !M_KEYBOARD_EVENTS
