#ifndef M_LEVEL_LOADER
#define M_LEVEL_LOADER

#include "LevelBreakout.h"
#include "Brick.h"
#include <SDL.h>
#include <entt/fwd.hpp>
#include <string>

namespace Breakout
{
    struct BrickAsset
    {
        BrickAsset(Brick::BrickType bType, int x, int y) :
            Type(bType),
            PosX(x),
            PosY(y) {}
        Brick::BrickType Type = Brick::BrickType::Blue;
        int PosX = 0;
        int PosY = 0;
    };

    struct LevelAssets
    {
        std::string Background;
        std::string Music;
        std::vector<BrickAsset> Bricks;
    };
    
    //Helper class to load all the appropriate textures for a level
    class LevelLoader
    {
    public:

        static std::string GetBrickPath(Brick::BrickType bType)
        {
            switch (bType)
            {
            case Brick::BrickType::Blue:
                return "Assets/Graphics/Bricks/element_blue_square.png";
            case Brick::BrickType::Green:
                return "Assets/Graphics/Bricks/element_green_square.png";
            case Brick::BrickType::Grey:
                return "Assets/Graphics/Bricks/element_grey_square.png";
            case Brick::BrickType::Purple:
                return "Assets/Graphics/Bricks/element_purple_square.png";
            case Brick::BrickType::Red:
                return "Assets/Graphics/Bricks/element_red_square.png";
            case Brick::BrickType::Yellow:
                return "Assets/Graphics/Bricks/element_yellow_square.png";
            }
            return std::string();
        }

        //Load the level json file into the entity registry and load all the textures
        static LevelBreakout LoadLevel(std::string filename, entt::registry& reg, SDL_Renderer* renderer);
        static void LoadBrickSprites(LevelBreakout& level, SDL_Renderer* renderer);
        static void SaveLevel(std::string filename, const LevelAssets &level, entt::registry &reg);

    private:

    };
}

#endif // !M_LEVEL_LOADER
