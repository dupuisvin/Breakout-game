#include "MenuSoundSystem.h"

#include "Sound.h"
#include "Music.h"
#include "MouseEvents.h"
#include "ButtonClickedEvent.h"
#include <entt/entt.hpp>

using namespace SDLEngine;
using namespace Breakout;

static constexpr char MOUSE_CLICK_SOUND[]   = "Assets/Sounds/mouseclick1.ogg";
static constexpr char MOUSE_RELEASE_SOUND[] = "Assets/Sounds/mouserelease1.ogg";
static constexpr char BUTTON_CLICKED_SOUND[] = "Assets/Sounds/upgrade1.ogg";

void MenuSoundSystem::Init(const std::string& musicPath)
{
    MouseClickSound = Registry.create();
    MouseReleaseSound = Registry.create();
    ButtonClickSound = Registry.create();

    Registry.emplace<Sound>(MouseClickSound, Mix_LoadWAV(MOUSE_CLICK_SOUND));
    Registry.emplace<Sound>(MouseReleaseSound, Mix_LoadWAV(MOUSE_RELEASE_SOUND));
    Registry.emplace<Sound>(ButtonClickSound, Mix_LoadWAV(BUTTON_CLICKED_SOUND));

    Dispatcher.sink<MouseButtonDownEvent>().connect<&MenuSoundSystem::PlayMouseClickSound>(this);
    Dispatcher.sink<MouseButtonUpEvent>().connect<&MenuSoundSystem::PlayMouseReleasekSound>(this);
    Dispatcher.sink<ButtonClickedEvent>().connect<&MenuSoundSystem::PlayButtonClickSound>(this);

    MusicPaused = false;
    if (!musicPath.empty())
    {
        StopMusic();
        MenuMusic = Registry.create();
        Registry.emplace<Music>(MenuMusic, Mix_LoadMUS(musicPath.c_str()));
        PlayMusic(MenuMusic);
    }
    else
    {
        if (IsMusicPlaying())
        {
            MusicPaused = true;
            PauseMusic();
        }
        else
            StopMusic();
    }
}

void MenuSoundSystem::Uninit()
{
    if (MusicPaused)
        ResumeMusic();
    else
        StopMusic();

    Dispatcher.sink<MouseButtonDownEvent>().disconnect<&MenuSoundSystem::PlayMouseClickSound>(this);
    Dispatcher.sink<MouseButtonUpEvent>().disconnect<&MenuSoundSystem::PlayMouseReleasekSound>(this);
    Dispatcher.sink<ButtonClickedEvent>().disconnect<&MenuSoundSystem::PlayButtonClickSound>(this);

    Registry.destroy(MouseClickSound);
    Registry.destroy(MouseReleaseSound);
    Registry.destroy(ButtonClickSound);

    if(MenuMusic != entt::null)
        Registry.destroy(MenuMusic);
}
