#ifndef M_LEVEL_BREAKOUT
#define M_LEVEL_BREAKOUT

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <vector>

namespace Breakout
{
    //Struct that contains all the entities of a level
    struct LevelBreakout
    {
        std::vector<entt::entity> BricksEntities;
        entt::entity BackgroundEntity;
        entt::entity PaddleEntity;
        entt::entity BallEntity;
        std::string LevelMusicPath;

        //Unload the entities from the registry
        void Unload(entt::registry& reg)
        {
            reg.destroy(BackgroundEntity);
            reg.destroy(PaddleEntity);
            reg.destroy(BallEntity);
            std::for_each(BricksEntities.begin(), BricksEntities.end(), [&](const auto& ent) { reg.destroy(ent); });
        }
    };
}

#endif // !M_LEVEL_BREAKOUT
