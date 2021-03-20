
#include "BreakoutGame.h"
#include "GameStatesEvents.h"
#include "MainMenuGameState.h"
#include "LevelGameState.h"

using namespace SDLEngine;
using namespace Breakout;

BreakoutGame::BreakoutGame() :
    GameSDL("Breakout Clone"),
    MainMenuState(Dispatcher),
    PauseState(Dispatcher),
    Level1State("Assets/Levels/Level1.json", Dispatcher),
    Level2State("Assets/Levels/Level2.json", Dispatcher),
    Level3State("Assets/Levels/Level3.json", Dispatcher),
    Level4State("Assets/Levels/Level4.json", Dispatcher)
{
    Dispatcher.sink<StartGameEvent>().connect<&BreakoutGame::StartGame>(this);
    Dispatcher.sink<LevelDoneEvent>().connect<&BreakoutGame::NextLevel>(this);
    Dispatcher.sink<PauseEvent>().connect<&BreakoutGame::PauseMenuOpened>(this);
    Dispatcher.sink<UnpauseEvent>().connect<&BreakoutGame::PauseMenuClosed>(this);
    Dispatcher.sink<ReturnToMenuEvent>().connect<&BreakoutGame::ReturnToMenu>(this);
    StateManager.AddState(&MainMenuState);

    Levels[0] = &Level1State;
    Levels[1] = &Level2State;
    Levels[2] = &Level3State;
    Levels[3] = &Level4State;
}

void BreakoutGame::StartGame()
{
    StepTimer.Start();
    CurrentLevel = 0;
    StateManager.AddState(&Level1State);
}

void BreakoutGame::NextLevel()
{
    CurrentLevel++;
    if (CurrentLevel < Levels.size())
    {
        StateManager.PopState();
        StateManager.AddState(Levels[CurrentLevel]);
    }
    else
    {
        //@todo End game screen and then back to main menu
        ReturnToMenu(); //@temp
    }
}

void BreakoutGame::PauseMenuOpened()
{
    StateManager.AddState(&PauseState);
    StepTimer.Stop();
}

void BreakoutGame::PauseMenuClosed()
{
    StateManager.PopState();
    StepTimer.Start();
}

void BreakoutGame::ReturnToMenu()
{
    CurrentLevel = 0;
    StateManager.Clear();
    StateManager.AddState(&MainMenuState);
}