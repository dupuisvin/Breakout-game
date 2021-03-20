#ifndef M_GAME_STATE_MANAGER
#define M_GAME_STATE_MANAGER

#include "RenderWindow.h"
#include "GameState.h"
#include <entt/signal/dispatcher.hpp>
#include <stdint.h>
#include <stack>

namespace SDLEngine
{
    class GameStateManager
    {
    public:

        GameStateManager(entt::dispatcher& dispatch, RenderWindow &window) :
            Dispatcher(dispatch),
            Window(window){}

        void AddState(GameState* state)
        {
            if (!States.empty())
                States.top()->DisconnectEvents();
            States.push(state);
            state->Init(Window);
        }

        void Clear() 
        { 
            while (!States.empty())
                PopState();
        }

        GameState* GetCurrentState() const { return States.top(); }

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
