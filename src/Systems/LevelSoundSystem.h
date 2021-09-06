#ifndef M_LEVEL_SOUND_SYSTEM
#define M_LEVEL_SOUND_SYSTEM

#include "SoundSystem.h"
#include <entt/fwd.hpp>
#include <entt/entity/entity.hpp>
#include <string>

namespace Breakout
{
    //Handles all the sound effects and music for a level
    class LevelSoundSystem : public SDLEngine::SoundSystem
    {
    public:

        LevelSoundSystem(entt::registry& reg, entt::dispatcher& dispatcher) :
            SoundSystem(reg, dispatcher){}
        ~LevelSoundSystem();

        //Load all the sounds and connect all the events
        void Init(const std::string &levelMusic);

        //Unload all the sounds and disconnect all the events
        void Uninit();

    private:

        void PlayBallHitBrickSound() { PlaySound(BallHitBrickSound); }
        void PlayBallHitPaddleSound() { PlaySound(BallHitPaddleSound); }
        void PlayBallHitWallSound() { PlaySound(BallHitWallSound); }
        void PlayBallDestroyedSound() { PlaySound(BallDestroyedSound); }
        void PlayBrickDestroyedSound() { PlaySound(BrickDestroyedSound); }
        void PlayExplosionSound() { PlaySound(ExplosionSound); }
        void PlayLevelMusic() { PlaySound(LevelMusic); }

        entt::entity BallHitBrickSound   = entt::null;
        entt::entity BallHitPaddleSound  = entt::null;
        entt::entity BallHitWallSound    = entt::null;
        entt::entity BallDestroyedSound  = entt::null;
        entt::entity BrickDestroyedSound = entt::null;
        entt::entity ExplosionSound      = entt::null;
        entt::entity LevelMusic          = entt::null;
    };
}

#endif // !M_LEVEL_SOUND_SYSTEM