#ifndef M_MUSIC
#define M_MUSIC

#include <SDL_mixer.h>
#include <memory>

namespace SDLEngine
{
    struct Music
    {
    private:

        //Deleter to remove the music chunk properly
        struct MusicDeleter
        {
            void operator()(Mix_Music* p) { Mix_FreeMusic(p); }
        };

    public:

        Music() = default;
        Music(Mix_Music* music) :
            Data(music) {}
        std::unique_ptr<Mix_Music, MusicDeleter> Data;
    };
}

#endif // !M_MUSIC
