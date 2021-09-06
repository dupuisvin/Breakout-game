#ifndef M_LEVEL_BREAKOUT
#define M_LEVEL_BREAKOUT

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include "Sprite.h"
#include "Brick.h"
#include <vector>

namespace Breakout
{
    //Struct that contains all the entities of a level
    struct LevelBreakout
    {
        std::array<SDLEngine::Sprite, 6> BrickTextures;
        std::vector<entt::entity> BricksEntities;
        entt::entity BackgroundEntity;
        entt::entity PaddleEntity;
        entt::entity BallEntity;
        std::string LevelMusicPath;
        SDLEngine::Sprite BallGraySprite;
        SDLEngine::Sprite BallBlueSprite;
        int BrickCount = 0;

        const SDLEngine::Sprite& GetBrickSprite(Brick::BrickType bType)
        {
            return BrickTextures[static_cast<int>(bType)];
        }
    };
}

#endif // !M_LEVEL_BREAKOUT
