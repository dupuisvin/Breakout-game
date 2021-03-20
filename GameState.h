#ifndef M_GAME_STATE
#define M_GAME_STATE

#include <entt/fwd.hpp>
#include <entt/entity/registry.hpp>

namespace SDLEngine
{
    class RenderWindow;

    //Base class for all game states
    class GameState
    {
    public:

        GameState(entt::dispatcher& dispatcher) :
            Dispatcher(dispatcher){}

        //Connect all the events to this state
        virtual void ConnectEvents() = 0;

        //Disconnect all the events to this state
        virtual void DisconnectEvents() = 0;

        //Initialize the game state, loading all the textures, ...
        virtual void Init(RenderWindow& window) = 0;

        //Render this game state to the specified window
        virtual void Render(RenderWindow& window) = 0;
        
        //Uninitialize the game state, unloading all the textures, ...
        virtual void Uninit() = 0;

        //Update this game state with the specified time step (in seconds)
        virtual void Update(float nStep) = 0;

    protected:

        entt::registry Registry;
        entt::dispatcher& Dispatcher;

    };
}

#endif // !#define M_MAIN_MENU_GAME_STATE
