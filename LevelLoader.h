#ifndef M_LEVEL_LOADER
#define M_LEVEL_LOADER

#include "LevelBreakout.h"
#include <SDL.h>
#include <entt/fwd.hpp>
#include <string>

namespace Breakout
{
    //Helper class to load all the appropriate textures for a level
    class LevelLoader
    {
    public:
        //Load the level json file into the entity registry and load all the textures
        static LevelBreakout LoadLevel(std::string filename, entt::registry &reg, SDL_Renderer* renderer);
    };
}

#endif // !M_LEVEL_LOADER
