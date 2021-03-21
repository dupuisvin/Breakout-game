#include "SoundSystem.h"

#include "Sound.h"
#include "Music.h"
#include <SDL_mixer.h>
#include <entt/entt.hpp>

using namespace SDLEngine;

void SoundSystem::PlaySound(entt::entity e)
{
    const auto& sound = Registry.get<Sound>(e);
    Mix_PlayChannel(-1, sound.Data.get(), 0);
}

void SoundSystem::PlayMusic(entt::entity e)
{
    const auto& music = Registry.get<Music>(e);
    Mix_VolumeMusic(static_cast<int>(0.1f * MIX_MAX_VOLUME));
    Mix_PlayMusic(music.Data.get(), -1);
}

void SoundSystem::StopMusic()
{
    Mix_HaltMusic();
}

void SoundSystem::PauseMusic()
{
    Mix_PauseMusic();
}

void SoundSystem::ResumeMusic()
{
    Mix_ResumeMusic();
}

bool SoundSystem::IsMusicPlaying()
{
    return Mix_PlayingMusic() != 0;
}