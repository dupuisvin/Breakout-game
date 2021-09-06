#ifndef M_SOUND
#define M_SOUND

#include <SDL_mixer.h>
#include <memory>

namespace SDLEngine
{
    //Struct that contains a sound asset
    struct Sound
    {
    private:

        //Deleter to remove the sound chunk properly
        struct SoundDeleter
        {
            void operator()(Mix_Chunk* p) { Mix_FreeChunk(p); }
        };

    public:

        Sound() = default;
        Sound(Mix_Chunk* chunk) :
            Data(chunk) {}

        std::unique_ptr<Mix_Chunk, SoundDeleter> Data;
    };
}

#endif // !M_SOUND
