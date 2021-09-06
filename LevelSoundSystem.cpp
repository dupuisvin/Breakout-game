#include "LevelSoundSystem.h"
#include <entt/entt.hpp>
#include <SDL_mixer.h>
#include "Sound.h"
#include "Music.h"
#include "BallDestroyedEvent.h"
#include "BallHitBrickEvent.h"
#include "BallHitPaddleEvent.h"
#include "BallHitWallEvent.h"
#include "BrickDestroyedEvent.h"
#include "BrickExplosionEvent.h"

using namespace SDLEngine;
using namespace Breakout;

static constexpr char DESTRUCTION_SOUND_3[] = "Assets/Sounds/explosion3.ogg";
static constexpr char DESTRUCTION_SOUND_4[] = "Assets/Sounds/explosion4.ogg";
static constexpr char EXPLOSION_SOUND[] = "Assets/Sounds/rumble3.ogg";
static constexpr char HIT_SOUND_2[] = "Assets/Sounds/hit2.ogg";
static constexpr char HIT_SOUND_3[] = "Assets/Sounds/hit3.ogg";
static constexpr char JUMP_SOUND_2[] = "Assets/Sounds/jump2.ogg";

LevelSoundSystem::~LevelSoundSystem()
{
}

void LevelSoundSystem::Init(const std::string& levelMusic)
{
    BallHitBrickSound   = Registry.create();
    BallHitPaddleSound  = Registry.create();
    BallHitWallSound    = Registry.create();
    BallDestroyedSound  = Registry.create();
    BrickDestroyedSound = Registry.create();
    ExplosionSound      = Registry.create();
    LevelMusic          = Registry.create();

    Registry.emplace<Sound>(BallHitBrickSound  , Mix_LoadWAV(HIT_SOUND_2));
    Registry.emplace<Sound>(BallHitPaddleSound , Mix_LoadWAV(JUMP_SOUND_2));
    Registry.emplace<Sound>(BallHitWallSound   , Mix_LoadWAV(HIT_SOUND_3));
    Registry.emplace<Sound>(BallDestroyedSound , Mix_LoadWAV(DESTRUCTION_SOUND_3));
    Registry.emplace<Sound>(BrickDestroyedSound, Mix_LoadWAV(DESTRUCTION_SOUND_4));
    Registry.emplace<Sound>(ExplosionSound     , Mix_LoadWAV(EXPLOSION_SOUND));
    Registry.emplace<Music>(LevelMusic         , Mix_LoadMUS(levelMusic.c_str()));

    Dispatcher.sink<BallDestroyedEvent>().connect<&LevelSoundSystem::PlayBallDestroyedSound>(this);
    Dispatcher.sink<BallHitBrickEvent>().connect<&LevelSoundSystem::PlayBallHitBrickSound>(this);
    Dispatcher.sink<BallHitPaddleEvent>().connect<&LevelSoundSystem::PlayBallHitPaddleSound>(this);
    Dispatcher.sink<BallHitWallEvent>().connect<&LevelSoundSystem::PlayBallHitWallSound>(this);
    Dispatcher.sink<BrickDestroyedEvent>().connect<&LevelSoundSystem::PlayBrickDestroyedSound>(this);
    Dispatcher.sink<BrickExplosionEvent>().connect<&LevelSoundSystem::PlayExplosionSound>(this);

    PlayMusic(LevelMusic);
}

void LevelSoundSystem::Uninit()
{
    StopMusic();

    Dispatcher.sink<BallDestroyedEvent>().disconnect<&LevelSoundSystem::PlayBallDestroyedSound>(this);
    Dispatcher.sink<BallHitBrickEvent>().disconnect<&LevelSoundSystem::PlayBallHitBrickSound>(this);
    Dispatcher.sink<BallHitPaddleEvent>().disconnect<&LevelSoundSystem::PlayBallHitPaddleSound>(this);
    Dispatcher.sink<BallHitWallEvent>().disconnect<&LevelSoundSystem::PlayBallHitWallSound>(this);
    Dispatcher.sink<BrickDestroyedEvent>().disconnect<&LevelSoundSystem::PlayBrickDestroyedSound>(this);
    Dispatcher.sink<BrickExplosionEvent>().disconnect<&LevelSoundSystem::PlayExplosionSound>(this);

    Registry.destroy(BallHitBrickSound  );
    Registry.destroy(BallHitPaddleSound );
    Registry.destroy(BallHitWallSound   );
    Registry.destroy(BallDestroyedSound );
    Registry.destroy(BrickDestroyedSound);
    Registry.destroy(ExplosionSound     );
    Registry.destroy(LevelMusic         );
}
