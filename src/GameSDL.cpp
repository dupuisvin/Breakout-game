
#include "GameSDL.h"

#include "GameState.h"
#include "KeyboardEvents.h"
#include "MouseEvents.h"
#include "QuitEvent.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdexcept>
#include <string>

using namespace std::string_literals;
using namespace SDLEngine;

static int resizeEventWatch(void* data, SDL_Event* event)
{
	if (event->type == SDL_WINDOWEVENT &&
		event->window.event == SDL_WINDOWEVENT_RESIZED)
	{
		SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);
		RenderWindow* pWindow = static_cast<RenderWindow*>(data);
		if (win == pWindow->GetWindow())
		{
			int width = 0;
			int height = 0;
			SDL_GetWindowSize(pWindow->GetWindow(), &width, &height);
			pWindow->UpdateWindowSize(width, height);
		}
	}
	return 0;
}

GameSDL::GameSDL(const std::string &windowName) :
	Window(windowName),
	StateManager(Dispatcher, Window),
	StepTimer(0.016f)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == 0)
	{
		Window.Init();
		if(TTF_Init() != 0)
			throw std::runtime_error("SDL could not intialize TTF; SDL_Error: % s\n"s + std::string(SDL_GetError()));
		if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
			throw std::runtime_error("SDL could not intialize Mixer; SDL_Error: % s\n"s + std::string(SDL_GetError()));
		SDL_AddEventWatch(resizeEventWatch, &Window);
	}
	else
		throw std::runtime_error("SDL could not intialize; SDL_Error: % s\n"s + std::string(SDL_GetError()));
}

GameSDL::~GameSDL()
{
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


void GameSDL::EventHandling()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_MOUSEMOTION:
			{
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				Dispatcher.trigger<MouseMoveEvent>(static_cast<float>(mouseX), static_cast<float>(mouseY));
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			{
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				Dispatcher.trigger<MouseButtonDownEvent>(static_cast<float>(mouseX), static_cast<float>(mouseY), e.button.button);
			}
			break;
		case SDL_MOUSEBUTTONUP:
			{
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				Dispatcher.trigger<MouseButtonUpEvent>(static_cast<float>(mouseX), static_cast<float>(mouseY), e.button.button);
			}
			break;
		case SDL_KEYDOWN:
			Dispatcher.trigger<KeyDownEvent>(e.key.keysym.sym);
			break;
		case SDL_KEYUP:
			Dispatcher.trigger<KeyUpEvent>(e.key.keysym.sym);
			break;
		case SDL_QUIT:
			Quit = true;
			break;
		}
	}
}


void GameSDL::Render(GameState &state)
{
	state.Render(Window);
}


void GameSDL::Run()
{	
	Dispatcher.sink<QuitEvent>().connect<&GameSDL::Exit>(this);
	GameState* currentState = nullptr;
	StepTimer.Start();

	while (!Quit)
	{
		EventHandling();

		auto state = StateManager.GetCurrentState();
		if (!state)
			throw std::runtime_error("Invalid game state");

		StepTimer.Tick();
		while (StepTimer.IsAccumulatorFull())
		{
			Update(*state, StepTimer.GetTimeStep());
			StepTimer.TickAccumulator();
		}
		
		Render(*state);
	}
}

void GameSDL::Update(GameState& state, float nStep)
{
	state.Update(nStep);
}