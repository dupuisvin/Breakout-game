#ifndef M_ELAPSED_TIMER
#define M_ELAPSED_TIMER

#include <SDL.h>

namespace SDLEngine
{
	class ElapsedTimer
	{
	public:
		
		ElapsedTimer() = default;

		float GetLimit() const { return Limit; }

		//Start the timer
		void Start(float limit)
		{
			Running = true;
			StartTime = SDL_GetTicks() / 1000.f;
			Limit = limit;
		}

		//Stop the timer
		void Stop()
		{
			Running = false;
		}

		//Advance the state of the timer and return the elapsed time since the start
		float Tick()
		{
			if (Running)
			{
				float newTime = SDL_GetTicks() / 1000.f;
				return newTime - StartTime;
			}
			return 0.0f;
		}

		//Advance the state of the timer and return true if the limit was reached, false if not
		bool TickAndValidate()
		{
			if (Running)
				return Tick() >= Limit;
			return false;
		}

		//Return true if the time is running
		bool IsRunning() { return Running; }

	private:

		bool Running = false;
		float StartTime = 0.0f;
		float Limit = 0.0f;
	};
}

#endif