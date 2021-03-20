#ifndef M_TIMER
#define M_TIMER

#include <SDL.h>

namespace SDLEngine
{
	//Timer that can used to get the timestep between each frame
	class Timer
	{
	public:
		
		void Start()
		{
			Started = true;
			StartTicks = SDL_GetTicks();
		}

		void Stop()
		{
			Started = false;
			StartTicks = 0;
		}

		void Tick()
		{
			if (Started)
				StartTicks = SDL_GetTicks();
		}

		uint32_t GetTicks()
		{
			uint32_t time = 0;
			if (Started)
				time = SDL_GetTicks() - StartTicks;
			return time;
		}
		bool IsStarted() { return Started; }

	private:
		uint32_t StartTicks = 0;
		bool Started = false;
	};
}

#endif