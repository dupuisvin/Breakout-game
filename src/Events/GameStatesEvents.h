#ifndef M_GAME_STATE_EVENTS
#define M_GAME_STATE_EVENTS

namespace Breakout
{
    //Event called when the game starts after the main menu
    struct StartGameEvent {};

    //Event called with a request to return to the main menu
    struct ReturnToMenuEvent{};

    //Event called when the current level is done
    struct LevelDoneEvent {};

    //Event called when the game is paused
    struct PauseEvent {};

    //Event called when the game is unpaused
    struct UnpauseEvent{};

    //Event called when the player has no more lives
    struct GameOverEvent {};

    //Event called when the level editor is opened
    struct OpenEditorEvent {};
}

#endif // !M_GAME_STATE_EVENTS
