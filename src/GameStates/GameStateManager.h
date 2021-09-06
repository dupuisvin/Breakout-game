#ifndef M_GAME_STATE_MANAGER
#define M_GAME_STATE_MANAGER

#include "RenderWindow.h"
#include "GameState.h"
#include <entt/signal/dispatcher.hpp>
#include <stdint.h>
#include <stack>

namespace SDLEngine
{
    //Manages the different states of the game and how they are handled 
    class GameStateManager
    {
    public:

        GameStateManager(entt::dispatcher& dispatch, RenderWindow &window) :
            Dispatcher(dispatch),
            Window(window){}

        //Add a game state onto the current state stack
        void AddState(GameState* state)
        {
            if (!States.empty())
                States.top()->DisconnectEvents();
            States.push(state);
            state->Init(Window);
        }

        //Remove all the game states
        void Clear() 
        { 
            while (!States.empty())
                PopState();
        }

        //Get the current game state (the top of the stack)
        GameState* GetCurrentState() const { return States.top(); }

        //Remove the current game state and set the next top of the stack as the current game state
        void PopState()
        {
            States.top()->Uninit();
            States.pop();
            if (!States.empty())
                States.top()->ConnectEvents();
        }

    private:

        entt::dispatcher& Dispatcher;
        std::stack<GameState*> States;
        GameState* LastState = nullptr;
        RenderWindow& Window;
    };
}

#endif // !M_GAME_STATE_MANAGER
