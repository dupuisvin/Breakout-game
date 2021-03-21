#ifndef M_TIMER
#define M_TIMER

#include <SDL.h>

namespace SDLEngine
{
	//Timer that can used to get the timestep between each frame
	//based on https://gafferongames.com/post/fix_your_timestep/
	class Timer
	{
	public:
		
		Timer(float step) :
			TimeStep(step) {}

		//Start the timer
		void Start()
		{
			Running = true;
			CurrentTime = SDL_GetTicks() / 1000.f;
			Accumulator = 0.0f;
		}

		//Stop the timer
		void Stop()
		{
			Running = false;
		}

		//Advance the state of the timer
		void Tick()
		{
			if (Running)
			{
				float newTime = SDL_GetTicks() / 1000.f;
				float frameTime = newTime - CurrentTime;
				CurrentTime = newTime;
				Accumulator += frameTime;
			}
		}

		//Remove a timestep from the accumulator
		void TickAccumulator()
		{
			Accumulator -= TimeStep;
		}

		//Return true if the accumulator contains at least one timestep
		bool IsAccumulatorFull()
		{
			return Accumulator >= TimeStep;
		}

		//Get the time step for the current frame
		float GetTimeStep() const { return TimeStep; }

		//Return true if the time is running
		bool IsRunning() { return Running; }

	private:

		bool Running = false;
		float CurrentTime = 0.0f;
		float Accumulator = 0.0f;
		float TimeStep = 0.0f;
	};
}

#endif