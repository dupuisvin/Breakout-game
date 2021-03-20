
#include "BreakoutGame.h"
#include "GameStatesEvents.h"
#include "MainMenuGameState.h"
#include "LevelGameState.h"

using namespace SDLEngine;
using namespace Breakout;

BreakoutGame::BreakoutGame() :
    GameSDL("Breakout Clone"),
    MainMenuState(Registry, Dispatcher),
    PauseState(Registry, Dispatcher),
    Level1State("Assets/Levels/Level1.json", Registry, Dispatcher),
    Level2State("Assets/Levels/Level2.json", Registry, Dispatcher)
{
    Dispatcher.sink<StartGameEvent>().connect<&BreakoutGame::StartGame>(this);
    Dispatcher.sink<LevelDoneEvent>().connect<&BreakoutGame::NextLevel>(this);
    Dispatcher.sink<PauseEvent>().connect<&BreakoutGame::PauseMenuOpened>(this);
    Dispatcher.sink<UnpauseEvent>().connect<&BreakoutGame::PauseMenuClosed>(this);
    Dispatcher.sink<ReturnToMenuEvent>().connect<&BreakoutGame::ReturnToMenu>(this);
    StateManager.AddState(&MainMenuState);

    Levels[0] = &Level1State;
    Levels[1] = &Level2State;
}

void BreakoutGame::StartGame()
{
    StepTimer.Start();
    CurrentLevel = 0;
    StateManager.PopState();
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
        //End game screen and then back to main menu
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