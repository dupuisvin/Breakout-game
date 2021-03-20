#ifndef M_GAME_SDL
#define M_GAME_SDL

#include "RenderWindow.h"
#include "RenderSystem.h"
#include "GameStateManager.h"
#include "Timer.h"
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <string>

namespace SDLEngine
{
	class GameState;

	//A game engine made with SDL
	class GameSDL
	{
	public:

		GameSDL(const std::string &windowName);
		~GameSDL();

		void Run();
		void Exit() { Quit = true; }

	protected:

		GameStateManager StateManager;
		entt::dispatcher Dispatcher;
		RenderWindow Window;
		RenderSystem RenderSys;
		Timer StepTimer;

	private:

		//Handle the input events
		void EventHandling();

		//Render the specified game state
		void Render(GameState& state);

		//Update the specified game state
		void Update(GameState& state, float nStep);

		bool Quit = false;

	};
}




#endif