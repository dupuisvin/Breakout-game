#ifndef M_GAME_STATE_EVENTS
#define M_GAME_STATE_EVENTS

namespace Breakout
{
    struct StartGameEvent {};
    struct ReturnToMenuEvent{};
    struct LevelDoneEvent {};
    struct PauseEvent {};
    struct UnpauseEvent{};
}

#endif // !M_GAME_STATE_EVENTS
