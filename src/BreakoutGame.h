#ifndef  M_BREAKOUT_GAME
#define  M_BREAKOUT_GAME

#include "GameSDL.h"

#include "MainMenuGameState.h"
#include "LevelGameState.h"
#include "PauseMenuGameState.h"
#include "GameOverGameState.h"
#include "LevelEditorGameState.h"

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

        void GameOver();
        void StartGame();
        void NextLevel();
        void OpenEditor();
        void PauseMenuOpened();
        void PauseMenuClosed();
        void ReturnToMenu();

        MainMenuGameState MainMenuState;
        LevelGameState Level1State;
        LevelGameState Level2State;
        LevelGameState Level3State;
        LevelGameState Level4State;
        PauseMenuGameState PauseState;
        GameOverGameState GameOverState;
        LevelEditorGameState LevelEditorState;
        uint32_t CurrentLevel = 0;
        std::array<SDLEngine::GameState*, 4> Levels;

    };
}

#endif // ! M_BREAKOUT_GAME
