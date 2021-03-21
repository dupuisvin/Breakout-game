#ifndef M_SOUND_SYSTEM
#define M_SOUND_SYSTEM

#include <entt/fwd.hpp>

namespace SDLEngine
{
    class SoundSystem
    {
    public:

        //System in charge of playing sounds and music
        SoundSystem(entt::registry& reg, entt::dispatcher& dispatcher) :
            Registry(reg),
            Dispatcher(dispatcher) {}

        //Play the sound associated with the specified entity
        void PlaySound(entt::entity e);

        //Play the music associated with the specified entity
        void PlayMusic(entt::entity e);

        //Stop the music currently playing
        void StopMusic();

        //Pause the music currently playing
        void PauseMusic();

        //Resume the music currently paused
        void ResumeMusic();

        //Return true if a music is currently playing
        bool IsMusicPlaying();

    protected:

        entt::registry& Registry;
        entt::dispatcher& Dispatcher;
    };
}

#endif // !M_SOUND_SYSTEM
