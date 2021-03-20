#ifndef  M_BREAKOUT_GAME
#define  M_BREAKOUT_GAME

#include "GameSDL.h"

#include "MainMenuGameState.h"
#include "LevelGameState.h"
#include "PauseMenuGameState.h"

namespace SDLEngine
{
    class GameState;
}

namespace Breakout
{
    class BreakoutGame : public SDLEngine::GameSDL
    {
    public:

        BreakoutGame();

    private:

        void StartGame();
        void NextLevel();
        void PauseMenuOpened();
        void PauseMenuClosed();
        void ReturnToMenu();

        MainMenuGameState MainMenuState;
        LevelGameState Level1State;
        LevelGameState Level2State;
        PauseMenuGameState PauseState;
        uint32_t CurrentLevel = 0;
        std::array<SDLEngine::GameState*, 2> Levels;

    };
}

#endif // ! M_BREAKOUT_GAME
