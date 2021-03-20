#ifndef M_MENU_SOUND_SYSTEM
#define M_MENU_SOUND_SYSTEM

#include "SoundSystem.h"
#include <entt/fwd.hpp>
#include <entt/entity/entity.hpp>
#include <string>

namespace Breakout
{
    class MenuSoundSystem : public SDLEngine::SoundSystem
    {
    public:

        MenuSoundSystem(entt::registry& reg, entt::dispatcher& dispatcher) :
            SoundSystem(reg, dispatcher) {}
        void Init(const std::string &musicPath);
        void Uninit();

    private:

        void PlayMenuMusic() { PlaySound(MenuMusic); }
        void PlayButtonClickSound() { PlaySound(ButtonClickSound); }
        void PlayMouseClickSound() { PlaySound(MouseClickSound); }
        void PlayMouseReleasekSound() { PlaySound(MouseReleaseSound); }

        entt::entity MouseClickSound   = entt::null;
        entt::entity MouseReleaseSound = entt::null;
        entt::entity ButtonClickSound  = entt::null;
        entt::entity MenuMusic         = entt::null;
        bool MusicPaused = false;

    };
}

#endif // !M_MENU_SOUND_SYSTEM
