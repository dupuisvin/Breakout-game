#ifndef M_SOUND_SYSTEM
#define M_SOUND_SYSTEM

#include <entt/fwd.hpp>

namespace SDLEngine
{
    class SoundSystem
    {
    public:

        SoundSystem(entt::registry& reg, entt::dispatcher& dispatcher) :
            Registry(reg),
            Dispatcher(dispatcher) {}

    protected:

        void PlaySound(entt::entity e);
        void PlayMusic(entt::entity e);
        void StopMusic();
        void PauseMusic();
        void ResumeMusic();
        bool IsMusicPlaying();

        entt::registry& Registry;
        entt::dispatcher& Dispatcher;
    };
}

#endif // !M_SOUND_SYSTEM
